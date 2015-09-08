#include <ncurses.h>
#include <map>
#include "attributes.h"

int Attributes::mMaxColorPair = 0;
std::map<int,int> Attributes::mColorMap;

void Attributes::set( void ) const
{
    int cattr = 0;

    if ( has_colors() == TRUE )
    {
        int colorKey = (int)mForeground << 16 | (int)mBackground;

        std::map<int,int>::iterator i = mColorMap.find( colorKey );
        if ( i != mColorMap.end() )
        {
            cattr = COLOR_PAIR( i->second );
        }
        else
        {
            mColorMap.insert( std::pair<int,int>( colorKey, ++mMaxColorPair ) );
            init_pair( mMaxColorPair, (int)mForeground, (int)mBackground );
            cattr = COLOR_PAIR( mMaxColorPair );
        }
    }

    // Next, intensity
    attron( cattr | (int)mIntensity );
}

void Attributes::clear( void ) const
{
    int cattr = 0;

    if ( has_colors() == TRUE )
    {
        int colorKey = (int)mForeground << 16 | (int)mBackground;

        std::map<int,int>::iterator i = mColorMap.find( colorKey );
        if ( i != mColorMap.end() )
        {
            cattr = COLOR_PAIR( i->second );
        }
        else
        {
            mColorMap.insert( std::pair<int,int>( colorKey, ++mMaxColorPair ) );
            init_pair( mMaxColorPair, (int)mForeground, (int)mBackground );
            cattr = COLOR_PAIR( mMaxColorPair );
        }
    }
    
    attroff( cattr | (int)mIntensity );
}
