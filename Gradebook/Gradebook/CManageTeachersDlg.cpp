#include "pch.h"
#include "Gradebook.h"
#include "CManageTeachersDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CManageTeachersDlg, CDialogEx)

CManageTeachersDlg::CManageTeachersDlg(CWnd* pParent )
    : CDialogEx(IDD_MANAGE_TEACHERS, pParent)
{
}

void CManageTeachersDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CManageTeachersDlg, CDialogEx)
END_MESSAGE_MAP()
