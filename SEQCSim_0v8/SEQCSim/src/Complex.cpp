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

// Complex.cpp - Implements a simple complex-number class with the features we need.

#include <istream>		// istream class, used in getFrom()
#include "Complex.h"
#include "debug.h"

using namespace std;

// getFrom() method - Reads into this complex number from the input stream.
// The input is expected to be formatted as "(R + i*I)", where R and I are readable as doubles.
// The current version of this code assumes the input is correctly formatted and does no error-checking.

void Complex::getFrom(istream &is){
	char junk;
	do {
		junk = is.get();	// Get the first character (expected to be '(') from stream, and ignore it.
	} while (junk != '(');
	if (ns_debug::trace) cout << "Complex::getFrom(): The open-paren character is: '" << junk << "'.\n";

	is >> this->R;		// Get the real part of this complex number from the stream.
	if (ns_debug::trace) cout << "Complex::getFrom(): The real part is: " << R << ".\n";

	do {
		junk = is.get();
	} while (junk != '+');
	if (ns_debug::trace) cout << "Complex::getFrom(): The plus-sign character is: '" << junk << "'.\n";
	
	do {
		junk = is.get();
	} while (junk != 'i' && junk != 'I');
	if (ns_debug::trace) cout << "Complex::getFrom(): The letter-i character is: '" << junk << "'.\n";
	
	do {
		junk = is.get();
	} while (junk != '*' && junk != '.');
	if (ns_debug::trace) cout << "Complex::getFrom(): The multiplication character is: '" << junk << "'.\n";
	
	is >> this->I;		// Get the imaginary part of this complex number from the stream.
	if (ns_debug::trace) cout << "Complex::getFrom(): The imaginary part is: " << I << ".\n";
	
	do {
		junk = is.get();	// Get the first character (expected to be '(') from stream, and ignore it.
	} while (junk != ')');
	if (ns_debug::trace) cout << "Complex::getFrom(): The close-paren character is: '" << junk << "'.\n";
}

// putTo() method - Prints the complex number to the given output stream, in the format (R + i*I).

void Complex::putTo(ostream &os) {
	os << "(" << R << " + i*" << I << ")";
}

ostream& operator>>(ostream& os, Complex& c) {
	c.putTo(os);
	return os;
}