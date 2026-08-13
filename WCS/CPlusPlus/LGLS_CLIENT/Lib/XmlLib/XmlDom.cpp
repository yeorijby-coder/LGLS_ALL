// XmlDom.cpp: implementation of the CXmlDom class.
//

#include "StdAfx.h"
//#include <atlbase.h>
#include "XmlDom.h"


// CXmlDom
CXmlDom::CXmlDom(void)
{
	m_pDocument = NULL;
	m_pSelectNodes = NULL;
	m_pNode = NULL;
	m_bInitilized = FALSE;
}

CXmlDom::~CXmlDom(void)
{
}

// CConvert 멤버 함수
void CXmlDom::SetErrorMessage(MSXML2::IXMLDOMParseErrorPtr& e, const CString& strMsg)
{
	if (strMsg.IsEmpty())
	{
		m_strErrorMessage.Format(_T("Source: %ld line, %ld pos\nDescription: %s"), 
			e->line, e->linepos, (LPCSTR)e->reason);
	}
	else
	{
		m_strErrorMessage.Format(_T("%s\n\nSource: %ld line, %ld pos\nDescription: %s"), 
			strMsg, e->line, e->linepos, (LPCSTR)e->reason);
	}
}

CString CXmlDom::GetErrorMessage(_com_error& e)
{
	CString strErrorMessage;
	strErrorMessage.Format(_T("Source: %s\nDescription: %s\nErrorMessage:%s"), 
		(LPCTSTR)e.Source(), (LPCTSTR)e.Description(), (LPCTSTR)e.ErrorMessage());

	return strErrorMessage;
}

CString& CXmlDom::GetErrorMessage()
{
	return m_strErrorMessage;
}

BOOL CXmlDom::InitializeXmlDom()
{
	if (m_bInitilized)
		return TRUE;

    // -- Instantiate an XML document --
    HRESULT hr = m_pDocument.CreateInstance(MSXML2::CLSID_DOMDocument);
    if (FAILED(hr))
    {
        m_strErrorMessage = _T("Failed to instantiate an XML DOM.");
		m_bInitilized = FALSE;
        return FALSE;
    }

	// async == TRUE면 load나 loadXML함수가 XML을 완전히 로드 하기전 리턴
	// async == TRUE면 객체 접근전 ReadyState을 체크후 처리
	m_pDocument->async = VARIANT_FALSE;

	// 문서 유효성 검사 안함
	m_pDocument->validateOnParse = VARIANT_FALSE;

	m_pNode = m_pDocument;
	m_bInitilized = TRUE;

    return TRUE;
}

BOOL CXmlDom::LoadXmlFile(LPCTSTR lpszXmlFile)
{
	_bstr_t bstrXmlFile(lpszXmlFile);

	if (m_pDocument->load(bstrXmlFile) != VARIANT_TRUE)
	{
		CString strMsg;
		strMsg.Format(_T("Failed to load %s."), lpszXmlFile);
		SetErrorMessage(m_pDocument->parseError, strMsg);
		return FALSE;
	}

	m_pNode = m_pDocument->documentElement;
	m_pSelectNodes = NULL;


#ifdef _DEBUG
	TestDebug(m_pDocument);
	TestDebug(m_pNode);
#endif
	
	return TRUE;
}

BOOL CXmlDom::LoadXmlString(LPCTSTR lpszXmlString)
{
	_bstr_t bstrXmlString(lpszXmlString);

	if (m_pDocument->loadXML(bstrXmlString) != VARIANT_TRUE)
	{
		CString strMsg;
		strMsg.Format(_T("Failed to load xml string."));
		SetErrorMessage(m_pDocument->parseError, strMsg);
		return FALSE;
	}

	m_pNode = m_pDocument->documentElement;
	m_pSelectNodes = NULL;

	return TRUE;
}

BOOL CXmlDom::SaveXmlFile(LPCTSTR lpszXmlFile)
{
	ArrangeXML(m_pDocument->documentElement);

	// Save the XML document
	HRESULT hr = m_pDocument->save(lpszXmlFile);
 
	if (FAILED(hr))
    {
		CString strMsg;
		strMsg.Format(_T("Failed to save %s."), lpszXmlFile);
		SetErrorMessage(m_pDocument->parseError, strMsg);
        return FALSE;
    }
	
	return TRUE;
}

void CXmlDom::MoveDocument()
{
	ASSERT(m_pDocument != NULL);

	m_pNode = m_pDocument;
}

void CXmlDom::MoveRoot()
{
	ASSERT(m_pDocument->documentElement != NULL);
	
	m_pNode = m_pDocument->documentElement;
}

void CXmlDom::MoveParent()
{
	ASSERT(m_pNode != NULL);

	if (m_pNode != m_pDocument->documentElement)
		m_pNode = m_pNode->parentNode;
}

void CXmlDom::MoveSiblingNext()
{
	ASSERT(m_pNode != NULL);

	if (m_pNode->nextSibling != NULL)
		m_pNode = m_pNode->nextSibling;
}

void CXmlDom::MoveSiblingPrev()
{
	ASSERT(m_pNode != NULL);

	if (m_pNode->previousSibling != NULL)
		m_pNode = m_pNode->previousSibling;
}

void CXmlDom::MoveChildFirst()
{
	ASSERT(m_pNode != NULL);

	if (m_pNode->firstChild != NULL)
		m_pNode = m_pNode->firstChild;
}

void CXmlDom::MoveChildLast()
{
	ASSERT(m_pNode != NULL);

	if (m_pNode->lastChild != NULL)
		m_pNode = m_pNode->lastChild;
}

void CXmlDom::MoveChild(int nIndex)
{
	ASSERT(m_pNode != NULL);
	ASSERT((nIndex > -1) && (nIndex < m_pNode->childNodes->length));

	m_pNode = m_pNode->childNodes->item[nIndex];
}

BOOL CXmlDom::MoveXPath(LPCTSTR lpszQuery, BOOL bFromDocument /* = TRUE */)
{
	MSXML2::IXMLDOMNodePtr nodePtr;

	if (bFromDocument == TRUE)
		nodePtr = SelectSingleNode(m_pDocument, lpszQuery);
	else
		nodePtr = SelectSingleNode(m_pNode, lpszQuery);

	if (nodePtr != NULL)
	{
		m_pNode = nodePtr;
		return TRUE;
	}

	return FALSE;
}

void CXmlDom::GetNodeName(LPCTSTR lpszQuery1, CString& strName, LPCTSTR lpszQuery2 /* = NULL */, BOOL bFromDocument /* = TRUE */)
{
	MSXML2::IXMLDOMNodePtr pNode;

	if (bFromDocument == TRUE)
		pNode = SelectSingleNode(m_pDocument, lpszQuery1);
	else
		pNode = SelectSingleNode(m_pNode, lpszQuery1);

	if (lpszQuery2 != NULL)
		pNode = SelectSingleNode(pNode, lpszQuery2);

	GetNodeName(pNode, strName);
}

void CXmlDom::GetNodeName(LPCTSTR lpszQuery1, int nIndex, CString& strName, LPCTSTR lpszQuery2 /* = NULL */, BOOL bFromDocument /* = TRUE */)
{
	MSXML2::IXMLDOMNodeListPtr pNodes;
	MSXML2::IXMLDOMNodePtr pNode;

	if (bFromDocument == TRUE)
		pNodes = SelectNodes(m_pDocument, lpszQuery1);
	else
		pNodes = SelectNodes(m_pNode, lpszQuery1);

	ASSERT(pNodes != NULL);
	ASSERT((nIndex > -1) && (nIndex < pNodes->length));
	pNode = pNodes->item[nIndex];

	if (lpszQuery2 != NULL)
		pNode = SelectSingleNode(pNode, lpszQuery2);

	GetNodeName(pNode, strName);
}

void CXmlDom::GetNodeValue(LPCTSTR lpszQuery1, CString& strValue, LPCTSTR lpszQuery2 /* = NULL */, BOOL bFromDocument /* = TRUE */)
{
	MSXML2::IXMLDOMNodePtr pNode;

	if (bFromDocument == TRUE)
		pNode = SelectSingleNode(m_pDocument, lpszQuery1);
	else
		pNode = SelectSingleNode(m_pNode, lpszQuery1);

	if (lpszQuery2 != NULL)
		pNode = SelectSingleNode(pNode, lpszQuery2);

	GetNodeValue(pNode, strValue);
}

void CXmlDom::GetNodeValue(LPCTSTR lpszQuery1, int nIndex, CString& strValue, LPCTSTR lpszQuery2 /* = NULL */, BOOL bFromDocument /* = TRUE */)
{
	MSXML2::IXMLDOMNodeListPtr pNodes;
	MSXML2::IXMLDOMNodePtr pNode;

	if (bFromDocument == TRUE)
		pNodes = SelectNodes(m_pDocument, lpszQuery1);
	else
		pNodes = SelectNodes(m_pNode, lpszQuery1);

	ASSERT(pNodes != NULL);
	ASSERT((nIndex > -1) && (nIndex < pNodes->length));
	pNode = pNodes->item[nIndex];

	if (lpszQuery2 != NULL)
		pNode = SelectSingleNode(pNode, lpszQuery2);

	GetNodeValue(pNode, strValue);
}

void CXmlDom::SetNodeValue(LPCTSTR lpszQuery, const CString& strValue, BOOL bFromDocument /* = TRUE */)
{
	MSXML2::IXMLDOMNodePtr pNode;

	if (bFromDocument == TRUE)
		pNode = SelectSingleNode(m_pDocument, lpszQuery);
	else
		pNode = SelectSingleNode(m_pNode, lpszQuery);

	SetNodeValue(pNode, strValue);
}

int CXmlDom::GetNodeCount(LPCTSTR lpszQuery, BOOL bFromDocument /* = TRUE */)
{
	MSXML2::IXMLDOMNodeListPtr pSelectNodes;

	if (bFromDocument == TRUE)
		pSelectNodes = SelectNodes(m_pDocument, lpszQuery);
	else
		pSelectNodes = SelectNodes(m_pNode, lpszQuery);

	if (pSelectNodes != NULL)
		return pSelectNodes->length;

	return 0;
}

void CXmlDom::MoveSelect(int nIndex)
{
	ASSERT(m_pSelectNodes != NULL);
	ASSERT((nIndex > -1) && (nIndex < m_pSelectNodes->length));

	m_pNode = m_pSelectNodes->item[nIndex];
}

BOOL CXmlDom::SelectElements(LPCTSTR lpszQuery, BOOL bFromDocument /* = TRUE */)
{
	ASSERT(lpszQuery != NULL);

	_bstr_t bstrQuery(lpszQuery);

	if (bFromDocument == TRUE)
		m_pSelectNodes = m_pDocument->selectNodes(bstrQuery);
	else		
		m_pSelectNodes = m_pNode->selectNodes(bstrQuery);

	if (m_pSelectNodes == NULL)
		return FALSE;

	return (m_pSelectNodes->length > 0);
}

BOOL CXmlDom::AddChildElement(LPCTSTR lpszName, LPCTSTR lpszValue /* = NULL */, BOOL bCData /* = FALSE */, LPCTSTR lpszPos /* = NULL */)
{
	ASSERT(lpszName != NULL);

	MSXML2::IXMLDOMNodePtr pNodeElmt = NULL;
	MSXML2::IXMLDOMNodePtr pNodeText = NULL;
	MSXML2::IXMLDOMNodePtr pNodePos = NULL;

	pNodePos = (lpszPos != NULL) ? SelectChildNode(m_pNode, lpszPos) : NULL;
	pNodeElmt = AddChild(m_pNode, pNodePos, MSXML2::NODE_ELEMENT, lpszName, NULL);

	if (lpszValue != NULL)
	{
		if (bCData == FALSE)
			pNodeText = AddChild(pNodeElmt, NULL, MSXML2::NODE_TEXT, NULL, lpszValue);
		else
			pNodeText = AddChild(pNodeElmt, NULL, MSXML2::NODE_CDATA_SECTION, NULL, lpszValue);
	}

	return (pNodeElmt != NULL);
}

BOOL CXmlDom::AddChildAttribute(LPCTSTR lpszName, LPCTSTR lpszValue)
{
	ASSERT((lpszName != NULL) && (lpszValue != NULL));

	MSXML2::IXMLDOMNodePtr pNodeAttr = NULL;
	pNodeAttr = AddChild(m_pNode, NULL, MSXML2::NODE_ATTRIBUTE, lpszName, lpszValue);

	return (pNodeAttr != NULL);
}

BOOL CXmlDom::AddChildProcessingInstruction(LPCTSTR lpszName, LPCTSTR lpszValue)
{
	ASSERT(lpszName != NULL);

	MSXML2::IXMLDOMNodePtr pNodePI = NULL;
	pNodePI = AddChild(m_pDocument, NULL, MSXML2::NODE_PROCESSING_INSTRUCTION, lpszName, lpszValue);

	return (pNodePI != NULL);
}

BOOL CXmlDom::AddChildComment(LPCTSTR lpszValue)
{
	ASSERT(lpszValue != NULL);

	MSXML2::IXMLDOMNodePtr pNodeCmt = NULL;
	pNodeCmt = AddChild(m_pNode, NULL, MSXML2::NODE_COMMENT, NULL, lpszValue);

	return (pNodeCmt != NULL);
}

BOOL CXmlDom::RemoveChildElement(LPCTSTR lpszName)
{
	ASSERT(lpszName != NULL);

	MSXML2::IXMLDOMNodePtr pElmt = NULL;
	pElmt = SelectChildNode(m_pNode, lpszName);

	return m_pNode->removeChild(pElmt) != NULL;
}

BOOL CXmlDom::RemoveChiilAttribute(LPCTSTR lpszName)
{
	ASSERT(lpszName != NULL);

	_bstr_t bstrName(lpszName);

	return m_pNode->attributes->removeNamedItem(bstrName) != NULL;
}

void CXmlDom::ArrangeXML(MSXML2::IXMLDOMNodePtr pCurrentNode)
{
	_bstr_t bstr1(_T("\n\t"));
	_bstr_t bstr2(_T("\n"));
	_bstr_t bstrTab(_T("\t"));
	MSXML2::IXMLDOMNodePtr temp = pCurrentNode->parentNode;

	while (temp != pCurrentNode->ownerDocument)
	{
		temp = temp->parentNode;
		bstr1 += bstrTab;
		bstr2 += bstrTab;
	}
	
	for (int i=0; i<pCurrentNode->childNodes->length; ++i)
	{
		MSXML2::IXMLDOMNodePtr node = pCurrentNode->childNodes->item[i];
		MSXML2::DOMNodeType type = node->nodeType;
		
		if (type == MSXML2::NODE_ELEMENT)
		{
			AddChild(pCurrentNode, node, MSXML2::NODE_TEXT, NULL, (LPCTSTR)bstr1);
			ArrangeXML(node);
			++i;

			if (node == pCurrentNode->lastChild)
				AddChild(pCurrentNode, NULL, MSXML2::NODE_TEXT, NULL, (LPCTSTR)bstr2);
		}
	}
}

MSXML2::IXMLDOMNodeListPtr CXmlDom::SelectNodes(MSXML2::IXMLDOMNodePtr pCurrentNode, LPCTSTR lpszQuery)
{
	ASSERT(pCurrentNode != NULL);
	ASSERT(lpszQuery != NULL);

	_bstr_t bstrQuery(lpszQuery);

	return pCurrentNode->selectNodes(bstrQuery);
}

MSXML2::IXMLDOMNodePtr CXmlDom::SelectSingleNode(MSXML2::IXMLDOMNodePtr pCurrentNode, LPCTSTR lpszQuery)
{
	ASSERT(pCurrentNode != NULL);
	ASSERT(lpszQuery != NULL);

	_bstr_t bstrQuery(lpszQuery);

	return pCurrentNode->selectSingleNode(bstrQuery);
}

MSXML2::IXMLDOMNodePtr CXmlDom::SelectChildNode(MSXML2::IXMLDOMNodePtr pCurrentNode, LPCTSTR lpszChildName)
{
	ASSERT(pCurrentNode != NULL);
	ASSERT(lpszChildName != NULL);

	CString strQueryString;
	strQueryString.Format(_T("./%s"), lpszChildName);
	_bstr_t bstrQuery((LPCTSTR)strQueryString);
	
	return pCurrentNode->selectSingleNode(bstrQuery);
}

MSXML2::IXMLDOMNodePtr CXmlDom::AddChild(MSXML2::IXMLDOMNodePtr pCurrentNode, MSXML2::IXMLDOMNodePtr pRefChildNode, MSXML2::DOMNodeType nodeType, 
								 LPCTSTR lpszName, LPCTSTR lpszData)
{
	ASSERT(pCurrentNode != NULL);

	MSXML2::IXMLDOMNodePtr pNewChildNode = NULL;

	if (nodeType == MSXML2::NODE_ELEMENT)
	{
		ASSERT(lpszName != NULL);
		_bstr_t bstrName(lpszName);
		MSXML2::IXMLDOMElementPtr elmt = NULL;
		elmt = m_pDocument->createElement(bstrName);

		if (pRefChildNode == NULL)
			pNewChildNode = pCurrentNode->appendChild(elmt);
		else
			pNewChildNode = pCurrentNode->insertBefore(elmt, pRefChildNode.GetInterfacePtr());
	}
	else if (nodeType == MSXML2::NODE_ATTRIBUTE)
	{
		ASSERT((lpszName != NULL) && (lpszData != NULL));
		_bstr_t bstrName(lpszName);
		_variant_t vValue(lpszData);
		MSXML2::IXMLDOMAttributePtr attr = NULL;
		attr = m_pDocument->createAttribute(bstrName);
		pNewChildNode = pCurrentNode->attributes->setNamedItem(attr);
		pNewChildNode->nodeValue = vValue;
	}
	else if (nodeType == MSXML2::NODE_TEXT)
	{
		ASSERT(lpszData != NULL);
		_bstr_t bstrData(lpszData);
		MSXML2::IXMLDOMTextPtr text = NULL;
		text = m_pDocument->createTextNode(bstrData);

		if (pRefChildNode == NULL)
			pNewChildNode = pCurrentNode->appendChild(text);
		else
			pNewChildNode = pCurrentNode->insertBefore(text, pRefChildNode.GetInterfacePtr());
	}
	else if (nodeType == MSXML2::NODE_CDATA_SECTION)
	{
		ASSERT(lpszData != NULL);
		_bstr_t bstrData(lpszData);
		MSXML2::IXMLDOMCDATASectionPtr cds = NULL;
		cds = m_pDocument->createCDATASection(bstrData);
		pNewChildNode = pCurrentNode->appendChild(cds);
	}
	else if (nodeType == MSXML2::NODE_PROCESSING_INSTRUCTION)
	{
		ASSERT((lpszName != NULL) && (lpszData != NULL));
		ASSERT(pCurrentNode->nodeType == MSXML2::NODE_DOCUMENT);
		_bstr_t bstrTarget(lpszName);
		_bstr_t bstrData(lpszData);
		MSXML2::IXMLDOMProcessingInstructionPtr pi = NULL;
		pi = m_pDocument->createProcessingInstruction(bstrTarget, bstrData);
		pNewChildNode = pCurrentNode->appendChild(pi);
	}
	else if (nodeType == MSXML2::NODE_COMMENT)
	{
		ASSERT(lpszData != NULL);
		_bstr_t bstrData(lpszData);
		MSXML2::IXMLDOMCommentPtr cmt = NULL;
		cmt = m_pDocument->createComment(bstrData);
		pNewChildNode = pCurrentNode->appendChild(cmt);
	}
	else
	{
		m_strErrorMessage.Format(_T("Failed to append <%s> node."), lpszName);
		return FALSE;
	}

	return pNewChildNode;
}

void CXmlDom::GetXML(MSXML2::IXMLDOMNodePtr nodePtr, CString& strXml)
{
	ASSERT(nodePtr != NULL);

	strXml = (LPCTSTR)nodePtr->xml;
}

void CXmlDom::GetNodeText(MSXML2::IXMLDOMNodePtr nodePtr, CString& strText)
{
	if (nodePtr == NULL) { strText.Empty(); return; }	// [LGLS] null 노드 안전 처리 (ASSERT 팝업이 초기화를 블로킹)

	strText = (LPCTSTR)nodePtr->text;
}

void CXmlDom::GetNodeName(MSXML2::IXMLDOMNodePtr nodePtr, CString& strName)
{
	if (nodePtr == NULL) { strName.Empty(); return; }	// [LGLS]

	strName = (LPCTSTR)nodePtr->nodeName;
}

void CXmlDom::GetNodeValue(MSXML2::IXMLDOMNodePtr nodePtr, CString& strValue)
{
	if (nodePtr == NULL) { strValue.Empty(); return; }	// [LGLS]

	MSXML2::IXMLDOMNodeListPtr& childNodes = nodePtr->childNodes;

	for (int i=0; i<childNodes->length; ++i)
	{
		MSXML2::IXMLDOMNodePtr& pNode = childNodes->item[i];
		MSXML2::DOMNodeType type = pNode->nodeType;

		if ((type == MSXML2::NODE_TEXT) || (type == MSXML2::NODE_CDATA_SECTION))
		{
			_bstr_t bstrValue(pNode->nodeValue);
			strValue = (LPCTSTR)bstrValue;
			return;
		}
	}

	strValue.Empty();
}

void CXmlDom::SetNodeValue(MSXML2::IXMLDOMNodePtr nodePtr, const CString& strValue)
{
	ASSERT(nodePtr != NULL);

	MSXML2::IXMLDOMNodeListPtr& childNodes = nodePtr->childNodes;

	for (int i=0; i<childNodes->length; ++i)
	{
		MSXML2::IXMLDOMNodePtr& pNode = childNodes->item[i];
		MSXML2::DOMNodeType type = pNode->nodeType;

		if ((type == MSXML2::NODE_TEXT) || (type == MSXML2::NODE_CDATA_SECTION))
		{
			_variant_t vValue((LPCTSTR)strValue);
			pNode->nodeValue = vValue;
			return;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 200071018
////////////////////////////////////////////////////////////////////////////////////////////////

CString CXmlDom::GetXPath()
{
	CString strXPath;

	if ((m_pNode != NULL) && (m_pNode->nodeType == MSXML2::NODE_ELEMENT))
	{
		CString strNodeName, strNodePath;
		MSXML2::IXMLDOMNodePtr nodePtr = m_pNode;
		while (nodePtr->parentNode != NULL)
		{
			GetNodeName(nodePtr, strNodeName);
			strNodePath.Format(_T("/%s"), strNodeName);
			strXPath = strNodePath + strXPath;
			nodePtr = nodePtr->parentNode;
		}
	}

	return strXPath;	
}

CString CXmlDom::GetElmtXML()
{
	CString strXml;
	if (m_pNode != NULL)
		GetXML(m_pNode, strXml);
	return strXml;
}

CString CXmlDom::GetElmtText()
{
	CString strText;
	if (m_pNode != NULL)
		GetNodeText(m_pNode, strText);
	return strText;
}

CString CXmlDom::GetElmtName()
{
	CString strName;
	if (m_pNode != NULL)
		GetNodeName(m_pNode, strName);
	return strName;
}

CString CXmlDom::GetElmtValue()
{
	CString strValue;
	if (m_pNode != NULL)
		GetNodeValue(m_pNode, strValue);
	return strValue;
}

CString CXmlDom::GetAttrName(int nIndex)
{
	CString strName;
	if ((m_pNode != NULL) && (nIndex >= 0) && (nIndex < m_pNode->attributes->length))
		GetNodeName(m_pNode->attributes->item[nIndex], strName);
	return strName;
}

CString CXmlDom::GetAttrValue(int nIndex)
{
	CString strValue;
	if ((m_pNode != NULL) && (nIndex >= 0) && (nIndex < m_pNode->attributes->length))
		GetNodeValue(m_pNode->attributes->item[nIndex], strValue);
	return strValue;
}

CString CXmlDom::GetAttrValue(CString lpszName)
{
	CString strValue;
	if (m_pNode != NULL)
	{
		_bstr_t bstrName(lpszName);
		MSXML2::IXMLDOMNodePtr nodePtr = m_pNode->attributes->getNamedItem(bstrName);
		if (nodePtr != NULL)
			GetNodeValue(nodePtr, strValue);
	}
	return strValue;
}

CString CXmlDom::GetChildElmtValue(LPCTSTR lpszName)
{
	CString strValue;
	if (m_pNode != NULL)
	{
		_bstr_t bstrName(lpszName);
		MSXML2::IXMLDOMNodePtr nodePtr = m_pNode->selectSingleNode(bstrName);
		if (nodePtr != NULL)
			GetNodeValue(nodePtr, strValue);
	}
	return strValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////
CXmlDomNode::CXmlDomNode(MSXML2::IXMLDOMNodePtr pNodePtr)
{
	m_pNodePtr = pNodePtr;
}

CXmlDomNode::~CXmlDomNode(void)
{
}


#ifdef _DEBUG

void CXmlDom::TestInfo(MSXML2::IXMLDOMNodePtr nodePtr)
{
	if (nodePtr == NULL)
		return;

	MSXML2::DOMNodeType type;
	_bstr_t bstr;
	_variant_t var;

	bstr = nodePtr->nodeName;
	bstr = nodePtr->nodeTypeString;
	bstr = nodePtr->text;
	bstr = nodePtr->xml;
	type = nodePtr->nodeType;
}

void CXmlDom::TestInfo(MSXML2::IXMLDOMNodeListPtr nodeListPtr)
{
	if (nodeListPtr == NULL)
		return;

	_bstr_t bstr;
	long lVal;

	lVal = nodeListPtr->length;
	for (int i=0; i<nodeListPtr->length; ++i)
		TestInfo(nodeListPtr->item[i]);
}

void CXmlDom::TestInfo(MSXML2::IXMLDOMNamedNodeMapPtr namedNodeMapPtr)
{
	if (namedNodeMapPtr == NULL)
		return;

	_bstr_t bstr;
	long lVal;

	lVal = namedNodeMapPtr->length;
	for (int i=0; i<namedNodeMapPtr->length; ++i)
		TestInfo(namedNodeMapPtr->item[i]);
}

void CXmlDom::TestDebug(MSXML2::IXMLDOMNodePtr nodePtr)
{
	if (nodePtr == NULL)
		return;

	_bstr_t bstr;
	_variant_t var;
	VARIANT_BOOL b;
	MSXML2::DOMNodeType type;
	MSXML2::IXMLDOMNodePtr node;
	MSXML2::IXMLDOMNodeListPtr nodes;
	MSXML2::IXMLDOMNamedNodeMapPtr attrs;
	MSXML2::IXMLDOMDocument2Ptr doc;

	doc = nodePtr->ownerDocument;
	bstr = nodePtr->baseName;
	bstr = nodePtr->namespaceURI;
	bstr = nodePtr->prefix;
	bstr = nodePtr->nodeName;
	bstr = nodePtr->nodeTypeString;
	bstr = nodePtr->text;
	bstr = nodePtr->xml;
	type = nodePtr->nodeType;
	var = nodePtr->nodeTypedValue;
	var = nodePtr->nodeValue; 
	b = nodePtr->parsed;
	b = nodePtr->specified;
	TestInfo(nodePtr->parentNode);
	TestInfo(nodePtr->firstChild);
	TestInfo(nodePtr->lastChild);
	TestInfo(nodePtr->nextSibling);
	TestInfo(nodePtr->previousSibling);
	TestInfo(nodePtr->childNodes);
	TestInfo(nodePtr->attributes);
}

#endif