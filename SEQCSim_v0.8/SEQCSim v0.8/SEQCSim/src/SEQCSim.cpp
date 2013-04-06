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

#include <string>			// Needed for string class.
#include <iostream>
#include <fstream>			// For ifstream
#include <sstream>			// Needed for istringstream.
#include <random>			// uniform_real class (pseudo-random number generator)
#include "index_types.h"		// For operators_index_t etc.
#include "SEQCSim.h"				// Header file declaring the class we're defining.
#include "SmartComplexVector.h"		// Includes a redundant sparse representation for fast iteration over nonzero entries.
#include "debug.h"			// ns_debug::trace

using namespace std;

namespace ns_SEQCSim {
	static const string default_operators_filename		= "..\\data\\qoperators.txt";
	static const string default_config_filename			= "..\\data\\qconfig.txt";
	static const string default_opseq_filename			= "..\\data\\qopseq.txt";
	static const string default_input_filename			= "..\\data\\qinput.txt";
}

void SEQCSim::read_operators() {
	if (ns_debug::trace) cout << "SEQCSim::read_operators(): Reading operator list...\n";

	// Create a FileReader object to read lines from the operators file.

	FileReader		operatorsReader(ns_SEQCSim::default_operators_filename);
	
	// Eat the file's first (magic-cookie) line.  We'll ignore its details.

	MagicCookie		operators_cookie(operatorsReader);
	if (!operators_cookie.valid()) {
		cout << "SEQCSim::read_operators(): Error! The file's magic cookie is not valid!  Ignoring...\n";
	} else {
		if (ns_debug::trace) cout << "SEQCSim::read_operators(): Finished eating the magic cookie.  Not bothering to digest it.\n";
	}

	// Get the next non-comment line, which should be "operators: <n>"

	if (ns_debug::trace) cout << "SEQCSim::read_operators(): Now looking for the number of operators...\n";
	auto_ptr<string>	nOperatorsLine = operatorsReader.getLine_ignoreComments();
	if (ns_debug::trace) cout << "SEQCSim::read_operators(): Getting ready to scan the line: [" << *nOperatorsLine << "]...\n";

	istringstream istr(*nOperatorsLine);	// Make an input string-stream out of it.
	
	// Some bins to hold the parsed contents of the line.
	string				ignored; 
	operator_index_t	nOperators;

	// Parse the line into the word "operators:" and the number n (space delimited).
	istr >> ignored >> nOperators;
	if (ns_debug::trace) cout << "SEQCSim::read_operators(): Parsed it into the ignored word [" << ignored << "] and the number " << nOperators << ".\n";

	// Go ahead and resize the vector of operators to be the appropriate size.
	operators.resize(nOperators);
	if (ns_debug::trace) cout << "SEQCSim::read_operators(): We're now expecting to read " << operators.size() << " operators.\n";

	// OK, now we're going to expect that many operators to actually be defined.
	for (operator_index_t	op_index = 0;
		 op_index < nOperators;
		 op_index++) {

		if (ns_debug::trace) cout << "SEQCSim::read_operators(): Getting ready to read operator number " << op_index << "...\n";

		// Get the next non-comment line, expected to be "operator #: <i>"
		auto_ptr<string>	operatorIdxLine = operatorsReader.getLine_ignoreComments();
		if (ns_debug::trace) cout << "SEQCSim::read_operators(): We expect that it has the operator number, but we're ignoring it.\n";

		// But then we'll ignore the index number given in the file, and just use our
		// automatically generated sequence number (op_index) instead.

		Operator& cur_op = operators.at(op_index);
		cur_op.id = op_index;
		if (ns_debug::trace) cout << "SEQCSim::read_operators(): Our current operator number is just " << cur_op.id << ".\n";

		// Tell that (as yet uninitialized) operator object to initialize itself using the FileReader.
		cur_op.initializeFrom(operatorsReader);
	}
	
}

void SEQCSim::read_config() {
	// Tell the Configuration object to initialize itself from the default file.
	qc_config.initFromFile(ns_SEQCSim::default_config_filename);
}

void SEQCSim::read_opseq() {
	//bool pushed_trace = ns_debug::trace; ns_debug::trace=false;

	if (ns_debug::trace) cout << "SEQCSim::read_opseq(): Reading operations list...\n";

	// Create a FileReader object to read lines from the opseq file.

	FileReader		operationReader(ns_SEQCSim::default_opseq_filename);
	
	// Eat the file's first (magic-cookie) line.  We'll ignore its details.

	MagicCookie		operators_cookie(operationReader);
	if (!operators_cookie.valid()) {
		if (ns_debug::trace) cout << "SEQCSim::read_opseq(): Error! The file's magic cookie is not valid!\n";
	} else {
		if (ns_debug::trace) cout << "SEQCSim::read_opseq(): Finished eating the magic cookie.  Not bothering to digest it.\n";
	}

	// Get the next non-comment line, which should be "operators: <n>"

	if (ns_debug::trace) cout << "SEQCSim::read_opseq(): Now looking for the number of operations...\n";
	auto_ptr<string>	nOperationsLine = operationReader.getLine_ignoreComments();
	if (ns_debug::trace) cout << "SEQCSim::read_opseq(): Getting ready to scan the line: [" << *nOperationsLine << "]...\n";

	istringstream istr(*nOperationsLine);	// Make an input string-stream out of it.
	
	// Some bins to hold the parsed contents of the line.
	string				ignored; 
	operation_index_t	nOperations;

	// Parse the line into the word "operators:" and the number n (space delimited).
	istr >> ignored >> nOperations;
	if (ns_debug::trace) cout << "SEQCSim::read_opseq(): Parsed it into the ignored word [" << ignored << "] and the number " << nOperations << ".\n";

	// Go ahead and resize the vector of operations to be the appropriate size.
	opn_seq.resize(nOperations);
	if (ns_debug::trace) cout << "SEQCSim::read_opseq(): We're now expecting to read " << opn_seq.size() << " operations.\n";

	// OK, now we're going to expect that many operations to actually be defined.
	for (operation_index_t	op_index = 0;
							op_index < nOperations;
							op_index++) 
	{

		if (ns_debug::trace) cout << "SEQCSim::read_opseq(): Getting ready to read operation number " << op_index << "...\n";

		// Get the next non-comment line, expected to be "operation #0: apply binary/unary operator <opName> to bits <bitName1>, <bitName2>"
		auto_ptr<string>	operationIdxLine = operationReader.getLine_ignoreComments();
		if (ns_debug::trace) cout << "SEQCSim::read_opseq(): Got the line: [" << *operationIdxLine << "].\n";
		
		if (ns_debug::trace) cout << "SEQCSim::read_opseq(): We expect that it has the operation number, but we're ignoring it.\n";

		istringstream istrOperationLine(*operationIdxLine);
		string ignore1, ignore2, ignore3, opType, ignore4, opName, ignore5, ignore6, bitName1, bitName2;
		Operation& cur_op = opn_seq.at(op_index);

		// Parse the operation line as follows:
		//                   Operation  #0:        apply      unary     operator   H         to         bits
		istrOperationLine >> ignore1 >> ignore2 >> ignore3 >> opType >> ignore4 >> opName >> ignore5 >> ignore6;
		
		// Get the name of the first bit.
		getline(istrOperationLine, bitName1, ',');
		bitName1 = bitName1.substr(1);

		if (ns_debug::trace) cout << "SEQCSim::read_opseq(): The first operand is " << bitName1 << ".\n";

		cur_op.operands.resize(1);	// Make sure the operation has room for at least one operand.
		
		// Set the first operand of the current operation to the address of the first bit.
		cur_op.operands[0] = qc_config.lookup_byName(bitName1);

		if (ns_debug::trace) cout << "SEQCSim::read_opseq(): The qubit address of the first operand is " << cur_op.operands[0] << ".\n";

		// If the operation type is binary,
		if(opType == "binary") {
			// Resize the current operation's operand list to be size 2.
			cur_op.operands.resize(2);
			
			// Read the name of the second operand bit.
			istrOperationLine >> bitName2;

			// Look up its bit address and store it as this operator's 2nd operand.
			cur_op.operands[1] = qc_config.lookup_byName(bitName2);

			if (ns_debug::trace) cout << "SEQCSim::read_opseq(): The qubit address of the second operand is " << cur_op.operands[1] << ".\n";
		}

		// Look up the ID number of this operation's operator from its name.
		cur_op.operator_id = operators.size();
		for(operator_index_t i = 0; i < operators.size(); i++)
		{
			if(operators[i].name == opName)
			{
				cur_op.operator_id = operators[i].id;
				if (ns_debug::trace) cout << "SEQCSim::read_opseq(): The operator ID #of the operator name is " << cur_op.operator_id << ".\n";
				break;
			}
		}
		if (cur_op.operator_id == operators.size()) {
			cout << "SEQCSim::read_opseq(): Error! Operator name \"" << opName << "\" is not defined.\n";
			exit(1);
		}

		if (ns_debug::trace) cout << "SEQCSim::read_opseq(): Our current operation number is just " << op_index << ".\n";

		// This next loop reverses the operand order for purposes of internal storage
		// because we are assuming that operand bits are given in big-endian order
		// where the most significant bit (with respect to matrix rank ordering) is 
		// first.  Thus, for example, in cNOT(a,b), a is the control bit, and is 
		// also the most-significant bit with respect to the ordering of matrix 
		// rows and columns, so that the operand matrix looks like this:
		//
		//    0  1  2  3 <- columns
		//   00 01 10 11
		// [(1, 0, 0, 0);   (row 0=00)
		//  (0, 1, 0, 0);   (row 1=01)
		//  (0, 0, 0, 1);   (row 2=10)
		//  (0, 0, 1, 0)]   (row 3=11)

		for (operand_index_t	early_opd_idx = 0;
								early_opd_idx < (cur_op.operands.size() >> 1);	// floor(size/2)
								early_opd_idx++) {

			operand_index_t		late_opd_idx = cur_op.operands.size() - 1 - early_opd_idx;

			qubit_index_t		early_opd = cur_op.operands.at(early_opd_idx);

			cur_op.operands.at(early_opd_idx) = cur_op.operands.at(late_opd_idx);
			cur_op.operands.at(late_opd_idx)  = early_opd;
		}							
	}
	//ns_debug::trace=pushed_trace;
}


void SEQCSim::read_input() {
	
	if (ns_debug::trace) cout << "SEQCSim::read_input(): ..\n";

	// Create a FileReader object to read lines from the input file.

	FileReader		inputReader(ns_SEQCSim::default_input_filename);
	
	// Eat the file's first (magic-cookie) line.  We'll ignore its details.

	MagicCookie		input_file_cookie(inputReader);
	if (!input_file_cookie.valid()) {
		cout << "SEQCSim::read_input(): Error! The file's magic cookie is not valid! Ignoring...\n";
	} else {
		if (ns_debug::trace) cout << "SEQCSim::read_input(): Finished eating the magic cookie.  Not bothering to digest it.\n";
	}

	auto_ptr<string>	contentLine;// = inputReader.getLine_ignoreComments();

	string			name, equal_sign;
	unsigned int	value;

	input_state.bits.resize(this->qc_config.nbits);

	do
	{
		contentLine = inputReader.getLine_ignoreComments();
		
		// If the fetched line is a NULL pointer, this means we're at the end of file - quit early.
		if (contentLine.get() == NULL) break;

		istringstream istr(*contentLine);	// Make an input string-stream out of it.
		istr >> name >> equal_sign >> value;
		
		for(unsigned int i = 0; i < this->qc_config.nNamedBits; i++)
		{
			//NamedBit &curentNamedBit = this->qc_config. namedBits.at(nNamedBits-1);
			if(this->qc_config.namedBits[i].name == name)
			{
				this->input_state[this->qc_config.namedBits[i].address] = (value>0?1:0) ;
				break;
			}
		}

		for(unsigned int i = 0; i < this->qc_config.nNamedBitArrays; i++)
		{
			if(this->qc_config.namedBitArrays[i].name == name)
			{
				for(unsigned int ii = 0; ii < this->qc_config.namedBitArrays[i].length; ii++)
				{
					unsigned int mask = 1 << ii;
					this->input_state[this->qc_config.namedBitArrays[i].baseAddress + ii] = ((value & mask) > 0 ? 1 : 0);
				}
				break;
			}
		}
	}while(!inputReader.eof());

	if (ns_debug::trace) {
		cout << "SEQCSim::read_input():  Finished reading the input state.  It is:\n\t"
			<< input_state << ".\n";
	}
}

// Returns TRUE iff the program_counter is already at the end of the quantum algorithm (operation sequence)
// to be simulated.
bool SEQCSim::done() {
	bool isDone = (program_counter >= opn_seq.size());
		// It should never be > anyway, but, just in case...
	if (isDone) {
		cout << "SEQCSim::done(): The PC value " << program_counter << 
			" is >= the number of operations " << opn_seq.size() << ".\n\tWe are done!\n";
	}
	return isDone;	
}

void SEQCSim::Bohm_step_forwards() {

	// Algorithm outline:
	//  -1. Make sure we're not already at the end of the program; if so exit.
	//   0. Figure out exactly what column vector of what operator matrix is indicated by the current state.
	//   1. If operation column is fully classical, just do it, inc program counter, and we're done.
	//   2. If operation column is single-valued (e.g. conditional phase shift), 
	//			just apply it to change our basis state and current amplitude, inc and done.
	//	 3. Otherwise, for each non-zero entry in the current matrix column, do:
	//	     3a. Recursively calculate the amplitude for the state corresponding to that entry,
	//              (unless it's the current state in which case we already know it).
	//		 3b. Accumulate it (multiplied by matrix element) into amplitude of resultant states.
	//   4. Calculate normalized probabilities of all possible output states.
	//   5. Choose one stochasically according to the probabilities.
	//	 6. Assign current state and amplitude based on that, & increment program counter.  Done.

	// === STEP -1. ===
	// Make sure we're not at the end of the given quantum algorithm already.
	if (done()) {
		cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Warning: Attempt to run off the end of the operation sequence.  Quantum algorithm is already complete!  Ignoring...\n";
		return;
	}

	if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The current program counter value (operation number) is " << program_counter << "\n";

	// === STEP 0. ===
	
	// Identify which operation in the sequence needs to be applied.
	Operation&		cur_opn = opn_seq.at(program_counter);

	if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The operation to be applied is " << cur_opn << "\n";
	
	// Identify which operator is to be applied in that particular operation.
	Operator&		cur_opr = operators.at(cur_opn.operator_id);
	
	if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The operator to be applied is " << cur_opr << "\n";

	// Get the arity of the current operator.
	operand_index_t	arity = cur_opr.arity;	// Number of operands of the current operator.

	if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The arity of that operator is " << (int)arity << "\n";

	// Identify the input index value to the current operation that's implied by the current state.
	// Do this by extracting from the current state the values of the qubits corresponding to the
	// operands of the current operation.

	size_t in_idx = current_state.extractBits(cur_opn.operands);

	if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The input index value is " << in_idx << ".\n";

	// At this point, the bit-vector representation of the input index should be fully calculated.
	// Now we can figure out what column of the operator matrix is indicated.

	SmartComplexVector& cur_col = cur_opr.U.cols.at(in_idx);
	
	if (ns_debug::trace) {
		cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The selected matrix column is: ";
		cur_col.putTo(cout);
		cout << ".\n";
	}
	
	// We're now ready to figure out exactly how best to apply the current operator.

	// Easiest case: Column vector has only 1 nonzero entry.  That means, the operator
	// matrix is diagonal (if appropriately ordered) in the sector corresponding to
	// the current state.  I.e., the output state and amplitude can be calculated from
	// the input state and amplitude and the single nonzero matrix element in this column.

	if (cur_col.isOnAxis()) {

		if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") This vector lies on an axis of the vector space.\n";

		// Get the index of the first nonzero element of the column vector, 
		// as a bit-vector with 1 bit per operand of the current operator.
		BitVector	out_idx_bv(arity);
		out_idx_bv = cur_col.idx_1st_nz();	

		if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The first nonzero vector element is at position " << (int)out_idx_bv << ".\n";

		// In the current state, set the qubits corresponding to the current operation's operands
		// to the bit-values corresponding to the unique output index in the current operator column.
		current_state.setBits(cur_opn.operands, out_idx_bv);

		if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The new state is now: " << current_state << ".\n";

		// If the current column isn't "classical" (0,1 elements only), then we also need to
		// update the amplitude by multiplying it by the nonzero matrix element in this column.
		if (!cur_col.isClassical()) {

			if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Nonclassical column - we also need to update state amplitude.\n";

			Complex		matrix_element = cur_col.first_nz_elem();

			if (ns_debug::trace) {
				cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The relevant matrix element is ";
				matrix_element.putTo(cout); cout << ".\n";
			}

			current_state.amp *= matrix_element;

			if (ns_debug::trace) {
				cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The new state amplitude is ";
				current_state.amp.putTo(cout); cout << ".\n";
			}
		}
	} else { // THE HARD PART.

		if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") This vector does NOT lie on an axis of the vector space.\n";

		// In this "else" branch, the current column has more than one nonzero element,
		// which means the operator matrix isn't diagonal in the sector corresponding to
		// the current state.  To calculate the next state, we will need to flesh out
		// an amplitude vector ranging over this state and all the rest of the "nearby"
		// states (the ones corresponding to the other nonzero elements in the same row,
		// with 0 in the other elements - to avoid double-counting contributions through
		// the diagonal sector of the matrix), then multiply this (truncated) vector by
		// the (non-diagonal sector of) the matrix, then stochasically select an output
		// state out of the normalized probability distribution induced on the possible
		// output states.

		//---------------------------------------------------------------------------------
		// To illustrate, here is an example of one of the "hard" cases for a 3-bit operator:
		//
		//          #0       #1       #2       #3       #4       #5       #6       #7                    
		//     _                                                                          _
		// #0 |      0        0     exp(iy)     0        0        0        0        0      |
		// #1 |      0        0        0        0        0      exp(iw)    0        0      |
		// #2 |      0      ( A )      0        0      ( B )      0        0        0      |
 		// #3 |      0        0        0      exp(iz)    0        0        0        0      |
		// #4 |      0        0        0        0        0        0     exp(iv)     0      |
		// #5 |   exp(ix)     0        0        0        0        0        0        0      |
		// #6 |      0      ( C )      0        0      ( D )      0        0        0      | 
		// #7 |_     0        0        0        0        0        0        0      exp(iu) _|
		//         _   _
		// Where  | A B | is unitary.  
		//        |_C D_|
		// 
		// The large matrix above is then also unitary in block-diagonal form, as can be
		// seen plainly if its rows and columns are appropriately reordered.  
		// 
		// As an example of the easy case, suppose the input state's operand bits are 
		// 101 (column 5), then we just change its operand bits to 001 (row 1) and 
		// multiply its amplitude by exp(iw).
		//
		// For the hard case, suppose the input state's operand bits are 001 (column 1).
		// Then we have to collect amplitudes for both this case and case 100 (column 4)
		// (which has nonzero matrix elements in the same rows as column 1) multiply that
		// vector by the [(A B)(C D)] matrix, and select either case 010 (row 2) or case
		// 110 (row 6) with appropriate relative probabilities.
		//---------------------------------------------------------------------------------

		// First, figure out the rank of the submatrix corresponding to the current
		// sector of the (possibly block-diagonal) current operator.  This is the 
		// same as the number of nonzero elements in the current column.

		size_t				block_rank = cur_col.nNonzeros();
		
		if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The number of nonzero elements of the column is " << block_rank << ".\n";

		// Next, go ahead and collect a list of all the row indices in the current sector.
		// This is the same as the list of indices, within the current column, of all its 
		// nonzero elements.

		vector<size_t>&		block_row_indices = cur_col.indices_of_nz_elems();

		if (ns_debug::trace) {
			cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The indices of the nonzero elements are: ";
			for (size_t i=0; i<block_row_indices.size(); i++) {
				cout << block_row_indices[i];
				if (i<block_row_indices.size()-1) cout << ",";
			}
			cout << ".\n";
		}
		
		// Next, get a hold of the index of one of the rows in the current block
		// of the matrix.  This corresponds to the index, within the current column, of
		// the first non-zero matrix element in that column.
		
		size_t  exemplar_row_index = block_row_indices.at(0); 

		if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") An example row-index in the current block is: " << exemplar_row_index << ".\n";

		// Now, to get a list of all the columns that we need to care about, ask that
		// matrix row for the list of the indices of all its nonzero elements.

		vector<size_t>&  block_column_indices = cur_opr.U.rows.at(exemplar_row_index).indices_of_nz_elems();

		if (ns_debug::trace) {
			cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The number of columns that we need to care about is: " << block_column_indices.size() << ".\n";
			
			cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The columns that we need to care about are are: ";
			for (size_t i=0; i<block_column_indices.size(); i++) {
				cout << block_column_indices[i];
				if (i<block_column_indices.size()-1) cout << ",";
			}
			cout << ".\n";
		}

		// Now we're ready to assemble the input vector of amplitudes to the block.
		// We'll start with a null vector of appropriate dimensions (the block rank).

		vector<Complex>  input_amplitudes(block_rank);

		// Now we iterate through the column indices in the block.  For the one corresponding
		// to the current column, we already have its amplitude (that of the current state),
		// but for others, their amplitudes will have to be calculated separately.  That is
		// the most time-consuming step of the whole simulator.

		// Loop over col. indices relative to the current block, from 0 to just under the block's rank.

		for (size_t blockrel_col_idx = 0;  blockrel_col_idx < block_rank;  blockrel_col_idx++) {

			if (ns_debug::trace) {
				cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") We're now looking at column #" <<
					blockrel_col_idx << " within the current block.\n";

				cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") This column's index within the operator is "
					<< block_column_indices.at(blockrel_col_idx) << ".\n";
			}

			// If the current column within the block is the same as our input index, this is
			// the current column and we already have an amplitude for it.

			if (block_column_indices.at(blockrel_col_idx) == in_idx) {

				if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") That matches our current input index " << in_idx << ".\n";

				input_amplitudes.at(blockrel_col_idx) = current_state.amp;

				if (ns_debug::trace) {
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") So the input amplitude for block column #" <<
						blockrel_col_idx << " is ";
					input_amplitudes.at(blockrel_col_idx).putTo(cout); cout << ".\n";
				}

			} else {
				if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") This is a neighbor column.\n";
				// This column corresponds to some other, "neighbor" state.  We have to
				// calculate its amplitude.  This is the "hard" part of the whole algorithm.

				// Outline:
				// 1. Save current state's amplitude.
				// 2. Change operand bits in state to values at neighbor index.
				// 3. Call a function to recursively calculate state amplitude via a backwards
				//		depth-first traversal of the execution tree (Feynman path integral).
				// 4. Take resultant amplitude and put it in input_amplitudes vector.
				// 5. Restore operand bits of current state to what they were originally (in_idx).
				// 6. Restore the amplitude of the current state to what it was originally.

				// 1. Temporarily save the amplitude of the current state for later restoration.
				Complex pushed_cur_state_amp = current_state.amp;

				if (ns_debug::trace) {
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") We temporarily push the current state amplitude of ";
					pushed_cur_state_amp.putTo(cout); cout << " onto the stack.\n";
				}

				// 2. Temporarily change the current state's operand bits to the values at the neighbor index.
				BitVector nbr_idx_bv(arity);
				nbr_idx_bv = block_column_indices.at(blockrel_col_idx);	// Current neighbor's column index (operand configuration).
				
				if (ns_debug::trace) {
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The neighbor column's index is " << nbr_idx_bv << ".\n";
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Now we're about to move to the neighbor state.\n";
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Before moving, our state is: " << current_state << ".\n";
				}

				current_state.setBits(
					cur_opn.operands,		// Qubit addresses of the current op's operands.
					nbr_idx_bv					// Neighbor's index, as a bit vector.
				);

				if (ns_debug::trace) {
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") After moving to the neighbor, our state is: " << current_state << ".\n";
				}

				// (At this point, the "current_state" object now really describes the state of one of the
				// real current state's neighbors (out of those in the same block of the operator matrix).)

				// 3. Recursively calculate the amplitude of the neighbor state using path-integral approach.
				//    This is the compute-intensive kernel of the entire simulation algorithm.

				if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Now we're going to recursively recalculate the amplitude of that neighbor state.\n";
				recursion_depth = 1;  // About to go into 1st level of recursive algorithm
				Complex neighbor_amp = recalc_amplitude();	// Recalculate the amplitude of the neighbor state.
				recursion_depth = 0;  // We're out of the recursion.

				if (ns_debug::trace) {
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Got an amplitude of ";
					neighbor_amp.putTo(cout); cout << " for the neighbor state.\n";
				}

				// 4. Stick the resulting amplitude into the input_amplitudes vector we're building up.

				input_amplitudes.at(blockrel_col_idx) = neighbor_amp;

				// 5. Restore the current_state object's bits to what the current state really is.

				if (ns_debug::trace) {
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Now we're going to return to the current state.\n";
				}
				BitVector in_idx_bv(arity);
				in_idx_bv = in_idx;
				current_state.setBits(
					cur_opn.operands,	// Qubit addresses of the current op's operands.
					in_idx_bv	// Input index to current operation - values of operands in current state. (We extracted these earlier.)
				);
				if (ns_debug::trace) {
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") We're back at state: " << current_state << ".\n";
				}

				// 6. Restore the current_state object's amplitude to what it really is.
				current_state.amp = pushed_cur_state_amp;

				if (ns_debug::trace) {
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") We restored the amplitude to: "; 
					current_state.amp.putTo(cout); cout << ".\n";
				}
			} // end "else" case looking at neighbor state

			if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Finished considering block column #" << blockrel_col_idx << ".\n";

		} // end "for" loop iterating over columns of the current block
		if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Finished going through the block columns.\n";

		// Next, we have to extract the sub-matrix for the current block,
		// then multiply the input_amplitudes vector by this sub-matrix,
		// then select a new next state based on the resultant vector.

		// Rather than explicitly constructing the sub-matrix, we'll just
		// specialize the matrix multiplication operation to a form that
		// dynamically selects the appropriate subsets of rows and columns.
		// This will avoid the overhead of constructing and deconstructing
		// the sub-matrix.

		if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Creating an uninitialized vector of " << block_rank << " output amplitudes.\n";

		// This will be the resultant vector of amplitudes for the different possible outputs in the block.		
		vector<Complex>  output_amplitudes(block_rank);

		if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Now we're about to go through the rows for the block matrix multiply.\n";
		
		// Iterate through the rows of the block, calculating the sum-of-products for each.
		for (size_t  blockrel_row_i = 0;  blockrel_row_i < block_rank;  blockrel_row_i++) {

			if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") We're on block row #" << blockrel_row_i << ".\n";

			// Find the row's index within the entire operator U matrix.
			size_t  row_i = block_row_indices.at(blockrel_row_i);

			if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The operator row index is " << row_i << ".\n";

			output_amplitudes[blockrel_row_i] = 0;	// Initialize the output amplitude accumulator for this row to 0.
			
			if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Initialized that row's output amplitude to 0.\n";
			
			// Iterate through the columns of the block, adding each product into the accumulated sum.
			for (size_t  blockrel_col_j = 0;  blockrel_col_j < block_rank;  blockrel_col_j++) {

				if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Now looking at block column #" << blockrel_col_j << ".\n";

				// Find the column's index within the entire operator U matrix.
				size_t  col_j = block_column_indices.at(blockrel_col_j);

				if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The operator column is " << col_j << ".\n";

				// The amplitude of the current output state is incremented by the product of the
				// matrix element at the present row-column intersection, and the amplitude of the
				// current input state.

				if (ns_debug::trace) {
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The input amplitude for column " << blockrel_col_j << " is ";
					input_amplitudes[blockrel_col_j].putTo(cout); cout << ".\n";
		
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The current matrix element is ";
					cur_opr.U.rows.at(row_i)[col_j].putTo(cout); cout << ".\n";
				}

				// Here we compute the product of complex matrix element with a complex vector element.
				// The "*" is supposed to be invoking the operator*() method on class Complex.
				Complex product = static_cast<Complex>(cur_opr.U.rows[row_i][col_j]) 
					* input_amplitudes[blockrel_col_j];

				if (ns_debug::trace) {
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The product of matrix and vector elements is ";
					product.putTo(cout); cout << ".\n";
				}

				output_amplitudes[blockrel_row_i] += product;

				if (ns_debug::trace) {
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Now this row's output amplitude is ";
					output_amplitudes[blockrel_row_i].putTo(cout);  cout << ".\n";
				}

			} // end "for" loop over block columns
			if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Finished going through the columns for this row.\n";
		} // end "for" loop over block rows
		if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Finished going through the rows.\n";

		// At this point the vector of output amplitudes for the current block should be all calculated.
		// Now we need to calculate their (normalized) probabilities.

		if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Calculating the squared norms of the output amplitudes.\n";
		//
		// First, calculate the squared norms of the output amplitudes, which are proportional to
		// the conditional probabilities of the output states, and their sum ("partition function").

		vector<double> output_squared_norms(block_rank);
		double Z = 0;  // "Partition function" - sum of (unnormalized) norms of output amplitudes
		for (size_t  i = 0;  i < block_rank;  i++){
			if (ns_debug::trace) {
				cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") For element " << i << " the squared norm is ";
			}
			double squared_norm = output_amplitudes.at(i).squared_norm();  // Calculate the squared norm of the output amp.
			if (ns_debug::trace) cout << squared_norm;
			output_squared_norms.at(i) = squared_norm;	// Store it for later use.
			Z += squared_norm;	// Increment the partition function accumulator.
			if (ns_debug::trace) cout << " and the new Z value is " << Z << ".\n";
		}

		if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Calculating the normalized output amplitudes.\n";
		// Now calculate the normalized conditional probabilities (summing to 1.0).
		vector<double> output_probs(block_rank);
		for (size_t  i = 0;  i < block_rank;  i++){
			output_probs[i] = output_squared_norms[i]/Z;
			if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") For element " << i << " the output probability is " << output_probs[i] << ".\n";
		}

		// Finally!  This is it.  We are ready to pick the randomized next state.  We select
		// a random number between 0 and 1, iterate through the output states, and select the
		// one where the cumulative probability is greater than the random number that was chosen.
		// The probability of picking a given output is then exactly what we calculated it should be.
		// This is sometimes called the "routlette wheel" algorithm for selecting from a distribution.

		if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Now choosing a random output.  Initializing cumulative output probability to 0.\n";

		double cumulative_prob = 0;			// Accumulator for cumulative probability.
		double marker_position = marker_picker(prng_engine);
			// Picks a random real between 0 and 1 using our pseudo-random number generation engine.
		
		if (ns_debug::trace) cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Chose a random marker position of " << marker_position << ".\n";

		for (size_t  blockrel_output_i = 0;  blockrel_output_i < block_rank;  blockrel_output_i++) {
		
			if (ns_debug::trace) {
				cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Considering output #" << blockrel_output_i << ".\n";
				cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Its probability is " << output_probs[blockrel_output_i] << ".\n";
			}

			// Update the cumulative probability accumulated so far.
			cumulative_prob += output_probs[blockrel_output_i];

			if (ns_debug::trace) {
				cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Cumulative output probability is now " << cumulative_prob << ".\n";
			}

			// If the cumulative probability is greater than the marker position, this is our chosen new state!
			if (cumulative_prob > marker_position) {

				if (ns_debug::trace) {
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The cumulative prob. is greater than the marker position!\n";
				}
				
				// Figure out what the actual index of this output state is.
				size_t  out_idx = block_row_indices.at(blockrel_output_i);

				if (ns_debug::trace) {
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The actual operator row index of this output state is " << out_idx << ".\n";
				}

				// The below code is where FINALLY our record of the real "current" state is updated.

				// Now, modify the current state's operand bits to correspond to that output index.
				BitVector out_idx_bv(arity);
				out_idx_bv = out_idx;
				current_state.setBits(
					cur_opn.operands,	// Qubit addresses of the current op's operands.
					out_idx_bv		// Output state's column index (configuration of operand bits).
				);

				if (ns_debug::trace) {
					cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") The amplitude of this new state is "; 
					output_amplitudes.at(blockrel_output_i).putTo(cout);
					cout << ".\n";
				}

				// The current state's amplitude is the one we calculated for this resultant state earlier.
				current_state.amp = output_amplitudes.at(blockrel_output_i);

				break;		// Terminate the loop early - no reason to continue.
			} // End "if" statement selecting the output state.
		} // End "for" loop through the possible output states.

		// Once we get to this point, the state has been updated.

	} // end "else" case handling off-axis operator columns (with >1 nonzero entry)

	cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ")\n   The new current state is " << current_state << ".\n";

	if (ns_debug::trace) {
		cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Finished executing operation #" << program_counter << "!\n";
		cout << "SEQCSim::Bohm_step_forwards(): (tPC=" << top_PC << ") Now incrementing PC from " << program_counter << " to " << (program_counter+1) << ".\n";
	}

	program_counter ++;
	top_PC = program_counter;
}

static void showRD(int rd){
	int i;
	for(i=0;i<rd;i++)
		cout << "->";
}

// Recalculate the amplitude of the current_state using our optimized version of
// the Feynman path-integral approach.  This involves generating all of the possible
// immediate predecessor states, recursively calculating their amplitudes (using this
// same function), and multiplying the resultant vector of amplitudes by the 
// appropriate row of the operator matrix.  The base case is that, at time 0, the
// amplitude is that of the input state if the current state is the same as the input
// state, and 0 otherwise.

Complex SEQCSim::recalc_amplitude() {

	if (ns_debug::trace) {
		cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
		showRD(recursion_depth); cout << "(PC=" << program_counter << ") Entering the recalc_amplitude() method.\n";
	}

	// Outline:
	// 1. If the current program counter is 0, check whether the input state is the
	//      same as the current state.  If it is, return the input state's amplitude.
	//		Else return 0.
	// 2. PC isn't 0.  Examine the operation at PC-1.
	// 3. Identify the possible predecessor states that could have led to the present
	//      state as a result of that operation.
	// 4. For each possible predecessor, recursively calculate its amplitude using the
	//		same procedure.
	// 5. Add the predecessor amplitudes, weighted by the corresponding operator
	//		matrix elements.
	// 6. The result is the current state's amplitude.

	// 1. Base case.  If we're at the initial (number 0) step of the program, no operations
	// have been performed yet, so we can't go backwards in the operation sequence any 
	// farther.  So, just directly compare the current state to the initial state.  If 
	// they're the same, use the initial state amplitude; if they're different, use 0.

	if (program_counter == 0) {
		if (ns_debug::trace) {
			cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
			showRD(recursion_depth); 
			cout << "(PC=" << program_counter << ") The current PC value is 0.\n";
			if (current_state == input_state) {
				cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
				showRD(recursion_depth); 
				cout << "(PC=" << program_counter << ") The current state matches the input state.  Returning the input state amplitude ";
				input_state.amp.putTo(cout);
				cout <<  "\n";
			} else {
				cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
				showRD(recursion_depth); 
				cout << "(PC=" << program_counter << ") The current state doesn't match the input state.  Returning 0 amplitude.\n";
			}
		}
		return (current_state == input_state) ? input_state.amp : 0;
	}
	
	// Recursive case.  We have to generate the possible predecessor states by applying
	// the previous program operator in reverse, and looking for nonzero matrix elements
	// in the same row of the unitary operator as the row selected by the current state.
	// The column indices of those matrix elements are the possible predecessor states.
	// Then we recursively calculate their amplitudes, then sum them up (weighted by the
	// matrix elements) to determine the current state's amplitude.

	// 2. Nonzero program counter means there is a previous operation in the operation
	//      sequence.  Temporarily decrement the program counter to look it up.

	// Decrement program counter.  We must remember to reincrement it before returning.
	program_counter--;
	if (ns_debug::trace) {
		cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
		showRD(recursion_depth); cout << "(PC=" << program_counter << ") Going back to PC value " << program_counter << ".\n";
	}

	// Identify which operation in the sequence needs to be applied (in reverse) to find our predecessors.
	Operation&  cur_opn = opn_seq.at(program_counter);

	if (ns_debug::trace) {
		cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
		showRD(recursion_depth); 
		cout << "(PC=" << program_counter << ") We have to go back through operation " << cur_opn << ".\n";
	}
	
	// Identify which operator was supposed to have been applied in that particular operation.
	Operator&  cur_opr = operators.at(cur_opn.operator_id);

	if (ns_debug::trace) {
		cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
		showRD(recursion_depth); 
		cout << "(PC=" << program_counter << ") The operator applied in that operation was " << cur_opr << ".\n";
	}

	// Identify the output index value from the current operation that's implied by the current state.
	// Do this by extracting from the current state the values of the qubits corresponding to the
	// operands of the current operation.

	size_t					out_idx = current_state.extractBits(cur_opn.operands);
	
	if (ns_debug::trace) {
		cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
		showRD(recursion_depth); 
		cout << "(PC=" << program_counter << ") The output index after that operation was " << out_idx << ".\n";
	}

	// Now we can figure out what row of the operator matrix is indicated.

	SmartComplexVector&		cur_row = cur_opr.U.rows.at(out_idx);

	if (ns_debug::trace) {
		cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
		showRD(recursion_depth); 
		cout << "(PC=" << program_counter << ") That row of the operator matrix is " << cur_row << ".\n";
	}

	// First, figure out the rank of the submatrix corresponding to the current
	// sector of the (possibly block-diagonal) current operator.  This is the 
	// same as the number of nonzero elements in the current row.

	size_t					block_rank = cur_row.nNonzeros();

	if (ns_debug::trace) {
		cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
		showRD(recursion_depth); 
		cout << "(PC=" << program_counter << ") That row's from a block of rank " << block_rank << ".\n";
	}
		
	// Next, go ahead and collect a list of all the column indices in the current block.
	// This is the same as the list of indices, within the current row, of all its 
	// nonzero elements.

	vector<size_t>&			block_col_indices = cur_row.indices_of_nz_elems();
	
	if (ns_debug::trace) {
		cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
		showRD(recursion_depth); 
		cout << "(PC=" << program_counter << ") The column indices in that block are: ";
		for (size_t i = 0; i < block_col_indices.size(); i++) {
			cout << block_col_indices[i];
			if (i < block_col_indices.size()-1) cout << ",";
		} 
		cout << ".\n";
	}

	// Next, we're going to loop through all those nonzero matrix elements, accumulating
	// predecessor amplitude through each of them.  Initialize an amplitude accumulator to 0.

	Complex					amp_accum = 0;

	if (ns_debug::trace) {
		cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
		showRD(recursion_depth); 
		cout << "(PC=" << program_counter << ") Initialized accumulated amplitude from predecessors to ";
		amp_accum.putTo(cout); cout << ".\n";
	}

	// Loop through the columns of the current block...
	for (size_t blockrel_col_idx = 0;  blockrel_col_idx < block_rank;  blockrel_col_idx++) {

		if (ns_debug::trace) {
			cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
			showRD(recursion_depth); 
			cout << "(PC=" << program_counter << ") Looking at block column #" << blockrel_col_idx << ".\n";
		}
		
		// Look up the column index, this will give the values of that predecessor's operand bits.
		size_t			pred_idx = block_col_indices.at(blockrel_col_idx);

		if (ns_debug::trace) {
			cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
			showRD(recursion_depth); 
			cout << "(PC=" << program_counter << ") The predecessor operator column index is " << pred_idx << ".\n";
		}

		if (ns_debug::trace) {
			cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
			showRD(recursion_depth); 
			cout << "(PC=" << program_counter << ") We're about to back up the current state from " << current_state << ".\n";
		}

		// Temporarily change the current state's operand bits to the values for the predecessor.
		BitVector		pred_idx_bv(cur_opr.arity);
		pred_idx_bv = pred_idx;
		current_state.setBits(
					cur_opn.operands,	// Qubit addresses of the current op's operands.
					pred_idx_bv	// Current predecessor's column index (operand configuration).
		);

		if (ns_debug::trace) {
			cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
			showRD(recursion_depth); 
			cout << "(PC=" << program_counter << ") We backed up the current state to " << current_state << ".\n";
		}

		if (ns_debug::trace) {
			cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
			showRD(recursion_depth); 
			cout << "(PC=" << program_counter << ") Now we're going to recursively recalculate that state's amplitude.\n";
		}

		// Calculate the predecessor state's amplitude, by using this same procedure recursively.
		recursion_depth ++; // to aid debugging
		Complex			pred_amp = recalc_amplitude();
		recursion_depth --; // to aid debugging

		if (ns_debug::trace) {
			cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
			showRD(recursion_depth); 
			cout << "(PC=" << program_counter << ") The recursively recalculated amplitude for the current state " << current_state << " is ";
			pred_amp.putTo(cout); cout << ".\n";
		}
		
		// Add the predecessor state's amplitude, weighted by its matrix element, into the
		// accumulated amplitude.

		if (ns_debug::trace) {
			cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
			showRD(recursion_depth); 
			cout << "(PC=" << program_counter << ") The corresponding operator matrix element is "; 
			cur_row[pred_idx].putTo(cout); 
			cout << ".\n";
		}

		// Compute product of predecessor amplitude and corresponding matrix element.
		// The "*" here is supposed to be invoking the operator*(Complex) method on class Complex.
		Complex product = pred_amp * Complex(cur_row[pred_idx]);
		//Complex product = pred_amp.operator *(Complex(cur_row[pred_idx]));	// Use this form to suppress automatic conversions
		
		if (ns_debug::trace) {
			cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
			showRD(recursion_depth); 
			cout << "(PC=" << program_counter << ") The matrix-vector element product is "; 
			product.putTo(cout); cout << ".\n";
		}

		amp_accum.operator +=(product);

		if (ns_debug::trace) {
			cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
			showRD(recursion_depth); 
			cout << "(PC=" << program_counter << ") The accumulated amplitude is ";
			amp_accum.putTo(cout); cout << ".\n";
		}
	}

	if (ns_debug::trace) {
		cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
		showRD(recursion_depth); 
		cout << "(PC=" << program_counter << ") We're about to restore the state " << current_state << " to the successor state.\n";
	}

	// Restore the current state's operand bits to the values at the previously-saved output index.
	BitVector out_idx_bv(cur_opr.arity);
	out_idx_bv = out_idx;
	current_state.setBits(
			cur_opn.operands,	// Qubit addresses of the current op's operands.
			out_idx_bv			// Output index (operand bit values from the orig. current state).
	);

	// Set the current state's amplitude to the accumulated amplitude from the precedessors.
	current_state.amp = amp_accum;

	if (ns_debug::trace) {
		cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; showRD(recursion_depth); cout << "(PC=" << program_counter << ") The successor state (with recalculated amplitude) has been restored: " << current_state << ".\n";
		cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; showRD(recursion_depth); cout << "(PC=" << program_counter << ") Now re-incrementing the PC from " << program_counter << " to " << program_counter+1 << ".\n";
	}

	// Re-increment the program counter to restore it to the value that it had on procedure entry.
	program_counter++;

	if (ns_debug::trace) {
		cout << "SEQCSim::recalc_amplitude():   (tPC=" << top_PC << ") "; 
		showRD(recursion_depth); 
		cout << "(PC=" << program_counter << ") Returning the new current-state amplitude "; 
		current_state.amp.putTo(cout); cout << ".\n";
	}

	// Return the amplitude of the current state (not strictly needed, since it is stored in this object too).
	return		current_state.amp;
}

SEQCSim::SEQCSim(void)
{
	if (ns_debug::trace) cout << "SEQCSim::SEQCSim(): Constructing simulator object...\n";

	// Construct the pseudo-random number generator.
	marker_picker = tr1::uniform_real<double>::uniform_real(0.0,1.0);

	read_operators();		// Read the definitions of the quantum operators (gate types) we'll be using.
	read_config();			// Read the configuration file
	read_opseq();			// (This routine still needs to be written.)
	read_input();			// (This routine still needs to be written.)
}

// Runs the entire quantum algorithm (starting from the beginning).

void SEQCSim::run(void)
{
	if (ns_debug::trace) cout << "SEQCSim::run(): Resetting virtual quantum computer to prep it for running...\n";

	// Reset the program counter to zero.

	program_counter = 0;
	
	// Initialize some debugging variables.
	top_PC = 0;		recursion_depth = 0;
	
	// Set the machine's current state equal to its input state.

	current_state = input_state;

	cout << "SEQCSim::run(): Initial state is " << current_state << ".\n";

	// Until the program counter runs off the end of the circuit,
	// take us forward through the program, one step at a time,
	// using Bohm's interpretation (stochastic Monte Carlo sim).

	if (ns_debug::trace) cout << "SEQCSim::run(): About to begin main loop iterating through quantum algorithm...\n";

	// Until we reach the end of the program,
	while (!done()) {
		if (ns_debug::trace) cout << "SEQCSim::run():   We're not done yet, so let's take a step forwards...\n";
		// Take a single randomized step forwards through the quantum
		// algorithm.  NOTE: The method used here gets exponentially
		// slower as we get farther and farther into the program.
		Bohm_step_forwards();
	}

	if (ns_debug::trace) cout << "SEQCSim::run(): Finished running the virtual quantum computer.\n";

	// At this point, current_state contains the final "measured" 
	// (i.e. fully classical) state of the quantum computer, and
	// amp contains the amplitude to get there from the initial
	// state.  (If the simulator algorithm is correct, the 
	// probability of getting to any given final state should
	// be the squared norm of amp.)
}

// Destructor.  Does nothing right now.  Really it should call deconstructors on
// all our data members, but at this point the program is finished anyway so we don't
// really care about memory leaks at this point.

SEQCSim::~SEQCSim(void)
{
}
