#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

Student *head = NULL;
Student *tail = NULL;

void init_student_list()
{
    head = NULL;
    tail = NULL;
}

void free_student_list()
{
    Student *current = head;
    while (current)
    {
        Student *temp = current;
        current = current->next;
        free(temp);
    }
}

void add_student(int roll_no, char *name, float cgpa, int num_courses)
{
    if (find_student(roll_no))
    {
        fprintf(stderr, "Error: Student with roll no %d already exists.\n", roll_no);
        return;
    }

    Student *new_student = (Student *)malloc(sizeof(Student));
    new_student->roll_no = roll_no;
    strcpy(new_student->name, name);
    new_student->cgpa = cgpa;
    new_student->num_courses = num_courses;
    new_student->courses_head = NULL;
    new_student->courses_tail = NULL;
    new_student->next = NULL;
    new_student->prev = NULL;

    if (!head)
    {
        head = tail = new_student;
    }
    else
    {
        tail->next = new_student;
        new_student->prev = tail;
        tail = new_student;
    }
    printf("Student added: Roll No = %d, Name = %s, CGPA = %.2f, Courses = %d\n", roll_no, name, cgpa, num_courses);
    write_to_file("23_09.out");
}

void modify_student(int roll_no, float cgpa)
{
    Student *student = find_student(roll_no);
    if (!student)
    {
        fprintf(stderr, "Error: Student with roll no %d does not exist.\n", roll_no);
        return;
    }
    student->cgpa = cgpa;
    printf("Student modified: Roll No = %d, New CGPA = %.2f\n", roll_no, cgpa);
    write_to_file("23_09.out");
}

void delete_student(int roll_no)
{
    Student *student = find_student(roll_no);
    if (!student)
    {
        fprintf(stderr, "Error: Student with roll no %d does not exist.\n", roll_no);
        return;
    }

    if (student->prev)
    {
        student->prev->next = student->next;
    }
    else
    {
        head = student->next;
    }

    if (student->next)
    {
        student->next->prev = student->prev;
    }
    else
    {
        tail = student->prev;
    }

    free(student);
    printf("Student deleted: Roll No = %d\n", roll_no);
    write_to_file("23_09.out");
}

Student *find_student(int roll_no)
{
    Student *current = head;
    while (current)
    {
        if (current->roll_no == roll_no)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
