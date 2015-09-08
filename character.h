#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "attributes.h"

class Character
{
public:
    Character( int row, int col, char ch, const Attributes& attr ):
        mRow( row ),
        mCol( col ),
        mChar(ch),
        mAttr(attr)
    {};
    ~Character( void ) {};

    int row( void ) const { return mRow; };

    int col( void ) const { return mCol; };

    char ch( void ) const { return mChar; };

    const Attributes& attr( void ) const { return mAttr; };
    void attr( const Attributes& attr ) { mAttr = attr; };
private:
    int mRow;
    int mCol;
    char mChar;
    Attributes mAttr;
};

#endif
