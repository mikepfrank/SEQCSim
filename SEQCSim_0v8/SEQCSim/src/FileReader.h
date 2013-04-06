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

// FileReader.h - Defines a class for a simple file-reading utility.

#pragma once

#include <string>		// Defines class string.
#include <iostream>
#include <fstream>		// Needed to fully define class ifstream in Visual C++.
#include <memory>		// Defines auto_ptr<> template for self-disposing pointers.

using namespace std;

// Forwards declaration for FileReader class, so we can
// refer to it from MagicCookie class definition.

class FileReader;

//===================================
// A MagicCookie object represents 
// the file format information that
// is generally expected to be seen
// in the first line of an input file.
//====================================

class MagicCookie {
	//-------------------------------
	// Public data members.

public:	
	string	formatType;	   // String labeling the file type.
	short	versionNumber;	// Integer version number of this format.
	
	//-------------------------------
	// Public constructors.
	
	MagicCookie(FileReader& fileReader);
	  // This constructor takes a FileReader object and uses it to consume
	  // a magic cookie from the first available line in the input file.
	  // the line read does not conform to the expected magic cookie
	  // format, the cookie created will have fileType NULL and 
	  // versionNumber 0.  Otherwise, the cookie will have the fileType and
	  // versionNumber as parsed from the magic cookie line.
	
	//-------------------------------
	// Public member functions.
	
	bool valid();
		// Returns true if the magic cookie data was able to be read, 
		// indicating a valid file format.
};

class FileReader {
private:
	// Private data members.
	ifstream inputFileStream;	// C++ IOStreams handle to the input file.

public:
	//-------------------------------
	// Public constructors.
	
	FileReader(const string& inputFileName);
	//  Creates a new FileReader which will read from the file named by the string inputFileName.
	//	This opens the specified file for reading.  The filename string provided is not modified 
	//	or destroyed.
	
	//-------------------------------
	// Public member functions.

	MagicCookie EatMagicCookie() { return MagicCookie(*this); }
	//	Look for a magic cookie line in the proper format, designating the file format
	//	type and version number.  Returns the cookie, which still needs to be digested.
	
	auto_ptr<string> getLine();
	//	Consume and return the next line from the input file. Returns NULL if no
	//	more lines are available.  Note: The pointer to the newly-created string 
	//	object is returned as an auto_ptr so that the string will be automatically
	//	deallocated when it goes out of scope.
	
	auto_ptr<string> getLine_ignoreComments();
	//	Consume and return the next line from the input file, skipping over lines that
	//	begin with the special character sequence "comment: " or that contain only
	//	whitespace.  Returns NULL if no more lines other than comments are available.
	//	Note: The pointer to the newly-created string object is returned as an 
	//	auto_ptr so that the string object will be automatically deallocated when it
	//	goes out of scope.  We're trying to prevent memory leaks here...

	bool eof(void) { return inputFileStream.eof(); }
	
	//-------------------------------
	// Public destructors.
	
	~FileReader() {inputFileStream.close();}
	//  Closes the open file in preparation for destroying the FileReader object.
};

