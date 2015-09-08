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
