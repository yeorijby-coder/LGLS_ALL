/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//	 Name:		  CXmlParser.cpp
//	 Version:	  2.0
//	 Purpose:	  Simple MSXML library Wrapper class for MFC/C++ (source file)
//	 Author:	  Andr?S?astien  (maximus@oreka.com)
//	 Copyright:   (c) ANDRE S?astien
//
//	 Version 2.0 What's news ?
// 
//		1. The parsing document start from document and not from root element
//			 - Let us to parse the header document informations
//		2. New XML header management
//			 - encoding format, stylesheet reference, etc...
//			 - SetHeader("xml","version","1.0")
//			 - SetHeader("xml","encoding","UTF-8"), etc...
//		3. New CData section management 
//			 - We can now read and add CData section with method like "AddLastChildCData(LPCTSTR data)"
//		4. Some new methods for more fun
//			 - like "Get_XML_Document()"
//		5. Minor design change for improve the class
//			 - Some methods have been renamed like "Get_Text()" to "GetTextValue()"
//		
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

#include <stdafx.h>
#include <atlbase.h>
#include "XmlParser.h"

CXmlParser::CXmlParser()
{
	m_strTag      = _T("");
	m_strName     = _T("");
	m_strChainTag = _T("");
	m_strText     = _T("");

	m_AttribNames.RemoveAll();
	m_AttribValues.RemoveAll();
	m_nIndex      = -1;
	m_IndexPos    = NULL;

	
	// Init MSXML members
	m_plDomDocument = NULL;
	m_pDocRoot      = NULL;
	m_pCurrentNode  = NULL;
	m_pCurrentNodeList = NULL;

	// -- Errors Init --
	m_strErrMsg = _T("");

	m_bInitialized = Init_MSXML();
}

CXmlParser::~CXmlParser()
{
	// Free ressource
    //Reset();
}

BOOL CXmlParser::IsInitialized()
{
	return m_bInitialized;
}

CString & CXmlParser::GetLastError()
{
	return m_strErrMsg;
}

BOOL CXmlParser::Init_MSXML()
{
    // -- Instantiate an XML document --
    HRESULT hr = m_plDomDocument.CreateInstance(MSXML2::CLSID_DOMDocument);
    if (FAILED(hr))
    {
		_com_error er(hr);
        m_strErrMsg = er.ErrorMessage();
        return FALSE;
    }

	//async == TRUE면 load나 loadXML함수가 XML을 완전히 로드 하기전 리턴
	//async == TRUE면 객체 접근전 ReadyState을 체크후 처리
	m_plDomDocument->async = FALSE;

	//문서 유효성 검사 안함
	m_plDomDocument->validateOnParse = FALSE;

    return TRUE;
}

void CXmlParser::Reset()
{
	m_strTag      = _T("");
	m_strName     = _T("");
	m_strChainTag = _T("");
	m_strText     = _T("");

	m_AttribNames.RemoveAll();
	m_AttribValues.RemoveAll();
	m_nIndex		= -1;
	m_IndexPos		= NULL;

	m_pCurrentNode	= NULL;
}

BOOL CXmlParser::LoadXmlFile(LPCTSTR strFileName)
{
	m_strErrMsg = _T("");

	// Reset Document
	Reset();

	// Convert xml file name string to something COM can handle (BSTR)
	CString sFileName = strFileName;
	_bstr_t bstrFileName = sFileName.AllocSysString();

	VARIANT_BOOL vResult = m_plDomDocument->load(bstrFileName);
	if ( vResult == FALSE )
	{
		MSXML2::IXMLDOMParseErrorPtr pParseError = m_plDomDocument->parseError;
		m_strErrMsg.Format(_T("LinePos[%d] 에러=%s"), pParseError->Getlinepos(), (LPCTSTR)pParseError->Getreason());
		return FALSE;
	}

	m_pDocRoot = m_plDomDocument->documentElement;

//	ParsingObjects(m_plDomDocument);
	GoToRoot();

	return TRUE;
}

BOOL CXmlParser::SaveXmlFile(LPCTSTR strFileName)
{
	// Save the XML document
	m_plDomDocument->save(strFileName);
	
	return true;
}

BOOL CXmlParser::LoadXmlString(LPCTSTR strSource)
{
	Reset();

	VARIANT_BOOL vResult = m_plDomDocument->loadXML(strSource);
	if ( vResult == FALSE )
	{
		MSXML2::IXMLDOMParseErrorPtr pParseError = m_plDomDocument->parseError;
		m_strErrMsg.Format(_T("LinePos[%d] 에러=%s"), pParseError->Getlinepos(), (LPCTSTR)pParseError->Getreason());
		return FALSE;
	}

	m_pDocRoot = m_plDomDocument->documentElement;

//	ParsingObjects(m_plDomDocument);
	GoToRoot();

	return TRUE;
}

CString CXmlParser::GetXML()
{
	if(m_pCurrentNode == m_pDocRoot)
		return GetRootXML();

	BSTR  pstrXml;
	m_pCurrentNode->get_xml(&pstrXml);
	
	return (LPCTSTR)pstrXml;
}

CString CXmlParser::GetRootXML()
{
	BSTR  pstrXml;
    m_plDomDocument->get_xml(&pstrXml);

	return (LPCTSTR)pstrXml;
}

void CXmlParser::ParsingObjects(MSXML2::IXMLDOMNodePtr pObject)
{
	GetNodeInformations(pObject);

	// Update "m_pCurrentNode"
	m_pCurrentNode = pObject;

	ParseXmlDocument();

	if(GetNodeType(pObject) != MSXML2::NODE_ELEMENT)
		return;

	// add the m_strChainTag
	AddChainTag(m_strTag);

	for (MSXML2::IXMLDOMNodePtr pChild = pObject->firstChild; pChild != NULL;  pChild = pChild->nextSibling)
	{
		// Parse Child nodes
		ParsingObjects(pChild);
	}

	// Remove Current Tag from m_strChainTag
	RemoveChainTag(1);
}

void CXmlParser::ParsingObjects(MSXML2::IXMLDOMNodePtr pObject, int nMaxLevel, int nLevel)
{
	GetNodeInformations(pObject);

	// Update "m_pCurrentNode"
	m_pCurrentNode = pObject;

	ParseXmlDocument();

	if(GetNodeType(pObject) != MSXML2::NODE_ELEMENT)
		return;

	// add the m_strChainTag
	AddChainTag(m_strTag);

	nLevel++;
	for (MSXML2::IXMLDOMNodePtr pChild = pObject->firstChild; pChild != NULL && nLevel <= nMaxLevel;  pChild = pChild->nextSibling)
	{
		// Parse Child nodes
		ParsingObjects(pChild, nMaxLevel, nLevel);
	}

	// Remove Current Tag from m_strChainTag
	RemoveChainTag(1);
}

BOOL CXmlParser::IsTag(LPCTSTR pTag)
{
	return GetCurrentTag() == pTag;
}

BOOL CXmlParser::IsTextNode()
{
	return (IsTag(_T("<#TEXT#>")));
}

BOOL CXmlParser::IsCDataSection()
{
	return (IsTag(_T("<#CDATA#>")));
}

CString & CXmlParser::GetCurrentTag()
{
	return m_strTag; 
}

CString & CXmlParser::GetCurrentName()
{
	return m_strName; 
}

BOOL CXmlParser::Is_Root()
{
	return (m_strChainTag.IsEmpty() != 0); 
}

BOOL CXmlParser::IsRoot()
{
	return (m_pDocRoot == m_pCurrentNode); 
}

BOOL CXmlParser::IsChildof(LPCTSTR parent_chain)
{
	int pos = m_strChainTag.Find(parent_chain);
	int len = strlen(CT2A(parent_chain));

	if(pos != -1)
	{
		// look if it's the end of  chainTag
		return ( m_strChainTag.GetLength() == pos + len );
	}
	
	return false;
}

CString & CXmlParser::GetTextValue()
{
	// Now you can handle the text value on the real node directly if you want
	
	if( GetNodeType(m_pCurrentNode) == MSXML2::NODE_TEXT ||
		GetNodeType(m_pCurrentNode) == MSXML2::NODE_CDATA_SECTION )
		return m_strText;
	
	// We must find his text value
	CString TextValue;
	
	// Find if a CHILD TEXT NODE exist or not
	if(m_pCurrentNode == NULL)
	{
		if(m_pDocRoot != NULL)
			m_pCurrentNode = m_pDocRoot;
		else
		{
			// We can't set the text
			m_strErrMsg = _T("CXmlParser::GetTextValue() Can't set text on NULL node");
			m_strText = _T("");
			return m_strText;
		}
	}
	
	// Find it now
	if(m_pCurrentNode->hasChildNodes())
	{
		MSXML2::IXMLDOMNodePtr pChild;
		for(pChild = m_pCurrentNode->firstChild; pChild != NULL; pChild = pChild->nextSibling)
		{
			// Find it's a NODE TEXT
			if(GetNodeType(pChild) == MSXML2::NODE_TEXT)
			{
				// Take informations from  Text Node
				GetNodeInformations(pChild);
				TextValue = m_strText;
				GetNodeInformations(m_pCurrentNode); // it remove ->m_strText
				m_strText = TextValue;
				return m_strText;
			}
		}
	}

	m_strText = _T("");
	
	return m_strText;
}

HRESULT CXmlParser::GetNodeType(MSXML2::IXMLDOMNodePtr pChild)
{
	if(pChild == NULL) return NULL;
	
	// I handle only few node type other are ignored and are considered as NODE_ELEMENT
	//
	// Handled Node type by  wrapper:
	//      - NODE_ELEMENT
	//      - NODE_TEXT
	//      - NODE_CDATA_SECTION
	//      - NODE_PROCESSING_INSTRUCTION
	
	if(pChild->nodeType == MSXML2::NODE_TEXT) return MSXML2::NODE_TEXT;
	if(pChild->nodeType == MSXML2::NODE_CDATA_SECTION) return MSXML2::NODE_CDATA_SECTION;
	if(pChild->nodeType == MSXML2::NODE_PROCESSING_INSTRUCTION ) return MSXML2::NODE_PROCESSING_INSTRUCTION;
	return MSXML2::NODE_ELEMENT;
}

void CXmlParser::GetNodeInformations(MSXML2::IXMLDOMNodePtr pChild)
{

	m_AttribNames.RemoveAll();
	m_AttribValues.RemoveAll();
	MSXML2::IXMLDOMNamedNodeMapPtr pAttribs = NULL;

	switch(GetNodeType(pChild))
	{
	case	MSXML2::NODE_ELEMENT:
		m_strTag.Format(_T("<%s>"),(const char*)(pChild->nodeName));
		m_strName = (const char*)(pChild->nodeName);
		
		pAttribs = pChild->Getattributes();
		if(pAttribs != NULL)
		{
			for(int i=0; i<pAttribs->Getlength(); ++i)
			{
				MSXML2::IXMLDOMNodePtr pAttrib = pAttribs->Getitem(i);
				if(pAttrib != NULL)
				{
					m_AttribNames.AddTail( (CString)(const char *)pAttrib->GetnodeName() );
					m_AttribValues.AddTail( (CString)(const char *)_bstr_t(pAttrib->GetnodeValue()) );
				}
			}
		}
		break;

	case	MSXML2::NODE_TEXT:
		m_strTag  = _T("<#TEXT#>");
		m_strName = _T("#TEXT#");
		
		m_strText = (const char*)(pChild->text);
		break;
	
	case	MSXML2::NODE_CDATA_SECTION:
		m_strTag  = _T("<#CDATA#>");
		m_strName = _T("#CDATA#");
		
		m_strText = (const char*)(pChild->text);
		break;

	case	MSXML2::NODE_PROCESSING_INSTRUCTION:
		/* Do nothing here */
		break;
	} 
}

BOOL CXmlParser::GetNodeList(CString strNode)
{
	_bstr_t bstrNode = _T("//") + strNode + _T("/*");

	m_AttribNames.RemoveAll();
	m_AttribValues.RemoveAll();

	m_pCurrentNodeList = m_plDomDocument->selectNodes(bstrNode);
	for(int i = 0; i < m_pCurrentNodeList->length; i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = m_pCurrentNodeList->item[i];
		
		m_AttribNames.AddTail((CString)(LPCTSTR)pNode->nodeName);
		m_AttribValues.AddTail((CString)(LPCTSTR)m_pCurrentNodeList->item[i]->text);
	}
	
	return TRUE;
}

void CXmlParser::AddChainTag(CString & val)
{
	if( m_strChainTag.IsEmpty() )
	{
		m_strChainTag = val;
		return;
	}

	m_strChainTag += val;
}

void CXmlParser::RemoveChainTag(int number)
{
	// Remove the n tag to the m_strChainTag
	for(int i = 0; i < number; i++ )
	{
		int pos = m_strChainTag.ReverseFind('<');
		if(pos == -1)
			m_strChainTag.Empty();
		else
			m_strChainTag = m_strChainTag.Left(pos);
	}
}

int CXmlParser::GetAttributeCount()
{
	return m_AttribNames.GetCount();
}

BOOL CXmlParser::GetAttributeName(int nIndex, CString &strName)
{ 
	if(nIndex < 0 || nIndex > GetAttributeCount())
		return FALSE;

	POSITION pos = m_AttribNames.FindIndex(nIndex);
	if( pos == NULL )	return FALSE;

	strName = m_AttribNames.GetAt(pos);
	return TRUE;
}

BOOL CXmlParser::GetAttributeValue(CString &strValue)
{
	if (m_IndexPos == NULL )	return FALSE;

	strValue = m_AttribValues.GetAt(m_IndexPos);
	return TRUE;
}

BOOL CXmlParser::GetAttributeValue(int nIndex, CString &strValue)
{
	if(nIndex < 0 || nIndex > GetAttributeCount())
		return FALSE;

	POSITION pos = m_AttribValues.FindIndex(nIndex);
	if( pos == NULL )	return FALSE;

	strValue = m_AttribValues.GetAt(pos);
	return TRUE;
}

BOOL CXmlParser::GetAttributeValue(int nIndex, CString strName, CString &strValue)
{
	if(nIndex < 0 || nIndex > GetAttributeCount())
		return FALSE;

	POSITION pos1 = m_AttribNames.FindIndex(nIndex);
	POSITION pos2 = m_AttribValues.FindIndex(nIndex);

	if( pos1 == NULL || pos2 == NULL )	return FALSE;

	CString strTemp = m_AttribNames.GetAt(pos1);
	if ( strTemp != strName )		return FALSE;

	strValue = m_AttribValues.GetAt(pos2);
	return TRUE;
}

BOOL CXmlParser::IsHavingAttribute(CString strName)
{
	m_IndexPos = NULL;

	POSITION pos1 = m_AttribNames.GetHeadPosition();
	POSITION pos2 = m_AttribValues.GetHeadPosition();
	
	for ( ; pos1 != NULL && pos2 != NULL; m_AttribNames.GetNext(pos2))
	{
		CString strTemp = m_AttribNames.GetNext(pos1);
		if( strTemp == strName )
		{
			// set index fot let user to retrieve value with "GetAttributeValue()" method
			m_IndexPos = pos2;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXmlParser::AddLastChild(MSXML2::IXMLDOMNodePtr newNode)
{
	// Attach the Node to the document
	if(m_pCurrentNode != NULL)
	{
		if(GetNodeType(m_pCurrentNode) != MSXML2::NODE_ELEMENT) return false;

		AddChainTag( GetCurrentTag() );
		newNode = m_pCurrentNode->appendChild(newNode);
	}
	else if(m_pDocRoot != NULL)
	{ 
		if(GetNodeType(m_pDocRoot) != MSXML2::NODE_ELEMENT) return false;

		AddChainTag( (CString)(const char*)m_pDocRoot->nodeName );
		newNode = m_pDocRoot->appendChild(newNode);
	}
	else
	{
		m_strChainTag.Empty();
		m_pDocRoot = newNode;
		m_plDomDocument->documentElement = m_pDocRoot;
	}
	
	// Update Current Node (cast operation)
	m_pCurrentNode = newNode;
	
	// Update information for  Node
	GetNodeInformations(m_pCurrentNode);
	
	return true;
}

BOOL CXmlParser::AddFirstChild(MSXML2::IXMLDOMNodePtr newNode)
{
	// Create Reference Node for the Insertion
	_variant_t NodeRef = (IUnknown *)m_pCurrentNode->firstChild;
	
	// Attach the Node to the document
	if(m_pCurrentNode != NULL)
	{
		if(GetNodeType(m_pCurrentNode) != MSXML2::NODE_ELEMENT) return false;

		AddChainTag( GetCurrentTag() );
		newNode = m_pCurrentNode->insertBefore(newNode,NodeRef);
	}
	else if(m_pDocRoot != NULL)
	{
		if(GetNodeType(m_pDocRoot) != MSXML2::NODE_ELEMENT) return false;

		AddChainTag( (CString)(const char*)m_pDocRoot->nodeName );
		newNode = m_pDocRoot->insertBefore(newNode,NodeRef);
	}
	else
	{
		m_strChainTag.Empty();
		m_pDocRoot = newNode;
		m_plDomDocument->documentElement = m_pDocRoot;
	}
	
	// Update Current Node (cast operation)
	m_pCurrentNode = newNode;
	
	// Update information for  Node
	GetNodeInformations(m_pCurrentNode);
	
	return true;
}

BOOL CXmlParser::AddBefore(MSXML2::IXMLDOMNodePtr newNode)
{
	if(Is_Root())
	{
		m_strErrMsg = _T("CXmlParser::AddBefore()   : Can't add node at same level that the root node");
		return false;
	}

	// Create Reference Node for the Insertion
	MSXML2::IXMLDOMNodePtr pParent;
	m_pCurrentNode->get_parentNode(&pParent);
	_variant_t NodeRef = (IUnknown *)m_pCurrentNode;

	// Attach the Node to the document
	if(m_pCurrentNode != NULL)
		newNode = pParent->insertBefore(newNode,NodeRef);
	else if(m_pDocRoot != NULL)
		newNode = m_pDocRoot->insertBefore(newNode,NodeRef);
	else
	{
		m_pDocRoot = newNode;
		m_plDomDocument->documentElement = m_pDocRoot;
	}
	
	// Update Current Node (cast operation)
	m_pCurrentNode = newNode;
	
	// Update information for  Node
	GetNodeInformations(m_pCurrentNode);
	
	return true;
}

BOOL CXmlParser::AddAfter(MSXML2::IXMLDOMNodePtr newNode)
{
	// If m_pCurrentNode->NextSibling == NULL then must call AddLastChildNode on Parent Node
	// Because we can't use InsertBefore on a NULL Reference ;o)
	//
	// We are sure that a Parent node exist because before we control that we aren't not on root node.
	if(m_pCurrentNode->nextSibling == NULL)
	{
		// Get Parent Node
		MSXML2::IXMLDOMNodePtr pParent;
		m_pCurrentNode->get_parentNode(&pParent);
		
		// Set Parent node as Current Node
		m_pCurrentNode = pParent;
		GetNodeInformations(pParent);
		
		// Add Node as Last Child Node
		return (AddLastChildNode(m_strName));
	}
	
	// Create Reference Node for the Insertion
	MSXML2::IXMLDOMNodePtr pParent;
	m_pCurrentNode->get_parentNode(&pParent);
	_variant_t NodeRef = (IUnknown *)m_pCurrentNode->nextSibling;
	
	
	// Attach the Node to the document
	if(m_pCurrentNode != NULL)
		newNode = pParent->insertBefore(newNode,NodeRef);
	else if(m_pDocRoot != NULL)
		newNode = m_pDocRoot->insertBefore(newNode,NodeRef);
	else
	{
		m_pDocRoot = newNode;
		m_plDomDocument->documentElement = m_pDocRoot;
	}
	
	// Update Current Node (cast operation)
	m_pCurrentNode = newNode;
	
	// Update information for  Node
	GetNodeInformations(m_pCurrentNode);
	
	return true;
}

BOOL CXmlParser::AddLastChildCData(LPCTSTR pstrData)
{
	// Nodes members
	MSXML2::IXMLDOMNodePtr newNode;
	
	CComVariant vType(MSXML2::NODE_CDATA_SECTION);
	newNode = m_plDomDocument->createNode(vType, _T(""), _T(""));
	
	// Attach the Node to the document
	BOOL Result = AddLastChild(newNode);
	SetTextValue(pstrData);
	
	return Result;
}

BOOL CXmlParser::AddFirstChildCData(LPCTSTR pstrData)
{
    // Nodes members
	MSXML2::IXMLDOMNodePtr newNode;
	
	// If no child then use AddLastChildCData or CurrentElement node not set yet
	if(m_pCurrentNode != NULL)
	{
		if( !m_pCurrentNode->hasChildNodes() )
			return (AddLastChildCData(pstrData));
	}
	else
		return (AddLastChildCData(pstrData));
	
	// Create the NODE
	CComVariant vType(MSXML2::NODE_CDATA_SECTION);
	newNode = m_plDomDocument->createNode(vType, _T(""), _T(""));
	
	
	BOOL Result = AddFirstChild(newNode);
	SetTextValue(pstrData);
	
	return Result;
}

BOOL CXmlParser::AddCDataBefore(LPCTSTR pstrData)
{
	// Nodes members
    // IXMLDOMElementPtr m_pCurrentNode;        /* Global Member */
    MSXML2::IXMLDOMNodePtr newNode;

    // Can't use  function on the root node
    if(Is_Root())
        return false;
        
    // Create the NODE
    CComVariant vType(MSXML2::NODE_CDATA_SECTION);
    newNode = m_plDomDocument->createNode(vType, _T(""), _T(""));

    BOOL Result = AddBefore(newNode);
    SetTextValue(pstrData);

    return Result;
}

BOOL CXmlParser::AddCDataAfter(LPCTSTR pstrData)
{
	// Nodes members
    MSXML2::IXMLDOMNodePtr newNode;

    // Can't use  function on the root node
    if(Is_Root())
    {
		m_strErrMsg = _T("CXmlParser::Add_CDataAfter(LPCTSTR)   : Can't add node at same level that the root node");
        return false;
    }

    // Create the NODE
    CComVariant vType(MSXML2::NODE_CDATA_SECTION);
    newNode = m_plDomDocument->createNode(vType, _T(""), _T(""));

    BOOL Result = AddAfter(newNode);
    SetTextValue(pstrData);

    return Result;
}

BOOL CXmlParser::AddLastChildNode(LPCTSTR pstrNode)
{
	// Nodes members
	MSXML2::IXMLDOMNodePtr newNode;
	
	// Create NODE TYPE
	CComVariant vType(MSXML2::NODE_ELEMENT);
	
	// Create the NODE
	newNode = m_plDomDocument->createNode(vType, pstrNode, _T(""));
	
	// Attach the Node to the document
	return AddLastChild(newNode);
}

BOOL CXmlParser::AddNodeBefore(LPCTSTR pstrNode)
{
	// Nodes members
	// IXMLDOMElementPtr m_pCurrentNode;        /* Global Member */
	MSXML2::IXMLDOMNodePtr newNode;
	
	// Can't use  function on the root node
	if(Is_Root())
		return false;
	// Create NODE TYPE
	CComVariant vType(MSXML2::NODE_ELEMENT);
	
	// Create the NODE
	newNode = m_plDomDocument->createNode(vType, pstrNode, _T(""));
	
	return AddBefore(newNode);
}

BOOL CXmlParser::AddNodeAfter(LPCTSTR pstrNode)
{
	// Nodes members
	// IXMLDOMElementPtr m_pCurrentNode;        /* Global Member */
	MSXML2::IXMLDOMNodePtr newNode;
	
	// Can't use  function on the root node
	if(Is_Root())
	{
		m_strErrMsg = _T("CXmlParser::Add_NodeAfter(LPCTSTR)   : Can't add node at same level that the root node");
		return false;
	}
	
	// Create NODE TYPE
	CComVariant vType(MSXML2::NODE_ELEMENT);
	
	// Create the NODE
	newNode = m_plDomDocument->createNode(vType, pstrNode, _T(""));
	
	return AddAfter(newNode);
}

BOOL CXmlParser::AddFirstChildNode(LPCTSTR pstrNode)
{
	// Nodes members
	// IXMLDOMElementPtr m_pCurrentNode;        /* Global Member */
	MSXML2::IXMLDOMNodePtr newNode;
	//IXMLDOMNodeListPtr ChildList;
	
	// If no child then use AddLastChildNode or CurrentElement node not set yet
	if(m_pCurrentNode != NULL)
	{
		if( !m_pCurrentNode->hasChildNodes() )
			return (AddLastChildNode(pstrNode));
	}
	else
		return (AddLastChildNode(pstrNode));
	
	// Create NODE TYPE
	CComVariant vType(MSXML2::NODE_ELEMENT);
	
	// Create the NODE
	newNode = m_plDomDocument->createNode(vType, pstrNode, _T(""));
	
	return AddFirstChild(newNode);
}

BOOL CXmlParser::SetAttribute(LPCTSTR AttribName,LPCTSTR AttribValue)
{
	// Nodes members
	MSXML2::IXMLDOMElementPtr CurrentElementNode = m_pCurrentNode;
	
	// Create Attribute variable
	CComVariant sAttribute(AttribValue);
	
	// Set the new attribute
	if(m_pCurrentNode != NULL)
	{
		CurrentElementNode->setAttribute( AttribName, sAttribute );
		m_pCurrentNode = CurrentElementNode;
		GetNodeInformations(m_pCurrentNode);
	}
	else
	{
		m_pDocRoot->setAttribute( AttribName, sAttribute );
		GetNodeInformations(m_pDocRoot);
	}
	
	// Return
	return true;
}

BOOL CXmlParser::RemoveAttribute(LPCTSTR AttribName)
{
	if(m_pCurrentNode == NULL)
	{
		if(m_pDocRoot != NULL)
			m_pCurrentNode = m_pDocRoot;
		else
		{
			m_strErrMsg = _T("CXmlParser::RemoveAttribute(LPCTSTR)   : Can't remove attribute on a NULL Node");
			return false;
		}
	}
	
	MSXML2::IXMLDOMElementPtr CurrentElementNode = m_pCurrentNode;
	CurrentElementNode->removeAttribute(AttribName);
	m_pCurrentNode = CurrentElementNode;
	
	return true;
}

BOOL CXmlParser::SetTextValue(LPCTSTR TextValue)
{
	// Nodes members
	// IXMLDOMElementPtr m_pCurrentNode;        /* Global Member */
	//
	MSXML2::IXMLDOMNodePtr newNode;
	
	// Find if a CHILD TEXT NODE exist or not
	//
	if(m_pCurrentNode == NULL)
	{
		if(m_pDocRoot != NULL)
			m_pCurrentNode = m_pDocRoot;
		else
		{
			m_strErrMsg = _T("CXmlParser::SetTextValue(LPCTSTR)   : Can't set a Text on a NULL Node");
			return false;
		}
	}
	
	if(GetNodeType(m_pCurrentNode) == MSXML2::NODE_CDATA_SECTION)
	{
		CComVariant sValue(TextValue);
		m_pCurrentNode->put_nodeValue(sValue);
		
		return true;
	}
	
	// Rq: a NODE_CDATA_SECTION can't have any childs
	if(m_pCurrentNode->hasChildNodes())
	{
		MSXML2::IXMLDOMNodePtr pChild;
		for(pChild = m_pCurrentNode->firstChild; pChild != NULL; pChild = pChild->nextSibling)
		{
			// Find it's a NODE TEXT
			if(GetNodeType(pChild) == MSXML2::NODE_TEXT)
			{
				// A Text Node is found, Replace it now!!
				CComVariant sValue(TextValue);
				pChild->put_nodeValue(sValue);
				
				return true;
			}
		}
	}
	
	// No previous Text was defined before, we can add it.
	if(GetNodeType(m_pCurrentNode) == MSXML2::NODE_ELEMENT)
	{
		// Create NODE TEXT type
		CComVariant vType(MSXML2::NODE_TEXT);
		
		// Create the node
		newNode = m_plDomDocument->createTextNode(TextValue);
		
		// Attach the Node to the document
		newNode = m_pCurrentNode->appendChild(newNode);
	}
	
	return true;
}

void CXmlParser::GoToRoot()
{ 
	m_nIndex = -1;
	m_pCurrentNode = m_pDocRoot;
//	GetNodeInformations(pChild);
	m_strChainTag.Empty();
}

BOOL CXmlParser::GoToChild()
{
	// Child node
	MSXML2::IXMLDOMNodePtr pChild;
	
	if(GetNodeType(m_pCurrentNode) != MSXML2::NODE_ELEMENT)
		return false;
	
	pChild = m_pCurrentNode->firstChild;
	if(pChild == NULL) return false;
	
	m_nIndex = -1;
	AddChainTag(GetCurrentTag());
	GetNodeInformations(pChild);
	
	m_pCurrentNode = pChild;
	return true;
}

BOOL CXmlParser::GoToParent()
{
	// Parent node
	MSXML2::IXMLDOMNodePtr pParent = NULL;
	
	if(m_pCurrentNode == m_pDocRoot)
		return false;
	
	m_pCurrentNode->get_parentNode(&pParent);
	m_pCurrentNode = pParent;
	
	m_nIndex = -1;
	RemoveChainTag(1);
	GetNodeInformations(m_pCurrentNode);
	return true;
}

BOOL CXmlParser::IsParentAttribute(CString strName, CString strValue)
{
	// Parent node
	MSXML2::IXMLDOMNodePtr pParent = NULL;
	if(m_pCurrentNode == m_pDocRoot)
		return FALSE;
	
	m_pCurrentNode->get_parentNode(&pParent);
	if ( GetNodeType(pParent) != MSXML2::NODE_ELEMENT )
		return FALSE;

	MSXML2::IXMLDOMNamedNodeMapPtr pAttribs = pParent->Getattributes();
	if(pAttribs == NULL)	return FALSE;

	for(int i=0; i<pAttribs->Getlength(); ++i)
	{
		MSXML2::IXMLDOMNodePtr pAttrib = pAttribs->Getitem(i);
		if(pAttrib == NULL)		return FALSE;

		CString strNodeName = (CString)(const char *)pAttrib->GetnodeName();
		CString strNodeValue = (CString)(const char *)_bstr_t(pAttrib->GetnodeValue());

		if ( strNodeName == strName && strNodeValue == strValue )
			return TRUE;
	}
	return FALSE;
}

BOOL CXmlParser::GetParentAttribute(CString strName, CString &strValue)
{
	// Parent node
	MSXML2::IXMLDOMNodePtr pParent = NULL;
	if(m_pCurrentNode == m_pDocRoot)
		return FALSE;
	
	m_pCurrentNode->get_parentNode(&pParent);
	if ( GetNodeType(pParent) != MSXML2::NODE_ELEMENT )
		return FALSE;

	MSXML2::IXMLDOMNamedNodeMapPtr pAttribs = pParent->Getattributes();
	if(pAttribs == NULL)	return FALSE;

	for(int i=0; i<pAttribs->Getlength(); ++i)
	{
		MSXML2::IXMLDOMNodePtr pAttrib = pAttribs->Getitem(i);
		if(pAttrib == NULL)		return FALSE;

		CString strNodeName = (CString)(const char *)pAttrib->GetnodeName();
		CString strNodeValue = (CString)(const char *)_bstr_t(pAttrib->GetnodeValue());

		if ( strNodeName == strName )
		{
			strValue == strNodeValue;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CXmlParser::GoForward()
{
	// Sibling node
	MSXML2::IXMLDOMNodePtr pSibling = NULL;
	
	if(GetNodeType(m_pCurrentNode) != MSXML2::NODE_ELEMENT && GetNodeType(m_pCurrentNode) != MSXML2::NODE_CDATA_SECTION)
		return false;

	pSibling = m_pCurrentNode->nextSibling;
	if(pSibling == NULL) return false;
	m_pCurrentNode = pSibling;
	
	m_nIndex = -1;
	GetNodeInformations(m_pCurrentNode);
	return true;
}

BOOL CXmlParser::GoBackward()
{
	// Sibling node
	MSXML2::IXMLDOMNodePtr pSibling;
	
	if(GetNodeType(m_pCurrentNode) != MSXML2::NODE_ELEMENT && GetNodeType(m_pCurrentNode) != MSXML2::NODE_CDATA_SECTION)
		return false;
	
	pSibling = m_pCurrentNode->previousSibling;
	if(pSibling == NULL) return false;
	m_pCurrentNode = pSibling;
	
	m_nIndex = -1;
	GetNodeInformations(pSibling);
	return true;
}

BOOL CXmlParser::GoToChild(LPCTSTR NodeName)
{
	MSXML2::IXMLDOMNodePtr pChild;
	
	if(GetNodeType(m_pCurrentNode) != MSXML2::NODE_ELEMENT)
		return false;

	AddChainTag(GetCurrentTag());

	for (pChild = m_pCurrentNode->firstChild;  pChild != NULL;  pChild = pChild->nextSibling)
	{
		GetNodeInformations(pChild);
		
		if(GetCurrentName() == NodeName)
		{ 
			m_nIndex = -1;
			m_pCurrentNode = pChild;
			return true;
		}
	}

	// Node Not Found (Restore Node informations)
	RemoveChainTag(1);
	GetNodeInformations(m_pCurrentNode);
	return false;
}

BOOL CXmlParser::GoToDescendant(LPCTSTR NodeName)
{
	MSXML2::IXMLDOMNodePtr pChild;

	MSXML2::IXMLDOMElementPtr pCurrent = m_pCurrentNode;

	if(GetNodeType(m_pCurrentNode) != MSXML2::NODE_ELEMENT)
		return false;

	AddChainTag(GetCurrentTag());

	for (pChild = m_pCurrentNode->firstChild;  NULL != pChild;  pChild = pChild->nextSibling)
	{

		GetNodeInformations(pChild);

		if(GetCurrentName() == NodeName)
		{
			// Update new Position
			m_nIndex = -1;
			m_pCurrentNode = pChild;
			return true;
		}

		// Try to go into Childs of  Child
		m_pCurrentNode = pChild;
		if( GoToDescendant(NodeName) )
		{
			return true;
		}

		// Restore Current node
		m_pCurrentNode = pCurrent;
	}

	// Node Not Found (Restore Node informations)
	RemoveChainTag(1);
	GetNodeInformations(m_pCurrentNode);
	return false;
}

BOOL CXmlParser::GoToParent(LPCTSTR NodeName)
{
	// Parent node
	MSXML2::IXMLDOMNodePtr pParent = NULL;
	MSXML2::IXMLDOMNodePtr oldCurrent = m_pCurrentNode;
	
	if(m_pCurrentNode == m_pDocRoot)
		return false;
	
	CString oldChainTag = m_strChainTag;
	
	m_pCurrentNode->get_parentNode(&pParent);
	
	while (true)
	{
		m_pCurrentNode = pParent;
		RemoveChainTag(1);
		GetNodeInformations(m_pCurrentNode);
		if(GetCurrentName() == NodeName)
		{
			m_nIndex = -1;
			return true;
		}
		
		if(m_pCurrentNode == m_pDocRoot)
			break;
		
		m_pCurrentNode->get_parentNode(&pParent);
	}

	// Parent not found
	m_pCurrentNode = oldCurrent;
	m_strChainTag = oldChainTag;
	GetNodeInformations(m_pCurrentNode);
	return false;
}

// Go to a Node attached at the same Node than the Current Node (Forward sens)
BOOL CXmlParser::GoForward(LPCTSTR NodeName)
{
	// Sibling node
	MSXML2::IXMLDOMNodePtr pSibling = NULL;
	
	if(GetNodeType(m_pCurrentNode) != MSXML2::NODE_ELEMENT && GetNodeType(m_pCurrentNode) != MSXML2::NODE_CDATA_SECTION)
		return false;

	for (pSibling= m_pCurrentNode;  true ;  pSibling = pSibling->nextSibling)
	{
		if(pSibling == NULL)
			break;
		
		// Grab Information from Sibling node
		GetNodeInformations(pSibling);
		
		if(GetCurrentName() == NodeName)
		{
			// Update new Position
			m_nIndex = -1;
			m_pCurrentNode = pSibling;
			return true;
		}
	}
	
	// Node Not Found (Restore Node informations)
	GetNodeInformations(m_pCurrentNode);
	return false;
}

BOOL CXmlParser::GoBackward(LPCTSTR NodeName)
{
	// Sibling node
	MSXML2::IXMLDOMNodePtr pSibling;
	
	if(GetNodeType(m_pCurrentNode) != MSXML2::NODE_ELEMENT && GetNodeType(m_pCurrentNode) != MSXML2::NODE_CDATA_SECTION)
		return false;
	
	for (pSibling = m_pCurrentNode;  NULL != pSibling;  pSibling = pSibling->previousSibling)
	{
		// Grab Information from Sibling node
		GetNodeInformations(pSibling);
		
		if(GetCurrentName() == NodeName)
		{
			// Update new Position
			m_nIndex = -1;
			m_pCurrentNode = pSibling;
			return true;
		}
	}
	
	// Node Not Found (Restore Node informations)
	GetNodeInformations(m_pCurrentNode);
	return false;
}

BOOL CXmlParser::Remove()
{
	// Parent node
	//
	MSXML2::IXMLDOMNodePtr pParent= NULL;
	
	if(m_pCurrentNode == NULL)
	{
		if(m_pDocRoot != NULL)
			m_pCurrentNode = m_pDocRoot;
		else
			return false;
	}
	
	if(m_pCurrentNode != m_pDocRoot)
	{
		m_pCurrentNode->get_parentNode(&pParent);
		pParent->removeChild(m_pCurrentNode);
		m_pCurrentNode = pParent;
		GetNodeInformations(m_pCurrentNode);
	}
	else
		Reset();

	m_nIndex = -1;
	return true;
}

BOOL CXmlParser::RemoveChild(LPCTSTR NodeName)
{
	// Find the Child
	if(GetNodeType(m_pCurrentNode) != MSXML2::NODE_ELEMENT) return false;
	
	if( GoToChild(NodeName) )
	{
		int cur_attrib_index = m_nIndex;
		BOOL result = Remove();
	
		m_nIndex = cur_attrib_index;
		return result;
	}

	return false;
}

// ***************************
// ** Header XML Management **
// ***************************
BOOL CXmlParser::SetHeader(LPCTSTR header,LPCTSTR name,LPCTSTR value)
{
	m_strErrMsg = _T("");
	BOOL empty_xml = false;
	
	CString strHeader = header;
	CString strName = name;
	CString strValue = value;
	
	BSTR bstr;
	CString cstr,cstr2;
	
	MSXML2::IXMLDOMNodePtr pChild = NULL;           // Working node
	pChild = m_plDomDocument->firstChild;   // Start at first document child
	
	
	
	if(pChild == NULL)        empty_xml = true;
	
	while(pChild != NULL)
	{ 
		if(pChild == m_pDocRoot) 
		{
			// Root document reach, it's done, the processing node wasn't found
            break;
		}
		
		if(pChild->nodeType != MSXML2::NODE_PROCESSING_INSTRUCTION) 
		{  pChild = pChild->nextSibling;          // Go to Next Processing instruction node
		continue;
		}
        
		
		pChild->get_baseName(&bstr);
		cstr = bstr;
		if(cstr == header) 
		{    
			// Correct header, check the correct property
			pChild->get_text(&bstr);
			cstr = bstr;
			
			int index = cstr.Find(name,0);
			if(index == -1)
			{
				// The property doesn't exist on  processing instruction");
				
				// Assume correct constraint about "xml" processing instruction
				{
					// must have version="xxx" in first
					// must have standalone="xxx" in last if exist
					cstr2.Empty();
					int standalone_index = cstr.Find(_T("standalone"), 0);
					if(standalone_index != -1)
					{
						cstr2 = cstr.Right( cstr.GetLength() - standalone_index + 1);
						cstr  = cstr.Left(standalone_index);
					}
					
					int version_index = cstr.Find(_T("version"), 0);
					if(version_index == -1 && strHeader == _T("xml"))
					{   
						CString strTemp = cstr;
						cstr = _T("version=\"1.0\" ");
						cstr += strTemp;
					}
					
					if(strName != _T("version"))
						cstr += _T(" ") + strName + _T("=\"") + strValue + _T("\" ") + cstr2;
					else
						cstr += cstr2;
				}
				
				// Create the new Processing Instruction node
				HRESULT hr;
				MSXML2::IXMLDOMProcessingInstruction *pIXMLDOMPInstr = NULL;
				hr = m_plDomDocument->raw_createProcessingInstruction(_bstr_t(strHeader), _bstr_t(cstr), &pIXMLDOMPInstr);
				
				if(SUCCEEDED(hr))
				{   
					// We succes the creation of the processing instruction
					// Replace the node
					m_plDomDocument->replaceChild(pIXMLDOMPInstr,pChild);
				}
				else
				{
					// Arf, i fails the creation, grrr, again
					m_strErrMsg = _T("CXmlParser::SetHeader() : Can't create the new processing instruction node");
					return false;
				}
				return true;
			}
			else
			{
				// The processing instruction node exist, must change it's value !! 
				int start,end;
				start = cstr.Find(_T('"'), index);

				if(start == -1)
				{
					m_strErrMsg = _T("CXmlParser::SetHeader() : bad value structure");
					return false;
				}
				end = cstr.Find(_T('"'), start+1);

				if(end == -1) 
				{
					m_strErrMsg = _T("CXmlParser::SetHeader() : bad value structure");
					return false;
				}
				
				cstr2 = cstr.Mid(0,start+1) + value + cstr.Mid(end,cstr.GetLength() - end);
				
				MSXML2::IXMLDOMNodePtr m_lpNode = NULL;
				MSXML2::IXMLDOMProcessingInstruction *pIXMLDOMPInstr = NULL;
				HRESULT hr;
				
				hr = m_plDomDocument->raw_createProcessingInstruction(_bstr_t(strHeader), _bstr_t(cstr2), &pIXMLDOMPInstr);
				
				if(SUCCEEDED(hr))
				{
					// We succes the creation of the processing instruction
					// Replace the node
					m_plDomDocument->replaceChild(pIXMLDOMPInstr,pChild);
				}
				else
				{
					m_strErrMsg = _T("CXmlParser::SetHeader() : Can't create the new processing instruction node");
					return false;
				}
				
				return true;
			}
        }
		
        pChild = pChild->nextSibling;   // Next Processing instruction node
     }
	 
     // No processing node for our header 
     { 
		 if(strName != _T("version") && strHeader == _T("xml"))
			 cstr = _T("version=\"1.0\" ") + strName + _T("=\"") + strValue + _T("\"");
		 else
			 cstr = strName + _T("=\"") + strValue + _T("\"");
		 
		 MSXML2::IXMLDOMNodePtr m_lpNode = NULL;
		 MSXML2::IXMLDOMProcessingInstruction *pIXMLDOMPInstr = NULL;
		 HRESULT hr;
		 
		 hr = m_plDomDocument->raw_createProcessingInstruction(_bstr_t(strHeader), _bstr_t(cstr), &pIXMLDOMPInstr);
		 if(SUCCEEDED(hr))
		 {  
			 if(!empty_xml)
			 {
				 _variant_t NodeRef = (IUnknown *)m_pDocRoot;
	             m_lpNode = NULL;
		         m_lpNode = m_plDomDocument->insertBefore(pIXMLDOMPInstr,NodeRef);
             
				 if(m_lpNode == NULL)
					 m_strErrMsg = _T("PARSER_XML::SetHeader() : Can't insert Processing node after the root document");

		         return (m_lpNode != NULL);
			 }
			 else
			 {
				 m_lpNode = NULL;
	             m_lpNode = m_plDomDocument->appendChild(pIXMLDOMPInstr);
		    
				 if(m_lpNode == NULL)
					 m_strErrMsg = _T("PARSER_XML::SetHeader() : Can't insert Processing node in the empty document");

			     return (m_lpNode != NULL);
			 }
		 }
		 
		 m_strErrMsg = _T("PARSER_XML::SetHeader() : Can't create new Processing node");
		 return false;
     } 
}

BOOL CXmlParser::GetHeader(LPCTSTR header,LPCTSTR name,CString & res)
{
	m_strErrMsg = _T("");
	
	MSXML2::IXMLDOMNodePtr pChild;                  // Working node
	res.Empty();
	
	pChild = m_plDomDocument;
	pChild = m_plDomDocument->firstChild;   // Start at first document child
	
	if(pChild == NULL) 
	{
		m_strErrMsg = _T("CXmlParser::GetHeader() : The XML Document is a null pointer");
		return false;  
	}

	while(pChild != NULL)
	{
		if(pChild->nodeType != MSXML2::NODE_PROCESSING_INSTRUCTION) break;
		
		BSTR bstr;
		CString cstr;
		
		pChild->get_baseName(&bstr);
		cstr = bstr;
		if(cstr == header) 
		{
			// Correct header, check the correct property
			//
			pChild->get_text(&bstr);
			cstr = bstr;
			
			int index = cstr.Find(name,0);
			if(index == -1)
				return false;
            
			int start,end;
			start = cstr.Find(_T('"'),index);
			if(start == -1) 
			{
				m_strErrMsg = _T("CXmlParser::GetHeader() : bad value structure");
				return false;
			}
			
			end = cstr.Find(_T('"'),start+1);
			if(end == -1) 
			{
				m_strErrMsg = _T("CXmlParser::GetHeader() : bad value structure");
				return false;
			}
			
			res = cstr.Mid(start+1,end-start-1);
			return true;
		}
		
		pChild = pChild->nextSibling;   // Next Processing instruction node
	}
	return false;
}
