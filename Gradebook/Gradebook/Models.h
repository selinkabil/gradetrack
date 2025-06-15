#pragma once
#include <afxdb.h>
#include <CString>
struct Teacher {
    int teacherID;
    CString fullName;
};

struct Class {
    int classID;
    CString classNumber;
    int classTeacherID;
};

struct Subject {
    int subjectID;
    CString subjectName;
    int teacherID;
    CString roomNumber;
};

struct Curriculum {
    int curriculumID;
    CString curriculumName;
    int classID;
    int subjectID;
};

struct Student {
    int studentID;
    int numberInClass;
    CString fullName;
    CString dateOfBirth;
    int classID;
};

struct Grade {
    int gradeID;
    int studentID;
    int subjectID;
    CString gradeDate;
    double gradeValue;
};
struct SubjectStats
{
    double minGrade;
    double maxGrade;
    double avgGrade;
    int totalGrades;
};

struct StudentGradeInfo
{
    int studentID;
    int numberInClass;
    CString fullName;
    CString dateOfBirth;
    int classID;           
    CString classNumber;   
    int subjectID;
    CString subjectName;
    double gradeValue;
};