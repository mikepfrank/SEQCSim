#include "State.h"
#include "BitVector.h"

size_t State::extractBits(vector<qubit_index_t> whichOnes) {

	// Identify just how many bits we're talking about.
	size_t howMany = whichOnes.size();
	
	// We'll assemble the output in a bit-vector of the same length as the input vector of indices.
	BitVector  qb_val_bv(howMany);
	
	for (operand_index_t  opd_i = 0;		// Loop an index over the bit indices...
						  opd_i < howMany;
						  opd_i++)
	{
		// Get the bit-address of the qubit that is the (opd_i)th operand of the current operation.
		qubit_index_t	qub_i = whichOnes.at(opd_i);	  

		// Get the (classical) value of the qubit located at that bit-address in the current basis state.
		bool			qub_val = (*this)[qub_i];

		// Store that bit-value in the bit-vector representation of the input index to the operation.
		qb_val_bv[opd_i] = qub_val;
	}

	return qb_val_bv;	// Convert vector of results to an integer and return it.
}

void State::setBits(vector<qubit_index_t> whichOnes, BitVector &bitValues) {
	
	// Identify just how many bits we're talking about.
	size_t howMany = whichOnes.size();
	
	for (operand_index_t  opd_i = 0;		// Loop an index over the bit indices...
						  opd_i < howMany;
						  opd_i++)
	{
		// Get the bit-address of the qubit that is the (opd_i)th operand of the current operation.
		qubit_index_t	qub_i = whichOnes.at(opd_i);	  

		// Set the (classical) value of the qubit located at that bit-address to the corresponding given value.
		(*this)[qub_i] = (bool)bitValues[opd_i];	// The cast prevents mere copying of reference information.

	}
}

int State::hammingDistanceFrom(State& other) {

	// If the two states aren't the same size, return -1 (incomparable).
	if (this->bits.size() != other.bits.size()) { return -1; }

	int dist = 0;	// Accumulator variable for the Hamming distance.
	for (size_t i = 0; i < size(); i++) {
		if (static_cast<bool>(bits[i]) != static_cast<bool>(other.bits[i])) 
		{	// If the two states' bitvectors aren't the same at this point,
			dist++;						//     increment their accumulated Hamming distance.
		}
	}

	// Return the accumulated Hamming distance between this state and the other one.
	return dist;
}

int hamming_distance(State& s1, State& s2) {
	return s1.hammingDistanceFrom(s2);
}

ostream& operator<<(ostream& os, State& state) {
	state.bits.putTo(os);			// os << state.bits converts bits to an integer first!
	os << " ==> ";
	state.amp.putTo(os);
	return os;
}