// This file exists because we cannot inline all functions in SmartComplexVector.h
// due to the (???)

#include "SmartComplexVector.h"

void SmartComplexVector::putTo(ostream& os) {
	SmartComplexVector& scv = *this;
	os << "[";  
	size_t vs = scv.size();
	for (size_t  i = 0; i < vs; i++) {
		scv[i].putTo(os);
		if (i < vs-1) os << ", ";
	}
	os << "]";
}

ostream& operator<<(ostream& os, SmartComplexVector& scv){
	scv.putTo(os);
	return os;
}
