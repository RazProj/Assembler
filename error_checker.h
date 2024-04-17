#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE_OF_LINE 1000

/*----------------------------------operands error identifier------------------------------------*/

/*Identifies the error in the first operand, and print a warning. */
int error_in_first_operand(char* current_line, int line_number, int ins_type, int label_flag);

/*Identifies the error in the second operand, and print a warning. */
int error_in_second_operand(char* current_line, int line_number, int ins_type, int label_flag);

/*Identifies the error in the direction line data, and prints a warning. */
int error_in_data_str_value(char* current_line, int line_number, int dir_type, int label_flag);

/*Identifies the error in the Ext\Ent operand, and print a warning. */
int error_in_extern_entry_operand(char* current_line, int line_number, int label_flag);

/*----------------------------------general error------------------------------------*/

/*Identifies the error, and print a warning. */
int error_in_the_ins_dir_word(char* current_line, int line_number, int label_flag);

/*The first word is syntactically not correct, print a warning. */
int error_in_the_first_word(char* current_line, int line_number);

/*----------------------------------others inside error checking------------------------------------*/

/* Checks whether a first operand has a comma in the beginning of the operand.
 * The reason for the method is to enable analysing each operand individually,and because there suppose to be
 * a comma between the first and second operands, in the main analysing it not catching this specific error.
 * Return "ON" if there is a comma,and "OFF" otherwise. */
int error_comma_first_letter_in_the_first_operand(char* current_line, int label_flag);

int error_comma_last_letter_in_the_last_operand(char* current_line);

/*----------------------------------rest of the row error------------------------------------*/

/*Checks for any error in the rest of the row,after analysing the line and
 * receive that the instruction\direction and the operands are correct. */


int error_in_the_rest_of_the_row_of_instruction(char* current_line, int line_number, int ins_type, int label_flag);

int error_in_the_rest_of_the_row_of_directive(char* current_line, int line_number, int dir_type, int label_flag);

/*----------------------------------identifies each instruction------------------------------------*/

/*Identifies each error by the instruction type. */
int groupOne_error_checker(char* portion, int second_operand_flag);

int groupThree_error_checker(char* portion, int second_operand_flag);

int groupFour_error_checker(char* portion, int second_operand_flag);

/*----------------------------------identifies each error in directive------------------------------------*/

int data_error_identifier(char* current_line, int line_number, int label_flag);

int string_error_identifier(char* current_line, int line_number, int label_flag);


/*----------------------------------rest of the row specific instruction type------------------------------------*/

/*Analysing the exact error by the instruction type. */

int error_in_the_rest_of_the_row_of_two_operand_instruction(char* current_line, int line_number, int label_flag);

int error_in_the_rest_of_the_row_of_one_operand_instruction(char* current_line, int line_number, int label_flag);

int error_in_the_rest_of_the_row_of_non_operand_instruction(char* current_line, int line_number, int label_flag);

