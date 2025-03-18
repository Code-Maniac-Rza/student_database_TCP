#include "course.h"
#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

void add_course(int roll_no, int code, int marks)
{
    Student *student = find_student(roll_no);
    if (!student)
    {
        fprintf(stderr, "Error: Student with roll no %d does not exist.\n", roll_no);
        return;
    }

    Course *current = student->courses_head;
    while (current)
    {
        if (current->code == code)
        {
            fprintf(stderr, "Error: Course %d already exists for student %d.\n", code, roll_no);
            return;
        }
        current = current->next;
    }

    Course *new_course = (Course *)malloc(sizeof(Course));
    new_course->code = code;
    new_course->marks = marks;
    new_course->next = NULL;
    new_course->prev = NULL;

    if (!student->courses_head)
    {
        student->courses_head = student->courses_tail = new_course;
    }
    else
    {
        student->courses_tail->next = new_course;
        new_course->prev = student->courses_tail;
        student->courses_tail = new_course;
    }

    student->num_courses++;
    printf("Course added: Roll No = %d, Course Code = %d, Marks = %d\n", roll_no, code, marks);
    write_to_file("23_09.out");
}

void modify_course(int roll_no, int code, int marks)
{
    Student *student = find_student(roll_no);
    if (!student)
    {
        fprintf(stderr, "Error: Student with roll no %d does not exist.\n", roll_no);
        return;
    }

    Course *current = student->courses_head;
    while (current)
    {
        if (current->code == code)
        {
            current->marks = marks;
            write_to_file("23_09.out");
            return;
        }
        current = current->next;
    }

    fprintf(stderr, "Error: Course %d does not exist for student %d.\n", code, roll_no);
    printf("Course modified: Roll No = %d, Course Code = %d, New Marks = %d\n", roll_no, code, marks);
    write_to_file("23_09.out");
}

void delete_course(int roll_no, int code)
{
    Student *student = find_student(roll_no);
    if (!student)
    {
        fprintf(stderr, "Error: Student with roll no %d does not exist.\n", roll_no);
        return;
    }

    Course *current = student->courses_head;
    while (current)
    {
        if (current->code == code)
        {
            if (current->prev)
            {
                current->prev->next = current->next;
            }
            else
            {
                student->courses_head = current->next;
            }

            if (current->next)
            {
                current->next->prev = current->prev;
            }
            else
            {
                student->courses_tail = current->prev;
            }

            free(current);
            student->num_courses--;
            write_to_file("23_09.out");
            return;
        }
        current = current->next;
    }

    fprintf(stderr, "Error: Course %d does not exist for student %d.\n", code, roll_no);
    printf("Course deleted: Roll No = %d, Course Code = %d\n", roll_no, code);
    write_to_file("23_09.out");
}

void print_courses(int roll_no)
{
    Student *student = find_student(roll_no);
    if (!student)
    {
        fprintf(stderr, "Student with roll no %d does not exist.\n", roll_no);
        return;
    }

    Course *current = student->courses_head;
    printf("Courses for student %d:\n", roll_no);
    while (current)
    {
        printf("Course Code: %d, Marks: %d\n", current->code, current->marks);
        current = current->next;
    }
}
