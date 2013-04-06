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

#include <iostream>
#include <fstream>
#include <sstream>		// istringstream
#include "Operator.h"
#include "debug.h"

using namespace std;

void Operator::initializeFrom(FileReader& r) {
	if (ns_debug::trace) cout << "Operator::initializeFrom(): Initializing operator #" << id << " from file...\n";

	// Get the line that we expect to say "name: <str>"
	auto_ptr<string> nameLine = r.getLine_ignoreComments();
	if (ns_debug::trace) cout << "Operator::initializeFrom(): We expect that line contains the operator name.\n";

	// Make it into a string-stream so we can parse it with >>
	istringstream nameStream(*nameLine);

	string ignore;  // For holding "name:", to ignore.
	
	// Parse the string into "name:" and the actual name string, delimited by space.
	nameStream >> ignore >> this->name;
	if (ns_debug::trace) cout << "Operator::initializeFrom(): We parsed it into the ignored string [" << ignore << "] and the name string [" << name << "].\n";

//	if (name == "H") {
//		cout << "Operator::initializeFrom(): This is the Hadamard operator! Let's turn on tracing for it.\n";
//		ns_debug::trace = true;
//	} else {
//		ns_debug::trace = false;
//	}
	
	// Get the line that we expect to say "size: <nbits>" (or "arity: <nbits>")
	auto_ptr<string> sizeLine = r.getLine_ignoreComments();
	if (ns_debug::trace) cout << "Operator::initializeFrom(): We expect that line contains the operator arity.\n";

    // Make it into a string-stream so we can parse it with >>
	istringstream sizeStream(*sizeLine);

	// Parse the string into "size:" and <nbits>, ignore anything else on the line (like "bits")

	int nonchar_arity;			     // This temp var is needed so the arity isn't read as a character.
	sizeStream >> ignore >> nonchar_arity;		// Skip "size:" and get the NUMERIC value of the arity.
	arity = (operand_index_t)nonchar_arity;		// Now we can cast it into the char-sized storage slot.

	if (ns_debug::trace) cout << "Operator::initializeFrom(): We parsed it into the ignored string [" << ignore << "] and the arity value " << (int)arity << ".\n";

	// Calculate the rank of the unitary matrix defining this operator.  (It is 2^arity.)
	size_t rank = 1<<arity;
	//size_t rank = 1;  int a = arity;
	//cout << "Just to confirm, the arity is " << a << ".\n";
	//for (int i = 0; i < a; i++) {
	//	rank = rank+rank;
	//}
	if (ns_debug::trace) cout << "Operator::initializeFrom(): Given that arity, the expected rank of the defining unitary matrix is " << rank << ".\n";
	
	// Size this operator's defining matrix to be a square matrix of the appropriate rank.
	U.set_rank(rank);
	if (ns_debug::trace) cout << "Operator::initializeFrom(): We have now set the rank of the current op's matrix data structure to " << U.rank() << ".\n";

	// Ask this matrix to please initialize itself using our FileReader.
	if (ns_debug::trace) cout << "Operator::initializeFrom(): Matrix, please initialize yourself from the file.\n";
	U.initializeFrom(r);

	if (ns_debug::trace) cout << "Operator::initializeFrom(): We have finished initializing operator #" << id << " from the file!\n";
}

ostream& operator<<(ostream& os, Operator& opr){
	os << opr.id << "`" << opr.name << "'[" << (int)opr.arity << "]";
	// We don't bother printing the full matrix because that would be too verbose.
	return os;
}