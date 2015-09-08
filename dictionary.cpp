#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "dictionary.h"

Dictionary::Dictionary( void )
{
    initialize_word_tree();
}

Dictionary::Dictionary( const char* word_file )
{
    initialize_word_tree();
    char word[128];

    FILE* d = fopen( word_file, "ra" );

    while ( fgets( word, sizeof( word ), d ) )
    {
        word[ strlen( word ) - 1 ] = '\0';

        add_word( word );
    }

    fclose( d );
}

Dictionary::~Dictionary( void )
{
    clear();
}

void Dictionary::add_word( const char* word )
{
    letter* l = m_dict_tree;
    const char* p = word;

    while ( *p )
    {
        while ( l->m_ch != *p && l->m_next )
        {
            l = l->m_next;
        }

        if ( l->m_ch != *p )
        {
            l->m_next = new letter( *p );
            l = l->m_next;
        }

        p++;

        if ( *p )
        {
            if ( l->m_child == NULL )
            {
                l->m_child = new letter( *p );
                l = l->m_child;
            }
            else
            {
                l = l->m_child;
            }
        }
    }

    l->m_is_word = 1;
}

bool Dictionary::find( const char* word, bool find_fragment ) const
{
    const letter* l = m_dict_tree;
    const char* p = word;

    while ( *p )
    {
        while ( l->m_ch != *p && l->m_next )
        {
            l = l->m_next;
        }

        if ( l->m_ch != *p )
        {
            return false;
        }

        p++;

        if ( *p )
        {
            if ( l->m_child == NULL )
            {
                return false;
            }
            else
            {
                l = l->m_child;
            }
        }
    }

    return find_fragment || l->m_is_word;
}

void Dictionary::clear( void )
{
    letter* l = m_dict_tree;

    while ( l->m_next )
    {
        if ( l->m_child )
        {
            remove_child( l->m_child );
            l->m_child = NULL;
        }
        l = l->m_next;
    }
}

void Dictionary::remove_child( letter* child )
{
    if ( child->m_child )
    {
        remove_child( child->m_child );
    }

    letter* next = child->m_next;

    delete child;

    if ( next )
    {
        remove_child( next );
    }
}

void Dictionary::initialize_word_tree( void )
{
    for ( char ch = 'a'; ch <= 'z'; ch++ )
    {
        m_dict_tree[ ch - 'a' ] = letter( ch );
        if ( ch != 'z' )
        {
            m_dict_tree[ ch - 'a' ].m_next = m_dict_tree + ( ch - 'a' + 1 );
        }
    }
}


CompiledDictionary::CompiledDictionary( const Dictionary& dict )
    : m_dict_tree( NULL )
{
    m_free_pointer = m_dict_tree = new letter[ ALLOCATE_BLOCK ];
    m_free_space = ALLOCATE_BLOCK;
    m_used_space = 0;

    const Dictionary::letter* l = dict.m_dict_tree;

    add_dictionary_level( l );
}

CompiledDictionary::CompiledDictionary( const char* dict_file )
    : m_dict_tree( NULL )
{
    struct stat st;

    int rc = stat( dict_file, &st );

    if ( rc != -1 )
    {
        int fd = open( dict_file, O_RDONLY );
        if ( fd != -1 )
        {
            m_used_space = st.st_size / sizeof ( letter );
            m_free_space = 0;
            m_free_pointer = NULL;
            m_dict_tree = new letter[ m_used_space ];
            rc = read( fd, m_dict_tree, sizeof( letter ) * m_used_space );
            if ( rc == -1 )
            {
                throw ( "read of compiled dictionary failed" );
            }
            close( fd );
        }
    }
}

CompiledDictionary::~CompiledDictionary( void )
{
    if ( m_dict_tree != NULL )
    {
        delete[] m_dict_tree;
    }
}

bool CompiledDictionary::find( const char* needle, bool find_fragment ) const
{
#ifdef USE_ASSEMBLY
#ifdef MSVCC
    __asm
    {
        mov   edx, needle            // edx is a cursor for needle
        mov   edi, [ecx]this.m_dict_tree // edi is a reference to m_dict_tree
        mov   ebx, edi               // ebx is a cursor for m_dict_tree

        mov   cl, [edx]              // inc edx after mov to stop AGI stall
        inc   edx
search_char:
        mov   al, [ebx]              // change ebx after move to avoid AGI stall
        add   ebx, 8                 // ebx += sizeof( letter )
        test  al, al
        jz    exit_find              // al is already 0, so this is return 0
        cmp   cl, al
        jne   search_char
char_found:
        sub   ebx, 8                 // we went one to far to avoid AGI stall
        mov   cl, [edx]              // inc edx after mov to stop AGI stall
        inc   edx
        test  cl, cl
        jz    word_found
        mov   eax, [ebx+4]
        test  eax, eax               // cmp l->m_child, 0
        jz    exit_find              // al is already 0, so this is return 0
has_child:
        lea   ebx, [ edi + eax * 8 ] // ebx = edi + eax * sizeof( letter )
        jmp   search_char
word_found:
        mov   al, find_fragment
        test  al, al
        jnz   exit_find              // al is already 1, so this is return 1
full_word:
        mov   al, [ebx+1]            // return l->m_is_word
exit_find:
    }
#endif // MSVCC

#ifdef GCC
    asm(
    "mov  %%edi, %%ebx\n\t"       // ebx is a cursor for m_dict_tree
    "mov  (%%edx), %%cl\n\t"      // inc edx after mov to stop AGI stall
    "inc  %%edx\n"
".L100SearchChar:\n\t"
    "mov  (%%ebx), %%al\n\t"      // change ebx after move to avoid AGI stall
    "add  $8, %%ebx\n\t"          // ebx += sizeof( letter )
    "test %%al, %%al\n\t"
    "jz   .L105ExitFind\n\t"      // al is already 0, so this is return 0 (false)
    "cmp  %%al, %%cl\n\t"
    "jne  .L100SearchChar\n"
".L101CharFound:\n\t"
    "sub  $8, %%ebx\n\t"          // we went one to far to avoid AGI stall
    "mov  (%%edx), %%cl\n\t"      // inc edx after mov to stop AGI stall
    "inc  %%edx\n\t"
    "test %%cl, %%cl\n\t"
    "jz   .L103WordFound\n\t"
    "mov  4(%%ebx), %%eax\n\t"
    "test %%eax, %%eax\n\t"       // cmp l->m_child, 0
    "jz   .L105ExitFind\n"        // al is already 0, so this is return 0
".L102HasChild:\n\t"
    "lea  (%%edi, %%eax, 8), %%ebx\n\t"// ebx = edi + eax * sizeof( letter )
    "jmp  .L100SearchChar\n"
".L103WordFound:\n\t"
    "mov  %%esi, %%eax\n\t"
    "test %%al, %%al\n\t"
    "jnz  .L105ExitFind\n"        // al is already 1, so this is return 1
".L104FullWord:\n\t"
    "mov  1(%%ebx), %%al\n"       // return l->m_is_word
".L105ExitFind:\n\t"
    "and  $0xff, %%eax\n\t"

    : // output operands
    : "d"(needle), "D"(m_dict_tree), "S"(find_fragment) // input operands
    : "%ebx", "%cl"// clobbered registers
    );
    
    // No return statement because the a register already has the correct
    // value
#endif // GCC

#else
    const letter* l = m_dict_tree;
    const char* p = needle;

    while ( *p )
    {
        while ( l->m_ch )
        {
            if ( l->m_ch == *p )
            {
                break;
            }
            l++;
        }
        if ( l->m_ch != *p )
        {
            return false;
        }

        p++;

        if ( *p )
        {
            if ( l->m_child == 0 )
            {
                return false;
            }
            else
            {
                l = m_dict_tree + l->m_child;
            }
        }
    }

    return find_fragment || l->m_is_word;
#endif
}

void CompiledDictionary::save( const char* dict_file ) const
{
    int fd = open( dict_file,
        O_TRUNC | O_CREAT | O_RDWR,
        S_IREAD | S_IWRITE );

    if ( fd >= 0 )
    {
        write( fd, m_dict_tree, sizeof( letter ) * m_used_space );
    }

    close( fd );
}

int CompiledDictionary::add_dictionary_level( const Dictionary::letter* s )
{
    const Dictionary::letter* l = s;

    int start = m_free_pointer - m_dict_tree;

    do
    {
        m_free_pointer->m_ch = l->m_ch;
        m_free_pointer->m_is_word = l->m_is_word;
        m_free_pointer->m_sibling_count = 0;
        m_free_pointer->m_child = 0;

        l = l->m_next;
        m_free_pointer++;
        m_used_space++;
        m_free_space--;

        allocate_new_space();
    } while ( l != NULL );

    m_free_pointer->m_ch = '\0';
    m_free_pointer++;
    m_used_space++;
    m_free_space--;
    allocate_new_space();

    l = s;
    int ltr = start;
    int sibling_count = 0;
    do
    {
        if ( l->m_child != NULL )
        {
            // This is a tricky sequence of code, because add_dictionary_level
            // can allocate a new m_dict_tree, so it makes the assignment
            // to m_dict_tree[ltr] after the call to add_dictionary_level.
            int child = add_dictionary_level( l->m_child );
            m_dict_tree[ltr].m_child = child;
        }
        l = l->m_next;
        ltr++;
        sibling_count++;
    } while ( l != NULL );

    m_dict_tree[ start ].m_sibling_count = sibling_count;

    qsort( m_dict_tree + start,
        m_dict_tree[ start ].m_sibling_count,
        sizeof ( letter ), compare_letters );

    return start;
}

// Sort dictionary letters by letter frequency. Most frequent letters first.
int CompiledDictionary::compare_letters( const void* elem1, const void* elem2 )
{
    static int lf[] = {
        8167,
        1492,
        2782,
        4253,
        12702,
        2228,
        2015,
        6094,
        6966,
        153,
        772,
        4025,
        2406,
        6749,
        7507,
        1929,
        95,
        5987,
        6327,
        9056,
        2758,
        978,
        2360,
        150,
        1974,
        74,
    };
    char ch1 = ((const letter*)elem1)->m_ch;
    char ch2 = ((const letter*)elem2)->m_ch;

    return lf[ch2-'a'] - lf[ch1-'a'];
}
