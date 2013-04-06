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

// FileReader.cpp - Implements the FileReader class declared in FileReader.h.

#include <string>			// Defines getline(istream&,string&), etc.
#include <iostream>
#include <fstream>			// For ifstream
#include <sstream>			// Needed to define istringstream in Visual C++.
#include "FileReader.h"
#include "debug.h"

using namespace std;

// Define our own namespace into which we embed some useful constants.
namespace ns_fileReader {
	const short invalidVersionNumber = -1;
		// This version number is considered invalid and indicates
		// a bad magic cookie.
}

// This MagicCookie constructor parses the magic cookie data from
// the first line of the input file.

MagicCookie::MagicCookie(FileReader& fileReader) {
	
	if (ns_debug::trace) cout << "MagicCookie::MagicCookie(): Looking for the magic cookie...\n";

	// Get the first line of the input file.
	
	auto_ptr<string> firstLine = fileReader.getLine();
	if (ns_debug::trace) cout << "MagicCookie::MagicCookie(): Looking at the line: [" << *firstLine << "].\n";

	// The following magic string should be in the magic cookie line.

	static const char * const magic_string = " format version ";

	// Look for the magic string " format version " in the line.  
	// Everything before it is the "format type" string, everything
	// after it is interpreted as the "format version" number.

	string::size_type magicStringLoc = firstLine->find(magic_string, 0);
	
	// If the magic string is not found, this isn't a valid input file.
	if (magicStringLoc == string::npos) {
		if (ns_debug::trace) cout << "MagicCookie::MagicCookie(): Error! Found no magic cookie in that line!\n";
		formatType = "";
		versionNumber = ns_fileReader::invalidVersionNumber;
	} 
	
	// Otherwise, if the magic string was found, parse the stuff around it.
	else {
		if (ns_debug::trace) cout << "MagicCookie::MagicCookie(): Found the magic cookie starting at character position " << magicStringLoc << ".\n";

		// Set the file format type to the part of the line before the magic string.
		formatType = firstLine->substr(0, magicStringLoc);
		if (ns_debug::trace) cout << "MagicCookie::MagicCookie(): The declared file format type is: [" << formatType << "].\n";
		
		// Read the file format version number from the part of the line after the magic string.
		
		string afterPart = firstLine->substr(magicStringLoc + strlen(magic_string));
			// Gets substring from just after magic string to end of line.

		istringstream parseAfter(afterPart);
			// Creates an input string stream from the after-part.
	
		parseAfter >> versionNumber;
			// Read the decimal version number from it.

		if (ns_debug::trace) cout << "MagicCookie::MagicCookie(): The declared file format version number is: " << versionNumber << ".\n";
	}
}


// A valid magic cookie is one whose format type name is not
// an empty string, and whose format version number is nonnegative.

bool MagicCookie::valid() {
	return !formatType.empty() && versionNumber >= 0;
}

FileReader::FileReader(const string& inputFileName) {
	if (ns_debug::trace) cout << "FileReader::FileReader(): Constructing FileReader object for file " << inputFileName << "\n";
	inputFileStream.open(inputFileName.c_str());
	if (inputFileStream.fail()) {
		cout << "FileReader::FileReader(): Error! Failed to open " << inputFileName << "\n";
		exit(1);
	} else {
		if (ns_debug::trace) cout << "File " << inputFileName << " was successfully opened.\n";
	}
}

// getLine returns, as an auto_ptr<string>, the next line
// of the input file, or a NULL pointer if no more lines remain.
// The point of the auto_ptr is that it automatically deallocates
// the string when it goes out of scope.

auto_ptr<string> FileReader::getLine() {
	if (inputFileStream.eof()) {
		if (ns_debug::trace) cout << "getLine(): Reached end-of-file.\n";
		auto_ptr<string> nullPtr;	// I'm assuming here that the default auto_ptr constructor creates a null pointer.
		nullPtr.release();			// This is here to make sure it is null.
		return nullPtr;
	}
	string *line = new string();
	getline(inputFileStream, *line);
	return auto_ptr<string>(line);		// Invokes auto_ptr construction from a normal pointer.
}

// This little helper function returns true for strings that
// consist entirely of whitespace.

static bool isAllWhitespace(string& str) {
	return str.find_first_not_of(" \t\v\n\r\f", 0) == string::npos;
	  // If the first character in the string that is not a space,
	  // tab, vertical tab, newline, carriage return, or formfeed
	  // is not at any position in the string, return true.
}

// Like the previous function, but skips lines starting with comments
// and lines that are entirely whitespace.

auto_ptr<string> FileReader::getLine_ignoreComments() {
	auto_ptr<string> nextLine;
	do {
		nextLine = getLine();	// Get the next line of the string.
		if (nextLine.get() == NULL) {
			if (ns_debug::trace) cout << "getLine_ignoreComments(): Reached end-of-file.\n";
			return nextLine;
		}
		if (ns_debug::trace) cout << "Got the line: [" << *nextLine << "].\n";
	} while (nextLine->find("comment:",0) == 0 ||
			 isAllWhitespace(*nextLine));
		// If the string begins with "comment: ", try again.
		// Also try again if the string is all whitespace.
	if (ns_debug::trace) cout << "Got the meaty line [" << *nextLine << "].\n";
	return nextLine;  // Return the first non-blank line not beginning "comment: ".
}

