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

// Complex.h - Defines a simple class for complex numbers.

#pragma once

#include <iostream>			// For istream, ostream classes used in reading/printing methods.
#include <math.h>				// For sqrt() function used in norm() method.
#include "FileReader.h"

class Complex {
	// We expose our data members since they are so straightforward.
	// Allows our users to fiddle with real/imag components directly.
public:
	double R,I;		// Two double-precision floating-point real numbers, R and I (real and imaginary).

	// Public member functions.
public:
	// Constructors.
	
	Complex(void) : R(0), I(0) {}		// If no args are given, initialize both R&I to 0.
	Complex(double i) : R(i), I(0) {}	// If only one number is given, initialize R to it, I to 0.
	Complex(double r, double i)			// If two numbers are given, init R to the first, I to the 2nd.
		: R(r), I(i) {}
    //Complex(Complex c) : R(c.r), I(c.i) {}	// If a complex is given, use an ordinary copy-constructor.
	// I don't think the above line is needed since a copy-constructor should exist automatically.

	// Non-constructor initialization functions.

	void getFrom(istream& is);	 // Initialize this complex from the given input stream.
	void putTo(ostream& os);	 // Print this complex to the given output stream.

	// Conversion operators.

	//operator bool(void) { return R || I; }	// Return TRUE (nonzero) iff either R or I is nonzero.
	//    WARNING: The existence of the above conversion causes arithmetic
	//       expressions involving Complex objects to be misinterpreted,
	//       via the conversion path Complex->bool->int->Complex!  (Thus,
	//       we comment it out, and use the below explicit conversion 
	//       method instead.)
	
	// Unary relational operations.

	bool isNonzero(void) { return R || I; }	 // Is this complex number not equal to zero?
	bool isZero(void) { return !isNonzero(); }	// It's 0 if it's not nonzero.

	// Comparison relational operators.

	bool operator==(double real) { return R == real && I == 0; }	// Real part matches, imag part is 0.
	bool operator==(Complex c)   { return R == c.R  && I == c.I; }	// Both real & imag parts match.
	bool operator!=(double real) { return !((*this) == real); }

	// Arithmetic unary operators.

	double squared_norm() {return R*R + I*I;}		// The square of the norm (magnitude) of this complex number.
	double norm() { return sqrt(squared_norm()); }	// The norm (magnitude) of this complex number.
	
	// Arithmetic binary operators.

	// Multiplication (product) operator. (R1+iI1)(R2+iI2) = (R1*R2 - I1*I2) + i*(R1*I2 + I1*R2)
	Complex operator*(Complex& multiplicand) {
		Complex product(									// Invokes default (copy-) constructor.
					R*multiplicand.R - I*multiplicand.I,	// Initialize new R with real part of product.
					R*multiplicand.I + I*multiplicand.R		// Initialize new I with imaginary part of product.
				);
		return product;
	}

	// Modification operators.
	
	Complex& operator*=(Complex multiplicand) {					// In-place complex multiplication.
		Complex  product = (*this)*multiplicand;
		(*this) = product;
		return (*this);
	}

	Complex& operator+=(Complex addend) {		// In-place complex addition.
		R += addend.R;  I += addend.I;
		return (*this);
	}
};

// Is this used?
//typedef Complex Amplitude;		// Let Amplitude be a synonym for Complex.
								// (Amplitudes must also have norm <=1, but we don't explicitly enforce this.)

using namespace std;
ostream& operator>>(ostream& os, Complex& c);
