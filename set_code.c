#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/*--------- Headers ---------*/
#include "second_pass.h"
#include "tree.h"
#include "general.h"

/*--------- Constants ---------*/
#define DEST_BIT 4
#define INST_BIT 32
#define SRC_BIT 512

#define DEST_REG_BIT 128
#define TARGET_REG_BIT 4
#define OP_BIT 4

#define BASE 64
#define MAX_OP_VAL 1024
#define MAX_VAL 4096

/*--------- Enumerations ---------*/
enum ARE { A, R, E };
enum address { immediate = 1, direct = 3, regist = 5 };

/*--------------- Function Prototypes ------------------*/
int set_inst_code(char machine_code[][2], int i, lex_tree* lex_arr, label* label_arr, ext_label* ext_arr, int* code_line, int* ext_cnt, int labels_size);
void set_string_code(char machine_code[][2], int i, lex_tree* lex_arr, int* code_line);
void set_data_code(char machine_code[][2], int i, lex_tree* lex_arr, int* code_line);
int label_memmory(char label_name[], lex_tree* lex_arr, label* label_arr, int labels_size);
void set_ext_call(char label_name[], int index, int code_line, ext_label** ext_arr, int* ext_cnt, lex_tree* lex_arr);


int set_line_code(char machine_code[][2], int lex_size, lex_tree* lex_arr, label* label_arr, ext_label* ext_arr, int* ext_cnt, int labels_size) {
	int i;
	int code_lines = 0;

	/* intruction first = DC */
	for (i = 0; i < lex_size; i++) {
		if (get_line_type(lex_arr, i) == inst) {
			if (!set_inst_code(machine_code, i, lex_arr, label_arr, ext_arr, &code_lines, ext_cnt, labels_size))
				return 0;
		}
	}

	/* directive = IC */
	for (i = 0; i < lex_size; i++) {
		if (get_line_type(lex_arr, i) == dir) {
			if (get_dir_type(lex_arr, i) == dir_string) {
				set_string_code(machine_code, i, lex_arr, &code_lines);
			}
			if (get_dir_type(lex_arr, i) == dir_data) {
				set_data_code(machine_code, i, lex_arr, &code_lines);
			}
		}
	}
	return 1;
}

/*
* Description:
* This function converts the data of an instruction line into a line of machine code,
* updating the machine_code array with the appropriate values.
*
* Parameters:
* @machine_code: A two-dimensional character array to store the generated machine code.
* @i: The index of the current line in the lex_arr array.
* @lex_arr: An array of lexical tree nodes containing the parsed program.
* @label_arr: An array of label structures containing label information.
* @ext_arr: An array of external label structures.
* @code_line: A pointer to an integer representing the current line in the machine_code array.
* @ext_cnt: A pointer to an integer representing the count of external labels.
* @labels_size: The size of the label_arr array.
*
* Returns:
* This function does not return a value.
*/
int set_inst_code(char machine_code[][2], int i, lex_tree* lex_arr, label* label_arr, ext_label* ext_arr, int* code_line, int* ext_cnt, int labels_size) {
	int temp1 = 0, temp2 = 0, temp3 = 0, temp_val;
	temp1 += A;
	temp1 += (get_inst_type(lex_arr, i) * INST_BIT);

	/* First set */
	if (get_inst_type(lex_arr, i) <= sub_inst || get_inst_type(lex_arr, i) == lea_inst) {
		/* First Operand */
		if (get_inst_first_set_opernd_type(lex_arr, i, FIRST_OP) == oper_num) {
			temp1 += (immediate * SRC_BIT);
			temp_val = get_inst_first_set_opernd_num(lex_arr, i, FIRST_OP);
			if (temp_val < 0)
				temp_val = MAX_OP_VAL + temp_val;
			temp2 += (temp_val * OP_BIT);
		}
		else if (get_inst_first_set_opernd_type(lex_arr, i, FIRST_OP) == oper_label) {
			temp1 += (direct * SRC_BIT);
			temp2 = label_memmory(get_inst_first_set_opernd_label(lex_arr, i, FIRST_OP), lex_arr, label_arr, labels_size);
			if (temp2 == R)
				set_ext_call(get_inst_first_set_opernd_label(lex_arr, i, FIRST_OP), i, 1, &ext_arr, ext_cnt, lex_arr);
			if (temp2 < 0)
				return 0;
		}
		else if (get_inst_first_set_opernd_type(lex_arr, i, FIRST_OP) == oper_regist) {
			temp1 += (regist * SRC_BIT);
			temp2 += (get_inst_first_set_opernd_register(lex_arr, i, FIRST_OP) * DEST_REG_BIT);
		}
		/* Second Operand */
		if (get_inst_first_set_opernd_type(lex_arr, i, SECOND_OP) == oper_num) {
			temp1 += (immediate * DEST_BIT);
			temp_val = get_inst_first_set_opernd_num(lex_arr, i, SECOND_OP);
			if (temp_val < 0)
				temp_val = MAX_OP_VAL + temp_val;
			temp3 += (temp_val * OP_BIT);
		}
		else if (get_inst_first_set_opernd_type(lex_arr, i, SECOND_OP) == oper_label) {
			temp1 += (direct * DEST_BIT);
			temp3 = label_memmory(get_inst_first_set_opernd_label(lex_arr, i, SECOND_OP), lex_arr, label_arr, labels_size);
			if (temp3 == R)
				set_ext_call(get_inst_first_set_opernd_label(lex_arr, i, SECOND_OP), i, 2, &ext_arr, ext_cnt, lex_arr);
			if (temp3 < 0)
				return 0;
		}
		else if (get_inst_first_set_opernd_type(lex_arr, i, SECOND_OP) == oper_regist) {
			temp1 += (regist * DEST_BIT);
			if (get_inst_first_set_opernd_type(lex_arr, i, FIRST_OP) == oper_regist)
				temp2 += (get_inst_first_set_opernd_register(lex_arr, i, SECOND_OP) * TARGET_REG_BIT);
			else
				temp3 += (get_inst_first_set_opernd_register(lex_arr, i,SECOND_OP) * TARGET_REG_BIT);
		}
	}
	/* Second set */
	else if (get_inst_type(lex_arr, i) <= jsr_inst) {
		if (get_inst_second_set_opernd_type(lex_arr, i) == oper_num) {
			temp1 += (immediate * DEST_BIT);
			temp_val = get_inst_second_set_opernd_num(lex_arr, i);
			if (temp_val < 0)
				temp_val = MAX_OP_VAL + temp_val;
			temp2 += (temp_val * OP_BIT);
		}
		else if (get_inst_second_set_opernd_type(lex_arr, i) == oper_label) {
			temp1 += (direct * DEST_BIT);
			temp2 = label_memmory(get_inst_second_set_opernd_label(lex_arr, i), lex_arr, label_arr, labels_size);
			if (temp2 == R)
				set_ext_call(get_inst_second_set_opernd_label(lex_arr, i), i, 1, &ext_arr, ext_cnt, lex_arr);
			if (temp2 < 0)
				return 0;
		}
		else if (get_inst_second_set_opernd_type(lex_arr, i) == oper_regist) {
			temp1 += (regist * DEST_BIT);
			temp2 += (get_inst_second_set_opernd_register(lex_arr, i) * TARGET_REG_BIT);
		}
	}
	set_base64(machine_code, temp1, code_line);
	if (get_line_cnt(lex_arr, i)>1) {
		set_base64(machine_code, temp2, code_line);
		if (get_line_cnt(lex_arr, i) == 3)
			set_base64(machine_code, temp3, code_line);
	}
	return 1;
}

/*
* Description:
* This function converts the data of a string directive line into a line of machine code,
* populating the machine_code array with encoded characters.
*
* Parameters:
* @machine_code: A two-dimensional character array to store the generated machine code.
* @i: The index of the current line in the lex_arr array.
* @lex_arr: An array of lexical tree nodes containing the parsed program.
* @code_line: A pointer to an integer representing the current line in the machine_code array.
*
* Returns:
* This function does not return a value.
*/
void set_string_code(char machine_code[][2], int i, lex_tree* lex_arr, int* code_line) {
	int j;
	char temp;
	char string[MAX_STR_LEN] = "";
	strcpy(string, get_dir_string(lex_arr, i));

	for (j = 0; j < get_line_cnt(lex_arr, i); j++) {
		temp = string[j];
		set_base64(machine_code, temp, code_line);
	}
}

/*
* Description:
* This function converts a decimal value into a base64 character representation and updates the machine_code array.
*
* Parameters:
* @machine_code: A two-dimensional character array to store the generated machine code.
* @val: The decimal value to be converted.
* @code_line: A pointer to an integer representing the current line in the machine_code array.
*
* Returns:
* This function does not return a value.
*/
void set_base64(char machine_code[][2], int val, int* code_line) {
	const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int temp;
	char letter;

	if (val < 0)
		val = MAX_VAL + val;

	if (val > MAX_VAL)
		fprintf(stderr, "WARNING: A value greater than the maximum value in the program.\n");

	temp = val % BASE;
	letter = base64_table[temp];
	machine_code[*code_line][SECOND_OP] = letter;

	temp = ((val / BASE) % BASE);
	letter = base64_table[temp];
	machine_code[*code_line][FIRST_OP] = letter;

	(*code_line)++;
}

/*
* Description:
* This function checks whether a label is external or internal and returns an appropriate value that represents the label's location in memory.
*
* Parameters:
* @label_name: The name of the label to be checked.
* @lex_arr: An array of lexical tree nodes containing the parsed program.
* @label_arr: An array of label structures containing label information.
* @labels_size: The size of the label_arr array.
*
* Returns:
* If the label is external, it returns R.
* If the label is internal, it returns the memory location of the label + E.
*/
int label_memmory(char label_name[], lex_tree* lex_arr, label* label_arr, int labels_size) {
	int i, temp = -1;
	for (i = 0; i < labels_size; i++) {
		if (strcmp(label_name, get_label_name(label_arr, i)) == 0) {
			if (get_label_type(label_arr, i) == ext) {
				return R;
			}
			else if (get_label_source(label_arr, i) == -1)
				break;
			else {
				temp = get_code_line(lex_arr, get_label_source(label_arr, i));
				return (temp * OP_BIT + E);
			}
			break;
		}
	}
	fprintf(stderr, "ERROR: '%s' Label is called but not defined or declared.\n", label_name);
	return temp;
}

/*
* Description:
* This function adds an external label call to the ext_arr array and increments the ext_cnt variable.
*
* Parameters:
* @label_name: The name of the external label to be added.
* @index: The index of the line containing the external label call.
* @ext_arr: A pointer to an array of external label structures.
* @ext_cnt: A pointer to an integer representing the count of external labels.
* @lex_arr: An array of lexical tree nodes containing the parsed program.
*
* Returns:
* This function does not return a value.
*/
void set_ext_call(char label_name[], int index, int code_line, ext_label** ext_arr, int* ext_cnt, lex_tree* lex_arr) {
	set_ext_label_name(*ext_arr, *ext_cnt, label_name);
	set_ext_label_source(*ext_arr, *ext_cnt, get_code_line(lex_arr, index) + code_line);
	(*ext_cnt)++;
}
