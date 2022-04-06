
// RandCallDlg.h: 头文件
//

#pragma once


// CRandCallDlg 对话框
class CRandCallDlg : public CDialogEx
{
	// 构造
public:
	CRandCallDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RANDCALL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 姓名列表
	CStringArray wszName;
	// 人数
	size_t cntName = 0;

	// 图片控件
	CStatic m_Pic;
	void ChangePic();
	// 按钮控件
	CButton m_btnPic;
	// 按钮点击事件
	afx_msg void OnBnClickedButtonpic();

	static DWORD WINAPI ThreadChangePic(LPVOID lParam)
	{
		CRandCallDlg* pThis = (CRandCallDlg*)lParam;
		pThis->ChangePic();
		return 0;
	}
};
