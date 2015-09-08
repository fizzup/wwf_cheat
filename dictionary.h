#ifndef __DICT_H__
#define __DICT_H__

#include <string>
#include <string.h>
#include "tile.h"

#define ALLOCATE_BLOCK 131072

class CompiledDictionary;

class Dictionary
{
public:
    Dictionary( void );
    Dictionary( const char* word_file );
    ~Dictionary( void );

    void add_word( const char* word );
    bool find( const char* word, bool find_fragment = false ) const;
    bool find( std::string word, bool find_fragment = false ) const
        { return find( word.c_str(), find_fragment ); };
    void clear( void );
protected:
private:

    struct letter
    {
        letter( char ch )
            : m_ch( ch ),
              m_is_word( 0 ),
              m_child( NULL ),
              m_next( NULL ) {};
        letter( void )
            : m_ch( '\0' ),
              m_is_word( 0 ),
              m_child( NULL ),
              m_next( NULL ) {};
        char m_ch;
        bool m_is_word;
        struct letter* m_child;
        struct letter* m_next;
    };

    letter m_dict_tree[26];

    void initialize_word_tree( void );
    void remove_child( letter* child );

    friend class CompiledDictionary;
};

class CompiledDictionary
{
public:
    CompiledDictionary( const Dictionary& dict );
    CompiledDictionary( const char* dict_file );
    ~CompiledDictionary( void );

    bool find( const char* word, bool find_fragment = false ) const;
    bool find( std::string word, bool find_fragment = false ) const
        { return find( word.c_str(), find_fragment ); };
    void save( const char* dict_file ) const;
protected:
private:

    int add_dictionary_level( const Dictionary::letter* l );
    static int compare_letters( const void* elem1, const void* elem2 );
    void allocate_new_space( void )
    {
        if ( m_free_space == 0 )
        {
            letter* new_word_tree = new letter[ m_used_space + ALLOCATE_BLOCK ];
            memcpy( new_word_tree, m_dict_tree, sizeof( letter ) * m_used_space );
            delete [] m_dict_tree;
            m_dict_tree = new_word_tree;
            m_free_space = ALLOCATE_BLOCK;
            m_free_pointer = m_dict_tree + m_used_space;
        }
    };

    struct letter
    {
        char m_ch;
        bool m_is_word;
        short m_sibling_count;
        int m_child;
    };

    letter* m_dict_tree;
    letter* m_free_pointer;
    int m_free_space;
    int m_used_space;
};


#endif
