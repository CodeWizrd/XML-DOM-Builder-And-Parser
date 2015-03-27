///////////////////////////////////////////////////////////////////
// XmlDocument.cpp - a container of XmlElement nodes             //
// Ver 1.2                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Dell Inspiron 7520, Win 8.1,					 //
//				Visual Studio 2013								 //
// Authors:     Sampath Toragaravalli Janardhan (315-664-8206)   //
//				sampathjanardhan@gmail.com						 //
//				Jim Fawcett, CST 4-187, 443-3948                 //
//              jfawcett@twcny.rr.com                            //
///////////////////////////////////////////////////////////////////

#include <iostream>
#include "XmlDocument.h"

using namespace XmlProcessing;

//----< Construct empty DOM >--------------------------------------------
XmlProcessing::XmlDocument::XmlDocument()
{
	sPtr declarElement = makeXmlDeclarElement();
	pDocElement_ = makeDocElement(declarElement);
}

//----< Construct DOM from input >---------------------------------------
XmlProcessing::XmlDocument::XmlDocument(const std::string& src, sourceType srcType)
{
	sPtr nodePtr;
	bool isFile;
	source = src;
	sType = to_string(srcType);
	if (srcType == string) {
		isFile = false;
	}
	else if (srcType == filename) {
		isFile = true;
	}
	try
	{
		Toker toker(src, isFile);
		toker.setMode(Toker::xml);
		XmlParts parts(&toker);
		while (parts.get()){
			std::string str;
			str.assign(parts.show());
			currentNode = nullptr;
			parseNode(str);
		}
		if (pDocElement_ == nullptr)
			pDocElement_ = makeDocElement(pRoot_);
		if (pRoot_ != nullptr)
			pDocElement_->addChild(pRoot_);
		
	}
	catch (std::exception ex)
	{
		std::cout << "\n  " << ex.what() << "\n\n";
	}

}

//----< Move Constructor >------------------------------------------------
XmlProcessing::XmlDocument::XmlDocument(XmlDocument&& xmlDOM) : pDocElement_(xmlDOM.pDocElement_), pRoot_(xmlDOM.pRoot_), 
	XmlDeclar(xmlDOM.XmlDeclar)
{
	cout << "\n\n\n Entering Move Constructor ";
	xmlDOM.pDocElement_ = nullptr;
	xmlDOM.pRoot_ = nullptr;
	xmlDOM.XmlDeclar = nullptr;
	if (pDocElement_ != nullptr)
		pDocElement_->resetTab();
	cout << "\n Exiting Move Constructor \n\n\n";
}

//----< Move Assignment Operator >----------------------------------------
XmlDocument& XmlDocument::operator=(XmlDocument&& XmlDom)
{
	cout << "\n\n Entering Move Assignment ";
	if (this == &XmlDom)
		return *this;
	this->pDocElement_ = XmlDom.pDocElement_;
	this->pRoot_ = XmlDom.pRoot_;
	this->XmlDeclar = XmlDom.XmlDeclar;

	XmlDom.pDocElement_ = nullptr;
	XmlDom.pRoot_ = nullptr;
	XmlDom.XmlDeclar = nullptr;
	if (pDocElement_ != nullptr)
		pDocElement_->resetTab();

	cout << "\n Exiting Move Assignment \n\n";

	return *this;
}

//----< Parse string and create respective nodes >-------------------------
void XmlDocument::parseNode(std::string& str)
{
	bool bisTagged = false;
	if (isComment(str)) 
	{
		size_t pos = str.find("< ! --");
		if (pos != std::string::npos)
			str = str.erase(0, pos+6);
		pos = str.find("-- >");
		if (pos != std::string::npos)
			str = str.erase(pos, 4);
		currentNode = makeCommentElement(str);
		if (pRoot_ == nullptr)
			pDocElement_->addChild(currentNode);
		else
			(nodeStack.top())->addChild(currentNode);
		return;
	}
	else if (isXmlDeclar(str)) 	{
		createXmlDeclarNode(str);
		return;
	}
	else if (isProcInstr(str)) {
		createPIENode(str);
		return;
	}
	else if (isTagged(str))
		bisTagged = manageTaggedNode(str);
	else
		currentNode = makeTextElement(str);
	if (!nodeStack.empty() && currentNode != nullptr)
		(nodeStack.top())->addChild(currentNode);
	if (bisTagged == true)
		nodeStack.push(currentNode);
}

//----< Handle Tagged Nodes  >---------------------------------------------
bool XmlDocument::manageTaggedNode(std::string& str)
{
	bool isOpenTag = false;
	if (isClosingTag(str) == true)
		nodeStack.pop();
	else if (isSelfClosingTag(str) == true) {
		createTaggedNode(str);
		if (!nodeStack.empty())
			(nodeStack.top())->addChild(currentNode);
	}
	else
	{
		createTaggedNode(str);
		isOpenTag = true;
	}
	if (pRoot_ == nullptr)
		pRoot_ = currentNode;
	return isOpenTag;
}

//----< Create a TagElement & extract attributes >-------------------------
void XmlDocument::createTaggedNode(std::string& str)
{
	size_t pos = 0;
	std::string tag = extractIdentifier(str, pos);
	currentNode = makeTaggedElement(tag);
	if (next(str))
	{
		attribsVec attributes_ = attributes(str);
		for (size_t i = 0; i < attributes_.size(); ++i)
		{
			currentNode->addAttrib(attributes_[i].first, attributes_[i].second);
		}
	}
}

//----< Create a Declare Element & extract attributes >--------------------
void XmlDocument::createXmlDeclarNode(std::string& str)
{
	size_t pos;
	str = trimFront(str);
	pos = str.find("?");
	while (pos != std::string::npos)
	{
		str.erase(pos, 1);
		pos = str.find("?");
	}
	str = trimFront(str);

	currentNode = makeXmlDeclarElement();
	if (next(str))
	{
		attribsVec attributes_ = attributes(str);
		for (size_t i = 0; i < attributes_.size(); ++i)
		{
			currentNode->addAttrib(attributes_[i].first, attributes_[i].second);
		}
	}
	XmlDeclar = currentNode;
	pDocElement_ = makeDocElement(currentNode);
}

//----< Create a Processing Instruction Element & extract attributes >-----
void XmlDocument::createPIENode(std::string& str)
{
	size_t pos;
	std::string content = str;
	std::string type;
	pos = str.find("?");
	content = content.erase(0, pos + 1);
	pos = content.find("?");
	content = content.erase(pos, 1);
	content = trimFront(content);

	pos = 0;
	while (true)
	{
		char ch = content[pos];
		if (!(specialChar(ch)))
		{
			type += ch;
			pos++;
		}
		else
		{
			char nextch = content[pos+1];
			if (isspace(ch) && nextch == '-')
			{
				type += nextch;
				pos += 3;
				continue;
			}

			else break;
		}
	}

	content = content.erase(0, pos);
	currentNode = makeProcInstrElement(type);
	content = "PIE " + content;

	attribsVec attributes_ = attributes(content);
	for (size_t i = 0; i < attributes_.size(); ++i)
	{	
		currentNode->addAttrib(attributes_[i].first, attributes_[i].second);
	}
	pDocElement_->addChild(currentNode);
}

//----< Programmatic creation of nodes (not reading XML) >-----------------
XmlDocument::sPtr XmlDocument::createNode(std::string& str)
{
	sPtr newNode;
	newElem = nullptr;
	try
	{
		Toker toker(str, false);
		toker.setMode(Toker::xml);
		XmlParts parts(&toker);
		while (parts.get()){
			std::string token;
			token.assign(parts.show());
			currentNode = nullptr;
			newNode = ParseNewNode(token);
		}
	}
	catch (std::exception ex)
	{
		std::cout << "\n  " << ex.what() << "\n\n";
	}
	return newNode;
}

//----< Parse string and create respective nodes >-------------------------
XmlDocument::sPtr XmlDocument::ParseNewNode(std::string& str)
{
	bool bisTagged = false;
	/*
	if (isComment(str)) {
	//tempNode = makeCommentElement(str);
	return tempNode;
	}
	else if (isProcInstr(str)) {
	//tempNode = makeProcInstrElement(str);
	return tempNode;
	}
	else if (isXmlDeclar(str)) 	{
	cout << "Node contains XML Declare Element!\n";
	return tempNode;
	}

	else*/ if (isTagged(str))
		bisTagged = manageTNode(str);
	else
		currentNode = makeTextElement(str);
	if (!tempStack.empty() && currentNode != nullptr)
		(tempStack.top())->addChild(currentNode);
	if (bisTagged == true)
		tempStack.push(currentNode);

	return currentNode;
}

//----< Manage new TagElement nodes during programmatic creation >---------
bool XmlDocument::manageTNode(std::string& str)
{
	bool isOpenTag = false;
	if (isClosingTag(str) == true)
		tempStack.pop();
	else if (isSelfClosingTag(str) == true) {
		createTaggedNode(str);
		if (!tempStack.empty())
			(tempStack.top())->addChild(currentNode);
	}
	else
	{
		createTaggedNode(str);
		isOpenTag = true;
	}
	if (newElem == nullptr)
		newElem = currentNode;

	return isOpenTag;
}

//----< Trim leading whitespaces >-----------------------------------------
std::string XmlDocument::trimFront(std::string& str)
{
	while (true)
	{
		if (isspace(str[0]))
			str = str.erase(0, 1);
		else if (str.compare("<") == 0 && str.compare(" ") == 1)
			str = str.erase(0, 1);
		else break;
	}
	return str;
}

//----< move to next XML tag >---------------------------------------------
bool XmlDocument::next(std::string str)
{
	position_ = 0;
	while (true)
	{
		position_ = str.find('<', position_);
		if (position_ >= str.size())
			return false;
		++position_;
		if (str[position_] != '/')
			break;
	}
	return true;
}

//----< Extract attributes >-----------------------------------------------
XmlDocument::attribsVec XmlDocument::attributes(std::string str)
{
	attribsVec attributes;
	attributes.clear();
	if (isComment(str))
		return attributes;
	size_t localposition = position_;
	// move past tag
	extractIdentifier(str, localposition);

	// find attributes
	size_t locpos = str.find('>', localposition);
	while (true)
	{
		std::string name = extractIdentifier(str, localposition);
		if (locpos < localposition)
			return attributes;
		std::string value = extractIdentifier(str, localposition);
		if (locpos < localposition)
			throw std::exception("malformed XML");
		std::pair<std::string, std::string> elem;
		elem.first = name;
		elem.second = value;
		attributes.push_back(elem);
	}
	return attributes;
}

//----< Helper for searching by attribute name >---------------------------
XmlDocument::sPtr XmlDocument::searchForId(std::shared_ptr<AbstractXmlElement> pNode)
{
	sPtr match;
	for (auto pChild : pNode->children())
	{
		for (auto iter : pChild->getAttribs())
		{
			if (searchId == iter.first)
			{
				foundId = pChild;
				return pChild;
			}
		}
		searchForId(pChild);
	}
	return match;
}

//----< Search for tag by attribute name >---------------------------------
XmlDocument::sPtr XmlDocument::getElementById(const std::string& tag)
{
	searchId = tag;
	foundId = nullptr;
	searchForId(pDocElement_);
	return foundId;
}

//----< Is it a comment element? >-----------------------------------------
bool XmlDocument::isComment(std::string& str)
{
	if (str.find("<!--") != string::npos || str.find("< ! --") != string::npos)
	{
		return true;
	}
	return false;
}

//----< Is it a processing instruction? >----------------------------------
bool XmlDocument::isProcInstr(std::string& str)
{
	if (str.find("<?") != string::npos || str.find("< ?") != string::npos)
	{
		return true;
	}
	return false;
}

//----< Is it a tag element? >---------------------------------------------
bool XmlDocument::isTagged(std::string& str)
{
	if (str.find("<") != string::npos)
	{
		return true;
	}
	return false;
}

//----< Is it an XML declaration? >----------------------------------------
bool XmlDocument::isXmlDeclar(std::string& str)
{
	if ((str.find("<?") != string::npos || str.find("< ?") != string::npos) && str.find("xml") != string::npos && str.find("version") != string::npos)
	{
		return true;
	}
	return false;
}

//----< Is it a closing tag? >---------------------------------------------
bool XmlDocument::isClosingTag(std::string& str)
{
	if (str.find("</") != string::npos || str.find("< /") != string::npos)
	{
		return true;
	}
	return false;
}

//----< Is it a self-closing tag? >----------------------------------------
bool XmlDocument::isSelfClosingTag(std::string& str)
{
	if (str.find("/>") != string::npos || str.find("/ >") != string::npos)
	{
		return true;
	}
	return false;
}

//----< helper finds identifiers >------------------------------------------
std::string XmlDocument::extractIdentifier(std::string& str, size_t & pos)
{
	std::string ident;
	while (true)
	{
		if (pos == str.size())
			return ident;
		char ch = str[pos];
		if (specialChar(ch))
			pos++;
		else
			break;
	}
	while (true)
	{
		char ch = str[pos];
		if (specialChar(ch))
			break;
		ident += ch;
		++pos;
	}
	return ident;
}

//----< helper identifies markup chars >------------------------------------
bool XmlDocument::specialChar(char ch)
{
	bool test = isspace(ch) || ch == '/' || ch == '>' || ch == '<' || ch == '=';
	test = test || ch == '\'' || ch == '"' || /*ch == '-' ||*/ ch == '!';
	return test;
}

//----< Returns the found_ vector >-----------------------------------------
std::vector<XmlDocument::sPtr>& XmlDocument::select()
{
	return std::move(found_);
}

//----< Search for first occurrence of a tag >-------------------------------
XmlDocument& XmlDocument::element(const std::string& tag, std::shared_ptr<AbstractXmlElement> pNode)
{
	if (found_.empty())
	{
		for (auto pChild : pNode->children())
		{
			if (tag == pChild->value())
			{
				found_.push_back(pChild);
				break;
			}
			element(tag, pChild);
		}
	}
	
	return *this;
}

//----< Search for all occurrences of a tag >--------------------------------
XmlDocument& XmlDocument::elements(const std::string& tag, std::shared_ptr<AbstractXmlElement> pNode)
{
	for (auto pChild : pNode->children())
	{
		if (tag == pChild->value())
		{
			found_.push_back(pChild);
		}
		elements(tag, pChild);
	}
	return *this;
}

//----< return a vector of attributes >--------------------------------------
XmlDocument::attribsVec XmlDocument::attributes(sPtr pNode)
{
	attribsVec attr;
	if (pNode == nullptr)
	{
		if (!found_.empty())
			pNode = found_[0];
		else
		{
			cout << "\n Element not present in the Tree! \n";
			return attr;
		}
	}
	return pNode->getAttribs();
}

//----< return a vector of pointers to children  >----------------------------
XmlDocument& XmlDocument::children(sPtr pNode, const std::string& tag)
{
	std::string found = tag;
	if (found == "")
	{

		if (!found_.empty())
		{
			found = found_[0]->value();
		}
	}
	for (auto pChild : pNode->children())
	{
		if (found == pChild->value())
		{
			found_ = pChild->children();
		}
		children(pChild, found);
	}
	return *this;
}

//----< clear the found_ vector >---------------------------------------------
void XmlDocument::resetFound()
{
	found_.clear();
}

//----< add attributes >------------------------------------------------------
bool XmlDocument::addAttrib(const std::string& name, const std::string& value, XmlDocument::sPtr node)
{
	sPtr parent = node;
	if (parent == nullptr)
	{
		if (!found_.empty())
			parent = found_.at(0);
		else
			cout << "Null reference to element!\n";
	}
	return parent->addAttrib(name, value);
}

//----< remove attributes >---------------------------------------------------
bool XmlDocument::removeAttrib(const std::string& name, XmlDocument::sPtr node)
{
	sPtr parent = node;
	if (parent == nullptr)
	{
		if (!found_.empty())
			parent = found_.at(0);
		else
			cout << "\n Invaid reference to element!\n";
	}
	return parent->removeAttrib(name);
}

//----< add child element >---------------------------------------------------
bool XmlDocument::addChild(XmlDocument::sPtr child, XmlDocument::sPtr parent)
{
	if (parent == nullptr)
	{
		if (!found_.empty())
			parent = found_.at(0);
		else
		{
			cout << "\n Invaid reference to element!\n";
			return false;
		}
	}
	return parent->addChild(child);
}

//----< remove child element through a reference to the child >----------------
bool XmlDocument::removeChild(XmlDocument::sPtr child, XmlDocument::sPtr parent)
{
	if(parent == nullptr)
	{
		if (!found_.empty())
			parent = found_.at(0);
		else
		{
			cout << "Null reference to element! Will DFS from the Root to find element.\n";
			parent = pDocElement_;
		}
	}
	return parent->removeChild(child);
}

//----< remove child element by searching based on tag------- >----------------
bool XmlDocument::removeChild(const std::string& value, XmlDocument::sPtr parent)
{
	if(parent == nullptr)
	{
		if (!found_.empty())
			parent = found_.at(0);
		else
		{
			cout << "Null reference to element! Will DFS from the Root to find element.\n";
			parent = pDocElement_;
		}
	}
	return parent->removeChild(value);
}


//Test Stub
#ifdef TEST_XMLDOCUMENT

int main()
{
	title("Testing XmlDocument class");

	//XmlDocument doc("foobar");

	std::cout << "\n ===================\n";
	std::cout << "\n  String Input \n";
	std::cout << "\n ===================\n";

	std::string Prologue = "<?xml version=\"1.0\"?><!-- top level comment -->";
	std::string test1 = Prologue;
	test1 += "<parent att1='val1' att2='val2'><child1 /><child2>child2 body<child1 /></child2></parent>";
	std::string test2 = Prologue;
	test2 += "<Dir><path>/TestFile/Dir1</path><File date='notCurrent'>Sampath<name type='file'>text1.txt</name><date>02/11/2012</date></File></Dir>";

	//XmlDocument doc1(test1);
	XmlDocument doc2(test2);

	std::cout << "\n ===================\n";
	std::cout << "\n  File Input \n";
	std::cout << "\n ===================\n";
	std::string fname = "LectureNote.xml";
	XmlDocument docf(fname, XmlDocument::filename);

	std::cout << "\n\n";
}
#endif