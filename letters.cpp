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

#include <fstream>
#include <string>
#include <iostream>
#include "letters.h"

Letters::Letters( const std::string& letterFileName )
{
    std::ifstream lf( letterFileName.c_str() );
    std::string line;

    if ( lf.is_open() )
    {
        char letter;
        while ( ( letter = lf.get() ) != -1 )
        {
            // Skip the comma.
            lf.get();

            int score = 0;
            char sch;
            while ( ( sch = lf.get() ) != '\n' )
            {
                score *= 10;
                score += sch - '0';
            }

            mLetterScores[ letter ] = score;
        }
    }

    lf.close();
}

int Letters::score( char letter ) const
{
    std::map<char,int>::const_iterator i = mLetterScores.find( letter );

    return i->second;
}
