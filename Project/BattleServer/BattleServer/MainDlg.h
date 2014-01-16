//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////



// MainDlg.h : 头文件
//

#pragma once
#include "Resource.h"
#include "IOCPModel.h"
#include <sstream>

// CMainDlg 对话框
class CMainDlg : public CDialog
{
	// 构造
public:
	CMainDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_PIGGYIOCPSERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	// 开始监听
	afx_msg void OnBnClickedOk();
	// 停止监听
	afx_msg void OnBnClickedStop();
	// "退出"按钮
	afx_msg void OnBnClickedCancel();
	///////////////////////////////////////////////////////////////////////
	//	系统退出的时候，为确保资源释放，停止监听，清空Socket类库
	afx_msg void OnDestroy();
	//afx_msg LRESULT OnNewMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:

	// 初始化Socket库以及界面信息
	void Init();

	// 初始化List控件
	void InitListCtrl();

	inline void ChageConnectInfo()
	{
		SetDlgItemInt( IDC_CONNECTS, m_Connects );
	}

public:

	// 当前客户端有新消息到来的时候，在主界面中显示新到来的信息(在类CIOCPModel中调用)
	// 为了减少界面代码对效率的影响，此处使用了内联
	inline void AddInformation(const CString strInfo)
	{
		CString str = CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S ");
		str += strInfo;
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);
		pList->InsertItem(0,str);
	}
	inline void Connects()
	{
		m_Connects++;
		this->ChageConnectInfo();
	}
	inline void Disconnect(int num)
	{
		m_Connects-=num;
		this->ChageConnectInfo();
	}

private:

	CIOCPModel m_IOCP;                         // 主要对象，完成端口模型

	int			m_Connects;

};
