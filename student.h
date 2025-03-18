#ifndef STUDENT_H
#define STUDENT_H

#include "course.h"

typedef struct Student
{
    int roll_no;
    char name[50];
    float cgpa;
    int num_courses;
    Course *courses_head;
    Course *courses_tail;
    struct Student *next;
    struct Student *prev;
} Student;

extern Student *head;
extern Student *tail;

void add_student(int roll_no, char *name, float cgpa, int num_courses);
void modify_student(int roll_no, float cgpa);
void delete_student(int roll_no);
Student *find_student(int roll_no);
void init_student_list(void);
void free_student_list(void);

#endif
