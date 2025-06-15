#pragma once
#include <afxdb.h>
#include <vector>
#include <memory>
#include "Models.h"
class GradebookDatabase
{
private:
    std::unique_ptr<CDatabase> m_pDatabase;
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
    std::vector<Teacher> GetAllTeachers();
    Teacher GetTeacher(int teacherID);

    // Class operations
    bool AddClass(const CString& classNumber, int classTeacherID, int& newClassID);
    bool UpdateClass(int classID, const CString& classNumber, int classTeacherID);
    bool DeleteClass(int classID);
    std::vector<Class> GetAllClasses();
    Class GetClass(int classID);

    // Subject operations
    bool AddSubject(const CString& subjectName, int teacherID, const CString& roomNumber, int& newSubjectID);
    bool UpdateSubject(int subjectID, const CString& subjectName, int teacherID, const CString& roomNumber);
    bool DeleteSubject(int subjectID);
    std::vector<Subject> GetAllSubjects();
    Subject GetSubject(int subjectID);

    // Student operations
    bool AddStudent(int numberInClass, const CString& fullName, const COleDateTime& dateOfBirth, int classID, int& newStudentID);
    bool UpdateStudent(int studentID, int numberInClass, const CString& fullName, const COleDateTime& dateOfBirth, int classID);
    bool DeleteStudent(int studentID);
    std::vector<Student> GetAllStudents();
    std::vector<Student> GetStudentsByClass(int classID);
    Student GetStudent(int studentID);

    // Grade operations
    bool AddGrade(int studentID, int subjectID, const COleDateTime& gradeDate, double gradeValue, int& newGradeID);
    bool UpdateGrade(int gradeID, int studentID, int subjectID, const COleDateTime& gradeDate, double gradeValue);
    bool DeleteGrade(int gradeID);
    std::vector<Grade> GetAllGrades();
    std::vector<Grade> GetGradesByStudent(int studentID);
    std::vector<Grade> GetGradesBySubject(int subjectID);

    // Curriculum operations
    bool AddCurriculum(const CString& curriculumName, int classID, int subjectID, int& newCurriculumID);
    bool UpdateCurriculum(int curriculumID, const CString& curriculumName, int classID, int subjectID);
    bool DeleteCurriculum(int curriculumID);
    std::vector<Curriculum> GetAllCurricula();

    // Utility methods
    CString GetLastError() const { return m_lastError; }

private:
    CString m_lastError;
    void SetLastError(const CString& error);
    bool ExecuteSQL(const CString& sql);
    CRecordset* ExecuteQuery(const CString& sql);
};