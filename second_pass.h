#pragma once

#include "tree.h"

#define MAX_LINE_SIZE 256
#define MAX_COMMAND_LENGTH 80
#define MAX_SIZE 81
#define MAX_MACHINE_CODE 924
#define MAX_INPUT 1000


/*--------------- Function Prototypes ------------------*/
/*
 * Perform the second pass of the assembly process.
 *
 * This function performs the second pass of the assembly process,
 * where it processes the code lines generated from the assembly source file.
 * It reads the intermediate data structures,
 * validates labels and generates machine code and output files.
 *
 * @param file_name The name of the assembly source file without the extension.
 * This is used as a base for generating output files.
 *
 * @return None.
 */
void second_pass(char* file_name);

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
int set_line_tree(char** lines, int line_cnt, int* ent_flag, int* ext_flag, lex_tree* lex_arr, label* label_arr, int* labels_cnt, int* DC, int* IC);

/*
* Description:
* This function iterates through the lexical analysis array,
* converts the data of instruction and directive lines to machine code,
* and populates the machine_code array.
* It delegates the conversion tasks to other functions based on the type of line.
*
* Parameters:
* @machine_code: A two-dimensional character array to store the generated machine code.
* @lex_size: The size of the lexical analysis array.
* @lex_arr: An array of lexical tree nodes containing the parsed program.
* @label_arr: An array of label structures containing label information.
* @ext_arr: An array of external label structures.
* @ext_cnt: A pointer to an integer representing the count of external labels.
* @labels_size: The size of the label_arr array.
*
* Returns:
* Returns 1 if the code initialized correctly, 0 if there was an error.
*/
int set_line_code(char machine_code[][2], int lex_size, lex_tree* lex_arr, label* label_arr, ext_label* ext_arr, int* ext_cnt, int labels_size);

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
void set_base64(char machine_code[][2], int val, int* code_line);


/*
* Description:
* This function creates an object file (.obj) containing the machine code generated from the assembly program.
* The machine code is provided as a two-dimensional array of characters.
* Each row in the array represents a machine code instruction, with two characters per instruction.
*
* Parameters:
* @filename: The base filename to be used for the object file.
* @machine_code: A two-dimensional array of characters containing the machine code.
* @size: The number of instructions in the machine code array.
*
* Returns:
* This function does not return a value.
*/
void create_obj_file(const char* filename, char machine_code[][2], int size, int DC, int IC);

/*
* Description:
* This function creates an entry file (.ent) containing the labels declared as entry points in the assembly program.
* It takes the lexical tree, label array, and the size of the label array as input.
* For each label of type ent, it outputs the label name and the corresponding code line from the lexical tree where the label is defined.
*
* Parameters:
* @filename: The base filename to be used for the entry file.
* @lex_arr: An array of lexical tree nodes containing information about the code lines.
* @label_arr: An array of label structures containing label information.
* @size: The number of elements in the label array.
*
* Returns:
* This function does not return a value.
*/
void create_ent_file(const char* filename, lex_tree* lex_arr, label* label_arr, int size);

/*
* Description:
* This function creates an external file (.ext) containing information about external labels used in the assembly program.
* It takes an array of external label structures and the size of the array as input.
* For each external label, it outputs the label name and the corresponding line number where the label is used.
*
* Parameters:
* @filename: The base filename to be used for the external file.
* @ext_arr: An array of external label structures containing information about external labels.
* @size: The number of elements in the external label array.
*
* Returns:
* This function does not return a value.
*/
void create_ext_file(const char* filename, ext_label* ext_arr, int size);

