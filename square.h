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
