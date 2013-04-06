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

#include <sstream>		// istringstream
#include "Matrix.h"
#include "debug.h"

// Initialize the matrix's redundant column representation based on its rows.
void Matrix::initColumns(void) {
	
	// Set the number of columns equal to the width of the first row.
	cols.resize(rows.at(0).size());
	for(size_t  col_i = 0;  col_i < cols.size();  col_i++){
		// The height of each column must be equal to the number of rows.
		cols.at(col_i).resize(rows.size());
	}

	// Copy all the element data from the rows into the columns.
	for(size_t  row_i = 0;  row_i < rows.size();  row_i++) {
		for(size_t  col_j = 0;  col_j < cols.size();  col_j++) {
			cols.at(col_j)[row_i] = rows.at(row_i)[col_j];
			if (ns_debug::trace) {
				cout << "Matrix::initColumns(): Column " << col_j << ", element " << row_i << " is ";
				cols.at(col_j)[row_i].putTo(cout);
				cout << ".\n";
			}
		}
	}
}

// Change this matrix to a square matrix of rank r.

void Matrix::set_rank(size_t r) {
	rows.resize(r);				// Change the number of rows to r.
	for(size_t i=0; i<r; i++){		// For each row i,
		rows.at(i).resize(r);	//     change the width of row i to r.
	}
}

// Initialize the matrix's contents using the given file reader.

void Matrix::initializeFrom(FileReader& r){
	if (ns_debug::trace) cout << "Matrix::initializeFrom(): Reading a rank-" << rank() << " matrix.\n";
	
	// Consume and ignore the "matrix:" line.
	auto_ptr<string> ignore = r.getLine_ignoreComments();
	if (ns_debug::trace) cout << "Matrix::initializeFrom(): Ignoring line: [" << *ignore << "].\n";

	// Read in the lines.
	for(size_t i=0; i<this->rank(); i++){
		if (ns_debug::trace) cout << "Matrix::initializeFrom(): About to read row #" << i << "...\n";
		
		// Get the next line as a string.
		auto_ptr<string> rowString = r.getLine_ignoreComments();
		if (ns_debug::trace) cout << "Matrix::initializeFrom(): The row text is: [" << *rowString << "].\n";
		
		// Convert it to a stream so we can use >> on it.
		istringstream rowStream(*rowString);
		
		// Get the complex numbers for each entry in the row.
		for(size_t j=0; j<rank(); j++){
			if (ns_debug::trace) cout << "Matrix::initializeFrom(): About to read entry #" << j << " on row #" << i << ".\n";
			rows.at(i)[j].getFrom(rowStream);
			
			if (ns_debug::trace) {
				cout << "Matrix::initializeFrom(): Read the complex number: ";
				rows.at(i)[j].putTo(cout);
				cout << ".\n";
			}
		}

		if (ns_debug::trace) {
			cout << "Matrix::initializeFrom(): row #" << i << " has " << rows[i].indices_of_nz_elems().size() << " nonzero elements.\n";
			cout << "Matrix::initializeFrom(): The nonzero elements in row #" << i << " have the following indices: ";
			for(size_t j=0; j<rows[i].indices_of_nz_elems().size(); j++){
				cout << rows[i].indices_of_nz_elems().at(j);
				if (j<rows[i].indices_of_nz_elems().size()-1) cout<<",";
			}
			cout << ".\n";
		}
	}

	// Initialize the column representation.
	initColumns();
}

Matrix::~Matrix(void)
{
}
