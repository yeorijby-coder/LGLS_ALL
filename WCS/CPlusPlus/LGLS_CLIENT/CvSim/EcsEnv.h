#ifndef __ECSENV_H__
#define __ECSENV_H__

//#define DEF_SIMULATION

// ListCtrl 
#define DEF_LVCOLUMN_IMG_SIZE		18
#define DEF_LVITEM_IMG_SIZE			18

// Button
#define BUTTON_TYPE	CShadeButtonST
#define INIT_BUTTON(btn_name, icon_image, w, h, tooltip_text) \
	btn_name.SetShade(CShadeButtonST::SHS_SOFTBUMP); \
	btn_name.SetIcon(icon_image, w, h); \
	btn_name.SetFlat(FALSE); \
	btn_name.EnableBalloonTooltip(); \
	btn_name.SetTooltipText(tooltip_text)

#define SET(xArg, yArg)			GetDlgItem(xArg)->SetWindowText(yArg)
#define GET(xArg, yArg)			GetDlgItem(xArg)->GetWindowText(yArg)
#define SET_INT(xArg, yArg)		SetDlgItemInt(xArg, yArg)
#define GET_INT(xArg)			GetDlgItemInt(xArg)
#define SET_TEXT(xArg, yArg)	SetDlgItemText(xArg, yArg)
#define GET_TEXT(xArg, yArg)	GetDlgItemText(xArg, yArg)

#define SET_CHECK(xArg, yArg)	((CButton*)GetDlgItem(xArg))->SetCheck(yArg)
#define GET_CHECK(xArg)			((CButton*)GetDlgItem(xArg))->GetCheck()

#define SET_SEL(xArg, yArg)		((CComboBox*)GetDlgItem(xArg))->SetCurSel(yArg)
#define GET_SEL(xArg)			((CComboBox*)GetDlgItem(xArg))->GetCurSel()

#define SET_BOX(xArg, yArg)			((CComboBox*)GetDlgItem(xArg))->SelectString(0, yArg)
#define GET_BOX(xArg, yArg)			((CComboBox*)GetDlgItem(xArg))->GetLBText(GET_SEL(xArg), yArg)

#define SEL_BOX(xArg, yArg)			SET_SEL(xArg, ((CComboBox*)GetDlgItem(xArg))->SelectString(-1, yArg))

#define ENABLE_WND(xArg, yArg)	GetDlgItem(xArg)->EnableWindow(yArg);

#define WM_USER_HOST_NOTIFY				(WM_USER + 1)
#define WM_USER_CV_NOTIFY				(WM_USER + 2)
#define WM_USER_SC_NOTIFY				(WM_USER + 3)
#define WM_USER_RTV_NOTIFY				(WM_USER + 4)
#define WM_USER_LGV_NOTIFY				(WM_USER + 5)
#define WM_USER_LIFTER_NOTIFY			(WM_USER + 6)
#define WM_USER_RFID_NOTIFY				(WM_USER + 7)
#define WM_USER_BCR_NOTIFY				(WM_USER + 8)
#define WM_USER_DISPLAY_NOTIFY			(WM_USER + 9)
#define WM_USER_THS_NOTIFY				(WM_USER + 10)
#define WM_USER_CONTROL_CLICK			(WM_USER + 11)
#define WM_USER_ALARM_NOTIFY			(WM_USER + 12)
#define WM_USER_REFRESH_NOTIFY			(WM_USER + 13)
#define WM_USER_MESSAGE_NOTIFY			(WM_USER + 14)

#define NOTIFY_REFRESH			1
#define NOTIFY_SEND				2
#define NOTIFY_RECEIVE 			3
#define NOTIFY_ERROR			4

#define ALLET_MESSAGE(xArg)	 m_pDoc->DisplayMessage(xArg, FALSE)
#define NORMAL_MESSAGE(xArg) m_pDoc->DisplayMessage(xArg)

#define SOH				0x01
#define	STX				0x02
#define	ETX				0x03
#define	EOT				0x04
#define	ENQ				0x05
#define	ACK				0x06
#define	NAK				0x15
#define	CR				0x0d
#define	LF				0x0a
#define GS				0x1D

// CEcsView 통신 RED용 플래그
#define COMM_STA_STOP			1
#define COMM_STA_SEND			2
#define COMM_STA_RECEIVE		3

// CSerial
#define ERR_PORT_NOT_OPEN		-1
#define ERR_INIT_FAIL			-2


#endif //__ECSENV_H__