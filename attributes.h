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

#ifndef __ATTRIBUTES_H__
#define __ATTRIBUTES_H__

#include <ncurses.h>
#include <map>

enum Color { Black   = COLOR_BLACK,
             Red     = COLOR_RED,
             Green   = COLOR_GREEN,
             Yellow  = COLOR_YELLOW,
             Blue    = COLOR_BLUE,
             Magenta = COLOR_MAGENTA,
             Cyan    = COLOR_CYAN,
             White   = COLOR_WHITE };

enum Intensity { Dim = A_DIM, Normal = A_NORMAL, Bold = A_BOLD };

class Attributes
{
public:
    Attributes( Intensity i = Normal, Color fg = Black, Color bg = White ) 
        : mIntensity( i ),
          mBackground( bg ),
          mForeground( fg ) {};

    void set( void ) const;
    void clear( void ) const;
private:
    Intensity mIntensity;
    Color mForeground;
    Color mBackground;

    static std::map<int,int> mColorMap;
    static int mMaxColorPair;
};

#endif
