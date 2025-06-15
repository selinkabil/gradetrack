#include "pch.h"
#include "GradebookDatabase.h"

bool GradebookDatabase::Connect()
{
    try
    {
        CString connectionStr = _T("Driver={SQL Server};Server=DESKTOP-EEV7FL8;Database=GradebookDB;Trusted_Connection=Yes;");
        db.OpenEx(connectionStr, CDatabase::noOdbcDialog);
        return true;
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
        return false;
    }
}

vector<Subject> GradebookDatabase::FetchSubjects()
{
    vector<Subject> subjects;
    CRecordset rs(&db);
    CString query = _T("SELECT subjectID, subjectName, teacherID, roomNumber FROM Subject");

    try
    {
        rs.Open(CRecordset::forwardOnly, query);
        while (!rs.IsEOF())
        {
            Subject s;
            rs.GetFieldValue((short)0, s.subjectID);
            rs.GetFieldValue((short)1, s.subjectName);
            rs.GetFieldValue((short)2, s.teacherID);
            rs.GetFieldValue((short)3, s.roomNumber);
            subjects.push_back(s);
            rs.MoveNext();
        }
        rs.Close();
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
    }
    return subjects;
}

vector<Student> GradebookDatabase::FetchStudents()
{
    vector<Student> students;
    CRecordset rs(&db);
    CString query = _T("SELECT studentID, numberInClass, fullName, CONVERT(nvarchar, dateOfBirth, 23), classID FROM Student");

    try
    {
        rs.Open(CRecordset::forwardOnly, query);
        while (!rs.IsEOF())
        {
            Student st;
            rs.GetFieldValue((short)0, st.studentID);
            rs.GetFieldValue((short)1, st.numberInClass);
            rs.GetFieldValue((short)2, st.fullName);
            rs.GetFieldValue((short)3, st.dateOfBirth); // date as string yyyy-mm-dd
            rs.GetFieldValue((short)4, st.classID);
            students.push_back(st);
            rs.MoveNext();
        }
        rs.Close();
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
    }
    return students;
}

vector<Teacher> GradebookDatabase::FetchTeachers()
{
    vector<Teacher> teachers;
    CRecordset rs(&db);
    CString query = _T("SELECT teacherID, fullName FROM Teacher");

    try
    {
        rs.Open(CRecordset::forwardOnly, query);
        while (!rs.IsEOF())
        {
            Teacher t;
            rs.GetFieldValue((short)0, t.teacherID);
            rs.GetFieldValue((short)1, t.fullName);
            teachers.push_back(t);
            rs.MoveNext();
        }
        rs.Close();
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
    }
    return teachers;
}

vector<Class> GradebookDatabase::FetchClasses()
{
    vector<Class> classes;
    CRecordset rs(&db);
    CString query = _T("SELECT classID, classNumber, classTeacherID FROM Class");

    try
    {
        rs.Open(CRecordset::forwardOnly, query);
        while (!rs.IsEOF())
        {
            Class c;
            rs.GetFieldValue((short)0, c.classID);
            rs.GetFieldValue((short)1, c.classNumber);
            rs.GetFieldValue((short)2, c.classTeacherID);
            classes.push_back(c);
            rs.MoveNext();
        }
        rs.Close();
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
    }
    return classes;
}

vector<Grade> GradebookDatabase::FetchGrades()
{
    vector<Grade> grades;
    CRecordset rs(&db);
    CString query = _T("SELECT gradeID, studentID, subjectID, CONVERT(nvarchar, gradeDate, 23), gradeValue FROM Grade");

    try
    {
        rs.Open(CRecordset::forwardOnly, query);
        while (!rs.IsEOF())
        {
            Grade g;
            rs.GetFieldValue((short)0, g.gradeID);
            rs.GetFieldValue((short)1, g.studentID);
            rs.GetFieldValue((short)2, g.subjectID);
            rs.GetFieldValue((short)3, g.gradeDate);
            rs.GetFieldValue((short)4, g.gradeValue);
            grades.push_back(g);
            rs.MoveNext();
        }
        rs.Close();
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
    }
    return grades;
}

vector<Curriculum> GradebookDatabase::FetchCurriculums()
{
    vector<Curriculum> curriculums;
    CRecordset rs(&db);
    CString query = _T("SELECT curriculumID, curriculumName, classID, subjectID FROM Curriculum");

    try
    {
        rs.Open(CRecordset::forwardOnly, query);
        while (!rs.IsEOF())
        {
            Curriculum cur;
            rs.GetFieldValue((short)0, cur.curriculumID);
            rs.GetFieldValue((short)1, cur.curriculumName);
            rs.GetFieldValue((short)2, cur.classID);
            rs.GetFieldValue((short)3, cur.subjectID);
            curriculums.push_back(cur);
            rs.MoveNext();
        }
        rs.Close();
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
    }
    return curriculums;
}
