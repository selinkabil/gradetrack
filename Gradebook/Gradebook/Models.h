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
