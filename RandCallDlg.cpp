
// RandCallDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "RandCall.h"
#include "RandCallDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRandCallDlg 对话框



CRandCallDlg::CRandCallDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RANDCALL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRandCallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC, m_Pic);
	DDX_Control(pDX, IDC_BUTTONPIC, m_btnPic);
}

BEGIN_MESSAGE_MAP(CRandCallDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONPIC, &CRandCallDlg::OnBnClickedButtonpic)
	//ON_WM_CLOSE()
END_MESSAGE_MAP()


// CRandCallDlg 消息处理程序

BOOL CRandCallDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_OPEN, L"打开图像文件位置");
			pSysMenu->AppendMenu(MF_STRING, IDM_GIT, L"作者的 GitHub 主页");
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// 打开文件
	HANDLE hFile = CreateFile(L"namelist.txt", GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"打开namelist.txt失败", L"错误", MB_ICONERROR);
		OnCancel();
		return TRUE;
	}

	// 读取文件
	DWORD dwRead;
	CStringA szRead;
	if (!ReadFile(hFile, szRead.GetBuffer(32768), 32768, &dwRead, NULL) || !dwRead)
	{
		MessageBox(L"读取namelist.txt失败", L"错误", MB_ICONERROR);
		OnCancel();
		return TRUE;
	}
	CloseHandle(hFile);
	szRead.ReleaseBuffer(dwRead);

	// 分离并存储到数组
	CString wszRead(szRead);
	size_t nPos = 0;
	while ((nPos = wszRead.FindOneOf(L"\r\n")) != -1)
	{
		wszName.Add(wszRead.Left(nPos)); ++cntName;
		if (wszName.GetAt(cntName - 1).IsEmpty())
		{
			wszName.RemoveAt(--cntName);
		}
		wszRead.Delete(0, nPos + 2);
	}
	if (!wszRead.IsEmpty())
	{
		wszName.Add(wszRead);
		++cntName;
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRandCallDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == IDM_OPEN)
	{
		ShellExecute(NULL, L"explore", L"p", NULL, NULL, SW_SHOW);
	}
	else if (nID == IDM_GIT)
	{
		ShellExecute(NULL, L"open", L"https://github.com/Little-Down", NULL, NULL, SW_SHOW);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRandCallDlg::OnPaint()
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

// 当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CRandCallDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRandCallDlg::OnBnClickedButtonpic()
{
	// TODO: 在此添加控件通知处理程序代码
	HANDLE hThread = CreateThread(NULL, 0, ThreadChangePic, this, 0, NULL);
	if (hThread) CloseHandle(hThread);
}


void CRandCallDlg::ChangePic()
{
	m_btnPic.EnableWindow(FALSE); // 禁用按扭

	// 抽取姓名
	srand(time(NULL));
	int nNum = rand() % cntName;
	CString wszCurName = wszName.GetAt(nNum);

	CImage Image;
	bool bText = FALSE;
	// 加载图像
	for (CString wszType : {L".bmp", L".jpg", L".jpeg", L".png", L"NO_IMG"})
	{
		if (wszType == L"NO_IMG")
		{
			bText = TRUE;
			Image.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP2);
		}
		else if (SUCCEEDED(Image.Load(L"p\\" + wszCurName + wszType))) break;
	}
	// 缩放、裁剪图片（方法为：填充）
	if (Image.GetWidth() != 256 || Image.GetHeight() != 256)
	{
		// 等比缩放到 ImageNew
		int nSrcW = Image.GetWidth(), nSrcH = Image.GetHeight();
		double fScale = max(256.0 / nSrcW, 256.0 / nSrcH);
		int nDestW = nSrcW * fScale, nDestH = nSrcH * fScale;
		CImage ImageNew;
		ImageNew.Create(nDestW, nDestH, 24);
		HDC hDCNew = ImageNew.GetDC();
		SetStretchBltMode(hDCNew, HALFTONE);
		Image.StretchBlt(hDCNew, 0, 0, nDestW, nDestH);
		ImageNew.ReleaseDC();
		Image.Destroy();
		// 裁剪中心部分到 Image
		Image.Create(256, 256, 24);
		ImageNew.BitBlt(Image.GetDC(), 0, 0, 256, 256, (nDestW - 256) / 2, (nDestH - 256) / 2);
		Image.ReleaseDC();
	}

	CDC* pDC = GetDC();	// 获取当前窗口DC
	CDC DCMem;			// 内存DC
	DCMem.CreateCompatibleDC(pDC);	// 创建兼容DC
	DCMem.SetBkMode(TRANSPARENT);
	DCMem.SelectObject(Image);		// 选择位图对象

	if (bText) // 显示文字
	{
		int nLen = CStringA(wszCurName).GetLength(); // 姓名长度
		if (nLen < 4) nLen = 4;
		int nWidth = (nLen > 7 ? 190 : 170) / nLen, nHeight = 2.3 * nWidth;	// 计算字体大小
		CFont Font;
		Font.CreateFont(nHeight, nWidth, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"等线");
		DCMem.SelectObject(Font);
		CRect Rect(0, 0, 256, 256);
		DCMem.DrawText(wszCurName, &Rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	// 逐层画图
	int nSgY;
	for (int nPixY = 0; nPixY < 256;)
	{
		nSgY = nPixY > 96 && nPixY < 160 ? 1 : 4;
		if (nPixY + nSgY > 256) nSgY = 256 - nPixY;
		pDC->BitBlt(22, 23 + nPixY, 256, nSgY, &DCMem, 0, nPixY, SRCCOPY); // 画图
		nPixY += nSgY;
		Sleep(5);
	}

	// 释放
	DCMem.DeleteDC();
	pDC->DeleteDC();
	m_Pic.SetBitmap(Image.Detach());

	m_btnPic.EnableWindow(); // 恢复按扭
	SetDlgItemText(IDC_BUTTONPIC, L"再抽一发！");
}
