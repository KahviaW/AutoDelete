
// AutoDeleteDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AutoDelete.h"
#include "AutoDeleteDlg.h"
#include "afxdialogex.h"
#include <string>
#include <iostream> 
#include <io.h>
#include <vector>
#include <windows.h>
#include <atlstr.h>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef CONFIG_FILE  
#define CONFIG_FILE     (TEXT("Config.ini"))  
#endif  

int szFileCount = 0;
// CAutoDeleteDlg �Ի���

#define WM_SHOWTASK (WM_USER +1)
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)



CAutoDeleteDlg::CAutoDeleteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_AUTODELETE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CAutoDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAutoDeleteDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MFCBUTTON_VIEW_PATH, &CAutoDeleteDlg::OnBnClickedMfcbuttonViewPath)
	ON_BN_CLICKED(IDC_MFCBUTTON_SAVE, &CAutoDeleteDlg::OnBnClickedMfcbuttonSave)
	ON_BN_CLICKED(IDC_MFCBUTTON_READ, &CAutoDeleteDlg::OnBnClickedMfcbuttonRead)
	ON_BN_CLICKED(IDC_MFCBUTTON_RELOAD_SIZE, &CAutoDeleteDlg::OnBnClickedMfcbuttonReloadSize)
	ON_BN_CLICKED(IDC_MFCBUTTON_RELOAD_COUNT, &CAutoDeleteDlg::OnBnClickedMfcbuttonReloadCount)
	ON_MESSAGE(WM_SHOWTASK, &CAutoDeleteDlg::OnShowTask)
	ON_BN_CLICKED(IDC_MFCBUTTON_TRAY, &CAutoDeleteDlg::OnBnClickedMfcbuttonTray)
END_MESSAGE_MAP()


// CAutoDeleteDlg ��Ϣ�������

BOOL CAutoDeleteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CAutoDeleteDlg::ReadConfig();
	CAutoDeleteDlg::ReloadSize();
	CAutoDeleteDlg::ReloadCount();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAutoDeleteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAutoDeleteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//���������ļ�
void CAutoDeleteDlg::SaveConfig()
{
	// ----------------------------------------  
	// ģ��д��һ��config.ini  
	// ----------------------------------------  
	CEdit* EditBox_Path;
	EditBox_Path = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
	CString str_Path;
	EditBox_Path->GetWindowText(str_Path);

	CEdit* EditBox_Size;
	EditBox_Size = (CEdit*)GetDlgItem(IDC_EDIT_SIZE);
	CString str_Size;
	EditBox_Size->GetWindowText(str_Size);

	CEdit* EditBox_Count;
	EditBox_Count = (CEdit*)GetDlgItem(IDC_EDIT_COUNT);
	CString str_Count;
	EditBox_Count->GetWindowText(str_Count);

	CEdit* EditBox_Cooldown;
	EditBox_Cooldown = (CEdit*)GetDlgItem(IDC_EDIT_COOLDOWN);
	CString str_Cooldown;
	EditBox_Cooldown->GetWindowText(str_Cooldown);

	CButton* pButton_Size = (CButton*)GetDlgItem(IDC_RADIO_CHECK_SIZE);
	CButton* pButton_Count = (CButton*)GetDlgItem(IDC_RADIO_CHECK_COUNT);

	// �õ�exeִ��·��.  
	TCHAR tcExePath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, tcExePath, MAX_PATH);
	// ����ini·����exeͬһĿ¼��  

	//_tcsrchr() ��������������һ��'\\'��λ�ã������ظ�λ�õ�ָ��  
	TCHAR *pFind = _tcsrchr(tcExePath, '\\');
	if (pFind == NULL)
	{
		return;
	}
	*pFind = '\0';
	CString szIniPath = tcExePath;
	szIniPath += "\\";
	szIniPath += CONFIG_FILE;

	//--------------------------------------------------------  
	//BOOL WritePrivateProfileString(  
	//                              LPCTSTR lpAppName,      //�ڵ����֣���һ����0�������ַ���  
	//                              LPCTSTR lpKeyName,      //�������֣���һ����0�������ַ�������ΪNULL����ɾ��������  
	//                              LPCTSTR lpString,       //����ֵ����һ����0�������ַ�������ΪNULL����ɾ����Ӧ�ļ�  
	//                              LPCTSTR lpFileName      //Ҫд����ļ����ļ���������ini�ļ��������ͬһ��Ŀ¼�£�  
	//                              )                          Ҳ��ʹ�����·��,������Ҫ��������·����  
	//���Ini�����ڣ������Զ���szIniPath�ϴ�����INI�ļ�.��ִ��д��.                             
	::WritePrivateProfileString(TEXT("Config"), TEXT("PATH"), str_Path, szIniPath);

	if (pButton_Size->GetCheck() == 1)
	{
		::WritePrivateProfileString(TEXT("Config"), TEXT("CHECKSIZE"), TEXT("1"), szIniPath);
	}
	else
		::WritePrivateProfileString(TEXT("Config"), TEXT("CHECKSIZE"), TEXT("0"), szIniPath);
	::WritePrivateProfileString(TEXT("Config"), TEXT("MAXSIZE"), str_Size, szIniPath);

	if (pButton_Count->GetCheck() == 1)
	{
		::WritePrivateProfileString(TEXT("Config"), TEXT("CHECKCOUNT"), TEXT("1"), szIniPath);
	}
	else
		::WritePrivateProfileString(TEXT("Config"), TEXT("CHECKCOUNT"), TEXT("0"), szIniPath);
	::WritePrivateProfileString(TEXT("Config"), TEXT("MAXCOUNT"), str_Count, szIniPath);

	::WritePrivateProfileString(TEXT("Config"), TEXT("COOLDOWN"), str_Cooldown, szIniPath);
}

//��ȡ�����ļ�
void CAutoDeleteDlg::ReadConfig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// �õ�exeִ��·��.  
	TCHAR tcExePath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, tcExePath, MAX_PATH);
	// ����ini·����exeͬһĿ¼��  
#ifndef CONFIG_FILE   
#endif  
	//_tcsrchr() ��������������һ��'\\'��λ�ã������ظ�λ�õ�ָ��  
	TCHAR *pFind = _tcsrchr(tcExePath, '\\');
	if (pFind == NULL)
	{
		return;
	}
	*pFind = '\0';

	CString szIniPath = tcExePath;
	szIniPath += "\\";
	szIniPath += CONFIG_FILE;


	//ִ�ж�ȡ ----------------------------------  
	if (!::PathFileExists(szIniPath))
	{
		return;
	}

	TCHAR PathValue[MAX_PATH] = { 0 };
	int CheckSizeValue = 0;
	TCHAR SizeValue[MAX_PATH] = { 0 };
	int CheckCountValue = 0;
	TCHAR CountValue[MAX_PATH] = { 0 };
	TCHAR CooldownValue[MAX_PATH] = { 0 };

	//--------------------------------------------------------  
	//DWORD GetPrivateProfileString(  
	//                              LPCTSTR lpAppName,            // ����  
	//                              LPCTSTR lpKeyName,            // ��������ȡ�ü���ֵ  
	//                              LPCTSTR lpDefault,            // ��ָ���ļ������ڣ���ֵ��Ϊ��ȡ��Ĭ��ֵ  
	//                              LPTSTR lpReturnedString,      // һ��ָ�򻺳�����ָ�룬���ն�ȡ���ַ���  
	//                              DWORD nSize,                  // ָ��lpReturnedStringָ��Ļ������Ĵ�С  
	//                              LPCTSTR lpFileName            // ��ȡ��Ϣ���ļ���������ini�ļ��������ͬһ��Ŀ¼�£�  
	//                                                                Ҳ��ʹ�����·��,������Ҫ��������·��  
	//UINT GetPrivateProfileInt(  
	//                              LPCTSTR lpAppName,            // ����  
	//                              LPCTSTR lpKeyName,            // ��������ȡ�ü���ֵ  
	//                              INT nDefault,                 // ��ָ���ļ��������ڣ���ֵ��Ϊ��ȡ��Ĭ��ֵ  
	//                              LPCTSTR lpFileName            // ͬ��  
	//  
	//--------------------------------------------------------  

	::GetPrivateProfileString(TEXT("Config"), TEXT("PATH"), NULL, PathValue, MAX_PATH, szIniPath);
	CheckSizeValue = ::GetPrivateProfileInt(TEXT("Config"), TEXT("CHECKSIZE"), 0, szIniPath);
	::GetPrivateProfileString(TEXT("Config"), TEXT("MAXSIZE"), NULL, SizeValue, MAX_PATH, szIniPath);
	CheckCountValue = ::GetPrivateProfileInt(TEXT("Config"), TEXT("CHECKCOUNT"), 0, szIniPath);
	::GetPrivateProfileString(TEXT("Config"), TEXT("MAXCOUNT"), NULL, CountValue, MAX_PATH, szIniPath);
	::GetPrivateProfileString(TEXT("Config"), TEXT("COOLDOWN"), NULL, CooldownValue, MAX_PATH, szIniPath);

	CEdit *p_EditPath = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
	p_EditPath->SetWindowText(PathValue);

	CEdit *p_EditSize = (CEdit*)GetDlgItem(IDC_EDIT_SIZE);
	p_EditSize->SetWindowText(SizeValue);

	CEdit *p_EditCount = (CEdit*)GetDlgItem(IDC_EDIT_COUNT);
	p_EditCount->SetWindowText(CountValue);

	CEdit *p_EditCooldown = (CEdit*)GetDlgItem(IDC_EDIT_COOLDOWN);
	p_EditCooldown->SetWindowText(CooldownValue);

	if (CheckSizeValue == 1 && CheckSizeValue != CheckCountValue)
	{
		CheckRadioButton(IDC_RADIO_CHECK_SIZE, IDC_RADIO_CHECK_COUNT, IDC_RADIO_CHECK_SIZE);
	}
	else if (CheckSizeValue == 0 && CheckSizeValue != CheckCountValue)
	{
		CheckRadioButton(IDC_RADIO_CHECK_SIZE, IDC_RADIO_CHECK_COUNT, IDC_RADIO_CHECK_COUNT);
	}
}

//��ȡ�ļ��д�С
DWORD TraversalFolder(string strPath, vector<string>& files)
{
	DWORD dwRtn = 0;
	intptr_t hFolder = 0;                                     //�ļ����
	struct _finddata_t fileinfo;                         //�ļ���Ϣ
	string strFileName = "";

	if ((hFolder = _findfirst(strFileName.assign(strPath).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			DWORD dwSize = 0;
			//�����Ŀ¼,����֮;�������,�����б�
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					dwSize = TraversalFolder(strFileName.assign(strPath).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(strFileName.assign(strPath).append("\\").append(fileinfo.name));
				dwSize = fileinfo.size;
			}
			dwRtn += dwSize;
		} while (_findnext(hFolder, &fileinfo) == 0);

		_findclose(hFolder);
	}
	return dwRtn;
}

//��ȡ�ļ����ļ�����
void TraversalFolderCount(string strPath, vector<string>& files)
{
	intptr_t hFile = 0;//�ļ�������������������
	struct _finddata_t fileinfo;//�ļ���Ϣ
	string p;
	DWORD dwCount = 0;

	if ((hFile = _findfirst(p.assign(strPath).append("//*").c_str(), &fileinfo)) != -1)
		//������ҵ���һ���ļ�
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))//������ļ���
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					TraversalFolderCount(p.assign(strPath).append("\\").append(fileinfo.name), files);
				}
			}
			else//������ļ�
			{
				files.push_back(p.assign(strPath).append("\\").append(fileinfo.name));
				szFileCount++;
			}
		} while (_findnext(hFile, &fileinfo) == 0);    //��Ѱ�ҵ������ļ�
		_findclose(hFile);    //�������ң��رվ��
	}
}

//Ŀ¼ѡ��ť
void CAutoDeleteDlg::OnBnClickedMfcbuttonViewPath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	wchar_t temp[256];
	GetEnvironmentVariable(_T("userprofile"), temp, 256);
	CFolderPickerDialog dlg(temp, 0, NULL, 0);
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetPathName(); // ��ȡ�ļ�·��                            
		CEdit *p_EditPath = (CEdit*)GetDlgItem(IDC_EDIT_PATH); // д��textbox
		p_EditPath->SetWindowText(str);
	}
}

//���水ť
void CAutoDeleteDlg::OnBnClickedMfcbuttonSave()
{
	CAutoDeleteDlg::SaveConfig();
}

//��ȡ��ť
void CAutoDeleteDlg::OnBnClickedMfcbuttonRead()
{
	CAutoDeleteDlg::ReadConfig();
}

//ˢ���ļ��д�С
void CAutoDeleteDlg::OnBnClickedMfcbuttonReloadSize()
{
	CAutoDeleteDlg::ReloadSize();
}

void CAutoDeleteDlg::ReloadSize()
{
	CEdit* EditBox_Path;
	EditBox_Path = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
	CString str_Path;
	EditBox_Path->GetWindowText(str_Path);

	float FileSize;
	vector<string> files;
	string strStPath;
	strStPath = CW2A(str_Path.GetString());
	FileSize = TraversalFolder(strStPath, files);
	FileSize = FileSize / 1024 / 1024 / 1024;
	CString strFileSize;
	strFileSize.Format(_T("%.2f"), FileSize);
	GetDlgItem(IDC_STATIC_SIZE)->SetWindowText(strFileSize);
}

//ˢ���ļ�������
void CAutoDeleteDlg::OnBnClickedMfcbuttonReloadCount()
{
	CAutoDeleteDlg::ReloadCount();
}

void CAutoDeleteDlg::ReloadCount()
{
	CEdit* EditBox_Path;
	EditBox_Path = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
	CString str_Path;
	EditBox_Path->GetWindowText(str_Path);

	vector<string> files;
	string strStPath;
	strStPath = CW2A(str_Path.GetString());
	szFileCount = 0;
	TraversalFolderCount(strStPath, files);
	CString strFileCount;
	strFileCount.Format(_T("%d"), szFileCount);
	GetDlgItem(IDC_STATIC_COUNT)->SetWindowText(strFileCount);
}

//��С��������
void CAutoDeleteDlg::ShowTray(bool bShow)
{
	CEdit* EditBox_Path;
	EditBox_Path = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
	CString str_Path;
	EditBox_Path->GetWindowText(str_Path);

	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;    //����ͼ���id
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK;//�Զ���Ļص���Ϣ����id
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));    //����ͼ��
	wcscpy(nid.szTip, str_Path + _T("\n�Զ�����ɾ��"));
	Shell_NotifyIcon(bShow ? NIM_ADD : NIM_DELETE, &nid);//����������ӻ�ɾ��ͼ��
	if (bShow == true)
	{
		ShowWindow(SW_HIDE);
	}
	else if (bShow == false)
	{
		ShowWindow(SW_SHOW);
	}
}

//����ͼ��Ĵ����¼�
LRESULT CAutoDeleteDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	NOTIFYICONDATA nid;
	//w���� ����id �� L����������Ϣid
	if (wParam != IDR_MAINFRAME)
		return 1;
	switch (lParam)
	{
	case WM_LBUTTONUP:  //���������ʾ������
		ShowWindow(SW_SHOW);
		ShowTray(false);
		//��������ͼ�ꡣ
		break;
	case WM_RBUTTONUP:  //�һ������˵�
		break;
	case WM_LBUTTONDBLCLK:  //���˫��������
		break;

	default:
		break;
	}
}

void CAutoDeleteDlg::OnBnClickedMfcbuttonTray()
{
	CAutoDeleteDlg::ShowTray(true);
}

void CAutoDeleteDlg::DeleteFilesbyCount()
{
	CAutoDeleteDlg::ReloadCount();
	CEdit* EditBox_FileCount;
	EditBox_FileCount = (CEdit*)GetDlgItem(IDC_STATIC_COUNT);
	CString Cstr_Count;
	EditBox_FileCount->GetWindowText(Cstr_Count);
	CEdit* EditBox_MaxFileCount;
	EditBox_MaxFileCount = (CEdit*)GetDlgItem(IDC_EDIT_COUNT);
	CString Cstr_MaxCount;
	EditBox_MaxFileCount->GetWindowText(Cstr_MaxCount);

	string str_Count = CW2A(Cstr_Count.GetString());	//CStringתString
	char* char_Count;
	string str_MaxCount = CW2A(Cstr_MaxCount.GetString());
	char* char_MaxCount;
	int int_Count = static_cast<int>(strtol(str_Count.c_str(), &char_Count, 16));	//Stringתchar*��ת��int
	int int_MaxCount = static_cast<int>(strtol(str_MaxCount.c_str(), &char_MaxCount, 16));
	int DeleteCount = int_Count - int_MaxCount;	//��ȡ��Ҫɾ�������ļ�


}


void CAutoDeleteDlg::DeleteFilesbySize()
{

}