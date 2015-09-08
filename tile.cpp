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
