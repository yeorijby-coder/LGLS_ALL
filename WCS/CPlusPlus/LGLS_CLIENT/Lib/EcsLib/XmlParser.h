

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 Name:        XML_PARSER.h
 Version:     2.0
 Purpose:     Simple MSXML library Wrapper class for MFC/C++ (header file)
 Author:      Andr?S?astien  (maximus@oreka.com)
 Copyright:   (c) ANDRE S?astien
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

#ifndef __XMLPARSER_H__
#define __XMLPARSER_H__

#include <afxtempl.h>

//^^^^^^Load MSXML library
#import <msxml3.dll> named_guids
using namespace MSXML2;


class AFX_EXT_CLASS CXmlParser
{
public:
	CXmlParser();                                                  // C'tor
	virtual ~CXmlParser();                                         // D'tor

public:
	MSXML2::IXMLDOMDocument2Ptr m_plDomDocument;							   // XML document pointer
	MSXML2::IXMLDOMElementPtr m_pDocRoot;								   // Root node of XML document
	MSXML2::IXMLDOMNodePtr m_pCurrentNode;				   				   // The current Node added
	MSXML2::IXMLDOMNodeListPtr m_pCurrentNodeList;

	CString m_strErrMsg;										   // Error Status String
	
	// Attrib Values
	//
	CString m_strTag;                                              // The last Tag (the name with "<...>")
	CString m_strName;                                             // The Name (without "<...>")
	CString m_strChainTag;                                         // The Chain Tag
	CString m_strText;                                             // The Text Value
	BOOL    m_bInitialized;

	CStringList m_AttribNames;
	CStringList m_AttribValues;

	POSITION m_IndexPos;
	int m_nIndex;

public:
	BOOL IsInitialized();
	// -- ********************************* --
	// -- *** LOADING / SAVING DOCUMENT *** --
	// -- ********************************* --
	BOOL LoadXmlFile(LPCTSTR strFileName);					       // Load an XML Document from File
	BOOL SaveXmlFile(LPCTSTR strFileName);			               // Save the Current XML Document into a file
	BOOL LoadXmlString(LPCTSTR source);						       // Load an XML Document from Buffer
	void Reset();			                                       // Clear the XML Document

	// -- ************************** --
	// -- *** XML REPRESENTATION *** --
	// -- ************************** --	
	CString GetXML();										       // Get the XML Representation of the current node
	CString GetRootXML();			                           // Get the XML Representation of the entire document
	
	// -- ***************************** --
	// -- *** XML Header management *** -- 
	// -- ***************************** --
	BOOL SetHeader(LPCTSTR header, LPCTSTR name, LPCTSTR value);   // Set an header property like <? xml version="1.0"?>
	BOOL GetHeader(LPCTSTR header, LPCTSTR name, CString & res);   // Return the header property like <? xml version="1.0"?>
																   // Rq: For request the version number like the example below
																   //     you must specify header = "xml" and name = "version"

	virtual void ParseXmlDocument() {};                            // ===> YOU MUST IMPLEMENT IT FOR PARSING PROCESS <===

	// -- *********************************
	// -- *** CURRENT NODE TYPE CONTROL ***
	// -- *********************************
	BOOL IsTag(LPCTSTR aTag);                                      // Return true if it's equal to the Current Tag
	BOOL IsChildof(LPCTSTR parent_chain);                          // Test if a chain Tag is the parent of current Tag
	BOOL Is_Root();                                                // Test if a Tag is the Root
	BOOL IsRoot(); 
	BOOL IsTextNode();                                             // Return true if it's a TEXT node    
	BOOL IsCDataSection();                                         // Return true if it's a CDATA section 
	
	// -- *******************************************
	// -- *** CURRENT NODE TYPE ACCESS PROPERTIES ***
	// -- *******************************************
	CString & GetCurrentTag();                                     // Get the Current Tag value  (with "<>")
	CString & GetCurrentName();                                    // Get the Current Name value (without "<>")
	CString & GetTextValue();                                      // Get the Text Value when it's a TEXT or CDATA node
	BOOL	  GetNodeList(CString strNode);						   // Get the Text Value when it's a NodeList

	BOOL      SetTextValue(LPCTSTR TextValue);                     // Set a Text to the Current Node
	// on a CData section, this method set the data
	
	// -- ************************* --
	// -- *** ATTRIBUTES ACCESS *** --
	// -- ************************* --
	BOOL IsHavingAttribute(CString strName);                       // Return true if current node have the specified attribute defined
	BOOL GetAttributeValue(CString &strValue);                     // Return the attribute value selected by "IsHavingAttribute()"
	
	int  GetAttributeCount();                                      // Return the number of attributes for the current node
	BOOL GetAttributeName(int index, CString &strName);			   // Return the attribute name  for the nth attribute
	BOOL GetAttributeValue(int index, CString &strValue);          // Return the attribute value for the nth attribute
	BOOL GetAttributeValue(int nIndex, CString strName, CString &strValue);
	BOOL GetParentAttribute(CString strName, CString &strValue);

	BOOL SetAttribute(LPCTSTR AttribName, LPCTSTR AttribValue);    // Set an attribute to the Current Node
	BOOL RemoveAttribute(LPCTSTR AttribName);                      // Remove an attribute the the Current Node
	
	// -- ****************************** --
	// -- *** CDATA SECTION CREATION *** --
	// -- ****************************** --
	BOOL AddLastChildCData(LPCTSTR pstrData);                      // Add a new CDATA Section at the End of Current Node
	BOOL AddFirstChildCData(LPCTSTR pstrData);                     // Add a new CDATA Section at the Begining of Current Node
	BOOL AddCDataBefore(LPCTSTR pstrData);                         // Add a new CDATA Section (same level) before the Current Node
	BOOL AddCDataAfter(LPCTSTR pstrData);                          // Add a new CDATA Section (same level?after the Current Node
																   // Rq: When Current Node is a CDATA Section, 
																   //     use GetTextValue() for retrieve DATA
																   //     and SetTextValue() for sets DATA
	
	// -- ********************* --
	// -- *** NODE CREATION *** --
	// -- ******************** --
	BOOL AddLastChildNode(LPCTSTR pstrNode);                       // Add a new CHILD NODE at the End of Current Node
	BOOL AddFirstChildNode(LPCTSTR pstrNode);                      // Add a new CHILD NODE at the Begining of Current Node
	BOOL AddNodeBefore(LPCTSTR pstrNode);                          // Add a new NODE (same level) before the Current Node
	BOOL AddNodeAfter(LPCTSTR pstrNode);                           // Add a new NOFR (same level) after the Current Node
	
	// -- *********************
	// -- *** NODE DELETION ***
	// -- *********************
	BOOL Remove();                                                 // Remove the Current Node (Move to the Parent Node)
	BOOL RemoveChild(LPCTSTR NodeName);                            // Remove a Child Node     (No Move);
	
	// -- ******************* --
	// -- *** NODE MOVING *** --
	// -- ******************* --
	void GoToRoot();                                               // Go to the Root node
	BOOL GoToChild();                                              // Go to the first child of the current node
	BOOL GoToParent();                                             // Go to the parent node of the current node
	BOOL GoForward();                                              // Go to the next     node of same level as the current node
	BOOL GoBackward();                                             // Go to the previous node of same level as the current node
	
	// -- *************************** --
	// -- *** NODE MOVING by name *** --
	// -- *************************** --
	BOOL GoToChild(LPCTSTR NodeName);                              // Go to a Direct Child Node
	BOOL GoToDescendant(LPCTSTR NodeName);                         // Go to a Descendant Node (Child, Little Child, Etc...)
	BOOL GoToParent(LPCTSTR NodeName);                             // Go to a Parent Node
	BOOL GoForward(LPCTSTR NodeName);                              // Go to a next Node at the same Node than the Current
	BOOL GoBackward(LPCTSTR NodeName);                             // Go to a previous Node attached at the same Node than the Current
	
	// -- ************************ --
	// -- *** Error Management *** --
	// -- ************************ --
	CString & GetLastError();                                     // Get the last error signification

private:
	// Node Value
	//

public:
	BOOL Init_MSXML();
	
	BOOL IsParentAttribute(CString strName, CString strValue);

	void ParsingObjects(MSXML2::IXMLDOMNodePtr pObject);
	void ParsingObjects(MSXML2::IXMLDOMNodePtr pObject, int nMaxLevel, int nLevel = 0);

	void GetNodeInformations(MSXML2::IXMLDOMNodePtr pChild);
	HRESULT GetNodeType(MSXML2::IXMLDOMNodePtr pChild);
	
	void AddChainTag(CString & val);
	void RemoveChainTag(int number);
	
	// Internal adding node
	BOOL AddLastChild(MSXML2::IXMLDOMNodePtr newNode);       
	BOOL AddFirstChild(MSXML2::IXMLDOMNodePtr newNode);                  
	BOOL AddBefore(MSXML2::IXMLDOMNodePtr newNode);                      
	BOOL AddAfter(MSXML2::IXMLDOMNodePtr newNode);                      
};

#endif
