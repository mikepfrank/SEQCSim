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

//------------------------------------------------------------------------
// seqcsim.cpp - Space-Efficient Quantum Computer SIMulator.  (Main file.)
//
//    C++ console application, suitable for compilation under
//    Microsoft Visual C++ 2008 Express Edition.  Currently requires
//    the C++ STL and some of the TR1 extensions to the C++ standard.
//
//    By Michael P. Frank, Uwe Meyer-Baese,  Irinel Chiorescu, 
//    and Liviu Oniciuc, Florida State University, 2008-2009.
//    Copyright (C) 2008-2009, FSU Board of Trustees.  See LICENSE.txt
//    for public licensing details.
//-------------------------------------------------------------------------
#include <iostream>		// Defines cout, etc.
#include <string>		// Defines string class
#include "SEQCSim.h"	// Defines main class: SEQCSim (simulator object).

using namespace std;	// Lets us say "cout" instead of "std::cout".

int main(int argc, char **argv) {
	cout << "\n\
Welcome to SEQCSim, the Space-Efficient Quantum Computer Simulator\n\
  (C++ console version 0.8)\n\
Copyright (C) 2008-2009  Florida State University Board of Trustees\n\
By Michael P. Frank, Liviu Oniciuc, Uwe Meyer-Baese, Irinel Chiorescu.\n\
\n\
Licensed under the GNU General Public License, version 3.\n\
This program comes with ABSOLUTELY NO WARRANTY; for details see \n\
the LICENSE.txt file distributed with the program.  This is free \n\
software, and you are welcome to redistribute it under certain \n\
conditions; see LICENSE.txt for details.\n";

	if (ns_debug::trace) {
		cout << "main(): In the current environment, numbers of type size_t (memory byte indices)\n\thave size " 
			<< sizeof(size_t) << " bytes or " << sizeof(size_t)*8 << " bits long.\n";
	}

	SEQCSim  simulator;	// Default constructor reads input files and initializes machine configuration.
	simulator.run();	// Start simulating the quantum computer, from the beginning.

	// At this point we should really do something useful like display the final state of the
	// quantum computer, or run it repeatedly and collect statistics on the distribution of outputs,
	// but that code hasn't been written yet.

	if (ns_debug::trace) {
		cout << "main(): INFO: The SE_QC_Sim program has finished executing and is exiting normally.";
	}

	// The following is just to give the user a chance to collect peak-memory and CPU time stats.
	cout << "Press enter to exit.\n";
	string *line = new string();
	getline(cin, *line);

	exit(0);	// Return a code of 0 (meaning "normal completion") to the runtime environment.
}