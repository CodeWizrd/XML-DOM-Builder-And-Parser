#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
///////////////////////////////////////////////////////////////////
// XmlDocument.h - a container of XmlElement nodes               //
// Ver 1.2                                                       //
// Application: Help for CSE687 Pr#2, Spring 2015                //
// Platform:    Dell XPS 2720, Win 8.1 Pro, Visual Studio 2013   //
// Authors:     Sampath Toragaravalli Janardhan (315-664-8206)   //
//				sampathajnardhan@gmail.com                       //
//				Jim Fawcett, CST 4-187, 443-3948                 //
//              jfawcett@twcny.rr.com                            //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package acts as a container for the XML Elements AST in 
* Project #2 - XML Document Model.  It uses C++11 constructs,
* most noteably std::shared_ptr.  The XML Document Model is essentially
* a program-friendly wrapper around an Abstract Syntax Tree (AST) used to
* contain the results of parsing XML markup.
*
* Abstract Syntax Trees, defined in this package, are unordered trees with 
* two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in the companion package, XmlElement, will be used in
* the AST defined in this package.  They are:
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration element with attributes
*
* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp, 
*     XmlElement.h, XmlElement.cpp
*
* Build Process:
* --------------
*   devenv XML_DOM.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.2 : 21 Feb 15
* - modified these comments
* ver 1.1 : 14 Feb 15
* - minor changes to comments, method arguments
* Ver 1.0 : 11 Feb 15
* - first release
*
* ToDo:
* -----
* This is the beginning of an XmlDocument class for Project #2.  
* It lays out a suggested design, which you are not compelled to follow.
* If it helps then use it.  If not you don't have to.
*
* Note that I've simply roughed in a design that is similar to that
* used in the .Net Framework XDocument class.
*/

#include <memory>
#include <string>
#include <stack>
#include "XmlElement.h"
#include "itokcollection.h"
#include "Tokenizer.h"
#include "xmlElementParts.h"

using namespace std;
namespace XmlProcessing
{
  class XmlDocument
  {
  public:
	  
	  XmlDocument(const XmlDocument& xmlDOM) = delete;
	  XmlDocument(XmlDocument&& xmlDOM);
	  XmlDocument& operator=(const XmlDocument& xmlDom) = delete;
	  XmlDocument& operator=(XmlDocument&& xmlDom);

    using sPtr = std::shared_ptr < AbstractXmlElement > ;
	using attribsVec = std::vector < std::pair<std::string, std::string> > ;
	std::string source, sType;

    enum sourceType { string, filename };

	stack<sPtr> nodeStack;

	XmlDocument();
    XmlDocument(const std::string& src, sourceType srcType=string);

    
    // queries return XmlDocument references so they can be chained, e.g., doc.element("foobar").descendents();

    XmlDocument& element(const std::string& tag, sPtr pNode);    // found_[0] contains first element (DFS order) with tag
    XmlDocument& elements(const std::string& tag, sPtr pNode);   // found_ contains all elements with tag
	XmlDocument& children(sPtr pNode, const std::string& tag = "");  // found_ contains sPtrs to children of prior found_[0] 
    XmlDocument& descendents(const std::string& tag = "");       // found_ contains sPtrs to descendents of prior found_[0]
    std::vector<sPtr>& select();                                 // return reference to found_.  Use std::move(found_) to clear found_
	attribsVec attributes(sPtr pNode = nullptr);						 // returns a vector of attributes, or an empty vector if no attributes
	const sPtr getDOM();
	sPtr ParseNewNode(std::string& str);
	sPtr createNode(std::string& str);
	const sPtr getNewElem();
	sPtr searchForId(std::shared_ptr<AbstractXmlElement> pNode);
	sPtr getElementById(const std::string& str);
	void resetFound();
	bool addAttrib(const std::string& name, const std::string& value, XmlDocument::sPtr parent = nullptr);
	bool removeAttrib(const std::string& name, XmlDocument::sPtr parent = nullptr);
	bool addChild(XmlDocument::sPtr child, XmlDocument::sPtr parent = nullptr);
	bool removeChild(XmlDocument::sPtr child, XmlDocument::sPtr parent = nullptr);
	bool removeChild(const std::string& value, XmlDocument::sPtr parent = nullptr);
	void resetTab() { pDocElement_->resetTab(); }
	


  private:
    sPtr pDocElement_ = nullptr;         // AST that holds procInstr, comments, XML root, and more comments
    std::vector<sPtr> found_;  // query results
	sPtr pRoot_, currentNode;
	sPtr XmlDeclar;
	sPtr newElem;
	size_t position_;
	std::string searchId;
	sPtr foundId;
	stack<sPtr> tempStack;

	//helper functions
	bool isComment(std::string&);
	bool isText(std::string&);
	bool isTagged(std::string&);
	bool isProcInstr(std::string&);
	bool isXmlDeclar(std::string&);
	bool isClosingTag(std::string& str);
	bool isSelfClosingTag(std::string& str);
	std::string trimFront(std::string& str);
	void parseNode(std::string& str);
	void createTaggedNode(std::string& str);
	void createXmlDeclarNode(std::string& str);
	void createPIENode(std::string& str);
	bool manageTaggedNode(std::string& str);
	bool manageTNode(std::string& str);
	std::string extractIdentifier(std::string& str, size_t& pos);
	attribsVec extractAttributes(sPtr pNode);
	attribsVec attributes(std::string str);
	bool specialChar(char ch);
	bool next(std::string str);

  };
  inline const XmlDocument::sPtr XmlDocument::getDOM() { return pDocElement_; }
  inline const XmlDocument::sPtr XmlDocument::getNewElem() { return newElem; }
  
}
#endif
