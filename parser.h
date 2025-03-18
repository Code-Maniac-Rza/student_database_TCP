#ifndef PARSER_H
#define PARSER_H

enum Function {
    ADD_STUDENT = 0,
    ADD_COURSE,
    MODIFY_STUDENT,
    MODIFY_COURSE,
    DELETE_STUDENT,
    DELETE_COURSE
};

void parse_request(char *buffer);
void write_to_file(const char *filename);
#endif
