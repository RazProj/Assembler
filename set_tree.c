#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/*--------- Headers ---------*/
#include "second_pass.h"
#include "tree.h"
#include "general.h"

/*--------- Constants ---------*/
#define INST_LEN 3
#define ENTRY_LEN 6
#define EXTERN_LEN 7
#define DATA_LEN 5
#define STRING_LEN 7
#define REG_LOC 2
#define LINE_START 100

/*--------------- Function Prototypes ------------------*/
void get_label(char *line, int index, lex_tree *array, label *label_arr, int *labels_cnt);

void
set_label(char label[], int index, lex_tree *lex_arr, asm_label **label_arr, enum label_type n_type, int *labels_cnt);

/*instruction*/
int is_inst(char *line, int index, lex_tree *array);

void set_first_set(char *line, int index, lex_tree *array, enum inst_name inst_name);

void set_first_set_operands(char operand[], int op_num, int index, lex_tree *array);

void set_second_set(char *line, int index, lex_tree *array, enum inst_name inst_name);

void set_third_set(int index, lex_tree *array, enum inst_name inst_name);

/*directive*/
int is_dir(char *line, int index, lex_tree *array, label *label_arr, int *ent_flag, int *ext_flag, int *labels_cnt);

void set_ext(char *line, int index, lex_tree *lex_arr, label *label_arr, int *labels_cnt);

void set_ent(char *line, int index, lex_tree *lex_arr, label *label_arr, int *labels_cnt);

void set_string(char *line, int index, lex_tree *lex_arr);

/*
* Description:
* This function processes the input lines, extracting information to populate the lexical analysis tree (lex_arr).
* It identifies instructions and directives, sets labels, and calculates memory offsets.
*
* Parameters:
* @lines: An array of strings containing the input lines.
* @line_cnt: The total number of input lines.
* @ent_flag: A pointer to a flag indicating the presence of an entry directive.
* @ext_flag: A pointer to a flag indicating the presence of an extern directive.
* @lex_arr: An array of lexical tree nodes where information will be stored.
* @label_arr: An array of label structures to store label information.
* @labels_cnt: A pointer to an integer representing the count of labels.
*
* Returns:
* An integer representing the total number of memory addresses needed for the program.
*/
int set_line_tree(char **lines, int line_cnt, int *ent_flag, int *ext_flag, lex_tree *lex_arr, label *label_arr,
                  int *labels_cnt, int *DC, int *IC) {
    int i;
    for (i = 0; i < line_cnt; i++) {
        get_label(lines[i], i, lex_arr, label_arr, labels_cnt);

        if (is_inst(lines[i], i, lex_arr)) {
            set_code_line(lex_arr, i, *DC + LINE_START);
            *DC += get_line_cnt(lex_arr, i);
        } else if (is_dir(lines[i], i, lex_arr, label_arr, ent_flag, ext_flag, labels_cnt)) {
            if (get_dir_type(lex_arr, i) == dir_data || get_dir_type(lex_arr, i) == dir_string)
                set_code_line(lex_arr, i, *IC);/*temp*/
            else if (is_label_in_line(lex_arr, i)) {
                /*Meaningless label*/
                set_label_name(label_arr, *labels_cnt - 1, "-");
            }
            *IC += get_line_cnt(lex_arr, i);
        }
    }
    for (i = 0; i < line_cnt; i++) {
        if (get_line_type(lex_arr, i) == dir) {
            set_code_line(lex_arr, i, get_code_line(lex_arr, i) + *DC + LINE_START);
        }
    }
    return *DC + *IC;
}

/**
* Description:
* This function extracts and processes a label from the input line, updating the lexical tree and label array.
*
* Parameters:
* @line: The input line containing the label.
* @index: The index of the current line.
* @lex_arr: An array of lexical tree nodes where information will be stored.
* @label_arr: An array of label structures to store label information.
* @labels_cnt: A pointer to an integer representing the count of labels.
*
* Returns:
* This function does not return a value.
*/
void get_label(char *line, int index, lex_tree *lex_arr, label *label_arr, int *labels_cnt) {
    int i = 0;
    int len = (int) strlen(line);
    char temp_label[MAX_LABEL_SIZE] = "";

    while (i < len) {
        if (line[i] == ':')
            break;
        i++;
    }
    if (i > 0 && i < len) {
        strncpy(temp_label, line, i);
        delete_pas(line, i + 1);  /* Delete the first word - include ':' */
        set_label(temp_label, index, lex_arr, &label_arr, imm, labels_cnt);
    }
}

/**
* Description:
* This function sets a label in the label array, updating its type and origin line information.
*
* Parameters:
* @label: The label to be set.
* @index: The index of the current line.
* @lex_arr: An array of lexical tree nodes where information will be stored.
* @label_arr: A pointer to an array of label structures.
* @n_type: The type of the label (enum label_type).
* @labels_cnt: A pointer to an integer representing the count of labels.
*
* Returns:
* This function does not return a value.
*/
void
set_label(char label[], int index, lex_tree *lex_arr, asm_label **label_arr, enum label_type n_type, int *labels_cnt) {
    int i = 0;
    int label_len = strlen(label);
    while (i < *labels_cnt) {
        if (strncmp(label, get_label_name(*label_arr, i), label_len) == 0) {
            if ((n_type == ext && get_label_type(*label_arr, i) != ext) ||
                (n_type != ext && get_label_type(*label_arr, i) == ext)) {
                set_error(lex_arr, index, "ERROR: Invalid extern label declaration - in line");
                return;
            } else if (n_type == ext && get_label_type(*label_arr, i) == ext) {
                return;
            } else if (n_type == ent) {
                if (get_label_type(*label_arr, i) == imm) {
                    set_label_type(*label_arr, i, ent);
                } else {
                    set_error(lex_arr, index, "WARNING: This label has already been declared as an entry - in line");
                }
                return;
            } else if (get_label_type(*label_arr, i) == ent && n_type == imm) {
                if (get_label_source(*label_arr, i) == -1) {
                    set_label_source(*label_arr, i, index);
                }
                return;
            }
        }
        i++;
    }

    set_label_name(*label_arr, i, label);
    set_label_type(*label_arr, i, n_type);
    if (n_type == imm) {
        set_label_source(*label_arr, i, index);
    } else
        set_label_source(*label_arr, i, -1);
    (*labels_cnt)++;
}

/*
* Description:
* This function checks if a given line represents an instruction and sets the corresponding information in the lexical tree.
*
* Parameters:
* @line: The input line to be checked.
* @index: The index of the current line.
* @lex_arr: An array of lexical tree nodes where information will be stored.
*
* Returns:
* Returns 1 if the line is an instruction, otherwise returns 0.
*/
int is_inst(char *line, int index, lex_tree *lex_arr) {
    /*First set - Two operands*/
    if (strncmp(line, "mov", INST_LEN) == 0) {
        set_first_set(line, index, lex_arr, mov_inst);
        return 1;
    } else if (strncmp(line, "cmp", INST_LEN) == 0) {
        set_first_set(line, index, lex_arr, cmp_inst);
        return 1;
    } else if (strncmp(line, "add", INST_LEN) == 0) {
        set_first_set(line, index, lex_arr, add_inst);
        return 1;
    } else if (strncmp(line, "sub", INST_LEN) == 0) {
        set_first_set(line, index, lex_arr, sub_inst);
        return 1;
    } else if (strncmp(line, "lea", INST_LEN) == 0) {
        set_first_set(line, index, lex_arr, lea_inst);
        return 1;
    }
        /*Second set - One operand*/
    else if (strncmp(line, "not", INST_LEN) == 0) {
        set_second_set(line, index, lex_arr, not_inst);
        return 1;
    } else if (strncmp(line, "clr", INST_LEN) == 0) {
        set_second_set(line, index, lex_arr, clr_inst);
        return 1;
    } else if (strncmp(line, "inc", INST_LEN) == 0) {
        set_second_set(line, index, lex_arr, inc_inst);
        return 1;
    } else if (strncmp(line, "dec", INST_LEN) == 0) {
        set_second_set(line, index, lex_arr, dec_inst);
        return 1;
    } else if (strncmp(line, "jmp", INST_LEN) == 0) {
        set_second_set(line, index, lex_arr, jmp_inst);
        return 1;
    } else if (strncmp(line, "bne", INST_LEN) == 0) {
        set_second_set(line, index, lex_arr, bne_inst);
        return 1;
    } else if (strncmp(line, "red", INST_LEN) == 0) {
        set_second_set(line, index, lex_arr, red_inst);
        return 1;
    } else if (strncmp(line, "prn", INST_LEN) == 0) {
        set_second_set(line, index, lex_arr, prn_inst);
        return 1;
    } else if (strncmp(line, "jsr", INST_LEN) == 0) {
        set_second_set(line, index, lex_arr, jsr_inst);
        return 1;
    }
        /*Third set - Zero operands*/
    else if (strncmp(line, "rts", INST_LEN) == 0) {
        set_third_set(index, lex_arr, rts_inst);
        return 1;
    } else if (strncmp(line, "stop", INST_LEN + 1) == 0) {
        set_third_set(index, lex_arr, stop_inst);
        return 1;
    } else return 0;
}

/*
* Description:
* This function checks if a given line represents a directive and sets the corresponding information in the lexical tree.
*
* Parameters:
* @line: The input line to be checked.
* @index: The index of the current line.
* @lex_arr: An array of lexical tree nodes where information will be stored.
* @label_arr: An array of label structures.
* @ent_flag: A pointer to a flag indicating the presence of an entry directive.
* @ext_flag: A pointer to a flag indicating the presence of an extern directive.
* @labels_cnt: A pointer to an integer representing the count of labels.
*
* Returns:
* Returns 1 if the line is a directive, otherwise returns 0.
*/
int is_dir(char *line, int index, lex_tree *lex_arr, label *label_arr, int *ent_flag, int *ext_flag, int *labels_cnt) {
    if (strncmp(line, ".entry", ENTRY_LEN) == 0) {
        *ent_flag = 1;
        set_ent(line, index, lex_arr, label_arr, labels_cnt);
        return 1;
    } else if (strncmp(line, ".extern", EXTERN_LEN) == 0) {
        *ext_flag = 1;
        set_ext(line, index, lex_arr, label_arr, labels_cnt);
        return 1;
    } else if (strncmp(line, ".data", DATA_LEN) == 0) {
        set_data(lex_arr, index, line);
        return 1;
    } else if (strncmp(line, ".string", STRING_LEN) == 0) {
        set_string(line, index, lex_arr);
        return 1;
    } else return 0;
}

/*
* Description:
* This function sets an instruction with two operands in the lexical tree.
*
* Parameters:
* @line: The input line containing the instruction.
* @index: The index of the current line.
* @lex_arr: An array of lexical tree nodes where information will be stored.
* @inst_name: The name of the instruction (enum inst_name).
*
* Returns:
* This function does not return a value.
*/
void set_first_set(char *line, int index, lex_tree *lex_arr, enum inst_name inst_name) {
    int j = INST_LEN, temp = 0;
    char first[MAX_LABEL_SIZE], second[MAX_LABEL_SIZE];

    clear_all_whites(line);

    set_inst_type(lex_arr, index, inst_name);

    /*first operand*/
    while (line[j] != ',' && line[j] != '\0') {
        first[temp] = line[j];
        j++;
        temp++;
    }
    first[temp] = '\0';
    if (line[j] == ',') { /* Check if the comma is found before proceeding */
        j++;
        temp = 0;
        /*second operand*/
        while (line[j] != '\0') {
            second[temp] = line[j];
            j++;
            temp++;
        }
        second[temp] = '\0'; /* Null-terminate the second operand string*/
    }
    set_first_set_operands(first, FIRST_OP, index, lex_arr);
    set_first_set_operands(second, SECOND_OP, index, lex_arr);

    if (get_inst_first_set_opernd_type(lex_arr, index, FIRST_OP) == oper_regist
        && get_inst_first_set_opernd_type(lex_arr, index, SECOND_OP) == oper_regist) {
        set_line_cnt(lex_arr, index, 2);
    } else {
        set_line_cnt(lex_arr, index, 3);
    }
}

/*
*
*/
void set_first_set_operands(char operand[], int op_num, int index, lex_tree *lex_arr) {
    int temp;

    temp = is_num(operand);
    /*number*/
    if (temp) {
        set_inst_first_set_num_opernd(lex_arr, index, op_num, temp);
    }
        /*register*/
    else if (operand[0] == '@') {
        set_inst_first_set_register_opernd(lex_arr, index, op_num, operand);
    }
        /*label*/
    else {
        set_inst_first_set_label_opernd(lex_arr, index, op_num, operand);
    }
}

/**
* Description:
* This function sets an instruction with one operand in the lexical tree.
*
* Parameters:
* @line: The input line containing the instruction.
* @index: The index of the current line.
* @lex_arr: An array of lexical tree nodes where information will be stored.
* @inst_name: The name of the instruction (enum inst_name).
*
* Returns:
* This function does not return a value.
*/
void set_second_set(char *line, int index, lex_tree *lex_arr, enum inst_name inst_name) {
    int temp;

    clear_all_whites(line);

    set_inst_type(lex_arr, index, inst_name);

    delete_pas(line, INST_LEN);

    temp = is_num(line);

    if (temp) {
        set_inst_second_set_num_opernd(lex_arr, index, temp);
    }

        /*number*/
    else if (line[0] == '@') {
        set_inst_second_set_register_opernd(lex_arr, index, line);
    }

        /*label*/
    else {
        set_inst_second_set_label_opernd(lex_arr, index, line);
    }
    set_line_cnt(lex_arr, index, 2);
}

/*
* Description:
* This function sets an instruction with no operands in the lexical tree.
*
* Parameters:
* @index: The index of the current line.
* @array: An array of lexical tree nodes where information will be stored.
* @inst_name: The name of the instruction (enum inst_name).
*
* Returns:
* This function does not return a value.
*/
void set_third_set(int index, lex_tree *lex_arr, enum inst_name inst_name) {
    set_inst_type(lex_arr, index, inst_name);
    set_line_cnt(lex_arr, index, 1);
}

/*
* Description:
* This function sets an extern directive in the lexical tree.
*
* Parameters:
* @line: The input line containing the extern directive.
* @index: The index of the current line.
* @lex_arr: An array of lexical tree nodes where information will be stored.
* @label_arr: An array of label structures.
* @labels_cnt: A pointer to an integer representing the count of labels.
*
* Returns:
* This function does not return a value.
*/
void set_ext(char *line, int index, lex_tree *lex_arr, label *label_arr, int *labels_cnt) {
    char *delim;
    char *token;

    clear_all_whites(line);

    set_dir_type(lex_arr, index, dir_ext);

    set_line_cnt(lex_arr, index, 0);

    delete_pas(line, EXTERN_LEN);

    /* Split the string on commas */
    delim = ",";
    token = strtok(line, delim);

    /* Iterate over the tokens and export each label to the set_label function */
    while (token != NULL) {
        set_label(token, index, lex_arr, &label_arr, ext, labels_cnt);
        token = strtok(NULL, delim);
    }
}

/**
* Description:
* This function sets an entry directive in the lexical tree.
*
* Parameters:
* @line: The input line containing the entry directive.
* @index: The index of the current line.
* @lex_arr: An array of lexical tree nodes where information will be stored.
* @label_arr: An array of label structures.
* @labels_cnt: A pointer to an integer representing the count of labels.
*
* Returns:
* This function does not return a value.
*/
void set_ent(char *line, int index, lex_tree *lex_arr, label *label_arr, int *labels_cnt) {
    char *delim;
    char *token;

    clear_all_whites(line);

    set_dir_type(lex_arr, index, dir_ent);

    set_line_cnt(lex_arr, index, 0);

    delete_pas(line, ENTRY_LEN);

    /* Split the string on commas */
    delim = ",";
    token = strtok(line, delim);

    /* Iterate over the tokens and export each label to the set_label function */
    while (token != NULL) {
        set_label(token, index, lex_arr, &label_arr, ent, labels_cnt);
        token = strtok(NULL, delim);
    }
}

/*
* Description:
* This function sets a string directive in the lexical tree.
*
* Parameters:
* @line: The input line containing the string directive.
* @index: The index of the current line.
* @lex_arr: An array of lexical tree nodes where information will be stored.
*
* Returns:
* This function does not return a value.
*/
void set_string(char *line, int index, lex_tree *lex_arr) {
    int len, i;

    delete_pas(line, STRING_LEN);  /* Delete the first word from the line */

    len = strlen(line);
    if (len >= 2 && line[0] == '"' && line[len - 1] == '"') { /* remove quotes */
        for (i = 0; i < len - 1; i++) {
            line[i] = line[i + 1];
        }
        line[len - 2] = '\0';
    }

    len = strlen(line);
    set_line_cnt(lex_arr, index, len + 1);

    set_dir_string(lex_arr, index, line);
}
