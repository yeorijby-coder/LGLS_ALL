// ColorBtn.cpp : implementation file

#include "stdafx.h"
#include "ColorPicker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorPicker

CColorPicker::CColorPicker()
{
    m_clrCurrent = RGB(255, 255, 255);

    dlg.parent = this;   // This will allow the dialog to position itself

    // Create the pens and brushes that we'll need to draw the button
}


CColorPicker::~CColorPicker()
{
}


BEGIN_MESSAGE_MAP(CColorPicker, CButton)
	//{{AFX_MSG_MAP(CColorPicker)	
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorPicker message handlers


void CColorPicker::DrawItem(LPDRAWITEMSTRUCT lpd)
{
    // Draw the button

    CDC DC;
    DC.Attach(lpd->hDC);
    
    int top    = lpd->rcItem.top;
    int left   = lpd->rcItem.left;
    int bottom = lpd->rcItem.bottom;
    int right  = lpd->rcItem.right;

	CPen nullPen, blackPen, whitePen, dkgrayPen;
	nullPen.CreateStockObject(NULL_PEN);
	blackPen.CreateStockObject(BLACK_PEN);
	whitePen.CreateStockObject(WHITE_PEN);

	dkgrayPen.CreatePen(PS_SOLID,1,RGB(128,128,128));

	CBrush backBrush;
	backBrush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));

    CBrush colorBrush;	
    colorBrush.CreateSolidBrush(m_clrCurrent);
    
	CPen *pOldpen = DC.SelectObject(&nullPen);
	CBrush *pOldbrush = DC.SelectObject(&backBrush);

    // Clear the background using the 3DFACE color.
    DC.Rectangle(&lpd->rcItem);

    // Draw the border
    if (!(lpd->itemState & ODS_SELECTED))
    {
        // Button is up
        DC.SelectObject(&blackPen);
        DC.MoveTo(left, bottom-1);
        DC.LineTo(right-1, bottom-1);
        DC.LineTo(right-1, top);

        DC.SelectObject(&dkgrayPen);
        DC.MoveTo(left+1, bottom-2);
        DC.LineTo(right-2, bottom-2);
        DC.LineTo(right-2, top+1);

		DC.SelectObject(&whitePen);
        DC.LineTo(left+1, top+1);
        DC.LineTo(left+1, bottom-2);
    }
    else
    {
        // Button is down
        DC.SelectObject(&dkgrayPen);            
        DC.MoveTo(left,bottom-1);
        DC.LineTo(left,top);
        DC.LineTo(right-1,top);

		DC.SelectObject(&whitePen);
        DC.MoveTo(right-1,top-1);
        DC.LineTo(right-1,bottom-1);
        DC.LineTo(left+1,bottom-1);

        DC.SelectObject(&blackPen);
        DC.MoveTo(left+1,bottom-2);
        DC.LineTo(left+1,top+1);
        DC.LineTo(right-2,top+1);

        // by moving this, we get the things inside the button
        // to draw themselves one pixel down and one to the right.
        // This completes the "pushed" effect

        left++;
        right++;
        bottom++;
        top++;
    }

    // The division

	DC.SelectObject(&whitePen);
    DC.MoveTo(right-10, top+4);
    DC.LineTo(right-10, bottom-4);

    DC.SelectObject(dkgrayPen);
	DC.MoveTo(right-11, top+4);
    DC.LineTo(right-11, bottom-4);

    // The triangle
    if (lpd->itemState & ODS_DISABLED)
        DC.SelectObject(dkgrayPen);
    else
        DC.SelectObject(&blackPen);
    
    DC.MoveTo(right-4, (bottom/2)-1);
    DC.LineTo(right-9, (bottom/2)-1);

    DC.MoveTo(right-5, (bottom/2));
    DC.LineTo(right-8, (bottom/2));

    if (lpd->itemState & ODS_DISABLED)    
    {
        DC.SetPixel(right-4, (bottom/2)-1, RGB(255,255,255));
        DC.SetPixel(right-5, (bottom/2), RGB(255,255,255));
        DC.SetPixel(right-6, (bottom/2)+1, RGB(255,255,255));
    }
    else
    {
        DC.SetPixel(right-6, (bottom/2)+1, RGB(0,0,0));
    }

    if (!(lpd->itemState & ODS_DISABLED))
    {
        // The color rectangle, only if enabled
        DC.SelectObject(&colorBrush);
        DC.Rectangle(left+5, top+4, right-15, bottom-4);    
    }

/*
    if (lpd->itemState & ODS_FOCUS)
    {
        // Draw the focus
        //
        // It would have been nice just to
        // draw a rectangle using a pen created
        // with the PS_ALTERNATE style, but
        // this is not supported by WIN95

        for (int i=left+3;i<right-4;i+=2)
        {
            DC.SetPixel(i, top+3, RGB(0,0,0));
            DC.SetPixel(i, bottom-4, RGB(0,0,0));
        }

        for (i=top+3;i<bottom-4;i+=2)
        {
            DC.SetPixel(left+3, i, RGB(0,0,0));
            DC.SetPixel(right-4, i, RGB(0,0,0));
        }       
    }
*/
    DC.SelectObject(pOldpen);
    DC.SelectObject(pOldbrush);

    DC.Detach();    
}


void CColorPicker::OnClicked() 
{
	if (dlg.DoModal() == IDOK)
    {
        m_clrCurrent = CColorPickerDlg::colors[dlg.m_nIndex];
        InvalidateRect(NULL);
    }	
}

// Store and Load use an undocumented CWinApp function
BOOL CColorPicker::Store()
{
    return (AfxGetApp()->WriteProfileBinary(_T("ColorData"), _T("ColorTable"), (LPBYTE)CColorPickerDlg::colors, sizeof(COLORREF)*20) &&
            AfxGetApp()->WriteProfileBinary(_T("ColorData"), _T("MRU"), (LPBYTE)CColorPickerDlg::used, sizeof(BYTE)*20));

}

BOOL CColorPicker::Load()
{
    BYTE *data = NULL;
    UINT size;

    // This function allocates the memory it needs
    AfxGetApp()->GetProfileBinary(_T("ColorData"), _T("ColorTable"), &data, &size);	

    if (data)
    {
        memcpy((void *)CColorPickerDlg::colors, (void *)data, size);
        free((void *)data);

        AfxGetApp()->GetProfileBinary(_T("ColorData"), _T("MRU"), &data, &size);	

        if (data)
        {
            memcpy((void *)CColorPickerDlg::used,(void *)data,size);
            free((void *)data);
            return TRUE;
        }
        
    }

    // If the loading fails, back to the defaults
	Reset();

    return FALSE;
}

void CColorPicker::Reset()
{
    CColorPickerDlg::colors[0]  = RGB(0,0,0);
    CColorPickerDlg::colors[1]  = RGB(128,0,0);
    CColorPickerDlg::colors[2]  = RGB(0,128,0);
    CColorPickerDlg::colors[3]  = RGB(128,128,0);
    CColorPickerDlg::colors[4]  = RGB(0,0,128);
    CColorPickerDlg::colors[5]  = RGB(128,0,128);
    CColorPickerDlg::colors[6]  = RGB(0,128,128);
    CColorPickerDlg::colors[7]  = RGB(192,192,192);
    CColorPickerDlg::colors[8]  = RGB(192,220,192);
    CColorPickerDlg::colors[9]  = RGB(166,202,240);
    CColorPickerDlg::colors[10] = RGB(255,251,240);
    CColorPickerDlg::colors[11] = RGB(160,160,164);
    CColorPickerDlg::colors[12] = RGB(128,128,128);
    CColorPickerDlg::colors[13] = RGB(255,0,0);
    CColorPickerDlg::colors[14] = RGB(0,255,0);
    CColorPickerDlg::colors[15] = RGB(255,255,0);
    CColorPickerDlg::colors[16] = RGB(0,0,255);
    CColorPickerDlg::colors[17] = RGB(255,0,255);
    CColorPickerDlg::colors[18] = RGB(0,255,255);
    CColorPickerDlg::colors[19] = RGB(255,255,255);

    // This "colorful" (no pun intended) order ensures
    // that the colors at the center of the color table
    // will get replaced first. This preserves the white
    // and black colors even if they're not used (They'll
    // get replaced last).
    
    CColorPickerDlg::used[0]= 1;
    CColorPickerDlg::used[1]= 3;
    CColorPickerDlg::used[2]= 5;
    CColorPickerDlg::used[3]= 7;
    CColorPickerDlg::used[4]= 9;
    CColorPickerDlg::used[5]= 11;
    CColorPickerDlg::used[6]= 13;
    CColorPickerDlg::used[7]= 15;
    CColorPickerDlg::used[8]= 17;
    CColorPickerDlg::used[9]= 19;
    CColorPickerDlg::used[10]= 20;
    CColorPickerDlg::used[11]= 18;
    CColorPickerDlg::used[12]= 16;
    CColorPickerDlg::used[13]= 14;
    CColorPickerDlg::used[14]= 12;
    CColorPickerDlg::used[15]= 10;
    CColorPickerDlg::used[16]= 8;
    CColorPickerDlg::used[17]= 6;
    CColorPickerDlg::used[18]= 4;
    CColorPickerDlg::used[19]= 2;
}

void CColorPicker::Serialize( CArchive& ar )
{
    if (ar.IsStoring())
    {
        ar.Write((void *)CColorPickerDlg::colors,sizeof(COLORREF)*20);
        ar.Write((void *)CColorPickerDlg::used,sizeof(BYTE)*20);
    }
    else
    {
        ar.Read((void *)CColorPickerDlg::colors,sizeof(COLORREF)*20);
        ar.Read((void *)CColorPickerDlg::used,sizeof(BYTE)*20);
    }
}

void CColorPicker::PreSubclassWindow() 
{
	SetButtonStyle(GetButtonStyle() | BS_OWNERDRAW);
	
	CButton::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CColorPickerDlg dialog


CColorPickerDlg::CColorPickerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorPickerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorPickerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CColorPickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorPickerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorPickerDlg, CDialog)
	//{{AFX_MSG_MAP(CColorPickerDlg)
    ON_BN_CLICKED(IDC_OTHER, OnOther)
	ON_WM_LBUTTONDOWN()	
	ON_WM_LBUTTONUP()
    ON_WM_DRAWITEM()	
	//}}AFX_MSG_MAP
    ON_COMMAND_RANGE(IDC_COLOR1,IDC_COLOR20,OnColor)
END_MESSAGE_MAP()


COLORREF CColorPickerDlg::colors[20] =
{
    RGB(0,0,0),
    RGB(128,0,0),
    RGB(0,128,0),
    RGB(128,128,0),
    RGB(0,0,128),
    RGB(128,0,128),
    RGB(0,128,128),
    RGB(192,192,192),
    RGB(192,220,192),
    RGB(166,202,240),
    RGB(255,251,240),
    RGB(160,160,164),
    RGB(128,128,128),
    RGB(255,0,0),
    RGB(0,255,0),
    RGB(255,255,0),
    RGB(0,0,255),
    RGB(255,0,255),
    RGB(0,255,255),
    RGB(255,255,255)
};

// MRU table. See notes for Reset()
BYTE CColorPickerDlg::used[20] =
{    
    1,3,5,7,9,11,13,15,17,19,20,18,16,14,12,10,8,6,4,2    
};

/////////////////////////////////////////////////////////////////////////////
// CColorPickerDlg message handlers

BOOL CColorPickerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    RECT r,r2;
	
	parent->GetWindowRect(&r);
    
    // Move the dialog to be below the button

    SetWindowPos(NULL,r.left,r.bottom,0,0,SWP_NOSIZE|SWP_NOZORDER);

    GetWindowRect(&r2);

    // Check to see if the dialog has a portion outside the
    // screen, if so, adjust.
    
    if (r2.bottom > GetSystemMetrics(SM_CYSCREEN))
    {   
        r2.top = r.top-(r2.bottom-r2.top);        
    }

    if (r2.right > GetSystemMetrics(SM_CXSCREEN))
    {
        r2.left = GetSystemMetrics(SM_CXSCREEN) - (r2.right-r2.left);
    }

    SetWindowPos(NULL,r2.left,r2.top,0,0,SWP_NOSIZE|SWP_NOZORDER);

    // Capture the mouse, this allows the dialog to close when
    // the user clicks outside.

    // Remember that the dialog has no "close" button.

    SetCapture();
	
	return TRUE; 
}

void CColorPickerDlg::EndDialog( int nResult )
{
    ReleaseCapture();

    CDialog::EndDialog(nResult);
}

void CColorPickerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
    RECT r;

    POINT p;
    p.x = point.x;
    p.y = point.y;

    ClientToScreen(&p);

    GetWindowRect(&r);

    // The user clicked...

    if (!PtInRect(&r,p))
    {
        //  ...outside the dialog, close.

        EndDialog(IDCANCEL);
    }
    else
    {
        //  ...inside the dialog. Since this window
        //     has the mouse captured, its children
        //     get no messages. So, check to see
        //     if the click was in one of its children
        //     and tell him.

        //     If the user clicks inside the dialog
        //     but not on any of the controls,
        //     ChildWindowFromPoint returns a
        //     pointer to the dialog. In this
        //     case we do not resend the message
        //     (obviously) because it would cause
        //     a stack overflow.
        
        CWnd *child = ChildWindowFromPoint(point);

        if (child && child != this)
            child->SendMessage(WM_LBUTTONDOWN,0,0l);
    }
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CColorPickerDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpd) 
{
	CDC dc;
    CPen nullpen;
    CBrush brush;
    CPen *oldpen;
    CBrush *oldbrush;

    // Draw the wells using the current color table

    nullpen.CreateStockObject(NULL_PEN);
    brush.CreateSolidBrush(colors[nIDCtl-IDC_COLOR1]);

    dc.Attach(lpd->hDC);

    oldpen = dc.SelectObject(&nullpen);
    oldbrush = dc.SelectObject(&brush);

    lpd->rcItem.right++;
    lpd->rcItem.bottom++;

    dc.Rectangle(&lpd->rcItem);

    dc.SelectObject(oldpen);
    dc.SelectObject(oldbrush);

    dc.Detach();
	
	CDialog::OnDrawItem(nIDCtl, lpd);
}

void CColorPickerDlg::OnColor(UINT id)
{

    // A well has been clicked, set the color index
    // and close.

    m_nIndex = id-IDC_COLOR1;
    
    int i;

    // This color is now the MRU

    for (i=0;i<20;i++)
    {
        if (used[m_nIndex] > used[i])
        {
            used[i]++;
        }
    }

    used[m_nIndex] = 1;

    EndDialog(IDOK);

}

void CColorPickerDlg::OnOther() 
{

    ReleaseCapture();

	CColorDialog dlg;
    dlg.m_cc.Flags |= CC_FULLOPEN;


    if (dlg.DoModal() == IDOK)
    {
		COLORREF newcolor = dlg.GetColor();            

        // Check to see if the selected color is already in the table.
        m_nIndex = -1;

        for (int i=0; i<20; i++)
        {
            if (colors[i] == newcolor)
            {
                m_nIndex = i;
                break;
            }
        }

        // If the color was not found, replace the LRU with this color
        if (m_nIndex == -1)
        {
            for (int i=0; i<20; i++)
            {
                if (used[i] == 20)
                {
                    colors[i] = newcolor;
                    m_nIndex = i;                                 
                    break;
                }
            }
        }

        // This is the new MRU
        for (int i=0; i<20; i++)
        {
            if (used[m_nIndex] > used[i])
            {
                used[i]++;
            }         
        }

        used[m_nIndex] = 1;

        EndDialog(IDOK);
        return;
    }

    // If the user clicked "Cancel" reclaim the mouse capture.
    SetCapture();        	
}

void CColorPickerDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{

    // See notes for OnLButtonDown.
    
    CWnd *child = ChildWindowFromPoint(point,CWP_ALL);
    
    if (child && child != this)
        child->SendMessage(WM_LBUTTONDOWN, 0, 0l);	
	
	CDialog::OnLButtonUp(nFlags, point);
}

