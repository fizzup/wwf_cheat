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

#include <string>
#include "board.h"
#include "tile.h"

std::string Tile::letters( const Tiles& tiles )
{
    Tiles::const_iterator i = tiles.begin();
    std::string word;

    while ( i != tiles.end() )
    {
        if ( i->isBlank() )
        {
            word.push_back( '.' );
        }
        else
        {
            word.push_back( i->letter() );
        }
        i++;
    }

    return word;
}

Tiles Tile::tiles( const std::string& word )
{
    Tiles tiles;
    std::string::const_iterator i = word.begin();

    while ( i != word.end() )
    {
        tiles.push_back( Board::letters().tile( *i ) );
        i++;
    }

    return tiles;
}

bool operator==( const Tiles& lhs, const Tiles& rhs )
{
    Tiles::const_iterator ri = rhs.begin();
    Tiles::const_iterator li = lhs.begin();

    while ( ri != rhs.end() && li != lhs.end() )
    {
        if ( *ri != *li )
        {
            return false;
        }
        ri++;
        li++;
    }

    return ri == rhs.end() && li == lhs.end();
}
