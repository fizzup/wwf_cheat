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

#ifndef __LETTERS_H__
#define __LETTERS_H__

#include <string>
#include <map>
#include "tile.h"

class Letters
{
public:
    Letters( void ) {};
    Letters( const std::string& letterFileName );
    Letters( const Letters& l ) : mLetterScores( l.mLetterScores ) {};
    ~Letters() {};

    const Letters& operator =( const Letters& rhs )
    {
        mLetterScores = rhs.mLetterScores;
        return *this;
    }

    int score( char letter ) const;

    Tile tile( char letter, bool isBlank = false ) const
    {
        if ( letter < 'a'  || 'z' < letter )
        {
            isBlank = true;
        }

        return Tile( letter, isBlank ? 0 : score( letter ), isBlank );
    };
private:

    std::map<char,int> mLetterScores;
};


#endif
