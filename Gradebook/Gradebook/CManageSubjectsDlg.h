#pragma once
#include <afxdialogex.h>

class CManageSubjectsDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CManageSubjectsDlg)

public:
    CManageSubjectsDlg(CWnd* pParent = nullptr); 

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MANAGE_SUBJECTS };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    

    DECLARE_MESSAGE_MAP()
};
