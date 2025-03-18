#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "student.h"
#include "course.h"
#include "parser.h"

void parse_request(char *buffer) {
    char operation[20];
    sscanf(buffer, "%s", operation);  

    if (strcmp(operation, "ADD_STUDENT") == 0) {
        int roll_no, num_courses;
        char name[50];
        float cgpa;
        printf("Inside add_student parser\n");
        sscanf(buffer, "ADD_STUDENT %d %s %f %d", &roll_no, name, &cgpa, &num_courses);
        add_student(roll_no, name, cgpa, num_courses);
        // printf("Student added: Roll No = %d, Name = %s, CGPA = %.2f, Courses = %d\n", roll_no, name, cgpa, num_courses);
    }
    else if (strcmp(operation, "MODIFY_STUDENT") == 0) {
        int roll_no;
        float cgpa;
        sscanf(buffer, "MODIFY_STUDENT %d %f", &roll_no, &cgpa);
        modify_student(roll_no, cgpa);
        // printf("Student modified: Roll No = %d, New CGPA = %.2f\n", roll_no, cgpa);
    }
    else if (strcmp(operation, "DELETE_STUDENT") == 0) {
        int roll_no;
        sscanf(buffer, "DELETE_STUDENT %d", &roll_no);
        delete_student(roll_no);
        // printf("Student deleted: Roll No = %d\n", roll_no);
    }
    else if (strcmp(operation, "ADD_COURSE") == 0) {
        int roll_no, course_code, marks;
        sscanf(buffer, "ADD_COURSE %d %d %d", &roll_no, &course_code, &marks);
        add_course(roll_no, course_code, marks);
        // printf("Course added: Roll No = %d, Course Code = %d, Marks = %d\n", roll_no, course_code, marks);
    }
    else if (strcmp(operation, "MODIFY_COURSE") == 0) {
        int roll_no, course_code, marks;
        sscanf(buffer, "MODIFY_COURSE %d %d %d", &roll_no, &course_code, &marks);
        modify_course(roll_no, course_code, marks);
        // printf("Course modified: Roll No = %d, Course Code = %d, New Marks = %d\n", roll_no, course_code, marks);
    }
    else if (strcmp(operation, "DELETE_COURSE") == 0) {
        int roll_no, course_code;
        sscanf(buffer, "DELETE_COURSE %d %d", &roll_no, &course_code);
        delete_course(roll_no, course_code);
        // printf("Course deleted: Roll No = %d, Course Code = %d\n", roll_no, course_code);
    }
    else {
        printf("Invalid operation: %s\n", operation);
    }
}

void write_to_file(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }

    Student *current_student = head;
    if(current_student == NULL){
        fprintf(file,"There is no data in the DLL\n");
    }
    while (current_student != NULL)
    {
        fprintf(file, "%d, %s, %.2f, %d\n", current_student->roll_no, current_student->name, current_student->cgpa, current_student->num_courses);

        Course *current_course = current_student->courses_head;
        while (current_course != NULL)
        {
            fprintf(file, "%d, %d\n", current_course->code, current_course->marks);
            current_course = current_course->next;
        }

        current_student = current_student->next;
    }

    fclose(file);
}