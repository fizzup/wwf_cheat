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
