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

// Operation.h - Defines a class for quantum logic operations ("gate instances").

#pragma once

#include <iostream>				// ostream class, for operator<< overloading
#include <vector>				// STL vector<> template
#include "index_types.h"		// Typedefs for operation_index_t, qubit_index_t

using namespace std;

class Operation {
	public:
		operation_index_t			operator_id;	// Unique ID of the quantum logic operator to be applied.
		vector<qubit_index_t>		operands;		// Indices of the qubits to which the operator is to be applied.
													//   These are stored in operator order.
};

// Overloaded << operator for printing Operation objects to ostreams.
ostream& operator<<(ostream& os, Operation& opn);

