#include <iostream>
#include "dictionary.h"

using namespace std;

int main( int argc, char** argv )
{
	char* word_file;
    char* dict_file;

	if ( argc > 1 )
	{
		word_file = argv[1];
	}
	else
	{
		word_file = (char*)"words";
	}

    if ( argc > 2 )
    {
        dict_file = argv[2];
    }
    else
    {
        dict_file = (char*)"dictionary";
    }

    cout << "Compiling dictionary " << word_file << "." << endl;

    cout << "Creating an in-memory dictionary from \"" << word_file << "\"." << endl;
	Dictionary dict( word_file );
    cout << "In-memory dictionary created." << endl;

    cout << "Compiling in-memory dictionary." << endl;
	CompiledDictionary cmp( dict );
    cout << "In-memory dictionary compiled." << endl;

    cout << "Writing compiled dictionary to disk." << endl;
	cmp.save( dict_file );
    cout << "Compiled dictionary saved to disk as \"" << dict_file << "\"." << endl;

	return 0;
}
