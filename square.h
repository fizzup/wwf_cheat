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

#ifndef __SQUARE_H__
#define __SQUARE_H__
#include "tile.h"

class Square
{
public:
    Square( int letterFactor = 1, int wordFactor = 1 ) :
        mIsPlayed( false ),
        mTile( NULL ),
        mLetterFactor( letterFactor ),
        mWordFactor( wordFactor ) {};
    
    Square( const Square& rhs ) :
        mIsPlayed( rhs.mIsPlayed ),
        mTile( mIsPlayed ? new Tile( *(rhs.mTile) ) : NULL ),
        mLetterFactor( rhs.mLetterFactor ),
        mWordFactor( rhs.mWordFactor ) {};

    ~Square() 
    {
        if ( mIsPlayed )
        {
            delete mTile;
        }
    };

    const Square &operator =( const Square& rhs )
    {
        mIsPlayed = rhs.mIsPlayed;
        mTile = mIsPlayed ? new Tile( *(rhs.mTile) ) : NULL;
        mLetterFactor = rhs.mLetterFactor;
        mWordFactor = rhs.mWordFactor;

        return *this;
    };

    void play( const Tile& tile )
    {
        mIsPlayed = true;
        mTile = new Tile( tile );
        mLetterFactor = 1;
        mWordFactor = 1;
    }

    bool hasTile( const Tile& tile ) const
    {
        return tile.letter() == letter();
    }

    bool isPlayed( void ) const { return mIsPlayed; };
    char letter( void ) const { return mIsPlayed ? mTile->letter() : ' '; };
    int score( void ) const { return mIsPlayed ? mTile->score() : 0; };
    bool isBlank( void ) const { return mIsPlayed ? mTile->isBlank() : false; };
    int letterFactor( void ) const { return mLetterFactor; };
    int wordFactor( void ) const { return mWordFactor; };

protected:
    bool mIsPlayed;
    Tile* mTile;
    int mLetterFactor;
    int mWordFactor;
private:
};

#endif
