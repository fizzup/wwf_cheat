#include <fstream>
#include <string>
#include <iostream>
#include "letters.h"

Letters::Letters( const std::string& letterFileName )
{
    std::ifstream lf( letterFileName.c_str() );
    std::string line;

    if ( lf.is_open() )
    {
        char letter;
        while ( ( letter = lf.get() ) != -1 )
        {
            // Skip the comma.
            lf.get();

            int score = 0;
            char sch;
            while ( ( sch = lf.get() ) != '\n' )
            {
                score *= 10;
                score += sch - '0';
            }

            mLetterScores[ letter ] = score;
        }
    }

    lf.close();
}

int Letters::score( char letter ) const
{
    std::map<char,int>::const_iterator i = mLetterScores.find( letter );

    return i->second;
}
