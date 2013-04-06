//-----------------------------------------------------------------------
// SmartComplexVector.h - Header file declaring class SmartComplexVector
//
// A SmartComplexVector is a vector of complex numbers that is "smart"
// for our purposes in that it automatically caches certain useful and
// frequently-accessed information that we need in our quantum simulator
// application, which will speed up our execution.  This includes the
// following:
//
// 1. Is this vector entirely "classical," meaning that it includes only
//      the values 0 and 1 (and only 1 occurrence of the number 1)?
//
// 2. If it is classical, what is the index of the entry that contains 1?
//
// 3. If it's not classical, is it at least single-valued, meaning that
//      it contains one and only one non-zero entry?
//
// 4. If it's single-valued, then what is the index of the non-zero entry,
//      and what is its value?
//
// 5. If it has multiple non-zero values, but is possibly still sparse, 
//      then what are the indices and values of the non-zero values?
//
// All of this information could of course be extracted by scanning the
// entire vector each time it is needed.  But if we cache it, we can 
// speed things up considerably.
//
// Essentially, this is just a redundant overlay of sparse and non-sparse
// vector representations.  It has the speed advantages but not the size
// advantages that a purely sparse vector representation would have.  This
// is OK, because we only intend to use it for moderately small vectors
// anyway.
//-------------------------------------------------------------------------

#pragma once

#include <vector>
#include <iostream>
#include "Complex.h"
#include "debug.h"

using namespace std;

class SmartComplexVector {
	// Private data members.
private:
	vector<Complex>		elements;		// All of the elements of the vector.
	vector<size_t>		nz_elem_idxs;	// Indices of the non-zero elements of the vector.

	// Private member functions.
private:
	// Re-scan all elements and reconstruct the nz_elem_idxs vector from scratch.
	void __rescan_elems(void) {
		nz_elem_idxs.clear();				// Turns the current list of nonzero elements to the empty list.
		for (size_t i=0; i<size(); i++) {
			if (elements[i].isNonzero()) {						// If the current complex is nonzero,
				nz_elem_idxs.insert(nz_elem_idxs.end(), i);		//   add the current index to the list.
			}
		}
	}

	// This public member class implements a "reference" to an element of a SmartComplexVector.
	// It is not a native C++ reference, but it can be used in a similar fashion.
	// It has the feature that when one alters the SmartComplexVector through it,
	// the cached internal information in the SmartComplexVector is automatically
	// updated as well.  Note this class is a friend class of SmartComplexVector
	// so it can access its private members.
	// 
	// Usage example:
	//		SmartComplexVector v(10);		// Creates a vector of size 10, initial value 0.
	//		v[5] = 1;						// Sets element 5 of the vector to 1.  Updates internal structures.
	//
	// Note that in the above example, the user code did not ever have to explicitly
	// refer to the SCV_elem_ref class.  Everything was handled automatically
	// "under the covers," as it were.  Note this entity works better than a simple
	// reference to Complex would, because this reference continues working even after
	// the entire vector is reallocated (for example as a result of a resize).
public:
	class elem_ref {
		private:
			SmartComplexVector&		vec;			// Reference to the entire original vector.
			size_t					elem_index;		// Index of the specific referenced element within the vector.
		public:
			// The normal constructor just sets up our private data members as given.
			elem_ref(SmartComplexVector& v, size_t i) : vec(v), elem_index(i) {}

			// Conversion to Complex: Just fetch the element from the vector.
			operator Complex(void) { return vec.elements.at(elem_index); }

			// Assignment from Complex: Set the element of the vector, and recalculate its cached information.
			elem_ref& operator=(Complex newval) {
//				if (ns_debug::trace) {
//					cout << "elem_ref::operator=(): Setting element " << elem_index << " of the destination vector to ";
//					newval.putTo(cout);
//					cout << ".\n";
//				} 
				vec.elements.at(elem_index) = newval;
				vec.__rescan_elems();
				return (*this);
			}

			// I/O methods which we just route through to the complex element in question.
			void getFrom(istream& is) { // Initialize this complex from the given input stream.
				vec.elements.at(elem_index).getFrom(is); 
				vec.__rescan_elems();
			} ;	 
			void putTo(ostream& os) { vec.elements.at(elem_index).putTo(os); } ;	 // Print this complex to the given output stream.
	};

	// Public member functions.
public:
	// Resizes the vector.
	void		resize(size_t newsize) { elements.resize(newsize); __rescan_elems(); }

	// Returns the size of (number of elements in) the vector.
	size_t		size() { return elements.size(); }

	// Add the given value as a new element at the end of the vector.  Increases its size by 1.
	// NOTE: This method is not presently used.
	void		add(Complex newelem) { elements.resize(size()+1); (*this)[size()-1] = newelem; }

	// Get a fancy smart reference thingy to a vector element.  Provides automatic updating features.
	elem_ref	operator[](size_t index) { return elem_ref(*this, index); } 

	// Returns the number of nonzero elements in the vector.
	size_t		nNonzeros() { return nz_elem_idxs.size(); }
	
	// Returns true iff the vector has only a single nonzero element.
	bool		isOnAxis() { return nNonzeros()==1; }

	// Returns the index of the first nonzero element of the vector.
	size_t		idx_1st_nz() { return nz_elem_idxs[0]; }

	// Returns the value of the first nonzero element of the vector.
	Complex		first_nz_elem() { return elements.at(idx_1st_nz()); }

	// Returns true iff the vector has only a single nonzero element, and that element is 1.
	bool isClassical() { 
		return isOnAxis() 
			&& first_nz_elem() == Complex(1);   // Use of constructor resolves conversion ambiguity.
	}

	// Returns a reference to the vector of the indices of the nonzero elements of
	// this vector.  Modification of this vector by the caller is disallowed, and 
	// could cause havoc.
	vector<size_t>& indices_of_nz_elems(void) { return nz_elem_idxs; }

	void		putTo(ostream& os);
};

ostream& operator<<(ostream& os, SmartComplexVector& scv);

