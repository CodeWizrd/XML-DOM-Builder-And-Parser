///////////////////////////////////////////////////////////////////
// Executive.cpp - a container to control execution flow         //
// Ver 1.0                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Dell Inspiron 7520, Win 8.1,					 //
//				Visual Studio 2013								 //
// Authors:     Sampath Toragaravalli Janardhan (315-664-8206)   //
//				sampathjanardhan@gmail.com						 //
//				Jim Fawcett, CST 4-187, 443-3948                 //
//              jfawcett@twcny.rr.com                            //
///////////////////////////////////////////////////////////////////

#include <iostream>
#include "Executive.h"

//----< Write AST to String >-------------------------------------
std::string Executive::writeToString()
{
	display.heading("String Output:");
	string str = pDocument->toString() + "\n";
	return str;
}

//----< Write AST to File >---------------------------------------
std::string Executive::writeToFile(const std::string& str)
{
	ofstream file;
	string fname = str;
	try
	{
		file.open(fname);
		if (!(file.is_open()))
			throw "Unable to open file!";
	}
	catch (char* errmsg)
	{
		cout << "Exception raised: " << errmsg << "\n";
	}
	file << pDocument->toString();
	file.close();
	return fname;
}

//----< Demonstrate Reuirement 3 >--------------------------------
void demonstrateRequirement3(string filename)
{
	Executive exec;

	exec.display.heading("String Input");
	std::string Prologue = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!-- top level comment --><?xml-stylesheet css=\"1.0\"?>";
	std::string test = Prologue;
	test += "<Dir><path>/TestFile/Dir1</path><!-- Comment in the body --><File date='notCurrent'>Sampath<name type='file'>text1.txt</name><date>02/11/2012</date></File></Dir>";
	exec.display.note("XML String:\t" + test);
	XmlDocument dom(test);
	exec.pDocument = dom.getDOM();
	if (exec.pDocument != nullptr)
	{
		exec.display.printAST(exec.pDocument);
		dom.resetTab();
	}
	else exec.display.note("\n DOM doesn't exist\n");
	
	std::string fname = filename;
	exec.display.heading("File Input :\t'" + fname + "'");
	XmlDocument docf(fname, XmlDocument::filename);
	exec.pDocument = docf.getDOM();
	if (exec.pDocument != nullptr)
	{
		exec.display.printAST(exec.pDocument);
		docf.resetTab();
	}
	else exec.display.note("\n DOM doesn't exist\n");
	cout << "\n\n";
}

//----< Demonstrate Reuirement 4 (I/O to file / string) >---------
void demonstrateReq4IO(string filename)
{
	Executive exec;
	exec.display.title2("Read/Write from/to Strings and XML files");

	exec.display.heading("String Input");
	std::string Prologue = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!-- top level comment -->";
	std::string test = Prologue;
	test += "<Dir><path>/TestFile/Dir1</path><File date='notCurrent'>Sampath<name type='file'>text1.txt</name><date>02/11/2012</date></File></Dir>";
	exec.display.note("XML String:\t" + test);
	exec.display.addLine();
	XmlDocument dom(test);
	exec.pDocument = dom.getDOM();
	if (exec.pDocument != nullptr)
	{
		string edited = exec.writeToString();
		exec.display.note(edited);
		dom.resetTab();
	}
	else exec.display.note("\n DOM doesn't exist\n");

	std::string fname = filename;
	exec.display.heading("File Input :\t'" + fname + "'");
	XmlDocument docf(fname, XmlDocument::filename);
	exec.pDocument = docf.getDOM();
	if (exec.pDocument != nullptr)
	{
		string edited = exec.writeToFile("Output1.XML");
		exec.display.heading("File Output: \tXML written to " + edited);
		docf.resetTab();
	}
	else exec.display.note("\n DOM doesn't exist\n");
	std::cout << "\n\n";
}

//----< Demonstrate Reuirement 4 (Move Ctor) >--------------------
void demonstrateReq4MoveCtor(string filename)
{
	Executive exec;
	exec.display.title2("Move Constructor");

	std::string fname = filename;
	exec.display.heading("File Input: " + fname);
	XmlDocument dom(fname, XmlDocument::filename);

	exec.display.heading("Original XML Tree");
	exec.pDocument = dom.getDOM();
	if (exec.pDocument != nullptr)
	{
		exec.display.printAST(exec.pDocument);

		XmlDocument xmlDom = std::move(dom);

		exec.display.addLine();
		exec.display.heading("Moved DOM: ");
		exec.pDocument = xmlDom.getDOM();
		exec.display.printAST(exec.pDocument);
	}
	else exec.display.note("\n No DOM to move\n");
}

//----< Demonstrate Reuirement 4 (Move Assignment Operator) >-----
void demonstrateReq4MoveAssgn(string filename)
{
	Executive exec;
	exec.display.title2("Move Assignment");
	
	std::string fname = filename;
	exec.display.heading("File Input: " + fname);
	XmlDocument dom(fname, XmlDocument::filename);

	exec.display.heading("Original XML Tree");
	exec.pDocument = dom.getDOM();
	if (exec.pDocument != nullptr)
	{
		exec.display.printAST(exec.pDocument);

		XmlDocument xmlDom;
		xmlDom = std::move(dom);

		exec.display.addLine();
		exec.display.heading("Moved DOM: ");
		exec.pDocument = xmlDom.getDOM();
		exec.display.printAST(exec.pDocument);
	}
	else exec.display.note("\n No DOM to move\n");
}

//----< Demonstrate Reuirement 4 >--------------------------------
void demonstrateRequirement4(string filename)
{
	demonstrateReq4MoveCtor(filename);
	demonstrateReq4MoveAssgn(filename);
	demonstrateReq4IO(filename);
}

//----< Demonstrate Reuirement 5 (Search by attribute name >------
void demonstrateRequirement5(string filename)
{
	Executive exec;

	std::string fname = filename;
	exec.display.heading("File Input: " + fname);
	XmlDocument doc(fname, XmlDocument::filename);

	exec.pDocument = doc.getDOM();
	if (exec.pDocument != nullptr)
	{
		string tag = "css";
		exec.display.heading("Searching for attribute '" + tag + "'");
		exec.pDocument = doc.getElementById(tag);
		if (exec.pDocument != NULL)
		{
			exec.display.note("Element with attribute name '" + tag + "' found..");
			exec.display.note(exec.pDocument->show());
		}
		else
			exec.display.note("-------Not Found!-------");
	}
	else exec.display.note("\n DOM doesnt exist\n");
}

//----< Demonstrate Reuirement 6v(Find first matching tag >-------
void demonstrateRequirement6First(string filename)
{
	Executive exec;
	exec.display.title2("Testing get first element with Tag");
	std::vector<std::shared_ptr<AbstractXmlElement>> searchResult;

	std::string fname = filename;
	exec.display.heading("File Input: " + fname);
	XmlDocument doc(fname, XmlDocument::filename);

	string tag = "name";
	exec.display.heading("Searching for tag " + tag);
	exec.pDocument = doc.getDOM();
	if (exec.pDocument != nullptr)
	{
		doc.resetFound();
		searchResult = doc.element(tag, exec.pDocument).select();
		if (!searchResult.empty())
		{
			exec.display.note("Element with tag '" + tag + "' found..");
			exec.display.note(searchResult[0]->show());
		}
		else
			exec.display.note("-------Not Found!-------");
	}
	else exec.display.note("\n DOM doesnt exist\n");
}

//----< Demonstrate Reuirement 6 (Find all matching tags >--------
void demonstrateRequirement6All(string filename)
{
	Executive exec;
	exec.display.title2("Testing get all elements with Tag");
	std::vector<std::shared_ptr<AbstractXmlElement>> searchResult;

	std::string fname = filename;
	exec.display.heading("File Input: " + fname);
	XmlDocument doc(fname, XmlDocument::filename);

	string tag = "name";
	exec.display.heading("Searching for tag '" + tag + "'");
	exec.pDocument = doc.getDOM();
	if (exec.pDocument != nullptr)
	{
		doc.resetFound();
		searchResult = doc.elements(tag, exec.pDocument).select();
		if (!searchResult.empty())
		{
			exec.display.note("Elements with tag '" + tag + "' found..");
			for (auto iter : searchResult)
				exec.display.note(iter->show());
		}
		else
			exec.display.note("-------Not Found!-------");
	}
	else exec.display.note("\n DOM doesn't exist!\n");
}

//----< Demonstrate Reuirement 6 >--------------------------------
void demonstrateRequirement6(string filename)
{
	demonstrateRequirement6First(filename);
	demonstrateRequirement6All(filename);
}

//----< Demonstrate Reuirement 7 (add child) >--------------------
void demonstrateReq7ChildAdd(string filename)
{
	Executive exec;
	bool addResult;
	std::vector<std::shared_ptr<AbstractXmlElement>> searchResult;
	exec.display.title2("Add Child");

	std::string fname = filename;
	exec.display.heading("File Input: " + fname);
	XmlDocument doc(fname, XmlDocument::filename);
	exec.pDocument = doc.getDOM();
	if (exec.pDocument != nullptr)
	{
		doc.resetFound();

		string newElement = "<Project name='XML_DOM' dueDate='19March'> <Task> Read XML </Task> <Task> Build Parse Tree </Task> <Task> Write XML </Task> </Project>";

		doc.createNode(newElement);
		XmlDocument::sPtr loc = doc.getNewElem();

		string tag = "LectureNote";
		exec.display.heading("Adding below node as a child to node '" + tag + "'");
		exec.display.note("Node: \t" + newElement);
		addResult = doc.element(tag, exec.pDocument).addChild(loc);
		if (addResult)
		{
			searchResult = doc.element(tag, exec.pDocument).select();
			if (!searchResult.empty())
			{
				exec.display.heading("New XML Content: ");
				exec.display.note(searchResult[0]->toString());
				string output = exec.writeToFile("Output2.XML");
				exec.display.addLine();
				exec.display.note("Modified XML content has been written out to File '" + output + "'");

				exec.pDocument = doc.getDOM();
				string edited = exec.writeToFile("Output7a.XML");
				exec.display.heading("File Output: \tXML written to " + edited);
				doc.resetTab();
			}

			else
				exec.display.note("Parent Node can't be found!");
		}
		else
			exec.display.note("Add child failed!");
	}
	else exec.display.note("\n DOM doesn't exist!\n");

}

//----< Demonstrate Reuirement 7 (remove child)>------------------
void demonstrateReq7ChildRemove(string filename)
{
	Executive exec;
	bool removeResult;
	std::vector<std::shared_ptr<AbstractXmlElement>> searchResult;

	exec.display.title2("Remove Child");

	std::string fname = filename;
	exec.display.heading("File Input: " + fname);
	XmlDocument doc(fname, XmlDocument::filename);
	exec.pDocument = doc.getDOM();
	if (exec.pDocument != nullptr)
	{
		doc.resetFound();

		string removeElem = "author";
		string tag = "reference";
		exec.display.heading("Removing node '" + removeElem + "' from parent node '" + tag + "'");
		removeResult = doc.element(tag, exec.pDocument).removeChild(removeElem);

		if (removeResult)
		{
			searchResult = doc.element(tag, exec.pDocument).select();
			if (!searchResult.empty())
			{
				exec.display.heading("New XML Content: ");
				exec.display.note(searchResult[0]->toString());
				string output = exec.writeToFile("Output3.XML");
				exec.display.addLine();
				exec.display.note("Modified XML content has been written out to File '" + output + "'");

				exec.pDocument = doc.getDOM();
				string edited = exec.writeToFile("Output7b.XML");
				exec.display.heading("File Output: \tXML written to " + edited);
				doc.resetTab();
			}
			else
				exec.display.note("Parent Node can't be found!");
		}
		else
			exec.display.note("Remove child failed!");

		exec.pDocument = doc.getDOM();
		string edited = exec.writeToFile("Output7b.XML");
		exec.display.heading("File Output: \tXML written to " + edited);
		doc.resetTab();
	}
	else exec.display.note("\n DOM doesn't exist!\n");
}

//----< Demonstrate Reuirement 7 (Programmatic DOM) >-------------
void demonstrateReq7Empty()
{
	Executive exec;
	bool success;
	exec.display.title2("Populate an empty DOM");

	XmlDocument doc;
	exec.pDocument = doc.getDOM();
	exec.display.heading("Empty DOM created");
	if (exec.pDocument != nullptr)
	{
		exec.display.note(exec.pDocument->toString());

		string newElement = "<EECS> <Course ID=\"687\"> <Session start=\"08:00AM\" end=\"09:20AM\" /> <Session start=\"11:00AM\" end=\"12:20PM\" /> <Instructor title='Professor'> Prof. Jim Fawcett </Instructor> </Course> </EECS>";
		doc.createNode(newElement);
		XmlDocument::sPtr loc = doc.getNewElem();

		success = doc.addChild(loc, exec.pDocument);
		if (success)
		{
			exec.display.heading(" Adding XML Elements to the DOM ");
			exec.display.note(exec.pDocument->toString());
		}
		else
			exec.display.heading("Unable to populate the DOM");

		exec.pDocument = doc.getDOM();
		string edited = exec.writeToFile("Output7c.XML");
		exec.display.heading("File Output: \tXML written to " + edited);
		doc.resetTab();


		string remElem = "Instructor";
		string parnt = "Course";
		success = doc.element(parnt, exec.pDocument).removeChild(remElem);
		if (success)
		{
			exec.display.heading(" Removing XML Elements from the DOM ");
			exec.display.note(exec.pDocument->toString());
		}
		else
			exec.display.heading("Unable to populate the DOM");

		exec.pDocument = doc.getDOM();
		edited = exec.writeToFile("Output7d.XML");
		exec.display.heading("File Output: \tXML written to " + edited);
		doc.resetTab();
	}
	else exec.display.note("\n DOM doesn't exist\n");
}

//----< Demonstrate Reuirement 7 >--------------------------------
void demonstrateRequirement7(string filename)
{	
	demonstrateReq7ChildAdd(filename);
	demonstrateReq7ChildRemove(filename);
	demonstrateReq7Empty();
}

//----< Demonstrate Reuirement 8 (Add / Remove attributes >-------
void demonstrateRequirement8Attribs(string filename)
{
	Executive exec;
	std::vector<std::shared_ptr<AbstractXmlElement>> searchResult;
	exec.display.title2("Return list of attributes");

	std::string fname = filename;
	exec.display.heading("File Input: " + fname);
	XmlDocument doc(fname, XmlDocument::filename);

	string tag = "LectureNote";
	exec.display.heading("Searching for attributes of tag: '" + tag + "'");
	exec.pDocument = doc.getDOM();
	if (exec.pDocument != nullptr)
	{
		doc.resetFound();
		XmlDocument::attribsVec temp = doc.element(tag, exec.pDocument).attributes();
		searchResult = doc.select();
		if (!searchResult.empty())
		{
			exec.display.addLine();
			exec.display.note("Element:");
			exec.display.note(searchResult[0]->show());
			exec.display.addLine();
			exec.display.note("Attribute List:");
			exec.display.noteVec(temp);
			exec.display.addLine();
			exec.pDocument = doc.getDOM();
			string edited = exec.writeToFile("Output8a.XML");
			exec.display.heading("File Output: \tXML written to " + edited);
			doc.resetTab();
		}
	}
	else exec.display.note("\n DOM doesn't exist\n");
}

//----< Demonstrate Reuirement 8 (Add / Remove children >---------
void demonstrateRequirement8Children(string filename)
{
	Executive exec;
	std::vector<XmlDocument::sPtr> children;
	int i = 0;
	exec.display.title2("Return list of children");

	std::string fname = filename;
	exec.display.heading("File Input: " + fname);
	XmlDocument doc(fname, XmlDocument::filename);

	string tag = "reference";
	exec.display.heading("Searching for children of tag: '" + tag + "'");
	exec.pDocument = doc.getDOM();
	if (exec.pDocument != nullptr)
	{
		doc.resetFound();
		children = doc.element(tag, exec.pDocument).children(exec.pDocument).select();
		if (!children.empty())
		{
			for (auto iter : children)
			{
				exec.display.heading(" Child: " + to_string(++i));
				exec.display.note(iter->show());
				exec.display.addLine();
			}
			exec.pDocument = doc.getDOM();
			string edited = exec.writeToFile("Output8b.XML");
			exec.display.heading("File Output: \tXML written to " + edited);
			doc.resetTab();
		}
		else
			exec.display.note("------- Search did not return any results!-------");
	}
	else exec.display.note("\n DOM doesn't exist!\n");
}

//----< Demonstrate Reuirement 8 >--------------------------------
void demonstrateRequirement8(string filename)
{
	demonstrateRequirement8Attribs(filename);
	demonstrateRequirement8Children(filename);
}

//----< Demonstrate Reuirement 9 ( Add attributes) >--------------
void demonstrateRequirement9Add(string filename)
{
	Executive exec;
	std::vector<std::shared_ptr<AbstractXmlElement>> searchResult;
	exec.display.title2("Testing Add Attribute");


	std::string fname = filename;
	exec.display.heading("File Input: " + fname);
	XmlDocument doc(fname, XmlDocument::filename);

	string tag = "author";
	string attribname = "salutation";
	string attribvalue = "Mr.";

	exec.pDocument = doc.getDOM();
	if (exec.pDocument != nullptr)
	{
		doc.resetFound();
		searchResult = doc.element(tag, exec.pDocument).select();

		exec.display.addLine();
		if (!searchResult.empty())
		{
			exec.display.note("Element:");
			exec.display.note(searchResult[0]->show());
			exec.display.heading("Add Attribute\t" + attribname + " = " + attribvalue);
			bool success = doc.addAttrib(attribname, attribvalue, searchResult.at(0));
			if (success)
			{
				exec.display.note("Successsfully Added");
				exec.display.note(searchResult[0]->show());
			}

			else
				exec.display.note("Add failed!");

			exec.pDocument = doc.getDOM();
			string edited = exec.writeToFile("Output9a.XML");
			exec.display.heading("File Output: \tXML written to " + edited);
			doc.resetTab();
		}
		else
			exec.display.note("-------Not Found!-------");
	}
	else exec.display.note("\n DOM doesn't exist\n");
}

//----< Demonstrate Reuirement 9 ( Remove attributes) >-----------
void demonstrateRequirement9Remove(string filename)
{
	Executive exec;
	std::vector<std::shared_ptr<AbstractXmlElement>> searchResult;
	exec.display.title2("Testing Remove Attribute");

	std::string fname = filename;
	exec.display.heading("File Input: " + fname);
	XmlDocument doc(fname, XmlDocument::filename);

	string tag = "note";
	string attrib = "Company";
	exec.pDocument = doc.getDOM();
	if (exec.pDocument != nullptr)
	{
		doc.resetFound();
		searchResult = doc.element(tag, exec.pDocument).select();
		exec.display.addLine();

		if (!searchResult.empty())
		{
			exec.display.note("Element:");
			exec.display.note(searchResult[0]->show());
			exec.display.addLine();
			exec.display.heading("Remove Attribute:  " + attrib);
			bool success = doc.removeAttrib(attrib, searchResult.at(0));
			if (success)
			{
				exec.display.note("Successsfully Removed");
				exec.display.note(searchResult[0]->show());
			}

			else
				exec.display.note("Remove failed!");

			exec.pDocument = doc.getDOM();
			string edited = exec.writeToFile("Output9b.XML");
			exec.display.heading("File Output: \tXML written to " + edited);
			doc.resetTab();
		}
		else
			exec.display.note("-------Not Found!-------");
	}
	else exec.display.note("\n DOM doesn't exist!\n");
}

//----< Demonstrate Reuirement 9 >--------------------------------
void demonstrateRequirement9(string filename)
{
	demonstrateRequirement9Add(filename);
	demonstrateRequirement9Remove(filename);
}

//----< Start point for the program >-----------------------------
int main(int argc, char* argv[])
{
	Executive exec;
	std::string fileName;
	if (argc == 2)
	{
		fileName = argv[1];
		exec.display.heading("\n\n  The following XML will be used where applicable: " + fileName + "\n\n");
	}
	else if (argc == 1)
	{
		fileName = "LectureNote.xml";
		exec.display.heading("\n\n  file not specified\n Using the file '" + fileName + "\n");
	}
	else 
		exec.display.heading("\n\n  Invalid Arguments! Ignoring the arguments..\n Using the file '" + fileName + "\n");
		

	exec.display.title1("Testing Requirement 3 (and Requirement 10)");
	demonstrateRequirement3(fileName);

	
	exec.display.title1("Testing Requirement 4 (and Requirement 10)");
	demonstrateRequirement4(fileName);
	
	exec.display.title1("Testing Requirement 5");
	demonstrateRequirement5(fileName);

	exec.display.title1("Testing Requirement 6");
	demonstrateRequirement6(fileName);

	exec.display.title1("Testing Requirement 7");
	demonstrateRequirement7(fileName);
	
	exec.display.title1("Testing Requirement 8");
	demonstrateRequirement8(fileName);

	exec.display.title1("Testing Requirement 9");
	demonstrateRequirement9(fileName);
	
	std::cout << "\n\n";
}