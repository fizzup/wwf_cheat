#ifndef __ANATIONARY_H__
#define __ANATIONARY_H__

#include <list>
#include <string>
#include "fingerprint.h"
#include "tile.h"

// A dictionary for searching anagrams. The words are stored as Fingerprint
// objects.
class Anationary
{
public:
    Anationary( void ) {};
    Anationary( const std::string& wordListFilename );
    Anationary( const Anationary& anationary )
        : mFingerprints( anationary.mFingerprints ) {};
    ~Anationary( void ) {};

    const Anationary& operator =( const Anationary& rhs )
        { mFingerprints = rhs.mFingerprints; return *this; };

    std::list<std::string> anagrams( const Fingerprint& rack ) const;

    // This function is scrabble-specific, because it deals with blanks
    // in the rack.
    std::list<Tiles> tanagrams( const Fingerprint& rack ) const;

private:
    std::list<Fingerprint> mFingerprints;
};

#endif
