///////////////////////////////////////////////////////////////////
// Display.cpp - a display handler used by the executive         //
// Ver 1.0                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Dell Inspiron 7520, Win 8.1,					 //
//				Visual Studio 2013								 //
// Authors:     Sampath Toragaravalli Janardhan (315-664-8206)   //
//				sampathjanardhan@gmail.com						 //
//				Jim Fawcett, CST 4-187, 443-3948                 //
//              jfawcett@twcny.rr.com                            //
///////////////////////////////////////////////////////////////////

#include "Display.h"

//----< Display the main Title >----------------------------------
void Display::title1(const std::string& title)
{
	std::cout << "\n=============================================================\n";
	std::cout << "  " << title;
	std::cout << "\n=============================================================\n";
}

//----< Display a Title >-----------------------------------------
void Display::title2(const std::string& title)
{
	std::cout << "\n ========================================\n";
	std::cout << "  " << title;
	std::cout << "\n ========================================\n";
}

//----< Display a heading >---------------------------------------
void Display::heading(const std::string& heading, char underlineChar)
{
	std::cout << "\n  " << heading << "\n ";
	std::cout << std::string(heading.size() + 2, underlineChar) << "\n";
}

//----< Display a note >------------------------------------------
void Display::note(const std::string& note)
{
	//std::cout << "----" << note << "----";
	std::cout << " " << note << "\n";
}

//----< Display Attributes >--------------------------------------
void Display::noteVec(XmlDocument::attribsVec attribs)
{

	std::vector<pair<std::string, std::string>>::iterator iter;
	cout << "\n --\tname\t = value\t--";
	cout << "\n ---------------------------------";
	for (iter = attribs.begin(); iter != attribs.end(); ++iter)
		cout << "\n \t" << iter->first << "\t = " << iter->second;
	cout << "\n\n";
}

//----< Display AST >---------------------------------------------
void Display::printAST(const XmlDocument::sPtr pDocument)
{
	cout << "\n\n Abstract Syntax Tree:\n";
	cout << " -----------------------\n";
	if (pDocument != nullptr)
	cout << " " << pDocument->toString() << "\n\n";
	//cout << "_______________________________________\n\n";
}