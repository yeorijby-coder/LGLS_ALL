// BuilderDoc.cpp : implementation of the CBuilderDoc class
//

#include "stdafx.h"
#include "Builder.h"

#include "BuilderDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBuilderDoc

IMPLEMENT_DYNCREATE(CBuilderDoc, CDocument)

BEGIN_MESSAGE_MAP(CBuilderDoc, CDocument)
	//{{AFX_MSG_MAP(CBuilderDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBuilderDoc construction/destruction

CBuilderDoc::CBuilderDoc()
{
	// TODO: add one-time construction code here

}

CBuilderDoc::~CBuilderDoc()
{
}

BOOL CBuilderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBuilderDoc serialization

void CBuilderDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBuilderDoc diagnostics

#ifdef _DEBUG
void CBuilderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBuilderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBuilderDoc commands
