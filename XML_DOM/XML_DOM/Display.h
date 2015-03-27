///////////////////////////////////////////////////////////////////
// XmlDocument.h - a handler for display events                  //
// Ver 1.0                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Dell Inspiron 7520, Win 8.1, Visual Studio 2013  //
// Author:      Sampath Toragaravalli Janardhan, (315-664-8206)  //
//              sampathjanardhan@gmail.com                       //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is used to handle display events as part of the
* Project #2 - XML Document Model.  
*
* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp,
*
* Build Process:
* --------------
*   devenv XML_DOM.sln /debug rebuild
*
* Maintenance History:
* --------------------
* Ver 1.0 : 25 Mar 15
* - first release
*
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "XmlDocument.h"

using namespace XmlProcessing;

class Display
{
public:
	void title1(const std::string& title);
	void title2(const std::string& title);
	void heading(const std::string& heading, char underlineChar = '-');
	void note(const std::string& note);
	void noteVec(XmlDocument::attribsVec attribs);
	void addLine();
	void printAST(const XmlDocument::sPtr pDocument);
private:

};
inline void Display::addLine() { cout << "\n"; }

#endif