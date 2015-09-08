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

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include "anationary.h"
#include "fingerprint.h"
#include "tile.h"

Anationary::Anationary( const std::string& wordListFilename )
{
    std::ifstream wlf;
    std::string word;

    wlf.open( wordListFilename.c_str() );

    while ( wlf.good() )
    {
        getline( wlf, word );
        mFingerprints.push_back( Fingerprint( word ) );
    }
}

std::list<std::string> Anationary::anagrams( const Fingerprint& rack ) const
{
    std::list<std::string> anagrams;
    std::list<Fingerprint>::const_iterator i = mFingerprints.begin();

    while( i != mFingerprints.end() )
    {
        if ( i->isSubAnagram( rack ) )
        {
            anagrams.push_back( i->word() );
        }

		++i;
    }

    return anagrams;
}

std::list<Tiles> Anationary::tanagrams( const Fingerprint& rack ) const
{
    std::list<Tiles> anagrams;
    std::list<Fingerprint>::const_iterator i = mFingerprints.begin();

    while( i != mFingerprints.end() )
    {
        if ( i->isSubAnagram( rack ) )
        {
            std::list<Tiles> blankWords = i->adjustForBlanks( rack );
            anagrams.insert( anagrams.end(),
                    blankWords.begin(), blankWords.end() );
        }

		++i;
    }

    return anagrams;
}
