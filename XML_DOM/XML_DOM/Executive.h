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
* This package is used to control the flow of program logic in
* Project #2 - XML Document Model.
* The executive demonstrates the fulfilment of all functional
* requirements outlined in the Project statement.
*
* Required Files:
* ---------------
*   - Executive.h, Executive.cpp
*     XmlDocument.h, XmlDocument.cpp,
*     Display.h, Display.cpp
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

#ifndef EXECUTIVE_H
#define EXECUTIVE_H

#include <iostream>
#include "XmlDocument.h"
#include "Display.h"

using namespace XmlProcessing;

class Executive
{
public:
	Executive() { }
	~Executive() {  }
	std::string writeToFile(const std::string& str);
	std::string writeToString();
	
	//data members
	Display display;
	XmlDocument::sPtr pDocument;
private:
	

};

#endif