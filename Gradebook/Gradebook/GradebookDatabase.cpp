#include "pch.h"
#include "GradebookDatabase.h"
using namespace std;

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
        Disconnect(); 

        m_pDatabase =  make_unique<CDatabase>();

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

 vector<Teacher> GradebookDatabase::GetAllTeachers()
{
     vector<Teacher> teachers;
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
//Subject operations

vector<Subject> GradebookDatabase::GetAllSubjects()
{
    vector<Subject> subjects;
    CString sql = _T("SELECT subjectID, subjectName, teacherID, roomNumber FROM Subject ORDER BY subjectName");

    CRecordset* pRecordset = ExecuteQuery(sql);
    if (pRecordset)
    {
        while (!pRecordset->IsEOF())
        {
            Subject subject;
            CString value;

            pRecordset->GetFieldValue((short)0, value);
            subject.subjectID = _ttoi(value);

            pRecordset->GetFieldValue(1, subject.subjectName);

            pRecordset->GetFieldValue(2, value);
            subject.teacherID = _ttoi(value);

            pRecordset->GetFieldValue(3, subject.roomNumber);

            subjects.push_back(subject);
            pRecordset->MoveNext();
        }

        pRecordset->Close();
        delete pRecordset;
    }

    return subjects;
}


// Student operations
bool GradebookDatabase::AddStudent(int numberInClass, const CString& fullName,
    const COleDateTime& dateOfBirth, int classID, int& newStudentID)
{
    CString sql;
    CString dateStr = dateOfBirth.Format(_T("%Y-%m-%d"));
    CString safeFullName = fullName;
    safeFullName.Replace(_T("'"), _T("''")); // Escape single quotes

    // First, execute the INSERT statement
    sql.Format(_T("INSERT INTO Student (numberInClass, fullName, dateOfBirth, classID) VALUES (%d, '%s', '%s', %d)"),
        numberInClass, (LPCTSTR)safeFullName, (LPCTSTR)dateStr, classID);

    if (!ExecuteSQL(sql))
    {
        return false;
    }

    // Then, get the last inserted ID
    CString idSql = _T("SELECT SCOPE_IDENTITY()");
    CRecordset* pRecordset = ExecuteQuery(idSql);
    if (pRecordset && !pRecordset->IsEOF())
    {
        CString value;
        pRecordset->GetFieldValue((short)0, value);
        newStudentID = _ttoi(value);
        pRecordset->Close();
        delete pRecordset;
        return true;
    }

    if (pRecordset)
    {
        pRecordset->Close();
        delete pRecordset;
    }
    return false;
}

bool GradebookDatabase::UpdateStudent(int studentID, int numberInClass, const CString& fullName, const COleDateTime& dateOfBirth, int classID)
{
    CString sql;
    CString dateStr = dateOfBirth.Format(_T("%Y-%m-%d"));
    sql.Format(_T("UPDATE Student SET numberInClass = %d, fullName = '%s', dateOfBirth = '%s', classID = %d WHERE studentID = %d"),
        numberInClass,
        (LPCTSTR)fullName,
        (LPCTSTR)dateStr,
        classID,
        studentID);

    return ExecuteSQL(sql);
}


bool GradebookDatabase::DeleteStudent(int studentID)
{
    CString sql;
    sql.Format(_T("DELETE FROM Student WHERE studentID = %d"), studentID);
    return ExecuteSQL(sql);
}

// Grade operations
bool GradebookDatabase::AddGrade(int studentID, int subjectID, const COleDateTime& gradeDate,
       double gradeValue, int& newGradeID)
   {
       CString dateStr = gradeDate.Format(_T("%Y-%m-%d"));
       CString insertSql;
       insertSql.Format(_T("INSERT INTO Grade (studentID, subjectID, gradeDate, gradeValue) VALUES (%d, %d, '%s', %.2f)"),
           studentID, subjectID, dateStr, gradeValue);

       if (!ExecuteSQL(insertSql))
           return false;

       CString idSql = _T("SELECT SCOPE_IDENTITY()");
       CRecordset* pRecordset = ExecuteQuery(idSql);
       if (pRecordset && !pRecordset->IsEOF())
       {
           CString value;
           pRecordset->GetFieldValue((short)0, value);
           newGradeID = _ttoi(value);
           pRecordset->Close();
           delete pRecordset;
           return true;
       }
       if (pRecordset) delete pRecordset;
       return false;
   }

 vector<Grade> GradebookDatabase::GetGradesByStudent(int studentID)
{
     vector<Grade> grades;
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

// Additional analytical functions

 vector<StudentGradeInfo> GradebookDatabase::GetAllStudents()
 {
     vector<StudentGradeInfo> students;

     if (!m_isConnected || !m_pDatabase)
     {
         return students;
     }

     try
     {
         CString sql = _T("SELECT s.studentID, s.numberInClass, s.fullName, s.dateOfBirth, s.classID, c.classNumber ")
             _T("FROM Student s ")
             _T("INNER JOIN Class c ON s.classID = c.classID ")
             _T("ORDER BY s.fullName");

         CRecordset recordset(m_pDatabase.get());

         if (recordset.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly))
         {
             while (!recordset.IsEOF())
             {
                 StudentGradeInfo student;
                 CString value;

                 try
                 {
                     recordset.GetFieldValue(_T("studentID"), value);
                     student.studentID = _ttoi(value);

                     recordset.GetFieldValue(_T("numberInClass"), value);
                     student.numberInClass = _ttoi(value);

                     recordset.GetFieldValue(_T("fullName"), student.fullName);
                     recordset.GetFieldValue(_T("dateOfBirth"), student.dateOfBirth);

                     recordset.GetFieldValue(_T("classID"), value);
                     student.classID = _ttoi(value);

                     recordset.GetFieldValue(_T("classNumber"), student.classNumber);

                     TRACE(_T("Retrieved: %s -> Class: '%s'\n"), student.fullName, student.classNumber);

                     if (student.studentID > 0)
                     {
                         students.push_back(student);
                     }
                 }
                 catch (CDBException* e)
                 {
                     TRACE(_T("Field access error: %s\n"), e->m_strError);
                     e->Delete();
                 }

                 recordset.MoveNext();
             }
             recordset.Close();
         }
     }
     catch (CDBException* e)
     {
         TRACE(_T("Exception in GetAllStudentsByName: %s\n"), e->m_strError);
         e->Delete();
     }

     return students;
 }

 vector<StudentGradeInfo> GradebookDatabase::GetTopStudents()
 {
     vector<StudentGradeInfo> results;
     CString sql;
     sql.Format(_T(
         "SELECT s.studentID, s.numberInClass, s.fullName, s.dateOfBirth, s.classID, c.classNumber, "
         "NULL AS subjectID, '' AS subjectName, AVG(g.gradeValue) AS gradeValue "
         "FROM Student s "
         "INNER JOIN Grade g ON s.studentID = g.studentID "
         "INNER JOIN Class c ON s.classID = c.classID "
         "GROUP BY s.studentID, s.numberInClass, s.fullName, s.dateOfBirth, s.classID, c.classNumber "
         "HAVING AVG(g.gradeValue) >= 5.50 "
         "ORDER BY AVG(g.gradeValue) DESC"));

     CRecordset* pRecordset = ExecuteQuery(sql);
     if (pRecordset)
     {
         while (!pRecordset->IsEOF())
         {
             StudentGradeInfo info;
             CString value;

             pRecordset->GetFieldValue((short)0, value);
             info.studentID = _ttoi(value);

             pRecordset->GetFieldValue(1, value);
             info.numberInClass = _ttoi(value);

             pRecordset->GetFieldValue(2, info.fullName);

             pRecordset->GetFieldValue(3, value);
             info.dateOfBirth = value;

             pRecordset->GetFieldValue(4, value);
             info.classID = _ttoi(value); 

             pRecordset->GetFieldValue(5, info.classNumber); 

             info.subjectID = 0;
             info.subjectName = _T("");

             pRecordset->GetFieldValue(8, value); 
             info.gradeValue = _ttof(value);

             results.push_back(info);
             pRecordset->MoveNext();
         }
         pRecordset->Close();
         delete pRecordset;
     }
     return results;
 }

 vector<StudentGradeInfo> GradebookDatabase::GetStudentsNeedingRemedialExam(int subjectID)
 {
     vector<StudentGradeInfo> results;
     CString sql;
     sql.Format(_T(
         "SELECT s.studentID, s.numberInClass, s.fullName, s.dateOfBirth, s.classID, c.classNumber, "
         "sub.subjectID, sub.subjectName, MIN(g.gradeValue) AS gradeValue "
         "FROM Student s "
         "INNER JOIN Grade g ON s.studentID = g.studentID "
         "INNER JOIN Subject sub ON g.subjectID = sub.subjectID "
         "INNER JOIN Class c ON s.classID = c.classID "
         "WHERE g.subjectID = %d AND g.gradeValue < 3.0 "
         "GROUP BY s.studentID, s.numberInClass, s.fullName, s.dateOfBirth, s.classID, c.classNumber, sub.subjectID, sub.subjectName "
         "ORDER BY s.fullName, sub.subjectName"
     ), subjectID);

     CRecordset* pRecordset = ExecuteQuery(sql);
     if (pRecordset)
     {
         while (!pRecordset->IsEOF())
         {
             StudentGradeInfo info;
             CString value;

             pRecordset->GetFieldValue((short)0, value);
             info.studentID = _ttoi(value);

             pRecordset->GetFieldValue(1, value);
             info.numberInClass = _ttoi(value);

             pRecordset->GetFieldValue(2, info.fullName);

             pRecordset->GetFieldValue(3, value);
             info.dateOfBirth = value;

             pRecordset->GetFieldValue(4, value);
             info.classID = _ttoi(value); 

             pRecordset->GetFieldValue(5, info.classNumber); 

             pRecordset->GetFieldValue(6, value);
             info.subjectID = _ttoi(value);

             pRecordset->GetFieldValue(7, info.subjectName);

             pRecordset->GetFieldValue(8, value);
             info.gradeValue = _ttof(value);

             results.push_back(info);
             pRecordset->MoveNext();
         }
         pRecordset->Close();
         delete pRecordset;
     }
     return results;
 }

 vector<StudentGradeInfo> GradebookDatabase::GetStudentsWithMultipleFailingSubjects()
 {
     vector<StudentGradeInfo> students;
     CString sql = _T("SELECT DISTINCT s.studentID, s.numberInClass, s.fullName, s.dateOfBirth, s.classID, c.classNumber ")
         _T("FROM Student s ")
         _T("INNER JOIN Grade g ON s.studentID = g.studentID ")
         _T("INNER JOIN Class c ON s.classID = c.classID ")
         _T("WHERE g.gradeValue < 3.00 ")
         _T("GROUP BY s.studentID, s.numberInClass, s.fullName, s.dateOfBirth, s.classID, c.classNumber ")
         _T("HAVING COUNT(DISTINCT g.subjectID) >= 3 ")
         _T("ORDER BY s.fullName");

     CRecordset* pRecordset = ExecuteQuery(sql);
     if (pRecordset)
     {
         while (!pRecordset->IsEOF())
         {
             StudentGradeInfo student;
             CString value;

             pRecordset->GetFieldValue((short)0, value);
             student.studentID = _ttoi(value);

             pRecordset->GetFieldValue(1, value);
             student.numberInClass = _ttoi(value);

             pRecordset->GetFieldValue(2, student.fullName);

             pRecordset->GetFieldValue(3, value);
             student.dateOfBirth = value;

             pRecordset->GetFieldValue(4, value);
             student.classID = _ttoi(value); 

             pRecordset->GetFieldValue(5, student.classNumber);

             students.push_back(student);
             pRecordset->MoveNext();
         }

         pRecordset->Close();
         delete pRecordset;
     }

     return students;
 }

 vector<StudentGradeInfo> GradebookDatabase::GetStudentsWithBirthdayToday()
 {
     vector<StudentGradeInfo> students;
     COleDateTime today = COleDateTime::GetCurrentTime();

     int month = today.GetMonth();
     int day = today.GetDay();

     CString sql;
     sql.Format(_T("SELECT s.studentID, s.numberInClass, s.fullName, s.dateOfBirth, s.classID, c.classNumber ")
         _T("FROM Student s ")
         _T("INNER JOIN Class c ON s.classID = c.classID ")
         _T("WHERE MONTH(s.dateOfBirth) = %d AND DAY(s.dateOfBirth) = %d ")
         _T("ORDER BY s.fullName"),
         month, day);

     CRecordset* pRecordset = ExecuteQuery(sql);
     if (pRecordset)
     {
         while (!pRecordset->IsEOF())
         {
             StudentGradeInfo student;
             CString value;

             pRecordset->GetFieldValue((short)0, value);
             student.studentID = _ttoi(value);

             pRecordset->GetFieldValue(1, value);
             student.numberInClass = _ttoi(value);

             pRecordset->GetFieldValue(2, student.fullName);

             pRecordset->GetFieldValue(3, value);
             student.dateOfBirth = value;

             pRecordset->GetFieldValue(4, value);
             student.classID = _ttoi(value); 

             pRecordset->GetFieldValue(5, student.classNumber); 

             students.push_back(student);
             pRecordset->MoveNext();
         }

         pRecordset->Close();
         delete pRecordset;
     }

     return students;
 }

 // Get class average grade for a specific subject 
 double GradebookDatabase::GetClassAverageGrade(int classNumber, int subjectID)
 {
     double average = 0.0;
     CString sql;
     sql.Format(_T("SELECT AVG(g.gradeValue) FROM Grade g ")
         _T("INNER JOIN Student s ON g.studentID = s.studentID ")
         _T("INNER JOIN Class c ON s.classID = c.classID ")
         _T("WHERE c.classNumber = %d AND g.subjectID = %d"),
         classNumber, subjectID);

     CRecordset* pRecordset = ExecuteQuery(sql);
     if (pRecordset && !pRecordset->IsEOF())
     {
         CString value;
         pRecordset->GetFieldValue((short)0, value);
         if (!value.IsEmpty())
         {
             average = _ttof(value);
         }

         pRecordset->Close();
         delete pRecordset;
     }

     return average;
 }

// Get student overall average grade
double GradebookDatabase::GetStudentOverallAverage(int studentID)
{
    double average = 0.0;
    CString sql;
    sql.Format(_T("SELECT AVG(gradeValue) FROM Grade WHERE studentID = %d"), studentID);

    CRecordset* pRecordset = ExecuteQuery(sql);
    if (pRecordset && !pRecordset->IsEOF())
    {
        CString value;
        pRecordset->GetFieldValue((short)0, value);
        if (!value.IsEmpty())
        {
            average = _ttof(value);
        }

        pRecordset->Close();
        delete pRecordset;
    }

    return average;
}

double GradebookDatabase::GetStudentAverageGrade(int studentID, int subjectID)
{
    double average = 0.0;
    CString sql;
    sql.Format(_T("SELECT AVG(gradeValue) FROM Grade WHERE studentID = %d AND subjectID = %d"), studentID, subjectID);

    CRecordset* pRecordset = ExecuteQuery(sql);
    if (pRecordset && !pRecordset->IsEOF())
    {
        CString value;
        pRecordset->GetFieldValue((short)0, value);
        if (!value.IsEmpty())
        {
            average = _ttof(value);
        }
        pRecordset->Close();
        delete pRecordset;
    }
    return average;
}

vector<Class> GradebookDatabase::GetAllClasses()
{
    vector<Class> classes;
    CString sql = _T("SELECT classID, classNumber, classTeacherID FROM Class ORDER BY classNumber");

    CRecordset* pRecordset = ExecuteQuery(sql);
    if (pRecordset)
    {
        while (!pRecordset->IsEOF())
        {
            Class c;
            CString value;

            pRecordset->GetFieldValue((short)0, value);
            c.classID = _ttoi(value);

            pRecordset->GetFieldValue(1, c.classNumber);

            pRecordset->GetFieldValue(2, value);
            c.classTeacherID = _ttoi(value);

            classes.push_back(c);
            pRecordset->MoveNext();
        }

        pRecordset->Close();
        delete pRecordset;
    }

    return classes;
}
