#include "pch.h"
#include "Gradebook.h"
#include "CManageSubjectsDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CManageSubjectsDlg, CDialogEx)

CManageSubjectsDlg::CManageSubjectsDlg(CWnd* pParent)
    : CDialogEx(IDD_MANAGE_SUBJECTS, pParent)
{
}

void CManageSubjectsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CManageSubjectsDlg, CDialogEx)
END_MESSAGE_MAP()
