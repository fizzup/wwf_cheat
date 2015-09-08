#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <string>
#include <list>
#include "character.h"

class Screen;

class Window
{
public:
    ~Window( void ) {};

    void clear( void ) { mContent = std::list<Character>(); drawFrame(); };
    void addChar( const Character& ch ) {
        mContent.push_back( ch );
        mCurRow = ch.row();
        mCurCol = ch.col();
        mCurAttr = ch.attr();
    };

    void setCursor( int row, int col, const Attributes& attr = Attributes() );
    void typeChar( char ch, bool protectBorder = true );
    void typeLine( const std::string& s, bool protectBorder = true );

    void chAttr( int row, int col, int h, int w, const Attributes& attr );

    void paint( void ) const;

    const std::string& name( void ) const { return mName; };

    void setFocus( Screen& s ) const;
protected:
    Window( const std::string& name, const std::string& title,
            int row, int col, int h, int w);
private:
    std::list<Character> mContent;
    int mRow;
    int mCol;
    int mHeight;
    int mWidth;

    std::string mTitle;
    std::string mName;

    int mCurRow;
    int mCurCol;
    Attributes mCurAttr;

    static const char mCorner = ' ';
    static const char mHorizontal = ' ';
    static const char mVertical = ' ';

    void drawFrame( void );

friend class Screen;
};

#endif
