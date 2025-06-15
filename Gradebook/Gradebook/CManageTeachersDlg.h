#pragma once
#include <afxdialogex.h>

class CManageTeachersDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CManageTeachersDlg)

public:
    CManageTeachersDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MANAGE_TEACHERS };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()
};
