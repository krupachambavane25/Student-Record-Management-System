#ifndef STUDENT_H
#define STUDENT_H

#define MAX_NAME 100
#define MAX_COURSE 100
#define MAX_EMAIL 100

typedef struct Student
{
    int rollNo;
    char name[MAX_NAME];
    char course[MAX_COURSE];
    char email[MAX_EMAIL];
    float cgpa;

    struct Student *next;
} Student;


/* Waiting List Queue */

typedef struct QueueNode
{
    int rollNo;
    char name[MAX_NAME];
    char course[MAX_COURSE];

    struct QueueNode *next;
} QueueNode;


/* Recently Deleted Stack */

typedef struct StackNode
{
    Student data;

    struct StackNode *next;
} StackNode;


/* Linked List */

void addStudent(
    int rollNo,
    const char name[],
    const char course[],
    const char email[],
    float cgpa
);

void displayStudents(void);

void searchStudent(int rollNo);

void updateStudent(
    int rollNo,
    const char name[],
    const char course[],
    const char email[],
    float cgpa
);

void deleteStudent(int rollNo);

void sortStudents(void);


/* Queue */

void enqueueStudent(
    int rollNo,
    const char name[],
    const char course[]
);

void dequeueStudent(void);

void displayQueue(void);


/* Stack */

void displayDeleted(void);

void undoDelete(void);


/* File handling */

void saveStudents(void);

void loadStudents(void);


/* CSV */

void exportCSV(void);


/* Statistics */

int getStudentCount(void);

float getAverageCGPA(void);


/* Used by server */

extern Student *head;

#endif