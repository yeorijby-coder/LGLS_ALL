
#ifndef _COLORPICKER_H_
#define _COLORPICKER_H_

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	In order to ease use, these values have been hard coded in colorpicker.rc
	This avoids the need for another header file.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
#define IDD_COLORBTN                    (5100)
#define IDC_COLOR1                      (5101)
#define IDC_COLOR20                     (5120)
#define IDC_OTHER                       (5121)


class AFX_EXT_CLASS CColorPickerDlg : public CDialog
{
public:
	CColorPickerDlg(CWnd* pParent = NULL);

public:
	static COLORREF colors[20];
    static BYTE used[20];

    CButton *parent;
    int m_nIndex;

public:   
// Dialog Data
	//{{AFX_DATA(CColorPickerDlg)
	enum { IDD = IDD_COLORBTN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorPickerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColorPickerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnOther();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	void OnColor(UINT id);    
    void EndDialog( int nResult );

	DECLARE_MESSAGE_MAP()

};

class AFX_EXT_CLASS CColorPicker : public CButton
{
public:
    CColorPicker();

public:
    COLORREF m_clrCurrent;

	void Serialize( CArchive& ar );
	static void Reset();

	static BOOL Load();
    static BOOL Store();
	  
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorPicker)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorPicker();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorPicker)
	afx_msg void OnClicked();
	//}}AFX_MSG

    void DrawItem(LPDRAWITEMSTRUCT);

	DECLARE_MESSAGE_MAP()

private:
	CColorPickerDlg dlg;
};

#endif
