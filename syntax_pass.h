#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE_OF_LINE 1000


/*The main function, receiving a file and checks each line separately.
 * returns "ON" if the file is syntactically correct AND "OFF" otherwise. */
int syntax_pass(char* source_file_name);

/*----------------------------------First Word checking functions------------------------------------*/

/*Checks if the specific line has a proper label define.
 * returns "ON" if the line has a proper label define AND "OFF" otherwise.*/
int is_label_line(char* current_line);

/*Checks if the specific line has a proper extern define.
 * returns "ON" if the line has a proper extern define AND "OFF" otherwise.*/
int is_extern_line(char* current_line, int label_flag);

/*Checks if the specific line has a proper entry define.
 * returns "ON" if the line has a proper entry define AND "OFF" otherwise.*/
int is_entry_line(char* current_line, int label_flag);

/*----------------------------------operation checking------------------------------------*/

/*Checks if the specific line is an instructive line.
 * returns the instruction type if the line has a proper instruction define AND "OFF" otherwise.*/
int is_instructive_line(char* current_line, int label_flag);

/*Checks if the specific line is an instructive line.
 * returns direction type if the line has a proper direction define AND "OFF" otherwise.*/
int is_directive_line(char* current_line, int label_flag);

/*----------------------------------operands OR data/string checking------------------------------------*/

/*Checks the first operand, by using the instruction type we identify in instruction analyse.
 * returns "ON" if the operand is defined properly AND "OFF" otherwise.*/
int first_operand_checker(int ins_type, char* current_line, int label_flag);

/*Checks the second operand, by using the instruction type we identify in instruction analyse.
 * returns "ON" if the operand is defined properly AND "OFF" otherwise.*/
int second_operand_checker(int ins_type, char* current_line, int label_flag);

/*Checks the operand of a directive DATA instruction.
 * returns "ON" if the operand is defined properly AND "OFF" otherwise.*/
int data_value_checker(char* current_line, int label_flag);

/*Checks the operand of a directive STRING instruction.
 * returns "ON" if the operand is defined properly AND "OFF" otherwise.*/
int string_value_checker(char* current_line, int label_flag);

/*Checks the operand of a directive EXT/ENT instruction.
 * returns "ON" if the operand is defined properly AND "OFF" otherwise.*/
int extern_entry_operand_checker(char* current_line, int label_flag);



/*----------------------------------instruction type------------------------------------*/
/*Identifies if the instruction is a two operands type.
 * returns "ON" if the instruction is a two operands type and "OFF" otherwise. */
int two_operand_instruction(int ins_type);

/*Identifies if the instruction is a one operand type.
 * returns "ON" if the instruction is a one operands type and "OFF" otherwise. */
int one_operand_instruction(int ins_type);

/*----------------------------------sub operands checking------------------------------------*/

/*Checks the exact operand by a given instruction type.
 * return the operand if it valid and "OFF" otherwise. */
int groupOne_operand_checker(char* portion, int second_operand_flag);

/*Checks the exact operand by a given instruction type.
 * return the operand if it valid and "OFF" otherwise. */
int groupTwo_operand_checker(char* portion, int second_operand_flag);

/*Checks the exact operand by a given instruction type.
 * return the operand if it valid and "OFF" otherwise. */
int groupThree_operand_checker(char* portion, int second_operand_flag);

/*Checks the exact operand by a given instruction type.
 * return the operand if it valid and "OFF" otherwise. */
int groupFour_operand_checker(char* portion, int second_operand_flag);

/*Checks the exact operand by a given instruction type.
 * return the operand if it valid and "OFF" otherwise. */
int groupFive_operand_checker(char* portion, int second_operand_flag);

/*----------------------------------identifies and check each operand------------------------------------*/

/*By a given string, it checks whether it match a known operand.
 * returns the operand it it matched, and "OFF" otherwise. */
int is_operand_is_label(char* portion);

/*By a given string, it checks whether it match a known operand.
 * returns the operand it it matched, and "OFF" otherwise. */
int is_operand_is_register(char* portion);

/*By a given string, it checks whether it match a known operand.
 * returns the operand it it matched, and "OFF" otherwise. */
int is_operand_is_number(char* portion);

/*----------------------------------Utils------------------------------------*/

/*Adding a space between the end of the label and the rest of the line, only
 * if the line is not a directive line. */
void adding_space_to_label_ins_line(char* current_line);

/*Checks whether the current line in above a correct length limit.
 * Return ON if the line exceeded and OFF otherwise.*/
int is_line_exceed_length_limit(char* current_line, int line_number);

