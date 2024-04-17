#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/*--------- Headers ---------*/
#include "second_pass.h"
#include "tree.h"
#include "general.h"


void second_pass(char* file_name) {
    char** lines;
    int line_cnt = 0, code_line=0;
    int error_flag = OFF, label_flag1=OFF, label_flag2=OFF; /* Turned off by default*/
    int ent_flag = OFF, ext_flag = OFF;
    int labels_cnt = 0;
    int ext_cnt = 0;
    int IC = 0, DC = 0;

    lex_tree* lex_arr = NULL;
    label* label_arr = NULL;
    ext_label* ext_arr = NULL;

    char machine_code[MAX_MACHINE_CODE][2]; /*Array holding machine code instructions*/

    lines = read_file_lines(file_name, ".am", &line_cnt, &error_flag);
    if (error_flag || lines == NULL) {
        return;
    }

    /* Allocate memory for lexical and labels arrays */
    lex_arr = create_lex_tree(line_cnt);
    if (lex_arr == NULL) {
        return;
    }

    label_arr = create_label_array(line_cnt);
    if (label_arr == NULL) {
        return;
    }

    ext_arr = create_ext_label_array(line_cnt);
    if (ext_arr == NULL) {
        return;
    }

    /* Read and set data for each line */
    code_line = set_line_tree(lines, line_cnt, &ent_flag, &ext_flag, lex_arr, label_arr, &labels_cnt, &DC, &IC);
    error_flag = set_line_code(machine_code, line_cnt, lex_arr, label_arr, ext_arr, &ext_cnt, labels_cnt);

    /* Validate label declarations and entries */
    label_flag1 = is_valid_labels_dec(lex_arr, line_cnt);
    label_flag2 = is_valid_ent_label(label_arr, labels_cnt);

    if (error_flag) {
        if (label_flag1 && label_flag2) {
            /* Create output files */
            create_obj_file(file_name, machine_code, code_line, DC, IC);
            if (ent_flag == ON)
                create_ent_file(file_name, lex_arr, label_arr, labels_cnt);
            if (ext_flag == ON)
                create_ext_file(file_name, ext_arr, ext_cnt);
        }
    }
    /* Free allocated memory */
    free_lines(lines, line_cnt);
    /*free(lines);*/
    free(lex_arr);
    free(label_arr);
    free(ext_arr);
}
