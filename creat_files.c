#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/*--------- Headers ---------*/
#include "second_pass.h"
#include "tree.h"
#include "general.h"
#include "pre_assembler.h"
#include "syntax_pass.h"

/*---------------------------------*/
void process_filenames(char* file_name) {
    int error = pre_assembler(file_name);
    if (error == e_MID) {
        fprintf(stderr, "File %s not found\n", file_name);
        return;
    }
    else if (error == e_ON) {
        return;
    }
    if (!syntax_pass(file_name)) {
        second_pass(file_name);
    }
    fprintf(stderr, "End File %s\n", file_name);
}
/*---------------------------------*/
void create_obj_file(const char* filename, char machine_code[][2], int size, int DC, int IC) {
    char obj_file[MAX_LINE_SIZE];
    FILE* file;
    int i = 0;

    strncpy(obj_file, filename, sizeof(obj_file) - 1);
    obj_file[sizeof(obj_file) - 1] = '\0';
    strcat(obj_file, ".obj");

    file = fopen(obj_file, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return;
    }
    fprintf(file, "%d \t %d\n", DC, IC);

    for (i = 0; i < size; i++) {
        fprintf(file, "%c%c\n", machine_code[i][0], machine_code[i][1]);
    }
    fclose(file);
}

/*---------------------------------*/
void create_ent_file(const char* filename, lex_tree* lex_arr, label* label_arr, int size) {
    char ent_file[MAX_LINE_SIZE];
    FILE* file;
    int i = 0;

    if (size == 0)
        return;

    strncpy(ent_file, filename, sizeof(ent_file) - 1);
    ent_file[sizeof(ent_file) - 1] = '\0';
    strcat(ent_file, ".ent");

    file = fopen(ent_file, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return;
    }

    for (i = 0; i <= size; i++) {
        if (get_label_type(label_arr, i) == ent) {
            fprintf(file, "%s\t %d\n", get_label_name(label_arr, i), get_code_line(lex_arr, get_label_source(label_arr, i)));
        }
    }
    fclose(file);
}

/*---------------------------------*/
void create_ext_file(const char* filename, ext_label* ext_arr, int size) {
    char ext_file[MAX_LINE_SIZE];
    FILE* file;
    int i = 0;

    if (size == 0)
        return;

    strncpy(ext_file, filename, sizeof(ext_file) - 1);
    ext_file[sizeof(ext_file) - 1] = '\0';
    strcat(ext_file, ".ext");

    file = fopen(ext_file, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return;
    }

    for (i = 0; i < size; i++) {
        fprintf(file, "%s\t %d\n", get_ext_label_name(ext_arr, i), get_ext_label_source(ext_arr, i));
    }
    fclose(file);
}
