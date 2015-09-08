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
