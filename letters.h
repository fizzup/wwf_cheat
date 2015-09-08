#ifndef __LETTERS_H__
#define __LETTERS_H__

#include <string>
#include <map>
#include "tile.h"

class Letters
{
public:
    Letters( void ) {};
    Letters( const std::string& letterFileName );
    Letters( const Letters& l ) : mLetterScores( l.mLetterScores ) {};
    ~Letters() {};

    const Letters& operator =( const Letters& rhs )
    {
        mLetterScores = rhs.mLetterScores;
        return *this;
    }

    int score( char letter ) const;

    Tile tile( char letter, bool isBlank = false ) const
    {
        if ( letter < 'a'  || 'z' < letter )
        {
            isBlank = true;
        }

        return Tile( letter, isBlank ? 0 : score( letter ), isBlank );
    };
private:

    std::map<char,int> mLetterScores;
};


#endif
