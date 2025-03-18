#ifndef COURSE_H
#define COURSE_H

typedef struct Course
{
    int code;
    int marks;
    struct Course *next;
    struct Course *prev;
} Course;

void add_course(int roll_no, int code, int marks);
void modify_course(int roll_no, int code, int marks);
void delete_course(int roll_no, int code);

#endif
