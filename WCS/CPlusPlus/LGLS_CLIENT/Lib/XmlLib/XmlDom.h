// XmlDom.h: interface for the CXmlDom class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

#import <msxml3.dll> named_guids
using namespace MSXML2;


class AFX_EXT_CLASS CXmlDom  
{
public:
	CXmlDom(void);
	virtual ~CXmlDom(void);

protected:
	MSXML2::IXMLDOMDocument2Ptr m_pDocument;		// xml dom document
	MSXML2::IXMLDOMNodeListPtr m_pSelectNodes;		// selected node list
	MSXML2::IXMLDOMNodePtr m_pNode;					// current node

protected:
	BOOL	m_bInitilized;
	CString m_strErrorMessage;

public:
	void SetErrorMessage(MSXML2::IXMLDOMParseErrorPtr& e, const CString& strMsg);
	CString GetErrorMessage(_com_error& e);
	CString& GetErrorMessage();

public:
	BOOL InitializeXmlDom();
	BOOL LoadXmlFile(LPCTSTR lpszXmlFile);
	BOOL LoadXmlString(LPCTSTR lpszXmlString);
	BOOL SaveXmlFile(LPCTSTR lpszXmlFile);

public:	// 20071018
 	CString GetXPath();
 	CString GetElmtXML();
 	CString GetElmtText();
 	CString GetElmtName();
 	CString GetElmtValue();
 	CString GetAttrName(int nIndex);
 	CString GetAttrValue(int nIndex);
 	CString GetAttrValue(CString lpszName);
	CString GetChildElmtValue(LPCTSTR lpszName);

public:
	void MoveDocument();
	void MoveRoot();
	void MoveParent();
	void MoveSiblingNext();
	void MoveSiblingPrev();
	void MoveChildFirst();
	void MoveChildLast();
	void MoveChild(int nIndex);

public:
	BOOL MoveXPath(LPCTSTR lpszQuery, BOOL bFromDocument = TRUE);
	void GetNodeName(LPCTSTR lpszQuery1, CString& strName, LPCTSTR lpszQuery2 = NULL, BOOL bFromDocument = TRUE);
	void GetNodeName(LPCTSTR lpszQuery1, int nIndex, CString& strName, LPCTSTR lpszQuery2 = NULL, BOOL bFromDocument = TRUE);
	void GetNodeValue(LPCTSTR lpszQuery1, CString& strValue, LPCTSTR lpszQuery2 = NULL, BOOL bFromDocument = TRUE);
	void GetNodeValue(LPCTSTR lpszQuery1, int nIndex, CString& strValue, LPCTSTR lpszQuery2 = NULL, BOOL bFromDocument = TRUE);
	void SetNodeValue(LPCTSTR lpszQuery, const CString& strValue, BOOL bFromDocument = TRUE);
	int GetNodeCount(LPCTSTR lpszQuery, BOOL bFromDocument = TRUE);

public:
	void MoveSelect(int nIndex);
	BOOL SelectElements(LPCTSTR lpszQuery, BOOL bFromDocument = TRUE);
	int GetSelectCount() { return m_pSelectNodes->length; }

public:
	BOOL AddChildElement(LPCTSTR lpszName, LPCTSTR lpszValue = NULL, BOOL bCData = FALSE, LPCTSTR lpszPos = NULL);
	BOOL AddChildAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	BOOL AddChildProcessingInstruction(LPCTSTR lpszName, LPCTSTR lpszValue);
	BOOL AddChildComment(LPCTSTR lpszValue);
	BOOL RemoveChildElement(LPCTSTR lpszName);
	BOOL RemoveChiilAttribute(LPCTSTR lpszName);

public:
	void GetElmtXML(CString& strXml) { GetXML(m_pNode, strXml);	}
	void GetElmtText(CString& strText) { GetNodeText(m_pNode, strText);	}
	void GetElmtName(CString& strName) { GetNodeName(m_pNode, strName); }
	void GetElmtValue(CString& strValue) { GetNodeValue(m_pNode, strValue); }
	void SetElmtValue(const CString& strValue) { SetNodeValue(m_pNode, strValue); }

public:
	void GetAttrName(int nIndex, CString& strName) { GetNodeName(m_pNode->attributes->item[nIndex], strName); }
	void GetAttrValue(int nIndex, CString& strValue) { GetNodeValue(m_pNode->attributes->item[nIndex], strValue); }
	void SetAttrValue(int nIndex, const CString& strValue) { SetNodeValue(m_pNode->attributes->item[nIndex], strValue); }
	void GetAttrValue(LPCTSTR lpszName, CString& strValue) { _bstr_t bstrName(lpszName); GetNodeValue(m_pNode->attributes->getNamedItem(bstrName), strValue); }
	void SetAttrValue(LPCTSTR lpszName, const CString& strValue) { _bstr_t bstrName(lpszName); SetNodeValue(m_pNode->attributes->getNamedItem(bstrName), strValue); }
	int GetAttrCount() { return m_pNode->attributes->length; }

public:
	void GetChildElmtName(int nIndex, CString& strName) { GetNodeName(m_pNode->childNodes->item[nIndex], strName); }
	void GetChildElmtValue(int nIndex, CString& strValue) { GetNodeValue(m_pNode->childNodes->item[nIndex], strValue); }
	void SetChildElmtValue(int nIndex, const CString& strValue) { SetNodeValue(m_pNode->childNodes->item[nIndex], strValue); }
	void GetChildElmtValue(LPCTSTR lpszName, CString& strValue) { GetNodeValue(SelectChildNode(m_pNode, lpszName), strValue); }
	void SetChildElmtValue(LPCTSTR lpszName, const CString& strValue) { SetNodeValue(SelectChildNode(m_pNode, lpszName), strValue); }
	int GetChildElmtCount() { return m_pNode->childNodes->length; }

protected:
	void ArrangeXML(MSXML2::IXMLDOMNodePtr pCurrentNode);
	MSXML2::IXMLDOMNodeListPtr SelectNodes(MSXML2::IXMLDOMNodePtr pCurrentNode, LPCTSTR lpszQuery);
	MSXML2::IXMLDOMNodePtr SelectSingleNode(MSXML2::IXMLDOMNodePtr pCurrentNode, LPCTSTR lpszQuery);
	MSXML2::IXMLDOMNodePtr SelectChildNode(MSXML2::IXMLDOMNodePtr pCurrentNode, LPCTSTR lpszChildName);
	MSXML2::IXMLDOMNodePtr AddChild(MSXML2::IXMLDOMNodePtr pCurrentNode, MSXML2::IXMLDOMNodePtr pRefChildNode, MSXML2::DOMNodeType nodeType, LPCTSTR lpszName, LPCTSTR lpszData);

public:
	void GetXML(MSXML2::IXMLDOMNodePtr nodePtr, CString& strXml);
	void GetNodeText(MSXML2::IXMLDOMNodePtr nodePtr, CString& strText);
	void GetNodeName(MSXML2::IXMLDOMNodePtr nodePtr, CString& strName);
	void GetNodeValue(MSXML2::IXMLDOMNodePtr nodePtr, CString& strValue);
	void SetNodeValue(MSXML2::IXMLDOMNodePtr nodePtr, const CString& strValue);

#ifdef _DEBUG
private:
	void TestInfo(MSXML2::IXMLDOMNodePtr nodePtr);
	void TestInfo(MSXML2::IXMLDOMNodeListPtr nodeListPtr);
	void TestInfo(MSXML2::IXMLDOMNamedNodeMapPtr namedNodeMapPtr);
	void TestDebug(MSXML2::IXMLDOMNodePtr nodePtr);
#endif
};

class AFX_EXT_CLASS CXmlDomNode : public CObject
{
public:
	CXmlDomNode(MSXML2::IXMLDOMNodePtr pNodePtr);
	virtual ~CXmlDomNode();

protected:
	MSXML2::IXMLDOMNodePtr m_pNodePtr;

public:
//	CString GetNodeName();
//	CString GetNodeValue();
//	MSXML2::DOMNodeType GetNodeType();
//	CString GetNodeTypeString();
//	BOOL GetSpecified();
//	CString GetText();
};

//#undef AFX_DATA
//#define AFX_DATA
