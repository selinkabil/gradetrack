#include "pch.h"
#include "Gradebook.h"
#include "CManageGradesDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CManageGradesDlg, CDialogEx)

CManageGradesDlg::CManageGradesDlg(CWnd* pParent )
    : CDialogEx(IDD_MANAGE_GRADES, pParent)
{
}

void CManageGradesDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CManageGradesDlg, CDialogEx)
END_MESSAGE_MAP()
