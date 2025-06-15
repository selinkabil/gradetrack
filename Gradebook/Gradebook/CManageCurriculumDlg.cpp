#include "pch.h"
#include "Gradebook.h"
#include "CManageCurriculumDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CManageCurriculumDlg, CDialogEx)

CManageCurriculumDlg::CManageCurriculumDlg(CWnd* pParent )
    : CDialogEx(IDD_MANAGE_CURRICILUM, pParent)
{
}

void CManageCurriculumDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CManageCurriculumDlg, CDialogEx)
END_MESSAGE_MAP()
