#pragma once
#include <afxdb.h>
#include <vector>
#include <memory>
#include "Models.h"
using namespace std;

class GradebookDatabase
{
private:
    unique_ptr<CDatabase> m_pDatabase;
    CString m_connectionString;
    bool m_isConnected;

public:
    // Constructor and Destructor
    GradebookDatabase();
    ~GradebookDatabase();

    // Connection management
    bool Connect(const CString& serverName = _T("(local)"),
        const CString& databaseName = _T("GradebookDB"),
        bool useWindowsAuth = true,
        const CString& username = _T(""),
        const CString& password = _T(""));
    void Disconnect();
    bool IsConnected() const { return m_isConnected; }

    // Teacher operations
    bool AddTeacher(const CString& fullName, int& newTeacherID);
    bool UpdateTeacher(int teacherID, const CString& fullName);
    bool DeleteTeacher(int teacherID);
     vector<Teacher> GetAllTeachers();
    Teacher GetTeacher(int teacherID);

    // Class operations

     vector<Class> GetAllClasses();


    // Subject operations
 
     vector<Subject> GetAllSubjects();
   
    // Student operations
    bool AddStudent(int numberInClass, const CString& fullName, const COleDateTime& dateOfBirth, int classID, int& newStudentID);
    bool UpdateStudent(int studentID, int numberInClass, const CString& fullName, const COleDateTime& dateOfBirth, int classID);
    bool DeleteStudent(int studentID);
     vector<StudentGradeInfo> GetAllStudents();


     bool AddGrade(int studentID, int subjectID, const COleDateTime& gradeDate, double gradeValue, int& newGradeID);

     vector<Grade> GetGradesByStudent(int studentID);

    // Utility methods
    CString GetLastError() const { return m_lastError; }

    // Additional analytical functions
     vector<StudentGradeInfo> GetTopStudents();
     vector<StudentGradeInfo> GetStudentsNeedingRemedialExam(int subjectID);
     vector<StudentGradeInfo> GetStudentsWithMultipleFailingSubjects();
     vector<StudentGradeInfo> GetStudentsWithBirthdayToday();

    // Utility functions for statistics
    double GetStudentAverageGrade(int studentID, int subjectID);
    double GetStudentOverallAverage(int studentID);
    double GetClassAverageGrade(int classID, int subjectID);

private:
    CString m_lastError;
    void SetLastError(const CString& error);
    bool ExecuteSQL(const CString& sql);
    CRecordset* ExecuteQuery(const CString& sql);
};