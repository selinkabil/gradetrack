#pragma once
#include <afxdialogex.h>

class CManageCurriculumDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CManageCurriculumDlg)

public:
    CManageCurriculumDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MANAGE_CURRICULUM };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()
};
