#pragma once

#include "tree.h"

/*---------------Function Prototypes------------------*/
/**
 * Processes filenames following the word "assembler" in the input string.
 *
 * This function processes individual words that appear after the word "assembler"
 * in the input string. Each word is extracted, pre-processed, and passed through
 * syntax and second passes for further processing. White spaces are ignored.
 *
 * @param input The input string containing filenames and "assembler" keyword.
 */
void process_filenames(char* input);
/*---------------------------------*/
/**
* Description:
* This function reads the lines from a file with a given filename and a specified suffix appended to it,
* and stores each line as a separate string in a dynamically allocated array.
* It also counts the number of lines in the file and returns it through the line_count parameter.
* If any errors occur during the process, it sets the error_flag to indicate the error and returns NULL.
*
* Parameters:
* @filename: A pointer to a null-terminated string representing the base filename.
* @suffix: A character array containing the suffix to be appended to the filename.
* @line_count: A pointer to an integer where the number of lines in the file will be stored.
* @error_flag: A pointer to an integer that will be set to 1 in case of an error.
*
* Returns:
* A dynamically allocated array of pointers to characters (strings), where each element of the array represents a line from the file.
* The last element of the array is set to NULL for termination.
*/
char** read_file_lines(const char* filename, char suffix[], int* line_count, int* error_flag);
/*---------------------------------*/
/**
* Description:
* This function checks whether a given line is either empty or a comment line (starts with a semicolon ';').
*
* Parameters:
* @line: A pointer to a null-terminated string representing the line to be checked.
*
* Returns:
* 1 if the line is empty or a comment line, 0 otherwise.
*/
int is_empty_comment_line(const char line[]);
/*---------------------------------*/
/**
* Description:
* This function counts the number of lines in a file with a given filename and sets an error_flag in case the file cannot be opened.
*
* Parameters:
* @filename: A pointer to a null-terminated string representing the filename.
* @error_flag: A pointer to an integer that will be set to 1 in case of an error.
*
* Returns:
* The number of lines in the file.
*/
int line_cnt(const char filename[], int* error_flag);
/*---------------------------------*/
/**
* Description:
* This function frees the memory allocated for an array of strings and the array itself.
*
* Parameters:
* @lines: An array of pointers to characters (strings) to be freed.
* @line_count: The number of elements in the array.
*
* Returns:
* This function does not return any value.
*/
void free_lines(char** lines, int line_count);
/*---------------------------------*/
/**
* Description:
* This function checks if all entry labels are valid (i.e., all entry labels have corresponding definitions).
*
* Parameters:
* @label_arr: An array of label structures representing labels and their information.
* @labels_cnt: The number of labels in the array.
*
* Returns:
* 1 if all entry labels are valid, 0 otherwise.
*/
int is_valid_ent_label(label* label_arr, int labels_cnt);
/*---------------------------------*/
/**
* Description:
* This function checks if all labels in the lexical analysis tree are valid (i.e., have no errors).
*
* Parameters:
* @lex_arr: An array of lexical tree nodes representing lexemes and their information.
* @lex_size: The number of elements in the array.
*
* Returns:
* 1 if all labels are valid, 0 otherwise.
*/
int is_valid_labels_dec(lex_tree* lex_arr, int lex_size);
/*---------------------------------*/
/**
* Description:
* This function removes all whitespace characters (spaces, tabs, newlines) from a given string.
*
* Parameters:
* @line: A pointer to a null-terminated string to remove whitespace from.
*
* Returns:
* This function does not return any value.
*/
void clear_all_whites(char* line);
/*---------------------------------*/
/**
* Description:
* This function deletes the first i characters from a given string, shifting the remaining characters to the left.
*
* Parameters:
* @line: A pointer to a null-terminated string to modify.
* @i: The number of characters to delete.
*
* Returns:
* This function does not return any value.
*/
void delete_pas(char* line, int i);
/*---------------------------------*/
/**
* Description:
* This function duplicates a given string by dynamically allocating memory for it.
*
* Parameters:
* @str: A pointer to a null-terminated string to duplicate.
*
* Returns:
* A pointer to a newly allocated memory containing the duplicated string.
*/
char* my_strdup(const char* str);
/*---------------------------------*/
/**
* Description:
* This function checks if a given string represents a valid integer number.
*
* Parameters:
* @line: A pointer to a null-terminated string to check.
*
* Returns:
* The parsed integer value if the string represents a valid number, 0 otherwise.
*/
int is_num(char* line);
/*---------------------------------*/
/**
* Description:
* This function removes leading whitespace characters (spaces, tabs) from a given string.
*
* Parameters:
* @str: A pointer to a null-terminated string to remove leading whitespace from.
*
* Returns:
* This function does not return any value.
*/
void delete_leading_whitespace(char* str);
