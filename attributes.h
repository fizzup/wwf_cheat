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
