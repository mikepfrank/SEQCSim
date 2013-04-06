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

//----------------------------------------------------------------------------------------
// Configuration.h - Header file for a class of object that defines a quantum computer's
//   named state variables & other general configuration information such as its size.
//
// (NOTE: this "general configuration" is a different kind of animal from a detailed
//  configuration of the machine, i.e. an assignment of values to all state variables; 
//  in this program, we call that entity a "state" and declare its structure in State.h.)
//-----------------------------------------------------------------------------------------

#pragma once

#include <string>			// STL string class
#include <vector>			// STL vector<> template
#include "index_types.h"	// qubit_index_t type

using namespace std;		// This line lets us say "string" instead of "std::string"

// A NamedBit object locates a named single-qubit-wide global state variable within the
// quantum computer.

class NamedBit {
public:
	string			name;		// Alphanumeric identifier for the named bit.
	qubit_index_t	address;	// Bit address of the named bit.
};
istream& operator>>(istream& is, NamedBit& nb);		// Print method.

// A NamedBit object locates a named multiple-qubit-wide global state variable within the
// quantum computer.  This could be, for example, an integer-valued quantum register.

class NamedBitArray {
public:
	// Public data members.
	string			name;			// Alphanumeric identifier for the named bit array.
	qubit_index_t	length;			// Length in bits of the named bit array.
	qubit_index_t	baseAddress;	// Bit address of the base of the named bit array.

	// Public member functions.
	qubit_index_t	addressOfBit(qubit_index_t	bitOffset) {return baseAddress + bitOffset;}
		// Calculates the bit-address of an element of this NamedBitArray.
};

ostream& operator<<(ostream& os, NamedBitArray& nba);	// Read method.

// A (general) configuration of a quantum computer.  Defines its size and its named qubit registers.
// Does not include the actual bit array (computational basis state, detailed configuration).

class Configuration {
public:
	qubit_index_t			nbits;				// Number of qubits in the quantum computer.
	
	// We could simplify the below a little by using the vectors' .size()s instead of
	// the n... variables.

	// The quantum computer's single-bit state variables.
	qubit_index_t			nNamedBits;			// Number of named qubits.
	vector<NamedBit>		namedBits;	        // Named qubits in the quantum computer.
	
	// The quantum computer's multi-bit state variables.
	unsigned short			nNamedBitArrays;	// Number of named qubit arrays.
	vector<NamedBitArray>	namedBitArrays;		// Named qubit arrays in the quantum computer.

private:
	// Private member functions.
	NamedBit*		_lookup_named_bit(const string& bitName);		   // Look up the NamedBit having the given name.
	NamedBitArray*	_lookup_named_bitarray(const string& bitArrName);  // Look up the NamedBitArray having the given name.
	
	// Public member functions.
public:
	// Default constructor (no arguments) - sets everything to 0/empty.
	Configuration(void){
		nbits = 0;
		
		nNamedBits = 0;
		namedBits.clear();
		
		nNamedBitArrays = 0;
		namedBitArrays.clear();
	}
	
	void initFromFile(const string& filename);	// Initialize QC config from the named file.

	// Look up a bit in the configuration, by name.
	qubit_index_t	lookup_byName(const string& bitName);
};

