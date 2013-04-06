// State.h - A classical computational basis state for the quantum computer.

#pragma once

#include <memory.h>			// auto_ptr (Is this even used here?)
#include "index_types.h"	// qubit_index_t
#include "BitVector.h"		// Our custom bit vector class.
#include "Complex.h"		// Out complex number class.

// Note this is not a quantum state vector, which would be a vector
// of 2^N complex numbers (very large).  This is only a classical
// computational basis state, essentially just a vector of N bits,
// with an attached complex amplitude.  It corresponds to just a 
// single component of a full quantum state vector.

class State {
public:
	BitVector	bits;	// The classical (0-1) qubit values for this basis state.
	Complex		amp;	// The complex amplitude of this basis state (when considered as a component of a full quantum state vector).

	// Public member functions.
public:
	// When creating a new state from scratch, initialize its amplitude to 1.0 (+0i) by default.

	State(void) : amp(1) {}

	// Returns the size of the state in bits.

	size_t size(void) { return bits.size(); }
	
	// Resize the state to the given number of bits.

	void  resize(qubit_index_t size) {bits.resize(size);}
	
	// Assignment operator for assigning to this state from another state.
	//
	// This is commented out because it automatically exists anyway in C++.

	//State&  operator=(State& source) {
	//	bits = source.bits;	// Operator method invocation.
	//	amp = source.amp;	// Copy the state's amplitude, without changing it.
	//	return *this;		// Needed for (s = ...) = ... to work
	//}

	// Returns a reference to a given bit of the state, which can be used like an lvalue.
	BitRef  operator[](qubit_index_t  bit_index) { return bits[bit_index]; }

	// Returns a bit-vector (in integer form) giving the values of all 
	// the qubits selected by the given vector<qubit_index_t>.  Needless to say,
	// the number of qubits extracted should not be too large to fit in an integer.

	size_t  extractBits(vector<qubit_index_t> whichOnes);

	// Given a vector of qubit indices, and a corresponding-length vector of bit values,
	// modify the current state to set the qubit values equal to the given bit values.
	// Does not change the state's amplitude.  NOTE: In this function, is no limit to
	// how many bits we can set in a single call (other than memory limits).

	void  setBits(vector<qubit_index_t> whichOnes, BitVector &bitValues);

	// Returns the number of bits that differ between this state and the given other state.
	// Or, returns -1 if the two states are incomparable (because of differing sizes).

	int  hammingDistanceFrom(State& other);

	// Compares two states for equality by verifying that their bit vectors
	// are equal.  Does not compare amplitudes.

	bool  operator==(State& other) { 
		return bits == other.bits;	// Compare the underlying BitVector objects for equality.
		// We could alternatively use the below method of checking the Hamming 
		// distance, but it is more time-consuming, since it does a bitwise instead of 
		// wordwise comparison.
		//
		// return hammingDistanceFrom(other) == 0; 
	}
};

// Declare a << operator to allow reading states from an input stream with the usual syntax.
using namespace std;
ostream& operator<<(ostream& os, State& state);