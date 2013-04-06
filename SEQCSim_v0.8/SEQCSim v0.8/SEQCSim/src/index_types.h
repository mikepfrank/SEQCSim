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

// index_types.h - Header file that declares typedefs for various types of indices to be used in the simulator.

#pragma once

// Define some integer numeric types of (supposedly) known sizes.

typedef		unsigned char		u8_t;			// Assume an unsigned char is 8 bits (true on most platforms).
typedef		unsigned short		u16_t;			// Assume an unsigned short is 16 bits (true on most platforms).
typedef		unsigned int		u32_t;			// Assume an unsigned int is 32 bits (true on most platforms).

// operand_index_t - Type for an index of an operand of a quantum logic operator.

typedef		u8_t	operand_index_t;		// Can't have more than 256 operands for a single operator.


// operator_index_t - Type for an index of a quantum logic operator in our library of available operators.

typedef		u16_t	operator_index_t;		// Can't have more than 65,536 different quantum logic operators.


// operation_index_t - Type for an index of a quantum logic operation (operator application) in a sequence of such.

typedef		u16_t	operation_index_t;		// Can't have more than 65,536 quantum logic operations making up a given circuit.


// qubit_index_t - Type for an index of a qubit within our quantum computer.

typedef		size_t	qubit_index_t;			// Limited by available memory (number of bytes that can be pointed to).
