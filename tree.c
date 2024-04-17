#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "tree.h"
#include "general.h"
#include "second_pass.h"

#define MAX_ERROR_LEN 150
#define MAX_NUM_SIZE 81
#define REGIST_NAME 2
#define REG_LOC 2
#define DATA_LEN 5

union operand_type_cont {
	int num;
	char label[MAX_LABEL_SIZE + 1];
	int regist;
};

/*--------------- Structures ------------------*/
struct asm_lex_tree {

	enum power error_flag;
	char error_type[MAX_ERROR_LEN];

	enum power label_flag;

	int lines_cnt; /*How many code lines this line gets*/
	int code_line; /*The line number in the code file*/

	enum line_type line_type; /* Instruction, Directive - line types options*/

	union {
		struct { /*Directive line*/
			enum dir_options dir_options; /*Four directive types options*/
			union {
				char string[MAX_STR_LEN];  /*.string*/
				struct { /*.data*/
					int num[MAX_NUM_SIZE + 1];
					int num_cnt;
				} num_array;
			} dir;
		} asm_dir;

		struct { /*Instruction line*/
			enum inst_name inst_name; /*opcode*/
			union {
				struct {
					enum operand_type op_arr[2]; /*source & target*/
					union operand_type_cont op_cont_arr[2];
				} asm_inst_first_set;

				struct {
					enum operand_type op;
					union operand_type_cont op_cont;
				} asm_inst_sec_set;
			} asm_inst_sets;
		} asm_inst;
	} asm_inst_dir;

};

struct asm_label {
	enum label_type type;
	int origin_line;
	char label_name[MAX_LABEL_SIZE];
};

struct asm_ext_label {
	char label_name[MAX_LABEL_SIZE];
	int line;
};

/*--------------- Functions - Tree ------------------*/
lex_tree* create_lex_tree(int size) {
	int i = 0;
	lex_tree* tree = (lex_tree*)malloc(size * sizeof(lex_tree));
	for (i = 0; i < size; i++) {
		/* Initialize the members of each lex_tree structure */
		tree[i].error_flag = OFF;
		strcpy(tree[i].error_type, "");
		tree[i].label_flag = OFF;
		tree[i].lines_cnt = 0;
		tree[i].code_line = 0;
		tree[i].line_type = -1;
	}
	return tree;
}

void set_error(lex_tree* lex_arr, int index, char error[]) {
	lex_arr[index].error_flag = ON;
	strcpy(lex_arr[index].error_type, error);
}

int is_error(lex_tree* lex_arr, int index) {
	return lex_arr[index].error_flag;
}

char* get_error(lex_tree* lex_arr, int index) {
	return lex_arr[index].error_type;
}

void set_label_in_tree(lex_tree* lex_arr, int index, char* line) {
	lex_arr[index].label_flag = ON;
}

int is_label_in_line(lex_tree* lex_arr, int index) {
	return lex_arr[index].label_flag;
}

void set_line_cnt(lex_tree* lex_arr, int index, int val) {
	lex_arr[index].lines_cnt = val;
}

int get_line_cnt(lex_tree* lex_arr, int index) {
	return lex_arr[index].lines_cnt;
}

void set_code_line(lex_tree* lex_arr, int index, int val) {
	lex_arr[index].code_line = val;
}

int get_code_line(lex_tree* lex_arr, int index) {
	return lex_arr[index].code_line;
}

void set_line_type(lex_tree* lex_arr, int index, enum line_type line_type) {
	lex_arr[index].line_type = line_type;
}

int get_line_type(lex_tree* lex_arr, int index) {
	return lex_arr[index].line_type;
}

void set_dir_type(lex_tree* lex_arr, int index, enum dir_options dir_options) {
	lex_arr[index].line_type = dir;
	lex_arr[index].asm_inst_dir.asm_dir.dir_options = dir_options;
}

int get_dir_type(lex_tree* lex_arr, int index) {
	return lex_arr[index].asm_inst_dir.asm_dir.dir_options;
}

void set_inst_type(lex_tree* lex_arr, int index, enum inst_name inst_name) {
	lex_arr[index].line_type = inst;
	lex_arr[index].asm_inst_dir.asm_inst.inst_name = inst_name;
}

int get_inst_type(lex_tree* lex_arr, int index) {
	return lex_arr[index].asm_inst_dir.asm_inst.inst_name;
}

/* First set - setters */
void set_inst_first_set_num_opernd(lex_tree* lex_arr, int index, int op_num, int val) {
	lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_first_set.op_arr[op_num] = oper_num;
	lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_first_set.op_cont_arr[op_num].num = val;
}
void set_inst_first_set_register_opernd(lex_tree* lex_arr, int index, int op_num, char operand[]) {
	lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_first_set.op_arr[op_num] = oper_regist;
	lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_first_set.op_cont_arr[op_num].regist = operand[REG_LOC] - '0';
}
void set_inst_first_set_label_opernd(lex_tree* lex_arr, int index, int op_num, char operand[]) {
	lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_first_set.op_arr[op_num] = oper_label;
	strcpy(lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_first_set.op_cont_arr[op_num].label, operand);
}

/* First set - getters */
int get_inst_first_set_opernd_type(lex_tree* lex_arr, int index, int op_num) {
	return lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_first_set.op_arr[op_num];
}
int get_inst_first_set_opernd_num(lex_tree* lex_arr, int index, int op_num) {
	return lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_first_set.op_cont_arr[op_num].num;
}
int get_inst_first_set_opernd_register(lex_tree* lex_arr, int index, int op_num) {
	return lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_first_set.op_cont_arr[op_num].regist;
}
char* get_inst_first_set_opernd_label(lex_tree* lex_arr, int index, int op_num) {
	return lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_first_set.op_cont_arr[op_num].label;
}

/* Second set - setters */
void set_inst_second_set_num_opernd(lex_tree* lex_arr, int index, int val) {
	lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_sec_set.op = oper_num;
	lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_sec_set.op_cont.num = val;
}
void set_inst_second_set_register_opernd(lex_tree* lex_arr, int index, char operand[]) {
	lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_sec_set.op = oper_regist;
	lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_sec_set.op_cont.regist = operand[REG_LOC] - '0';
}
void set_inst_second_set_label_opernd(lex_tree* lex_arr, int index, char operand[]) {
	lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_sec_set.op = oper_label;
	strcpy(lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_sec_set.op_cont.label, operand);
}

/* Second set - getters */
int get_inst_second_set_opernd_type(lex_tree* lex_arr, int index) {
	return lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_sec_set.op;
}
int get_inst_second_set_opernd_num(lex_tree* lex_arr, int index) {
	return lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_sec_set.op_cont.num;
}
int get_inst_second_set_opernd_register(lex_tree* lex_arr, int index) {
	return lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_sec_set.op_cont.regist;
}
char* get_inst_second_set_opernd_label(lex_tree* lex_arr, int index) {
	return lex_arr[index].asm_inst_dir.asm_inst.asm_inst_sets.asm_inst_sec_set.op_cont.label;
}

void set_data(lex_tree* lex_arr, int index, char* line) {
	char* token;
	int num_cnt = 0, temp;

	clear_all_whites(line);

	lex_arr[index].line_type = dir;
	lex_arr[index].asm_inst_dir.asm_dir.dir_options = dir_data;

	delete_pas(line, DATA_LEN);  /* Delete the first word from the line */

	token = strtok(line, ",");

	while (token != NULL && num_cnt < MAX_NUM_SIZE) {
		temp = atoi(token);
		lex_arr[index].asm_inst_dir.asm_dir.dir.num_array.num[num_cnt] = temp;
		num_cnt++;
		token = strtok(NULL, ",");
	}
	lex_arr[index].asm_inst_dir.asm_dir.dir.num_array.num_cnt = num_cnt;
	lex_arr[index].lines_cnt = num_cnt;
}

void set_data_code(char machine_code[][2], int i, lex_tree* lex_arr, int* code_line) {
	int j, temp;
	for (j = 0; j < lex_arr[i].lines_cnt; j++) {
		temp = lex_arr[i].asm_inst_dir.asm_dir.dir.num_array.num[j];
		set_base64(machine_code, temp, code_line);
	}
}

void set_dir_string(lex_tree* lex_arr, int index, char* string) {
	lex_arr[index].line_type = dir;
	lex_arr[index].asm_inst_dir.asm_dir.dir_options = dir_string;
	strcpy(lex_arr[index].asm_inst_dir.asm_dir.dir.string, string);
}

char* get_dir_string(lex_tree* lex_arr, int index) {
	return lex_arr[index].asm_inst_dir.asm_dir.dir.string;
}


/*--------------- Functions - Labels ------------------*/
label* create_label_array(int size) {
	int i = 0;
	label* array = (label*)malloc(size * sizeof(*array));
	if (array != NULL) {
		for (i = 0; i < size; i++) {
			/* Initialize label type as LABEL_TYPE_UNKNOWN */
			array[i].type = -1;

			/* Initialize origin line to -1 */
			array[i].origin_line = -1;

			/* Initialize label name as an empty string */
			memset(array[i].label_name, 0, sizeof(array[i].label_name));
		}
	}
	return array;
}

label* resize_label_array(label* label_arr, int size) {
	label* array = (label*)realloc(label_arr, size * sizeof(*array));
	return array;
}

void set_label_type(label* label_arr, int index, enum label_type type) {
	label_arr[index].type = type;
}
int get_label_type(label* label_arr, int index) {
	return label_arr[index].type;
}

void set_label_name(label* label_arr, int index, char label[]) {
	strcpy(label_arr[index].label_name, label);
}
char* get_label_name(label* label_arr, int index) {
	return label_arr[index].label_name;
}

void set_label_source(label* label_arr, int index, int line_num) {
	label_arr[index].origin_line = line_num;
}
int get_label_source(label* label_arr, int index) {
	return label_arr[index].origin_line;
}


ext_label* create_ext_label_array(int size) {
	int i = 0;
	ext_label* array = (ext_label*)malloc(size * sizeof(*array));
	if (array != NULL) {
		for (i = 0; i < size; i++) {
			/* Initialize origin line to -1 */
			array[i].line = -1;

			/* Initialize label name as an empty string */
			memset(array[i].label_name, 0, sizeof(array[i].label_name));
		}
	}
	return array;
}

ext_label* resize_ext_label_array(ext_label* ext_arr, int size) {
	ext_label* new_array = (ext_label*)realloc(ext_arr, size * sizeof(*new_array));
	return new_array;
}

void set_ext_label_name(ext_label* ext_arr, int index, char label[]) {
	strcpy(ext_arr[index].label_name, label);
}
char* get_ext_label_name(ext_label* ext_arr, int index) {
	return ext_arr[index].label_name;
}

void set_ext_label_source(ext_label* ext_arr, int index, int line_num) {
	ext_arr[index].line = line_num;
}
int get_ext_label_source(ext_label* ext_arr, int index) {
	return ext_arr[index].line;
}

