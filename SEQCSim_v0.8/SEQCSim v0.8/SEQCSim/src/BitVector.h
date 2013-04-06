//========================================================================
// SEQCSim version 0.8 - The Space-Efficient Quantum Computer Simulator
// By Michael P. Frank, Liviu Oniciuc, Uwe Meyer-Baese, and Liviu Oniciuc.
// Copyright (C) 2008-2009  Florida State University Board of Trustees
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// You may contact the author at michael.patrick.frank@gmail.com, or at
// Michael P. Frank, PO Box 025250 #24985, Miami, FL  33102-5250.
//========================================================================

//-------------------------------------------------------------
// BitVector.h - Header file for our custom bit-vector class.
//
// Why define our own bit vector class?  
//
//   1. SGI's good old bit_vector class isn't available in Visual C++ 2008 Express.
//   2. bitset<N> objects can't be dynamically sized.
//   3. The vector<bool> specialization available in older versions of the C++ standard 
//        is now deprecated, for numerous good reasons.
//   4. The C++ standard's new std::dynamic_bitset class isn't available in Visual C++ 2008 Express.
//   5. We don't want to use any Microsoft-specific classes, for portability.
//
// What features do we want in our bit vector class?
//
//   1. Size chosen dynamically (i.e., at vector creation time).  Dynamically resizeable.
//   2. Space-efficient storage (i.e., packed as individual bits).
//   3. Straightforward conversion to and from numeric data types, in the case of smaller bit-vector sizes.
//   4. Decent performance.
//   5. Clean interface.
//
// This class stores bits in little-endian format.  That is, the
// first bit (bit #0) in the BitVector is stored in the least
// significant bit of a machine word.
//--------------------------------------------------------------

#pragma once

#include <vector>			// STL vector<> template.
#include <iostream>			// ostream for operator>> function.
#include "index_types.h"	// Our u32_t, u8_t typedefs.

using namespace std;		// For vector<>, ostream.

// This helper class emulates a "reference" to an individual bit
// in a BitVector.  I.e. once we have a handle on that bit, we can
// fetch and modify its value without going through the BitVector
// object that it's contained in.

class BitRef {
private:
	u32_t*	wordPtr;	// C++ pointer to the unsigned 32-bit word containing the bit.
	u8_t	bitIndex;	// Index of the individual bit within the word.
public:
	// Default constructor, just sets the data members as provided.
	BitRef(u32_t* w, u8_t b) : wordPtr(w), bitIndex(b) { }

	// Return the bit's current value as a Boolean true/false value.
	operator bool(void) { return ((*wordPtr)&(1<<bitIndex))!=0; }

	// The following may not be needed since there may be an automatic 
	// conversion from bool to numeric types.
	// Conversion operator, returns the bit value as a numeric type.
	// Returns the bit's current value as an unsigned 8-bit number 
	// (this should be unsigned char on most systems).
	//operator u8_t(void) { return ((bool)*this)?1:0; }

	// Do we really need both of the below, or will the 2nd one work
	// automatically if the 1st one is available?

	// Set the bit's current value based on the given Boolean true/false value.
	BitRef& operator=(bool value) {
		if (value) {
			(*wordPtr) |= (1<<bitIndex);	// OR a 1 into the correct bit-position.
		} else {
			(*wordPtr) &= ~(1<<bitIndex);	// AND a 0 into the correct bit-position.
		}
		return *this;	// Needed for expressions of the form (v = a) = b to work...
	}

	// Set the bit's current value based on the given integer value (1 if nonzero, 0 if zero).
	//BitRef& operator=(int value) {
	//	(*this) = (value!=0);
	//	return *this;	// Needed for expressions of the form (v = a) = b to work...
	//}

};

class BitVector {
	// Private data members.  For class-internal use only.
private:
	vector<size_t>	bitWords;		// Defaults to 0 words.
	size_t			nBits;			// Defaults to 0 bits.

private:
	// Compute the minimum number of words needed to store the given number of bits.
	size_t _nWords_from_nBits(size_t nBits) { return nBits ? ((nBits-1) >> 5) + 1 : 0; }
	size_t _nWords_needed() { return _nWords_from_nBits(nBits); }

	// Public members.  External interface for class users.
public:
	// The default constructor just initializes the size to 0.
	BitVector(void) : nBits(0) { }

	// This constructor creates a BitVector of a given size, in bits.
	BitVector(size_t size) : nBits(size), bitWords(_nWords_from_nBits(size)) { }

	// This is for resizing a bit vector after it has already been created.
	void  resize(size_t newsize) {
		nBits = newsize;	// Keep track of the exact size in bits.
		// Use the smallest number of 32-bit words that will hold nBits bits.
		bitWords.resize(_nWords_needed());
	}

	// Return the size of this bit vector in bits.
	size_t  size(void) { return nBits; }

	// Given a bit index, this method returns a BitRef, which functions
	// like a reference to an individual bit should; it can be used like
	// an lvalue.  However, it is not really a C++ reference.  See the 
	// BitRef class definition above for the declaration of its access 
	// methods.

	BitRef  operator[](size_t bitIndex) {
		size_t	wordIndex			= bitIndex>>5;		// The bits above the low 5 index the 32-bit words.
		u8_t	bitIndex_inWord		= bitIndex&0x1f;	// AND'ing with 31 extracts low 5 bits of the index.
		
		// Create the BitRef object, which effectively points to an individual bit in an individual word.
		BitRef bitRef(&bitWords[wordIndex],		// Normal C pointer to the 32-bit word
					  bitIndex_inWord);			// Index (0-31) of the bit within the word.

		// Note that we return a copy of the bitRef object, rather than, say, an auto_ptr to a newly
		// allocated object.  This is because a bitRef is such a simple object that dynamic allocation
		// of it would be overkill.
		return bitRef;
	}

	// This conversion function specifies conversion of BitVectors to
	// various numeric data types.  The conversion works by just converting
	// the first (low-order) word of the BitVector.  It therefore loses
	// information unless there are 32 or fewer bits in the BitVector.

	operator size_t(void) { return bitWords[0]; }

	// This is for assigning the bits of a BitVector from an integer.  It
	// works by just assigning the first word.  The size of the BitVector 
	// is not changed.  This can lead to later undesired behavior (in ==)
	// if the integer contains nonzero bits past the logical end of the 
	// BitVector.  Users of this routine should ensure that's not the case.

	BitVector& operator=(size_t integer) { bitWords[0] = integer; return *this; }

	// This is for setting the current BitVector to be a copy of another one.

	BitVector& operator=(BitVector &source) {
		resize(source.size());
		bitWords.assign(source.bitWords.begin(), source.bitWords.end());
		return (*this);
	}

	// For printing a BitVector.  Stupid >> operator suffers from ambiguous conversions.
	void putTo(ostream& os);

	// For testing a pair of BitVectors for equality.  Works by just comparing
	// the lengths and the underlying vectors of words.  Note that for this to 
	// be correct, there should not be any extra nonzero bits in the last word 
	// beyond the length of the actual BitVector.
	//
	// NOTE: The following definition is not actually needed, because a ==
	// method that does the same thing exists automatically in C++ by default.
	// That's why we have commented it out.
	//
	//bool operator==(BitVector &other_bv) {
	//	return nBits == other_bv.nBits && bitWords == other_bv.bitWords;
	//}
};

// This operator seems to never get invoked, because the compiler just
// applies a BitVector-to-number conversion and prints that instead.
// ostream& operator>>(ostream& os, BitVector& bv);