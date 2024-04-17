#ifndef LEX_TREE_H
#define LEX_TREE_H

/*--------- Constants ---------*/
#define FIRST_OP 0
#define SECOND_OP 1

#define MAX_LABEL_SIZE 81
#define MAX_STR_LEN 32

/*--------- Enumerations ---------*/
enum operand_type { oper_num, oper_label, oper_regist };

enum inst_name {
	/*First set - Two operands*/
	mov_inst, cmp_inst, add_inst, sub_inst, lea_inst=6,
	/*Second set - One operand*/
	not_inst=4, clr_inst=5, inc_inst=7, dec_inst, jmp_inst, bne_inst, red_inst, prn_inst, jsr_inst,
	/*Third set - Zero operands*/
	rts_inst, stop_inst
};

enum dir_options { dir_data, dir_string, dir_ext, dir_ent };

enum label_type { ext, ent, imm };

enum line_type { inst, dir };

enum power { OFF, ON };

/*---------------Structure ------------------*/
/* structure to store information about assembly code lines */
typedef struct asm_lex_tree lex_tree;

/* structure to store information about labels in the assembly code lines */
typedef struct asm_label label; 
typedef struct asm_label asm_label;

/* structure to store information about extern labels in the assembly code lines */
typedef struct asm_ext_label ext_label;

/*--------------- Functions - Tree ------------------*/
lex_tree* create_lex_tree(int size);

void set_error(lex_tree* lex_arr, int index, char error[]);
int is_error(lex_tree* lex_arr, int index);
char* get_error(lex_tree* lex_arr, int index);

void set_label_in_tree(lex_tree* lex_arr, int index, char* line);
int is_label_in_line(lex_tree* lex_arr, int index);

void set_line_cnt(lex_tree* lex_arr, int index, int val);
int get_line_cnt(lex_tree* lex_arr, int index);

void set_code_line(lex_tree* lex_arr, int index, int val);
int get_code_line(lex_tree* lex_arr, int index);

void set_line_type(lex_tree* lex_arr, int index, enum line_type line_type);
int get_line_type(lex_tree* lex_arr, int index);

void set_dir_type(lex_tree* lex_arr, int index, enum dir_options dir_options);
int get_dir_type(lex_tree* lex_arr, int index);

void set_inst_type(lex_tree* lex_arr, int index, enum inst_name inst_name);
int get_inst_type(lex_tree* lex_arr, int index);

/* First set - setters */
void set_inst_first_set_num_opernd(lex_tree* lex_arr, int index, int op_num, int val);
void set_inst_first_set_register_opernd(lex_tree* lex_arr, int index, int op_num, char operand[]);
void set_inst_first_set_label_opernd(lex_tree* lex_arr, int index, int op_num, char operand[]);

/* First set - getters */
int get_inst_first_set_opernd_type(lex_tree* lex_arr, int index, int op_num);
int get_inst_first_set_opernd_num(lex_tree* lex_arr, int index, int op_num);
int get_inst_first_set_opernd_register(lex_tree* lex_arr, int index, int op_num);
char* get_inst_first_set_opernd_label(lex_tree* lex_arr, int index, int op_num);

/* Second set - setters */
void set_inst_second_set_num_opernd(lex_tree* lex_arr, int index, int val);
void set_inst_second_set_register_opernd(lex_tree* lex_arr, int index, char operand[]);
void set_inst_second_set_label_opernd(lex_tree* lex_arr, int index, char operand[]);

/* Second set - getters */
int get_inst_second_set_opernd_type(lex_tree* lex_arr, int index);
int get_inst_second_set_opernd_num(lex_tree* lex_arr, int index);
int get_inst_second_set_opernd_register(lex_tree* lex_arr, int index);
char* get_inst_second_set_opernd_label(lex_tree* lex_arr, int index);

void set_data(lex_tree* lex_arr, int index, char* line);
void set_data_code(char machine_code[][2], int i, lex_tree* lex_arr, int* code_line);

void set_dir_string(lex_tree* lex_arr, int index, char* string);
char* get_dir_string(lex_tree* lex_arr, int index);



/*--------------- Functions - Labels ------------------*/
/*label*/
label* create_label_array(int size);
label* resize_label_array(label* label_arr, int size);

void set_label_type(label* label_arr, int index, enum label_type type);
int get_label_type(label* label_arr, int index);

void set_label_name(label* label_arr, int index, char label[]);
char* get_label_name(label* label_arr, int index);

void set_label_source(label* label_arr, int index, int line_num);
int get_label_source(label* label_arr, int index);

/*extern label*/
ext_label* create_ext_label_array(int size);
ext_label* resize_ext_label_array(ext_label* ext_arr, int size);

void set_ext_label_name(ext_label* ext_arr, int index, char label[]);
char* get_ext_label_name(ext_label* ext_arr, int index);

void set_ext_label_source(ext_label* ext_arr, int index, int line_num);
int get_ext_label_source(ext_label* ext_arr, int index);

#endif
