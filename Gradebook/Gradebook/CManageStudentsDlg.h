#pragma once
#include <afxdialogex.h>
#include "resource.h" 
#include "GradebookDatabase.h"
class CManageStudentsDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CManageStudentsDlg)

public:
    CManageStudentsDlg(CWnd* pParent = nullptr);   
    virtual ~CManageStudentsDlg();
    GradebookDatabase db;
    enum { IDD = IDD_MANAGE_STUDENTS
    };  

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()

private:
    CListCtrl m_studentsListCtrl;
    CDateTimeCtrl m_birthDatePicker;
    CComboBox m_classesComboBox;
    CButton m_btnAddEdit;
    CButton m_btnDelete;
    CButton m_btnClear;

    CString m_fullName;
    CString m_classNumber;

    afx_msg void OnBnClickedAddEdit();
    afx_msg void OnBnClickedClear();
    afx_msg void OnBnClickedDelete();
    afx_msg void OnStudentSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);

    void ClearFields();
public:
    afx_msg void OnEnChangeEdit1();
    void LoadStudents();
    void PopulateFieldsFromSelected();
    void PopulateClassComboBox();
    void PopulateSubjectComboBox();
    void OnCalculateAverageGrade();
    afx_msg void OnBnClickedButton1();
};

