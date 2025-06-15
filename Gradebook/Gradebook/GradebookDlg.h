// GradebookDlg.h : header file
//

#pragma once


// CGradebookDlg dialog
class CGradebookDlg : public CDialogEx
{
// Construction
public:
	CGradebookDlg(CWnd* pParent = nullptr);
	afx_msg void OnCbnSelchangeSubjectComboBox();
	afx_msg void OnManageStudents();
	afx_msg void OnSubjectsManageSubjects();
	afx_msg void OnGradesManageGrades();
	afx_msg void OnClassesManageClasses();
	afx_msg void OnCurriculumsManageCurriculums();
	afx_msg void OnTeachersManageTeachers();
	afx_msg void OnHelpAbout();
	afx_msg void OnFileExit();


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRADEBOOK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	


// Implementation
protected:
	HICON m_hIcon;

	//controls
	CFont m_customFont;
	CListCtrl m_topStudentsListCtrl;
	CListCtrl m_remedialExamListCtrl;
	CListCtrl m_failingStudentsListCtrl;
	CListCtrl m_studentsBirthdayListCtrl;

	CComboBox m_SubjectComboBox;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedRemedialtesttext2();
};
