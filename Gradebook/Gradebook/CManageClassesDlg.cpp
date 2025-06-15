#include "pch.h"
#include "Gradebook.h"
#include "CManageClassesDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CManageClassesDlg, CDialogEx)

CManageClassesDlg::CManageClassesDlg(CWnd* pParent )
    : CDialogEx(IDD_MANAGE_CLASSES, pParent)
{
}

void CManageClassesDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CManageClassesDlg, CDialogEx)
END_MESSAGE_MAP()
