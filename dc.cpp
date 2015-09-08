// Copyright (c) 2015 Michael Hamilton
//
// This file is part of wwf_cheat.

// wwf_cheat is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.

// wwf_cheat is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with wwf_cheat.  If not, see <http://www.gnu.org/licenses/>.

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
