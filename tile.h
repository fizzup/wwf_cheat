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

#ifndef __TILE_H__
#define __TILE_H__

#include <list>

class Letters;
class Tile;

typedef std::list<Tile> Tiles;

class Tile
{
public:
    Tile( char letter, int score, bool isBlank = false )
        : mLetter( letter ),
          mScore( score ),
          mIsBlank( isBlank ) { };
    Tile( const Tile& tile ) 
        : mLetter( tile.mLetter ),
          mScore( tile.mScore ),
          mIsBlank( tile.mIsBlank ) {};

    char letter( void ) const { return mLetter; };
    int score( void ) const { return mIsBlank ? 0 : mScore; };
    void score( int score ) { mScore = score; }
    bool isBlank( void ) const { return mIsBlank; };
    void isBlank( bool isBlank ) { mIsBlank = isBlank; };

    bool operator==( const Tile& rhs ) const
    {
        return mLetter == rhs.mLetter &&
               mScore == rhs.mScore &&
               mIsBlank == rhs.mIsBlank;
    }

    bool operator!=( const Tile& rhs ) const
    {
        return !(*this == rhs);
    }

    const Tile& operator =( const Tile& tile )
    {
        mLetter = tile.mLetter;
        mScore = tile.mScore;
        mIsBlank = tile.mIsBlank;

        return *this;
    }

    static std::string letters( const Tiles& tiles );
    static Tiles tiles( const std::string& word );
private:
    char mLetter;
    int mScore;
    bool mIsBlank;

friend class Letters;
};

bool operator==( const Tiles& lhs, const Tiles& rhs );

#endif
