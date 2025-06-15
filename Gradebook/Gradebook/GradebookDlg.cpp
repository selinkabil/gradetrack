// GradebookDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Gradebook.h"
#include "GradebookDlg.h"
#include "afxdialogex.h"
#include <afxdb.h> 
#include "GradebookDatabase.h"
#include "CManageStudentsDlg.h"
#include "CManageSubjectsDlg.h"
#include "CManageGradesDlg.h"
#include "CManageClassesDlg.h"
#include "CManageCurriculumDlg.h"
#include "CManageTeachersDlg.h"
#include <iostream>
#include <vector>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
GradebookDatabase db;


//function identifiers
void SetMainMenu(HWND hWnd);



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGradebookDlg dialog



CGradebookDlg::CGradebookDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GRADEBOOK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGradebookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);  

	DDX_Control(pDX, IDC_TopStudentsListCtrl, m_topStudentsListCtrl);
	DDX_Control(pDX, IDC_SubjectComboBox, m_SubjectComboBox);
	DDX_Control(pDX, IDC_RemedialExamStudents, m_remedialExamListCtrl);
	DDX_Control(pDX, IDC_FailingStudents, m_failingStudentsListCtrl);
	DDX_Control(pDX, IDC_StudentsBirthday, m_studentsBirthdayListCtrl);

}


BEGIN_MESSAGE_MAP(CGradebookDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_SubjectComboBox, &CGradebookDlg::OnCbnSelchangeSubjectComboBox)
	ON_COMMAND(ID_STUDENTS_VIEWSTUDENTS, &CGradebookDlg::OnManageStudents)
	ON_COMMAND(ID_SUBJECTS_MANAGESUBJECTS, &CGradebookDlg::OnSubjectsManageSubjects)
	ON_COMMAND(ID_GRADES_MANAGEGRADES, &CGradebookDlg::OnGradesManageGrades)
	ON_COMMAND(ID_CLASSES_MANAGECLASSES, &CGradebookDlg::OnClassesManageClasses)
	ON_COMMAND(ID_CURRICULUMS_MANAGECURRICILUMS, &CGradebookDlg::OnCurriculumsManageCurriculums)
	ON_COMMAND(ID_TEACHERS_MANAGETEACHERS, &CGradebookDlg::OnTeachersManageTeachers)
	ON_COMMAND(ID_HELP_ABOUT, &CGradebookDlg::OnHelpAbout)
	ON_COMMAND(ID_FILE_EXIT, &CGradebookDlg::OnFileExit)
END_MESSAGE_MAP()


// CGradebookDlg message handlers

BOOL CGradebookDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Initialize the combo box

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	SetMainMenu(this->m_hWnd);

	//connect to database
	db.Connect(_T("DESKTOP-EEV7FL8"));


	//create font for static text

	m_customFont.CreateFont(
		22,                         
		0,                          
		0,                         
		0,                          
		FW_BOLD,                   
		FALSE,                      
		FALSE,                      
		0,                          
		ANSI_CHARSET,               
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial"));

	GetDlgItem(IDC_TopStudentsText)->SetFont(&m_customFont);
	GetDlgItem(IDC_RemedialTestText)->SetFont(&m_customFont);
	GetDlgItem(IDC_StudentsBirthdaytext)->SetFont(&m_customFont);
	GetDlgItem(IDC_FailingStudentsText)->SetFont(&m_customFont);
	GetDlgItem(IDC_REPORTSTEXT)->SetFont(&m_customFont);
	//Populates list control with top students 

	m_topStudentsListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_topStudentsListCtrl.InsertColumn(0, _T("Class Number"), LVCFMT_LEFT, 80);  
	m_topStudentsListCtrl.InsertColumn(1, _T("Number in Class"), LVCFMT_LEFT, 120);
	m_topStudentsListCtrl.InsertColumn(2, _T("Full Name"), LVCFMT_LEFT, 150);
	m_topStudentsListCtrl.InsertColumn(3, _T("Date of Birth"), LVCFMT_LEFT, 100);
	m_topStudentsListCtrl.InsertColumn(4, _T("Average Grade"), LVCFMT_LEFT, 100);
	m_topStudentsListCtrl.DeleteAllItems();
	vector<StudentGradeInfo> topStudents = db.GetTopStudents();

	int index = 0;
	for (const auto& student : topStudents)
	{
		m_topStudentsListCtrl.InsertItem(index, student.classNumber); 

		CString temp;
		temp.Format(_T("%d"), student.numberInClass);
		m_topStudentsListCtrl.SetItemText(index, 1, temp);

		m_topStudentsListCtrl.SetItemText(index, 2, student.fullName);
		m_topStudentsListCtrl.SetItemText(index, 3, student.dateOfBirth);

		temp.Format(_T("%.2f"), student.gradeValue);
		m_topStudentsListCtrl.SetItemText(index, 4, temp);

		index++;
	}
	// Populate Subject ComboBox
	m_SubjectComboBox.ResetContent();

	vector<Subject> subjects = db.GetAllSubjects();
	for (const auto& subject : subjects)
	{
		int index = m_SubjectComboBox.AddString(subject.subjectName);
		m_SubjectComboBox.SetItemData(index, subject.subjectID);
	}

	if (m_SubjectComboBox.GetCount() > 0)
		m_SubjectComboBox.SetCurSel(0);

	
	m_remedialExamListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_remedialExamListCtrl.InsertColumn(0, _T("Number in Class"), LVCFMT_LEFT, 120);
	m_remedialExamListCtrl.InsertColumn(1, _T("Full Name"), LVCFMT_LEFT, 120);
	m_remedialExamListCtrl.InsertColumn(2, _T("Date of Birth"), LVCFMT_LEFT, 100);
	m_remedialExamListCtrl.InsertColumn(3, _T("Class"), LVCFMT_LEFT, 60);
	m_remedialExamListCtrl.InsertColumn(4, _T("Subject"), LVCFMT_LEFT, 100);
	m_remedialExamListCtrl.InsertColumn(5, _T("Grade"), LVCFMT_LEFT, 60);

	OnCbnSelchangeSubjectComboBox();

	

	//populate list control with failing students
	m_failingStudentsListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_failingStudentsListCtrl.InsertColumn(0, _T("Class"), LVCFMT_LEFT, 60);
	m_failingStudentsListCtrl.InsertColumn(1, _T("Number in Class"), LVCFMT_LEFT, 120);
	m_failingStudentsListCtrl.InsertColumn(2, _T("Full Name"), LVCFMT_LEFT, 150);
	m_failingStudentsListCtrl.InsertColumn(3, _T("Date of Birth"), LVCFMT_LEFT, 100);

	m_failingStudentsListCtrl.DeleteAllItems();

	vector<StudentGradeInfo> failingStudents = db.GetStudentsWithMultipleFailingSubjects();

	int idx = 0;
	for (const auto& student : failingStudents)
	{
		m_failingStudentsListCtrl.InsertItem(idx, student.classNumber); 

		CString temp;

		temp.Format(_T("%d"), student.numberInClass);
		m_failingStudentsListCtrl.SetItemText(idx, 1, temp);

	
		m_failingStudentsListCtrl.SetItemText(idx, 2, student.fullName);

	
		m_failingStudentsListCtrl.SetItemText(idx, 3, student.dateOfBirth);

		idx++;
	}

	m_failingStudentsListCtrl.Invalidate();
	m_failingStudentsListCtrl.UpdateWindow();

	//populate list control with students with birthdays today
	m_studentsBirthdayListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_studentsBirthdayListCtrl.InsertColumn(0, _T("Number in Class"), LVCFMT_LEFT, 120);
	m_studentsBirthdayListCtrl.InsertColumn(1, _T("Full Name"), LVCFMT_LEFT, 150);
	m_studentsBirthdayListCtrl.InsertColumn(2, _T("Date of Birth"), LVCFMT_LEFT, 100);
	m_studentsBirthdayListCtrl.InsertColumn(3, _T("Class Number"), LVCFMT_LEFT, 80);  

	m_studentsBirthdayListCtrl.DeleteAllItems();

	vector<StudentGradeInfo> students = db.GetStudentsWithBirthdayToday();

	idx = 0;
	for (const auto& student : students)
	{
		CString temp;

		temp.Format(_T("%d"), student.numberInClass);
		m_studentsBirthdayListCtrl.InsertItem(idx, temp);

		m_studentsBirthdayListCtrl.SetItemText(idx, 1, student.fullName);
		m_studentsBirthdayListCtrl.SetItemText(idx, 2, student.dateOfBirth);

		m_studentsBirthdayListCtrl.SetItemText(idx, 3, student.classNumber); 

		idx++;
	}

	m_studentsBirthdayListCtrl.Invalidate();
	m_studentsBirthdayListCtrl.UpdateWindow();
	

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGradebookDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGradebookDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGradebookDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void SetMainMenu(HWND hWnd) {
	CMenu mainMenu;
	mainMenu.LoadMenu(IDR_MENU1);
	SetMenu(hWnd, mainMenu);
}
void CGradebookDlg::OnCbnSelchangeSubjectComboBox()
{
	m_remedialExamListCtrl.DeleteAllItems();

	int selIndex = m_SubjectComboBox.GetCurSel();
	if (selIndex == CB_ERR)
		return;

	if (!db.IsConnected()) {
		db.Connect(_T("DESKTOP-EEV7FL8"));
	}

	int selectedSubjectID = (int)m_SubjectComboBox.GetItemData(selIndex);

	vector<StudentGradeInfo> remedialStudents = db.GetStudentsNeedingRemedialExam(selectedSubjectID);

	int idx = 0;
	for (const auto& student : remedialStudents)
	{
		CString temp;
		temp.Format(_T("%d"), student.numberInClass);
		m_remedialExamListCtrl.InsertItem(idx, temp);

		m_remedialExamListCtrl.SetItemText(idx, 1, student.fullName);
		m_remedialExamListCtrl.SetItemText(idx, 2, student.dateOfBirth);

		m_remedialExamListCtrl.SetItemText(idx, 3, student.classNumber); 

		m_remedialExamListCtrl.SetItemText(idx, 4, student.subjectName);

		temp.Format(_T("%.2f"), student.gradeValue);
		m_remedialExamListCtrl.SetItemText(idx, 5, temp);

		idx++;
	}

	// Force refresh of the list control
	m_remedialExamListCtrl.Invalidate();
	m_remedialExamListCtrl.UpdateWindow();
	m_remedialExamListCtrl.RedrawWindow();
}
void CGradebookDlg::OnStnClickedRemedialtesttext2()
{
	// TODO: Add your control notification handler code here
}
void CGradebookDlg::OnManageStudents()
{
	CManageStudentsDlg dlg;  
	dlg.DoModal();          
}
void CGradebookDlg::OnSubjectsManageSubjects()
{
	CManageSubjectsDlg dlg;
	dlg.DoModal();
}
void CGradebookDlg::OnGradesManageGrades()
{
	CManageGradesDlg dlg;
	dlg.DoModal();
}
void CGradebookDlg::OnClassesManageClasses()
{
	CManageClassesDlg dlg;
	dlg.DoModal();
}
void CGradebookDlg::OnCurriculumsManageCurriculums()
{
	CManageCurriculumDlg dlg;
	dlg.DoModal();
}
void CGradebookDlg::OnTeachersManageTeachers()
{
	CManageTeachersDlg dlg;
	dlg.DoModal();
}
void CGradebookDlg::OnHelpAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
void CGradebookDlg::OnFileExit()
{
	OnOK();  
}