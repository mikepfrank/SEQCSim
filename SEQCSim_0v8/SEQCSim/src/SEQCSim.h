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

// Main class for the Space-Efficient Quantum Computer Simulator.

#pragma once

#include <vector>			// We're using STL vectors instead of plain C++ arrays, for safety & flexibility.
#include <random>			// For tr1::uniform_real.  TR1 (Tech. Report 1) is a forthcoming extension to the C++ standard.
#include "Operator.h"		// Defines Operator class for quantum logic operators (gate types).
#include "Configuration.h"	// Defines Configuration class for general configuration of quantum computer.
#include "Operation.h"		// Defines Operation class, for quantum logic operations (gate instances).
#include "State.h"			// Defines State class for computational basis states.


// Create a specialization of the uniform_real distribution class which we'll use.
typedef  std::tr1::uniform_real<double>  uniform_double;

// Objects of the SEQCSim class hold all the information needed to simulate the execution
// of a given quantum algorithm.

class SEQCSim {
	// The following data members remain constant once the machine has been initialized.

	vector<Operator>	operators;			// List of the available quantum logic operators.
	Configuration		qc_config;			// General configuration of the quantum computer.
	vector<Operation>	opn_seq;			// Sequence of quantum operators to be executed (quantum circuit, quantum algorithm).
	State				input_state;		// The quantum computer is initialized in this computational basis state.

	// These data members are dynamically modified in the course of running the simulation.

	operation_index_t	program_counter;	// Which operation in the quantum algorithm are we currently executing (or about to execute)?
	
	State				current_state;		// What is the current basis state of the quantum computer?
		// (NOTE: This is a classical, not a quantum state, but it includes a component amplitude.)

		// NOTE: The above item only makes sense for a simulator like ours in which we stochastically
		// simulate a trajectory through basis states, rather then evolving the entire state vector
		// (wavefunction).  This is the entire point of our simulator, since it is more space efficient.

	// Private data members.
private:
	uniform_double  marker_picker;	// Pseudo-random number generator for doubles.
	tr1::mt19937	prng_engine;	// Pseudo-random-number generation engine.  Use a standard Mersenne Twister engine with default parameterizations.

	operation_index_t	top_PC;				// Remembers the original "topmost" PC as we are going into depths of the algorithm.  For debugging.
	int					recursion_depth;	// How deep are we into the recursion in recalc_amplitude()


	// Private member functions.
private:
	// These are used by the constructor during initialization.
	void read_operators();
	void read_config();
	void read_opseq();
	void read_input();

	// These are used during simulation.
	bool done();					// Returns TRUE if the quantum algorithm is finished running.
	void Bohm_step_forwards();		// Take one step forwards through the program using Bohm's algorithm.
	Complex recalc_amplitude();		// Recalculate the amplitude of the current_state recursively
									//		via (somewhat optimized) Feynman path-integral approach.
	
	// Public member functions.
public:

	SEQCSim(void);		// Constructor.  Reads input files & initializes this virtual quantum computer.
	
	void run(void);		// Run the quantum computer simulation.  (A single pass, with a single final state.)
	
	~SEQCSim(void);		// Destructor.
};
