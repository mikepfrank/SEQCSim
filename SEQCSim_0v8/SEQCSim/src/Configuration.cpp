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

// Configuration.cpp - Defines initFromFile() method for reading in objects
//    of class Configuration.

#include <string>
#include <sstream>			// istringstream
#include "Configuration.h"
#include "FileReader.h"
#include "debug.h"

ostream& operator<<(ostream& os, NamedBitArray& nba){
	os << nba.name << "[" << nba.length << "]@" << nba.baseAddress;
	return os;
}

void Configuration::initFromFile(const string& filename) {
	if (ns_debug::trace) cout << "Configuration::initFromFile(): Reading machine configuration...\n";

	// Create a FileReader to read lines from the configuration file.
	FileReader configReader(filename);

	// Eat the file's first (magic-cookie) line.  We'll ignore its details.
	MagicCookie	config_cookie(configReader);
	if (!config_cookie.valid()) {
		cout << "Configuration::initFromFile(): Error! The file's magic cookie is not valid! Ignoring...\n";
	} else {
		if (ns_debug::trace) cout << "Configuration::initFromFile(): Finished eating the magic cookie.  Not bothering to digest it though.\n";
	}

	// Get the next non-comment line, which should be of the form "bits: <n>"
	if (ns_debug::trace) cout << "Configuration::initFromFile(): Now looking for the number of qubits...\n";
	auto_ptr<string>	nBitsLine = configReader.getLine_ignoreComments();
	if (ns_debug::trace) cout << "Configuration::initFromFile(): Got the line: [" << *nBitsLine << "]...\n";

	istringstream nBitsStream(*nBitsLine);	// Make an input string-stream out of it.

	// Some bins to hold the parsed contents of the line.
	string			ignored; 

	// Parse the line into the word "bits:" and the number n (space delimited).
	nBitsStream >> ignored >> this->nbits;
	if (ns_debug::trace) cout << "Configuration::initFromFile(): Parsed it into the ignored word [" << ignored << "] and the number " << nbits << ".\n";

	// Now, just repeatedly get information about various named bits and bitarrays, until we run out of lines.
	do {
		auto_ptr<string> nextLine = configReader.getLine_ignoreComments();
		if (nextLine.get() == NULL) {
			if (ns_debug::trace) cout << "Configuration::initFromFile(): Reached end of file.\n";
			break;
		}
		if (ns_debug::trace) cout << "Configuration::initFromFile(): Got the line [" << *nextLine << "].\n";
		istringstream lineStream(*nextLine);

		string ignored_word, important_word, ignore_at_sign;
		lineStream >> ignored_word >> important_word;

		if (important_word.compare("bit:") == 0) {	// This line defines a named bit.

			nNamedBits++;
			namedBits.resize(nNamedBits);
			NamedBit &thisNamedBit = namedBits.at(nNamedBits-1);
			lineStream >> thisNamedBit.name >> ignore_at_sign >> thisNamedBit.address;

		} else if (important_word.compare("bitarray:") == 0) {

			nNamedBitArrays++;
			namedBitArrays.resize(nNamedBitArrays);
			NamedBitArray &thisNamedBitArray = namedBitArrays.at(nNamedBitArrays-1);
			
			// Bitarray declaration format is:  "name[size] @ baseaddr"
			// Read name of bit array up to [, then size of bit array, then ], then @, then base address.
			string toParse;

			getline(lineStream, thisNamedBitArray.name, '[');
			thisNamedBitArray.name = thisNamedBitArray.name.substr(1);
			getline(lineStream, toParse, ']');

			istringstream sLength(toParse);
			sLength >> thisNamedBitArray.length;

			lineStream >> ignore_at_sign >> thisNamedBitArray.baseAddress;
			//getline(lineStream, thisNamedBitArray.baseAddress, 

			cout << "\n";

		} // End "else if" case looking for "bitarray:"
	} while (!configReader.eof());		// end of "do" loop
}

// Allows initializing a NamedBit object from the given input stream.
istream& operator>>(istream& is, NamedBit& nb) {
	// STILL NEEDS TO BE WRITTEN
	return is;
}

NamedBit*		Configuration::_lookup_named_bit(const string& bitName){
	
	for (qubit_index_t		named_bit_index = 0;
							named_bit_index < namedBits.size();
							named_bit_index++						)
	{
		if (namedBits.at(named_bit_index).name == bitName) {
			return &namedBits.at(named_bit_index);
		}
	}
	cout << "Configuration::__lookup_named_bit(): Error: Unable to locate a bit named " << bitName << ".\n";
	return (NamedBit*)NULL;
}

NamedBitArray*	Configuration::_lookup_named_bitarray(const string& bitArrName){

	if (ns_debug::trace) cout << "Configuration::__lookup_named_bitarray(): Looking up the bit-array named " << bitArrName << ".\n";

	for (unsigned short		named_bitarray_index = 0;
							named_bitarray_index < namedBitArrays.size();
							named_bitarray_index++							)
	{
		if (ns_debug::trace) cout << "Configuration::__lookup_named_bitarray(): Comparing it with NamedBitArray " << namedBitArrays[named_bitarray_index] << ".\n";
		if (namedBitArrays.at(named_bitarray_index).name == bitArrName) {
			if (ns_debug::trace) cout << "Configuration::__lookup_named_bitarray(): The names match!  Returning that NamedBitArray.\n";
			return &namedBitArrays.at(named_bitarray_index);
		}
	}
	if (ns_debug::trace) cout << "Configuration::__lookup_named_bitarray(): Error: Unable to locate a bitarray named " << bitArrName << ".\n";
	return (NamedBitArray*)NULL;
}

// Looks up a given bit in the configuration by name.
// The name may have an array qualifier, e.g., a[0].
qubit_index_t	Configuration::lookup_byName(const string& bitName){
	if (ns_debug::trace) cout << "Configuration::lookup_byName(): Looking up the address of the bit named " << bitName << ".\n";
	size_t		lbrace_pos;		// Position of left brace in the string, if any.
	// First determine if we are looking up an array element.
	if ((lbrace_pos = bitName.find_first_of('[')) != string::npos) {
		if (ns_debug::trace) cout << "Configuration::lookup_byName(): Noticed there is a left-brace at position " << lbrace_pos << ".\n";
		// Get the name of the array.
		string			arrName		= bitName.substr(0, lbrace_pos);
		if (ns_debug::trace) cout << "Configuration::lookup_byName(): The array name is " << arrName << ".\n";
		// Lookup the NamedBitArray object for it.
		NamedBitArray*	bitArray	= _lookup_named_bitarray(arrName);
		if (ns_debug::trace) cout << "Configuration::lookup_byName(): This is the NamedBitArray " << *bitArray << ".\n";
		// Get the position of the right-brace after the left-brace.
		size_t			rbrace_pos	= bitName.find_first_of(']', lbrace_pos);
		if (ns_debug::trace) cout << "Configuration::lookup_byName(): Found the right-brace at position " << rbrace_pos << ".\n";
		// Get the offset within the array.
		string			arrOffsetString = bitName.substr(lbrace_pos+1, rbrace_pos - lbrace_pos - 1);
		if (ns_debug::trace) cout << "Configuration::lookup_byName(): The array offset string is \"" << arrOffsetString << "\".\n";
		istringstream	arrOffsetStream(arrOffsetString);
		qubit_index_t	arrOffset;
		arrOffsetStream >> arrOffset;
		if (ns_debug::trace) cout << "Configuration::lookup_byName(): The array offset value is " << arrOffset << ".\n";
		// Calculate & return the qubit index within the array.
		qubit_index_t	bit_location = bitArray->addressOfBit(arrOffset);
		if (ns_debug::trace) cout << "Configuration::lookup_byName(): The address of the selected qubit is " << bit_location << ".\n";
		return bit_location;
	} else {
		// Lookup the NamedBit object for this bit.
		NamedBit*		namedBit	= _lookup_named_bit(bitName);
		// Lookup & return its bit address.
		return			namedBit->address;
	}
}
