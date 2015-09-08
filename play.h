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

#ifndef __PLAY_H__
#define __PLAY_H__

#include <string>
#include <iostream>
#include "tile.h"

class Board;

class Play
{
public:
    Play( int row, int col, bool h, const Tiles tiles, int score )
        : mRow( row ),
          mCol( col ),
          mHorizontal( h ),
          mTiles( tiles ),
          mScore( score ) {};

    Play( const Play& play ) :
        mRow( play.mRow ),
        mCol( play.mCol ),
        mHorizontal( play.mHorizontal ),
        mTiles( play.mTiles ),
        mScore( play.mScore ) {};

    ~Play() {};

    bool operator <( const Play& rhs ) const
    {
        return mScore < rhs.mScore;
    };

    bool operator ==( const Play& rhs ) const
    {
        return mRow == rhs.mRow &&
               mCol == rhs.mCol &&
               mHorizontal == rhs.mHorizontal &&
               (std::string)*this == (std::string)rhs;
    }

    operator std::string() const
    {
        Tiles::const_iterator i = mTiles.begin();
        std::string word;

        while ( i != mTiles.end() )
        {
            word.push_back( i->letter() );
            ++i;
        }

        return word;
    }

    const Play& operator =( const Play& rhs )
    {
        mRow = rhs.mRow;
        mCol = rhs.mCol;
        mHorizontal = rhs.mHorizontal;
        mTiles = rhs.mTiles;
        mScore = rhs.mScore;
        
        return *this;
    };

    void play( Board& board ) const;

    int row( void ) const { return mRow; };
    int col( void ) const { return mCol; };
    bool horizontal( void ) const { return mHorizontal; };
    const Tiles& tiles( void ) const { return mTiles; };
private:
    int mRow, mCol;
    int mScore;
    bool mHorizontal;
    Tiles mTiles;

friend std::ostream& operator<<( std::ostream& os, const Play& play );
};

inline std::ostream& operator<<( std::ostream& os, const Play& play )
{
    if ( play.mHorizontal )
    {
        os << "> ";
    }
    else
    {
        os << "V ";
    }
    os << (std::string)play << " - " << play.mScore;

    return os;
}

#endif
