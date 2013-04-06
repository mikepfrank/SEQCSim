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

// This class is for a matrix of complex numbers.  It has special features to allow us
// to very quickly scan through just the nonzero elements of a given row or column.

#pragma once

#include <vector>					// STL vector template, used for accessing lists of rows/columns.
#include "Complex.h"				// Our matrices are always implicitly complex-valued.
#include "FileReader.h"				// Defines FileReader class used when reading matrix data from a file.
#include "SmartComplexVector.h"		// Defines SmartComplexVector class.

class Matrix {
public:
	// Our primary matrix representation is row-based.
	vector<SmartComplexVector> rows;		// Define a matrix as a (vertical) vector of row vectors.
	
	// The following, column-based representation is also used
	// (redundantly) for faster access to individual columns.
	vector<SmartComplexVector> cols;		// It can also be considered a (horizontal) vector of column vectors.
	
	// Private member functions.
private:
	void	initColumns(void);		// Initializes the internal column-based representation
									//		from the row-based representation already stored.
	
	// Public member functions.
public:
	Matrix(void){}							// Default no-arguments constructor: Do nothing.
	
	void	set_rank(size_t r);					// Change this matrix into a square matrix of rank r.
	size_t	rank(void) { return rows.size(); }  // Assuming this is a square matrix, return its rank.
	void	initializeFrom(FileReader& r);		// Initialize this matrix using the given FileReader.
	
	~Matrix(void);
};
