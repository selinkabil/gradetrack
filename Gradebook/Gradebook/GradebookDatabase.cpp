#include "pch.h"
#include "GradebookDatabase.h"

GradebookDatabase::GradebookDatabase()
    : m_pDatabase(nullptr)
    , m_isConnected(false)
{
}

GradebookDatabase::~GradebookDatabase()
{
    Disconnect();
}

bool GradebookDatabase::Connect(const CString& serverName, const CString& databaseName,
    bool useWindowsAuth, const CString& username, const CString& password)
{
    try
    {
        Disconnect(); // Ensure we're disconnected first

        m_pDatabase = std::make_unique<CDatabase>();

        if (useWindowsAuth)
        {
            m_connectionString.Format(_T("DRIVER={ODBC Driver 17 for SQL Server};SERVER=%s;DATABASE=%s;Trusted_Connection=yes;"),
                serverName, databaseName);
        }
        else
        {
            m_connectionString.Format(_T("DRIVER={ODBC Driver 17 for SQL Server};SERVER=%s;DATABASE=%s;UID=%s;PWD=%s;"),
                serverName, databaseName, username, password);
        }

        m_pDatabase->Open(NULL, FALSE, FALSE, m_connectionString);
        m_isConnected = true;
        m_lastError.Empty();
        return true;
    }
    catch (CDBException* e)
    {
        SetLastError(e->m_strError);
        e->Delete();
        m_isConnected = false;
        return false;
    }
    catch (...)
    {
        SetLastError(_T("Unknown error occurred during connection"));
        m_isConnected = false;
        return false;
    }
}

void GradebookDatabase::Disconnect()
{
    if (m_pDatabase && m_pDatabase->IsOpen())
    {
        m_pDatabase->Close();
    }
    m_pDatabase.reset();
    m_isConnected = false;
}

void GradebookDatabase::SetLastError(const CString& error)
{
    m_lastError = error;
}

bool GradebookDatabase::ExecuteSQL(const CString& sql)
{
    if (!m_isConnected || !m_pDatabase)
    {
        SetLastError(_T("Database not connected"));
        return false;
    }

    try
    {
        m_pDatabase->ExecuteSQL(sql);
        m_lastError.Empty();
        return true;
    }
    catch (CDBException* e)
    {
        SetLastError(e->m_strError);
        e->Delete();
        return false;
    }
}

CRecordset* GradebookDatabase::ExecuteQuery(const CString& sql)
{
    if (!m_isConnected || !m_pDatabase)
    {
        SetLastError(_T("Database not connected"));
        return nullptr;
    }

    try
    {
        CRecordset* pRecordset = new CRecordset(m_pDatabase.get());
        pRecordset->Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
        m_lastError.Empty();
        return pRecordset;
    }
    catch (CDBException* e)
    {
        SetLastError(e->m_strError);
        e->Delete();
        return nullptr;
    }
}

// Teacher operations
bool GradebookDatabase::AddTeacher(const CString& fullName, int& newTeacherID)
{
    CString sql;
    sql.Format(_T("INSERT INTO Teacher (fullName) VALUES ('%s'); SELECT SCOPE_IDENTITY();"), fullName);

    CRecordset* pRecordset = ExecuteQuery(sql);
    if (pRecordset)
    {
        if (!pRecordset->IsEOF())
        {
            CString value;
            pRecordset->GetFieldValue((short)0, value);
            newTeacherID = _ttoi(value);
        }
        pRecordset->Close();
        delete pRecordset;
        return true;
    }
    return false;
}

bool GradebookDatabase::UpdateTeacher(int teacherID, const CString& fullName)
{
    CString sql;
    sql.Format(_T("UPDATE Teacher SET fullName = '%s' WHERE teacherID = %d"), fullName, teacherID);
    return ExecuteSQL(sql);
}

bool GradebookDatabase::DeleteTeacher(int teacherID)
{
    CString sql;
    sql.Format(_T("DELETE FROM Teacher WHERE teacherID = %d"), teacherID);
    return ExecuteSQL(sql);
}

std::vector<Teacher> GradebookDatabase::GetAllTeachers()
{
    std::vector<Teacher> teachers;
    CString sql = _T("SELECT teacherID, fullName FROM Teacher ORDER BY fullName");

    CRecordset* pRecordset = ExecuteQuery(sql);
    if (pRecordset)
    {
        while (!pRecordset->IsEOF())
        {
            Teacher teacher;
            CString value;

            pRecordset->GetFieldValue((short)0, value);
            teacher.teacherID = _ttoi(value);

            pRecordset->GetFieldValue((short)1, teacher.fullName);

            teachers.push_back(teacher);
            pRecordset->MoveNext();
        }

        pRecordset->Close();
        delete pRecordset;
    }

    return teachers;
}

Teacher GradebookDatabase::GetTeacher(int teacherID)
{
    Teacher teacher = {};
    CString sql;
    sql.Format(_T("SELECT teacherID, fullName FROM Teacher WHERE teacherID = %d"), teacherID);

    CRecordset* pRecordset = ExecuteQuery(sql);
    if (pRecordset && !pRecordset->IsEOF())
    {
        CString value;
        pRecordset->GetFieldValue((short)0, value);
        teacher.teacherID = _ttoi(value);

        pRecordset->GetFieldValue((short)1, teacher.fullName);

        pRecordset->Close();
        delete pRecordset;
    }

    return teacher;
}

// Student operations
bool GradebookDatabase::AddStudent(int numberInClass, const CString& fullName,
    const COleDateTime& dateOfBirth, int classID, int& newStudentID)
{
    CString sql;
    CString dateStr = dateOfBirth.Format(_T("%Y-%m-%d"));
    sql.Format(_T("INSERT INTO Student (numberInClass, fullName, dateOfBirth, classID) VALUES (%d, '%s', '%s', %d); SELECT SCOPE_IDENTITY();"),
        numberInClass, fullName, dateStr, classID);

    CRecordset* pRecordset = ExecuteQuery(sql);
    if (pRecordset)
    {
        if (!pRecordset->IsEOF())
        {
            CString value;
            pRecordset->GetFieldValue((short)0, value);
            newStudentID = _ttoi(value);
        }
        pRecordset->Close();
        delete pRecordset;
        return true;
    }
    return false;
}

std::vector<Student> GradebookDatabase::GetStudentsByClass(int classID)
{
    std::vector<Student> students;
    CString sql;
    sql.Format(_T("SELECT studentID, numberInClass, fullName, dateOfBirth, classID FROM Student WHERE classID = %d ORDER BY numberInClass"), classID);

    CRecordset* pRecordset = ExecuteQuery(sql);
    if (pRecordset)
    {
        while (!pRecordset->IsEOF())
        {
            Student student;
            CString value;

            pRecordset->GetFieldValue((short)0, value);
            student.studentID = _ttoi(value);

            pRecordset->GetFieldValue(1, value);
            student.numberInClass = _ttoi(value);

            pRecordset->GetFieldValue(2, student.fullName);

            pRecordset->GetFieldValue(3, value);
            student.dateOfBirth=value;

            pRecordset->GetFieldValue(4, value);
            student.classID = _ttoi(value);

            students.push_back(student);
            pRecordset->MoveNext();
        }

        pRecordset->Close();
        delete pRecordset;
    }

    return students;
}// Grade operations
bool GradebookDatabase::AddGrade(int studentID, int subjectID, const COleDateTime& gradeDate,
    double gradeValue, int& newGradeID)
{
    CString sql;
    CString dateStr = gradeDate.Format(_T("%Y-%m-%d"));
    sql.Format(_T("INSERT INTO Grade (studentID, subjectID, gradeDate, gradeValue) VALUES (%d, %d, '%s', %.2f); SELECT SCOPE_IDENTITY();"),
        studentID, subjectID, dateStr, gradeValue);

    CRecordset* pRecordset = ExecuteQuery(sql);
    if (pRecordset)
    {
        if (!pRecordset->IsEOF())
        {
            CString value;
            pRecordset->GetFieldValue((short)0, value);
            newGradeID = _ttoi(value);
        }
        pRecordset->Close();
        delete pRecordset;
        return true;
    }
    return false;
}

std::vector<Grade> GradebookDatabase::GetGradesByStudent(int studentID)
{
    std::vector<Grade> grades;
    CString sql;
    sql.Format(_T("SELECT gradeID, studentID, subjectID, gradeDate, gradeValue FROM Grade WHERE studentID = %d ORDER BY gradeDate DESC"), studentID);

    CRecordset* pRecordset = ExecuteQuery(sql);
    if (pRecordset)
    {
        while (!pRecordset->IsEOF())
        {
            Grade grade;
            CString value;

            pRecordset->GetFieldValue((short)0, value);
            grade.gradeID = _ttoi(value);

            pRecordset->GetFieldValue(1, value);
            grade.studentID = _ttoi(value);

            pRecordset->GetFieldValue(2, value);
            grade.subjectID = _ttoi(value);

            pRecordset->GetFieldValue(3, value);
            grade.gradeDate=(value);

            pRecordset->GetFieldValue(4, value);
            grade.gradeValue = _ttof(value);

            grades.push_back(grade);
            pRecordset->MoveNext();
        }

        pRecordset->Close();
        delete pRecordset;
    }

    return grades;
}