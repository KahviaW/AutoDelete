
// AutoDeleteDlg.cpp : 实现文件
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
// CAutoDeleteDlg 对话框

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


// CAutoDeleteDlg 消息处理程序

BOOL CAutoDeleteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CAutoDeleteDlg::ReadConfig();
	CAutoDeleteDlg::ReloadSize();
	CAutoDeleteDlg::ReloadCount();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAutoDeleteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAutoDeleteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//保存配置文件
void CAutoDeleteDlg::SaveConfig()
{
	// ----------------------------------------  
	// 模拟写入一个config.ini  
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

	// 得到exe执行路径.  
	TCHAR tcExePath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, tcExePath, MAX_PATH);
	// 设置ini路径到exe同一目录下  

	//_tcsrchr() 反向搜索获得最后一个'\\'的位置，并返回该位置的指针  
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
	//                              LPCTSTR lpAppName,      //节的名字，是一个以0结束的字符串  
	//                              LPCTSTR lpKeyName,      //键的名字，是一个以0结束的字符串。若为NULL，则删除整个节  
	//                              LPCTSTR lpString,       //键的值，是一个以0结束的字符串。若为NULL，则删除对应的键  
	//                              LPCTSTR lpFileName      //要写入的文件的文件名。若该ini文件与程序在同一个目录下，  
	//                              )                          也可使用相对路径,否则需要给出绝度路径。  
	//如果Ini不存在，它会自动在szIniPath上创建此INI文件.再执行写入.                             
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

//读取配置文件
void CAutoDeleteDlg::ReadConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	// 得到exe执行路径.  
	TCHAR tcExePath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, tcExePath, MAX_PATH);
	// 设置ini路径到exe同一目录下  
#ifndef CONFIG_FILE   
#endif  
	//_tcsrchr() 反向搜索获得最后一个'\\'的位置，并返回该位置的指针  
	TCHAR *pFind = _tcsrchr(tcExePath, '\\');
	if (pFind == NULL)
	{
		return;
	}
	*pFind = '\0';

	CString szIniPath = tcExePath;
	szIniPath += "\\";
	szIniPath += CONFIG_FILE;


	//执行读取 ----------------------------------  
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
	//                              LPCTSTR lpAppName,            // 节名  
	//                              LPCTSTR lpKeyName,            // 键名，读取该键的值  
	//                              LPCTSTR lpDefault,            // 若指定的键不存在，该值作为读取的默认值  
	//                              LPTSTR lpReturnedString,      // 一个指向缓冲区的指针，接收读取的字符串  
	//                              DWORD nSize,                  // 指定lpReturnedString指向的缓冲区的大小  
	//                              LPCTSTR lpFileName            // 读取信息的文件名。若该ini文件与程序在同一个目录下，  
	//                                                                也可使用相对路径,否则需要给出绝度路径  
	//UINT GetPrivateProfileInt(  
	//                              LPCTSTR lpAppName,            // 节名  
	//                              LPCTSTR lpKeyName,            // 键名，读取该键的值  
	//                              INT nDefault,                 // 若指定的键名不存在，该值作为读取的默认值  
	//                              LPCTSTR lpFileName            // 同上  
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

//获取文件夹大小
DWORD TraversalFolder(string strPath, vector<string>& files)
{
	DWORD dwRtn = 0;
	intptr_t hFolder = 0;                                     //文件句柄
	struct _finddata_t fileinfo;                         //文件信息
	string strFileName = "";

	if ((hFolder = _findfirst(strFileName.assign(strPath).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			DWORD dwSize = 0;
			//如果是目录,迭代之;如果不是,加入列表
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

//获取文件夹文件数量
void TraversalFolderCount(string strPath, vector<string>& files)
{
	intptr_t hFile = 0;//文件句柄，过会儿用来查找
	struct _finddata_t fileinfo;//文件信息
	string p;
	DWORD dwCount = 0;

	if ((hFile = _findfirst(p.assign(strPath).append("//*").c_str(), &fileinfo)) != -1)
		//如果查找到第一个文件
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))//如果是文件夹
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					TraversalFolderCount(p.assign(strPath).append("\\").append(fileinfo.name), files);
				}
			}
			else//如果是文件
			{
				files.push_back(p.assign(strPath).append("\\").append(fileinfo.name));
				szFileCount++;
			}
		} while (_findnext(hFile, &fileinfo) == 0);    //能寻找到其他文件
		_findclose(hFile);    //结束查找，关闭句柄
	}
}

//目录选择按钮
void CAutoDeleteDlg::OnBnClickedMfcbuttonViewPath()
{
	// TODO: 在此添加控件通知处理程序代码
	wchar_t temp[256];
	GetEnvironmentVariable(_T("userprofile"), temp, 256);
	CFolderPickerDialog dlg(temp, 0, NULL, 0);
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetPathName(); // 获取文件路径                            
		CEdit *p_EditPath = (CEdit*)GetDlgItem(IDC_EDIT_PATH); // 写回textbox
		p_EditPath->SetWindowText(str);
	}
}

//保存按钮
void CAutoDeleteDlg::OnBnClickedMfcbuttonSave()
{
	CAutoDeleteDlg::SaveConfig();
}

//读取按钮
void CAutoDeleteDlg::OnBnClickedMfcbuttonRead()
{
	CAutoDeleteDlg::ReadConfig();
}

//刷新文件夹大小
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

//刷新文件夹数量
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

//最小化到托盘
void CAutoDeleteDlg::ShowTray(bool bShow)
{
	CEdit* EditBox_Path;
	EditBox_Path = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
	CString str_Path;
	EditBox_Path->GetWindowText(str_Path);

	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;    //托盘图标的id
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK;//自定义的回调消息名称id
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));    //设置图标
	wcscpy(nid.szTip, str_Path + _T("\n自动定期删除"));
	Shell_NotifyIcon(bShow ? NIM_ADD : NIM_DELETE, &nid);//在托盘区添加或删除图标
	if (bShow == true)
	{
		ShowWindow(SW_HIDE);
	}
	else if (bShow == false)
	{
		ShowWindow(SW_SHOW);
	}
}

//托盘图标的处理事件
LRESULT CAutoDeleteDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	NOTIFYICONDATA nid;
	//w参数 代表id ， L参数代表消息id
	if (wParam != IDR_MAINFRAME)
		return 1;
	switch (lParam)
	{
	case WM_LBUTTONUP:  //左键单击显示主界面
		ShowWindow(SW_SHOW);
		ShowTray(false);
		//不用销毁图标。
		break;
	case WM_RBUTTONUP:  //右击弹出菜单
		break;
	case WM_LBUTTONDBLCLK:  //左键双击不处理
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

	string str_Count = CW2A(Cstr_Count.GetString());	//CString转String
	char* char_Count;
	string str_MaxCount = CW2A(Cstr_MaxCount.GetString());
	char* char_MaxCount;
	int int_Count = static_cast<int>(strtol(str_Count.c_str(), &char_Count, 16));	//String转char*再转给int
	int int_MaxCount = static_cast<int>(strtol(str_MaxCount.c_str(), &char_MaxCount, 16));
	int DeleteCount = int_Count - int_MaxCount;	//获取需要删除多少文件


}


void CAutoDeleteDlg::DeleteFilesbySize()
{

}