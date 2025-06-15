#include "pch.h"
#include "CManageStudentsDlg.h"
#include "afxdialogex.h"
#include "resource.h" 
#include <vector>
#include "GradebookDatabase.h" 


IMPLEMENT_DYNAMIC(CManageStudentsDlg, CDialogEx)

CManageStudentsDlg::CManageStudentsDlg(CWnd* pParent)
    : CDialogEx(IDD_MANAGE_STUDENTS, pParent) 
{
}

CManageStudentsDlg::~CManageStudentsDlg()
{
}

void CManageStudentsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STUDENTS_LIST, m_studentsListCtrl);
    DDX_Control(pDX, IDC_DATETIMEPICKER1, m_birthDatePicker);
    DDX_Control(pDX, IDC_CLASSES_CB, m_classesComboBox);
    DDX_Control(pDX, IDC_ADD_EDIT_BUTTON, m_btnAddEdit);
    DDX_Control(pDX, IDC_DELETE_BUTTON, m_btnDelete);
    DDX_Control(pDX, IDC_CLEAR_BUTTON, m_btnClear);
    DDX_Text(pDX, IDC_FULL_NAME_EDIT, m_fullName);
    DDX_Text(pDX, IDC_CLASS_NUMBER_EDIT, m_classNumber);
}

BEGIN_MESSAGE_MAP(CManageStudentsDlg, CDialogEx)
    ON_BN_CLICKED(IDC_ADD_EDIT_BUTTON, &CManageStudentsDlg::OnBnClickedAddEdit)
    ON_BN_CLICKED(IDC_CLEAR_BUTTON, &CManageStudentsDlg::OnBnClickedClear)
    ON_BN_CLICKED(IDC_DELETE_BUTTON, &CManageStudentsDlg::OnBnClickedDelete)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_STUDENTS_LIST, &CManageStudentsDlg::OnStudentSelectionChanged)
    ON_EN_CHANGE(IDC_EDIT1, &CManageStudentsDlg::OnEnChangeEdit1)
    ON_BN_CLICKED(IDC_BUTTON1, &CManageStudentsDlg::OnCalculateAverageGrade)
END_MESSAGE_MAP()


BOOL CManageStudentsDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    if (!db.Connect(_T(""), _T(""), true)) 
    {
        CString error = db.GetLastError();
        CString msg;
        msg.Format(_T("Failed to connect to database: %s"), error);
        AfxMessageBox(msg);
        return FALSE;
    }
    m_studentsListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_studentsListCtrl.InsertColumn(0, _T("Number in Class"), LVCFMT_LEFT, 100);
    m_studentsListCtrl.InsertColumn(1, _T("Full Name"), LVCFMT_LEFT, 160);
    m_studentsListCtrl.InsertColumn(2, _T("Date of Birth"), LVCFMT_LEFT, 110);
    m_studentsListCtrl.InsertColumn(3, _T("Class"), LVCFMT_LEFT, 80);

    m_btnDelete.EnableWindow(FALSE);

    PopulateClassComboBox();
    PopulateSubjectComboBox();

    LoadStudents();
    return TRUE;
}

void CManageStudentsDlg::LoadStudents()
{
    m_studentsListCtrl.DeleteAllItems(); 
    vector<StudentGradeInfo> students = db.GetAllStudents();
    int index = 0;
    for (const StudentGradeInfo& student : students)
    {
        CString numberStr;
        numberStr.Format(_T("%d"), student.numberInClass);
        int item = m_studentsListCtrl.InsertItem(index, numberStr); 
        m_studentsListCtrl.SetItemText(item, 1, student.fullName); 
        m_studentsListCtrl.SetItemText(item, 2, student.dateOfBirth); 
        m_studentsListCtrl.SetItemText(item, 3, student.classNumber);
        m_studentsListCtrl.SetItemData(item, student.studentID); 
        index++;
    }
}


void CManageStudentsDlg::OnBnClickedAddEdit()
{
    UpdateData(TRUE);

    CString buttonText;
    m_btnAddEdit.GetWindowText(buttonText);

    int classComboIndex = m_classesComboBox.GetCurSel();
    if (classComboIndex == CB_ERR) {
        AfxMessageBox(_T("Please select a class."));
        return;
    }
    int classID = (int)m_classesComboBox.GetItemData(classComboIndex);

    COleDateTime dob;
    m_birthDatePicker.GetTime(dob);

    if (buttonText == _T("Add Student"))
    {
        int numberInClass = _ttoi(m_classNumber);
        int newStudentID = 0;
        if (db.AddStudent(numberInClass, m_fullName, dob, classID, newStudentID)) {
            LoadStudents();
        } else {
            AfxMessageBox(_T("Failed to add student: ") + db.GetLastError());
        }
    }
    else
    {
        // Edit existing student
        POSITION pos = m_studentsListCtrl.GetFirstSelectedItemPosition();
        if (pos)
        {
            int nItem = m_studentsListCtrl.GetNextSelectedItem(pos);
            int numberInClass = _ttoi(m_studentsListCtrl.GetItemText(nItem, 0));
            CString fullName = m_studentsListCtrl.GetItemText(nItem, 1);
            CString dobStr = m_studentsListCtrl.GetItemText(nItem, 2);
            int studentID = -1;

           
            vector<StudentGradeInfo> students = db.GetAllStudents();
            for (const auto& s : students) {
                if (s.numberInClass == numberInClass && s.fullName == fullName && s.dateOfBirth == dobStr) {
                    studentID = s.studentID;
                    break;
                }
            }
            if (studentID > 0) {
                int newNumberInClass = _ttoi(m_classNumber);
                if (db.UpdateStudent(studentID, newNumberInClass, m_fullName, dob, classID)) {
                    LoadStudents();
                } else {
                    AfxMessageBox(_T("Failed to update student: ") + db.GetLastError());
                }
            }
        }
    }

    OnBnClickedClear();
}

void CManageStudentsDlg::OnBnClickedClear()
{
    m_fullName = _T("");
    m_classNumber = _T("");
    UpdateData(FALSE);

    m_birthDatePicker.SetTime(COleDateTime::GetCurrentTime());
    m_classesComboBox.SetCurSel(0);

    m_studentsListCtrl.SetItemState(-1, 0, LVIS_SELECTED);

    m_btnAddEdit.SetWindowText(_T("Add Student"));
    m_btnDelete.EnableWindow(FALSE);
}

void CManageStudentsDlg::OnBnClickedDelete()
{
    POSITION pos = m_studentsListCtrl.GetFirstSelectedItemPosition();
    if (pos)
    {
        int nItem = m_studentsListCtrl.GetNextSelectedItem(pos);
        int numberInClass = _ttoi(m_studentsListCtrl.GetItemText(nItem, 0));
        CString fullName = m_studentsListCtrl.GetItemText(nItem, 1);
        CString dobStr = m_studentsListCtrl.GetItemText(nItem, 2);
        int studentID = -1;

        vector<StudentGradeInfo> students = db.GetAllStudents();
        for (const auto& s : students) {
            if (s.numberInClass == numberInClass && s.fullName == fullName && s.dateOfBirth == dobStr) {
                studentID = s.studentID;
                break;
            }
        }
        if (studentID > 0) {
            if (db.DeleteStudent(studentID)) {
                m_studentsListCtrl.DeleteItem(nItem);
            } else {
                AfxMessageBox(_T("Failed to delete student: ") + db.GetLastError());
            }
        }
    }
    OnBnClickedClear();
}

void CManageStudentsDlg::OnStudentSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    POSITION pos = m_studentsListCtrl.GetFirstSelectedItemPosition();
    if (pos)
    {
        m_btnAddEdit.SetWindowText(_T("Save Changes"));
        m_btnDelete.EnableWindow(TRUE);
        PopulateFieldsFromSelected();
    }
    else
    {
        m_btnAddEdit.SetWindowText(_T("Add Student"));
        m_btnDelete.EnableWindow(FALSE);
    }

    *pResult = 0;
}

void CManageStudentsDlg::PopulateFieldsFromSelected()
{
    POSITION pos = m_studentsListCtrl.GetFirstSelectedItemPosition();
    if (!pos) return;

    int nItem = m_studentsListCtrl.GetNextSelectedItem(pos);

    m_classNumber = m_studentsListCtrl.GetItemText(nItem, 0);
    m_fullName = m_studentsListCtrl.GetItemText(nItem, 1);

    CString dobStr = m_studentsListCtrl.GetItemText(nItem, 2);
    COleDateTime dob;
    dob.ParseDateTime(dobStr);
    m_birthDatePicker.SetTime(dob);

    CString className = m_studentsListCtrl.GetItemText(nItem, 3);
    int index = m_classesComboBox.FindStringExact(0, className);
    if (index != CB_ERR)
        m_classesComboBox.SetCurSel(index);

    UpdateData(FALSE);
}
void CManageStudentsDlg::OnEnChangeEdit1()
{
   
}

void CManageStudentsDlg::PopulateClassComboBox()
{
    m_classesComboBox.ResetContent();
    vector<Class> classes = db.GetAllClasses();
    for (const auto& c : classes) {
        int idx = m_classesComboBox.AddString(c.classNumber);
        m_classesComboBox.SetItemData(idx, c.classID);
    }
    if (m_classesComboBox.GetCount() > 0)
        m_classesComboBox.SetCurSel(0);
}

void CManageStudentsDlg::PopulateSubjectComboBox()
{
    CComboBox* pSubjectCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
    if (!pSubjectCombo) return;
    pSubjectCombo->ResetContent();
    vector<Subject> subjects = db.GetAllSubjects();
    for (const auto& s : subjects) {
        int idx = pSubjectCombo->AddString(s.subjectName);
        pSubjectCombo->SetItemData(idx, s.subjectID);
    }
    if (pSubjectCombo->GetCount() > 0)
        pSubjectCombo->SetCurSel(0);
}

void CManageStudentsDlg::OnCalculateAverageGrade()
{
    POSITION pos = m_studentsListCtrl.GetFirstSelectedItemPosition();
    if (!pos) {
        AfxMessageBox(_T("Select a student first."));
        return;
    }
    int nItem = m_studentsListCtrl.GetNextSelectedItem(pos);
    int studentID = (int)m_studentsListCtrl.GetItemData(nItem);

    if (studentID <= 0) {
        AfxMessageBox(_T("Could not find student ID."));
        return;
    }

    // Subject average
    CComboBox* pSubjectCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
    int subjectIdx = pSubjectCombo->GetCurSel();
    double subjectAvg = 0.0;
    if (subjectIdx != CB_ERR) {
        int subjectID = (int)pSubjectCombo->GetItemData(subjectIdx);
        subjectAvg = db.GetStudentAverageGrade(studentID, subjectID);
    }
    CString avgStr;

    avgStr.Format(_T("%.2f"), subjectAvg);
    SetDlgItemText(IDC_EDIT1, avgStr);

    double overallAvg = db.GetStudentOverallAverage(studentID);
    avgStr.Format(_T("%.2f"), overallAvg);
    SetDlgItemText(IDC_EDIT2, avgStr);
}
