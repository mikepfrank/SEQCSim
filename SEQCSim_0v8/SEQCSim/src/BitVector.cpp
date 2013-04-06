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

#include "BitVector.h"

void BitVector::putTo(ostream& os) {
	BitVector&		bv = *this;
	size_t			sz = bv.size();
	os << sz-1 << "->";
	for (size_t		i = sz-1;  ;  i--) {
		os << ((bool)bv[i])?1:0;
		if (i == 0) break;
	}
	os << "<-0 (" << sz << " bits)";
}

ostream& operator>>(ostream& os, BitVector& bv) {
	bv.putTo(os);
	return os;
}