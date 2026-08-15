#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"


/* =====================================================
   GLOBAL DATA STRUCTURES
   ===================================================== */

/* Linked List */
Student *head = NULL;

/* Queue */
QueueNode *front = NULL;
QueueNode *rear = NULL;

/* Stack */
StackNode *top = NULL;


/* =====================================================
   LINKED LIST - ADD STUDENT
   ===================================================== */

void addStudent(
    int rollNo,
    const char name[],
    const char course[],
    const char email[],
    float cgpa
)
{
    Student *newStudent;
    Student *temp;

    newStudent = (Student *)malloc(sizeof(Student));

    if (newStudent == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    newStudent->rollNo = rollNo;

    strncpy(newStudent->name, name, MAX_NAME - 1);
    newStudent->name[MAX_NAME - 1] = '\0';

    strncpy(newStudent->course, course, MAX_COURSE - 1);
    newStudent->course[MAX_COURSE - 1] = '\0';

    strncpy(newStudent->email, email, MAX_EMAIL - 1);
    newStudent->email[MAX_EMAIL - 1] = '\0';

    newStudent->cgpa = cgpa;

    newStudent->next = NULL;


    /* First node */
    if (head == NULL)
    {
        head = newStudent;
    }
    else
    {
        temp = head;

        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newStudent;
    }

    saveStudents();
}


/* =====================================================
   DISPLAY STUDENTS
   ===================================================== */

void displayStudents(void)
{
    Student *temp;

    temp = head;

    if (temp == NULL)
    {
        printf("No students found.\n");
        return;
    }

    printf("\n");
    printf("-------------------------------------------------------------\n");
    printf("Roll No | Name | Course | Email | CGPA\n");
    printf("-------------------------------------------------------------\n");

    while (temp != NULL)
    {
        printf(
            "%d | %s | %s | %s | %.2f\n",
            temp->rollNo,
            temp->name,
            temp->course,
            temp->email,
            temp->cgpa
        );

        temp = temp->next;
    }

    printf("-------------------------------------------------------------\n");
}


/* =====================================================
   SEARCH STUDENT
   ===================================================== */

void searchStudent(int rollNo)
{
    Student *temp;

    temp = head;

    while (temp != NULL)
    {
        if (temp->rollNo == rollNo)
        {
            printf("\nStudent Found!\n");
            printf("Roll Number : %d\n", temp->rollNo);
            printf("Name        : %s\n", temp->name);
            printf("Course      : %s\n", temp->course);
            printf("Email       : %s\n", temp->email);
            printf("CGPA        : %.2f\n", temp->cgpa);

            return;
        }

        temp = temp->next;
    }

    printf("Student not found.\n");
}


/* =====================================================
   UPDATE STUDENT
   ===================================================== */

void updateStudent(
    int rollNo,
    const char name[],
    const char course[],
    const char email[],
    float cgpa
)
{
    Student *temp;

    temp = head;

    while (temp != NULL)
    {
        if (temp->rollNo == rollNo)
        {
            strncpy(temp->name, name, MAX_NAME - 1);
            temp->name[MAX_NAME - 1] = '\0';

            strncpy(temp->course, course, MAX_COURSE - 1);
            temp->course[MAX_COURSE - 1] = '\0';

            strncpy(temp->email, email, MAX_EMAIL - 1);
            temp->email[MAX_EMAIL - 1] = '\0';

            temp->cgpa = cgpa;

            saveStudents();

            printf("Student updated successfully.\n");

            return;
        }

        temp = temp->next;
    }

    printf("Student not found.\n");
}


/* =====================================================
   DELETE STUDENT
   Linked List + Stack
   ===================================================== */

void deleteStudent(int rollNo)
{
    Student *temp;
    Student *previous;
    StackNode *deleted;

    temp = head;
    previous = NULL;


    while (temp != NULL)
    {
        if (temp->rollNo == rollNo)
        {
            /*
               Store deleted student in stack
               for Undo operation.
            */

            deleted = (StackNode *)malloc(sizeof(StackNode));

            if (deleted != NULL)
            {
                deleted->data = *temp;

                /*
                   Do not keep the old linked-list
                   pointer inside the stack.
                */

                deleted->data.next = NULL;

                deleted->next = top;

                top = deleted;
            }


            /* Delete first node */
            if (previous == NULL)
            {
                head = temp->next;
            }
            else
            {
                previous->next = temp->next;
            }

            free(temp);

            saveStudents();

            printf("Student deleted successfully.\n");

            return;
        }

        previous = temp;
        temp = temp->next;
    }

    printf("Student not found.\n");
}


/* =====================================================
   SORT STUDENTS BY CGPA
   Highest CGPA first
   ===================================================== */

void sortStudents(void)
{
    Student *i;
    Student *j;

    if (head == NULL)
    {
        return;
    }


    for (i = head; i != NULL; i = i->next)
    {
        for (j = i->next; j != NULL; j = j->next)
        {
            if (i->cgpa < j->cgpa)
            {
                int tempRoll;
                float tempCGPA;

                char tempName[MAX_NAME];
                char tempCourse[MAX_COURSE];
                char tempEmail[MAX_EMAIL];


                tempRoll = i->rollNo;
                i->rollNo = j->rollNo;
                j->rollNo = tempRoll;


                tempCGPA = i->cgpa;
                i->cgpa = j->cgpa;
                j->cgpa = tempCGPA;


                strcpy(tempName, i->name);
                strcpy(i->name, j->name);
                strcpy(j->name, tempName);


                strcpy(tempCourse, i->course);
                strcpy(i->course, j->course);
                strcpy(j->course, tempCourse);


                strcpy(tempEmail, i->email);
                strcpy(i->email, j->email);
                strcpy(j->email, tempEmail);
            }
        }
    }

    saveStudents();
}


/* =====================================================
   QUEUE - ADD TO WAITING LIST
   ===================================================== */

void enqueueStudent(
    int rollNo,
    const char name[],
    const char course[]
)
{
    QueueNode *newNode;

    newNode = (QueueNode *)malloc(sizeof(QueueNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }


    newNode->rollNo = rollNo;

    strncpy(newNode->name, name, MAX_NAME - 1);
    newNode->name[MAX_NAME - 1] = '\0';

    strncpy(newNode->course, course, MAX_COURSE - 1);
    newNode->course[MAX_COURSE - 1] = '\0';

    newNode->next = NULL;


    /* Queue is empty */
    if (rear == NULL)
    {
        front = newNode;
        rear = newNode;
    }
    else
    {
        rear->next = newNode;
        rear = newNode;
    }

    printf("Student added to waiting list.\n");
}


/* =====================================================
   QUEUE - REMOVE
   ===================================================== */

void dequeueStudent(void)
{
    QueueNode *temp;

    if (front == NULL)
    {
        printf("Waiting list is empty.\n");
        return;
    }

    temp = front;

    front = front->next;

    if (front == NULL)
    {
        rear = NULL;
    }

    printf(
        "Student admitted: %s\n",
        temp->name
    );

    free(temp);
}


/* =====================================================
   DISPLAY WAITING LIST
   ===================================================== */

void displayQueue(void)
{
    QueueNode *temp;

    temp = front;

    if (temp == NULL)
    {
        printf("Waiting list is empty.\n");
        return;
    }


    printf("\nWaiting List\n");
    printf("-----------------------------\n");

    while (temp != NULL)
    {
        printf(
            "%d | %s | %s\n",
            temp->rollNo,
            temp->name,
            temp->course
        );

        temp = temp->next;
    }

    printf("-----------------------------\n");
}


/* =====================================================
   DISPLAY RECENTLY DELETED
   ===================================================== */

void displayDeleted(void)
{
    StackNode *temp;

    temp = top;

    if (temp == NULL)
    {
        printf("No recently deleted students.\n");
        return;
    }


    printf("\nRecently Deleted Students\n");
    printf("-----------------------------\n");

    while (temp != NULL)
    {
        printf(
            "%d | %s | %.2f\n",
            temp->data.rollNo,
            temp->data.name,
            temp->data.cgpa
        );

        temp = temp->next;
    }

    printf("-----------------------------\n");
}


/* =====================================================
   UNDO LAST DELETE
   ===================================================== */

void undoDelete(void)
{
    StackNode *temp;
    Student *newStudent;
    Student *last;


    if (top == NULL)
    {
        printf("Nothing to undo.\n");
        return;
    }


    newStudent = (Student *)malloc(sizeof(Student));

    if (newStudent == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }


    newStudent->rollNo = top->data.rollNo;

    strcpy(
        newStudent->name,
        top->data.name
    );

    strcpy(
        newStudent->course,
        top->data.course
    );

    strcpy(
        newStudent->email,
        top->data.email
    );

    newStudent->cgpa = top->data.cgpa;

    newStudent->next = NULL;


    /* Add back to linked list */

    if (head == NULL)
    {
        head = newStudent;
    }
    else
    {
        last = head;

        while (last->next != NULL)
        {
            last = last->next;
        }

        last->next = newStudent;
    }


    /* Remove from stack */

    temp = top;

    top = top->next;

    free(temp);

    saveStudents();

    printf("Last deletion has been undone.\n");
}


/* =====================================================
   SAVE STUDENTS
   ===================================================== */

void saveStudents(void)
{
    FILE *file;
    Student *temp;

    file = fopen("students.dat", "wb");

    if (file == NULL)
    {
        printf("Could not save student records.\n");
        return;
    }


    temp = head;

    while (temp != NULL)
    {
        /*
           Save only student data.
           Pointer is NOT stored.
        */

        fwrite(
            &temp->rollNo,
            sizeof(int),
            1,
            file
        );

        fwrite(
            temp->name,
            sizeof(char),
            MAX_NAME,
            file
        );

        fwrite(
            temp->course,
            sizeof(char),
            MAX_COURSE,
            file
        );

        fwrite(
            temp->email,
            sizeof(char),
            MAX_EMAIL,
            file
        );

        fwrite(
            &temp->cgpa,
            sizeof(float),
            1,
            file
        );

        temp = temp->next;
    }

    fclose(file);
}


/* =====================================================
   LOAD STUDENTS
   ===================================================== */

void loadStudents(void)
{
    FILE *file;

    int rollNo;
    float cgpa;

    char name[MAX_NAME];
    char course[MAX_COURSE];
    char email[MAX_EMAIL];

    Student *newStudent;
    Student *last;


    file = fopen("students.dat", "rb");

    if (file == NULL)
    {
        return;
    }


    while (1)
    {
        if (fread(
            &rollNo,
            sizeof(int),
            1,
            file
        ) != 1)
        {
            break;
        }


        if (fread(
            name,
            sizeof(char),
            MAX_NAME,
            file
        ) != MAX_NAME)
        {
            break;
        }


        if (fread(
            course,
            sizeof(char),
            MAX_COURSE,
            file
        ) != MAX_COURSE)
        {
            break;
        }


        if (fread(
            email,
            sizeof(char),
            MAX_EMAIL,
            file
        ) != MAX_EMAIL)
        {
            break;
        }


        if (fread(
            &cgpa,
            sizeof(float),
            1,
            file
        ) != 1)
        {
            break;
        }


        newStudent =
            (Student *)malloc(
                sizeof(Student)
            );


        if (newStudent == NULL)
        {
            break;
        }


        newStudent->rollNo = rollNo;

        strcpy(
            newStudent->name,
            name
        );

        strcpy(
            newStudent->course,
            course
        );

        strcpy(
            newStudent->email,
            email
        );

        newStudent->cgpa = cgpa;

        newStudent->next = NULL;


        if (head == NULL)
        {
            head = newStudent;
        }
        else
        {
            last = head;

            while (last->next != NULL)
            {
                last = last->next;
            }

            last->next = newStudent;
        }
    }


    fclose(file);
}


/* =====================================================
   EXPORT TO CSV
   ===================================================== */

void exportCSV(void)
{
    FILE *file;
    Student *temp;


    file = fopen(
        "student_records.csv",
        "w"
    );


    if (file == NULL)
    {
        printf("Could not create CSV file.\n");
        return;
    }


    fprintf(
        file,
        "Roll Number,Name,Course,Email,CGPA\n"
    );


    temp = head;


    while (temp != NULL)
    {
        fprintf(
            file,
            "%d,%s,%s,%s,%.2f\n",
            temp->rollNo,
            temp->name,
            temp->course,
            temp->email,
            temp->cgpa
        );

        temp = temp->next;
    }


    fclose(file);

    printf(
        "Student records exported successfully.\n"
    );
}


/* =====================================================
   STUDENT COUNT
   ===================================================== */

int getStudentCount(void)
{
    Student *temp;
    int count = 0;


    temp = head;


    while (temp != NULL)
    {
        count++;

        temp = temp->next;
    }


    return count;
}


/* =====================================================
   AVERAGE CGPA
   ===================================================== */

float getAverageCGPA(void)
{
    Student *temp;

    float total = 0.0f;

    int count = 0;


    temp = head;


    while (temp != NULL)
    {
        total += temp->cgpa;

        count++;

        temp = temp->next;
    }


    if (count == 0)
    {
        return 0.0f;
    }


    return total / count;
}