#pragma once
#include <afxdialogex.h>

class CManageClassesDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CManageClassesDlg)

public:
    CManageClassesDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MANAGE_CLASSES };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()
};
