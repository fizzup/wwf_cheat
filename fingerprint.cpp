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

#include <string>
#include <stdlib.h>
#include "fingerprint.h"
#include "board.h"
#include "tile.h"

// Optimization - store the fingerprint letters in commonality sequence.
// I found this sequence by experimentation. It seems to be mostly in 
// commonality sequence, most common letters first. We need to be able to
// translate from a letter into an index, and from an index to a letter,
// so these two arrays are each other's complement. (i.e., the first has a '1'
// in the 0th position, and the second has a '0' in the 1st position.)
int Fingerprint::mLetterSequence[LETTERS] =
    { 1, 16, 9, 10, 0, 18, 14, 15, 3, 24, 20, 8, 13,
      7, 6, 12, 25, 4, 2, 5, 11, 19, 21, 23, 17, 22 };
int Fingerprint::mLetterUnsequence[LETTERS] =
    { 4, 0, 18, 8, 17, 19, 14, 13, 11, 2, 3, 20, 15,
      12, 6, 7, 1, 24, 5, 21, 10, 22, 25, 23, 9, 16 };

Fingerprint::Fingerprint( const std::string& word )
    : mWord( word ),
      mLetterCount( LETTERS ),
      mBlankCount( 0 )
{
    std::string::const_iterator i = word.begin();

    while( i != word.end() )
    {
        if ( 'a' <= *i && *i <= 'z' )
        {
            mLetterCount[ mLetterSequence[ *i - 'a' ] ] += 1;
        }
        else
        {
            mBlankCount += 1;
        }
        i++;
    }
}

bool Fingerprint::isSubAnagram( const Fingerprint& rack ) const
{
    int neededBlanks = 0;
    std::vector<int>::const_iterator ri = rack.mLetterCount.begin();
    std::vector<int>::const_iterator mi = mLetterCount.begin();

    while ( ri != rack.mLetterCount.end() )
    {
        if ( *ri < *mi )
        {
            neededBlanks += *mi - *ri;
            if ( neededBlanks > rack.mBlankCount )
            {
                return false;
            }
        }
        ri++;
        mi++;
    }

    return true;
}

std::list<Tiles> Fingerprint::adjustForBlanks( const Fingerprint& rack ) const
{
    std::list<Tiles> words;

    words.push_back( Tile::tiles( mWord ) );

    for ( int i = 0; i < LETTERS; i++ )
    {
        int neededBlanks = mLetterCount[i] - rack.mLetterCount[i];
        char ch = 'a' + mLetterUnsequence[ i ];

        for ( int b = 0; b < neededBlanks; b++ )
        {
            std::list<Tiles> newWords;
            std::list<Tiles>::const_iterator wi = words.begin();
            
            while ( wi != words.end() )
            {
                Tiles word = *wi;
                Tiles::iterator li = word.begin();

                while ( li != word.end() )
                {
                    if ( !li->isBlank() && li->letter() == ch )
                    {
                        li->isBlank( true );
                        newWords.push_back( word );
                        li->isBlank( false );
                    }
                    li++;
                }

                wi++;
            }

            words = newWords;
        }
    }

    std::list<Tiles>::iterator ff = words.begin();
    while ( ff != words.end() )
    {
        std::list<Tiles>::iterator wf = ff;
        wf++;

        while ( wf != words.end() )
        {
            if ( *ff == *wf )
            {
                wf = words.erase( wf );
            }
            else
            {
                wf++;
            }
        }

        ff++;
    }

    return words;
}

bool Fingerprint::isBlank( char ch )
{
    return ch < 'a' || 'z' < ch;
}
