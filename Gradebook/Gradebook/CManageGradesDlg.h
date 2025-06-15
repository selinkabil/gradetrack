#pragma once
#include <afxdialogex.h>

class CManageGradesDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CManageGradesDlg)

public:
    CManageGradesDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MANAGE_GRADES };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    
    DECLARE_MESSAGE_MAP()
};
