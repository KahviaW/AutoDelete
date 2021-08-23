
// AutoDeleteDlg.h : ͷ�ļ�
//

#pragma once


// CAutoDeleteDlg �Ի���
class CAutoDeleteDlg : public CDialogEx
{
// ����
public:
	CAutoDeleteDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AUTODELETE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMfcbuttonViewPath();
	afx_msg void OnBnClickedMfcbuttonSave();
	afx_msg void SaveConfig();
	afx_msg void OnBnClickedMfcbuttonRead();
	afx_msg void ReadConfig();
	afx_msg void OnBnClickedMfcbuttonReloadSize();
	afx_msg void ReloadSize();
	afx_msg void OnBnClickedMfcbuttonReloadCount();
	afx_msg void ReloadCount();
	afx_msg void ShowTray(bool bShow);
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedMfcbuttonTray();
	afx_msg void DeleteFilesbyCount();
	afx_msg void DeleteFilesbySize();
};
