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
