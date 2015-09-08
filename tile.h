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
