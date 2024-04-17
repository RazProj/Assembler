#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE_OF_LINE 1000

/* OFF - means there is no error in the file.
 * ON - errors like macro name wrong or syntax after macro defining.
 * MID - error in the file like file not found. */

enum error {
    e_OFF, e_MID, e_ON
};

typedef struct macro_node* macro_point;

/*The main function. */
int pre_assembler(char* source_file);

/*----------------------------------Utils------------------------------------*/

/*taking a file name and adding an ending by the requirements. */
void adding_as_to_the_file_name(char* p);

/*taking a file name and adding an ending by the requirements. */
void adding_am_to_the_file_name(char* p);

void delete_leading_whitespaces(char* current_line);

/*----------------------------------First word functions------------------------------------*/

/*Identifying and saving the first word of the line. */
int identifying_first_word(char current_line[], macro_point head_of_mcros, int line_number);

/*Name of a mcro. */
int is_first_word_name(char current_line[], macro_point head_of_mcros, int line_number);

/*----------------------------------inserting/receiving/creating data to/from mcro node------------------------------------*/

/*Creating a new node, after identifying a new mcro defining. */
macro_point create_macro_node(char current_line[], macro_point head_of_mcros, macro_point last_macro_created, int line_number);

/*Finding the right mcro for inserting the line to the "am" file. */
macro_point find_macro_node(char current_line[], macro_point head_of_mcros);

/*----------------------------------Data transportation------------------------------------*/

/*receive the data line from the specific mcro node and inserting it to the "am" file. */
void insert_lines_to_file(macro_point called_macro, FILE* des_file);

/*After declaring a new mcro defining, inserting the mcro line to the mcro node. */
void insert_line_to_macro(char current_line[], macro_point last_macro_created);

/*----------------------------------Error identifying------------------------------------*/

/*checks whether a mcro name is equal by name to one of the saved word.
 * Return ON if it does and OFF otherwise.*/
int is_mcro_name_wrong(char* portion);

/*Checks whether after a mcro definition, the line has more unwanted writing.
 * Returns ON it is does and OFF otherwise. */
int rest_of_mcro_definition(char current_line[]);


