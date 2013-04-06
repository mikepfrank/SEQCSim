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

// Operator.h - Defines a class for quantum logic operators ("gate types").

#pragma once					// Don't need to include this header file more than once.

#include <iostream>				// For ostream
#include <string>				// We use the string class for the operator's identifier.
#include "index_types.h"		// Typedefs for various index types incldg. operator_index_t, operand_index_t
#include "FileReader.h"			// Defines class FileReader, for line-oriented file input.
#include "Matrix.h"				// Defines class Matrix, for reading & storing complex matrices.

using namespace std;			// For ostream

class Operator {
public:						// For simplicity, for now, let all fields be public.

	operator_index_t	id;				// Numerical unique ID for this operator.
	string				name;			// Alphanumeric identifier; the operator's name.
	operand_index_t		arity;			// The arity (number of bits) of this operator.
	Matrix				U;	 	        // Two-dimensional array (indexed by row, column) of complex numbers.
										//   Note there are 2^arity rows and the same # of columns.	 
										//   U must be unitary, but we do no error checking.
	
	// Public member functions.
public:
	// This handy procedure initializes the operator from a text file using a FileReader object.
	void initializeFrom(FileReader& r);
};

// For displaying an operator.
ostream& operator<<(ostream& os, Operator& opr);
