#include "pre_assembler.h"

/*Changing the first enum variable value to avoid overlap between values.
 * Uninitialized - before Identifying the word.
 * CommentLine - if the ';' char appears, it emphasizes the line as comment line.
 * Mcro - means it`s the beginning of a macro defining line.
 * EndMcro - End of macro defining line.
 * McroName - calling a macro.
 * JustRegularLine - just a line to copy to the AM file. */
enum line_word_identifier {
    Uninitialized = 10, CommentLine, Mcro, EndMcro, McroName, JustRegularLine
};
/*represents if the source file has an error.*/
int error_flag = e_OFF;

struct macro_node {
    char macro_name[MAX_SIZE_OF_LINE];
    char **macro_strings;
    macro_point next;
};

int pre_assembler(char *source_file) {
    FILE *des_file, *src_file;
    char current_line[MAX_SIZE_OF_LINE], source_file_name[MAX_SIZE_OF_LINE], des_file_name[MAX_SIZE_OF_LINE];
    int is_mcro_flag, first_mcro_flag, first_word_sign, line_number,is_empty;
    macro_point head_of_mcros, last_macro_created, called_macro;
    error_flag = e_OFF;
    strcpy(source_file_name, source_file);
    strcpy(des_file_name, source_file);
    adding_as_to_the_file_name(source_file_name);
    src_file = fopen(source_file_name, "r");
    if (src_file == NULL) {

        error_flag = e_MID;
        return error_flag;
    }
    adding_am_to_the_file_name(des_file_name);
    des_file = fopen(des_file_name, "w");
    is_mcro_flag = e_OFF;
    first_mcro_flag = e_OFF;
    head_of_mcros = NULL;
    last_macro_created = NULL;
    called_macro = NULL;
    while (fgets(current_line, MAX_SIZE_OF_LINE, src_file)) {
        line_number++;
        delete_leading_whitespaces(current_line);
        /*fseek(src_file, 0, SEEK_END);
        is_empty = ftell(src_file);
        if (is_empty == 0){
            fprintf(stderr, "File %s is empty.\n", source_file);
            error_flag = e_ON;
            return error_flag;
            fclose(src_file);
        }*/
        if (strlen(current_line) > 81)
            current_line[82] = '\0';
        first_word_sign = identifying_first_word(current_line, head_of_mcros, line_number);
        if ((first_word_sign == Mcro) && (first_mcro_flag == e_OFF)) {
            first_mcro_flag = e_ON;
            is_mcro_flag = e_ON;
            last_macro_created = create_macro_node(current_line, head_of_mcros, last_macro_created, line_number);
            head_of_mcros = last_macro_created;
        } else if (first_word_sign == Mcro) {
            last_macro_created = create_macro_node(current_line, head_of_mcros, last_macro_created, line_number);
            is_mcro_flag = e_ON;
        } else if (first_word_sign == EndMcro) {
            is_mcro_flag = e_OFF;
        } else if (first_word_sign == CommentLine) {/*do nothing*/ }
        else if (is_mcro_flag == e_ON) {
            insert_line_to_macro(current_line, last_macro_created);
        } else if (first_word_sign == McroName) {
            called_macro = find_macro_node(current_line, head_of_mcros);
            insert_lines_to_file(called_macro, des_file);
        } else {
            fputs(current_line, des_file);
        }
    }
    fseek(src_file, 0, SEEK_END);
    is_empty = ftell(src_file);
    if (is_empty == 0){
        fprintf(stderr, "File %s is empty.\n", source_file);
        error_flag = e_ON;
        return error_flag;
        fclose(src_file);
    }
    fclose(src_file);
    fclose(des_file);
    if (error_flag == e_ON) {
        remove(des_file_name);
    }
    return error_flag;
}

/*----------------------------------Utils------------------------------------*/

void adding_as_to_the_file_name(char *p) {
    char str[] = ".as";
    strcat(p, str);
}

void adding_am_to_the_file_name(char *p) {

    char str[MAX_SIZE_OF_LINE];
    strcpy(str, ".am");
    strcat(p, str);
}

void delete_leading_whitespaces(char *current_line) {
    int i = 0, j = 0;
    if (current_line == NULL || current_line[i] == '\0') {
        return;
    }
    while (isspace((unsigned char) current_line[i])) {
        i++;
    }
    if (current_line[i] == '\0') {
        return;
    }
    while (current_line[i] != '\0') {
        current_line[j] = current_line[i];
        i++;
        j++;
    }
    current_line[j] = '\0';
}

/*----------------------------------First word functions------------------------------------*/

int identifying_first_word(char current_line[], macro_point head_of_mcros, int line_number) {
    char current_line_temp[MAX_SIZE_OF_LINE];
    int identifier_first_word, i, count, comment, first_quete, second_quete;
    identifier_first_word = Uninitialized;
    i = 0;
    count = e_OFF;
    comment = e_OFF;
    first_quete = e_OFF;
    second_quete = e_OFF;
    strcpy(current_line_temp, current_line);
    while (current_line_temp[i] != '\n' && current_line_temp[i] != '\0') {
        if (!(isspace(current_line_temp[i])) && current_line_temp[i] != '\0') {
            count = e_ON;
            if (current_line_temp[i] == '"' && first_quete == e_OFF) {
                first_quete = e_ON;
            } else if (current_line_temp[i] == '"' && second_quete == e_OFF) {
                second_quete = e_ON;
            } else if (current_line_temp[i] == ';' && (first_quete == e_OFF || second_quete == e_ON)) {
                comment = e_ON;
            } else if (current_line_temp[i] == ';' && first_quete == e_ON && second_quete == e_OFF) {/*do nothing*/ }
        }
        i++;
    }
    if (count == e_OFF || comment == e_ON)
        return CommentLine;
    i = 0;
    identifier_first_word = strcmp(strtok(current_line_temp, " \t\n"), "mcro");
    if (identifier_first_word == 0) {
        if (rest_of_mcro_definition(current_line)) {
            fprintf(stderr, "Redundant writing in the mcro definition line - line %d.\n", line_number);
            error_flag = e_ON;
        }
        return Mcro;
    }
    strcpy(current_line_temp, current_line);
    identifier_first_word = strcmp(strtok(current_line_temp, " \t\n"), "endmcro");
    if (identifier_first_word == 0) {
        if (rest_of_mcro_definition(current_line)) {
            fprintf(stderr, "Redundant writing in the endmcro line - line %d.\n", line_number);
            error_flag = e_ON;
        }
        return EndMcro;
    }
    identifier_first_word = is_first_word_name(current_line, head_of_mcros, line_number);
    if (identifier_first_word == 0)
        return McroName;
    return JustRegularLine;
}

int is_first_word_name(char current_line[], macro_point head_of_mcros, int line_number) {
    char str_temp[MAX_SIZE_OF_LINE];
    macro_point temp_macro_point;
    temp_macro_point = head_of_mcros;
    strcpy(str_temp, current_line);
    strtok(str_temp, " \t\n");
    while (temp_macro_point != NULL) {
        if (strcmp(temp_macro_point->macro_name, str_temp) == 0)
            return 0;
        else
            temp_macro_point = temp_macro_point->next;
    }
    return 1;
}

/*----------------------------------inserting/receiving/creating data to/from mcro node------------------------------------*/

macro_point
create_macro_node(char current_line[], macro_point head_of_mcros, macro_point last_macro_created, int line_number) {
    char str_temp[MAX_SIZE_OF_LINE];
    char *portion;
    macro_point temp_macro_point;
    temp_macro_point = head_of_mcros;
    while ((temp_macro_point) != NULL)
        temp_macro_point = temp_macro_point->next;
    temp_macro_point = calloc(1, sizeof(struct macro_node));
    if (last_macro_created != NULL)
        last_macro_created->next = temp_macro_point;
    strcpy(str_temp, current_line);
    portion = strtok(str_temp, " \t\n");
    portion = strtok(NULL, " \t\n");
    if (portion == NULL) {
        fprintf(stderr, "Missing mcro name - line %d.\n", line_number);
        error_flag = e_ON;
        return temp_macro_point;
    } else if (is_mcro_name_wrong(portion)) {
        fprintf(stderr, "Invalid mcro name - line %d.\n", line_number);
        error_flag = e_ON;
        return temp_macro_point;
    } else {
        strcpy(temp_macro_point->macro_name, portion);
        return temp_macro_point;
    }
    free(temp_macro_point);
}

macro_point find_macro_node(char current_line[], macro_point head_of_mcros) {
    char str_temp[MAX_SIZE_OF_LINE];
    char *portion;
    macro_point temp_macro_point;
    temp_macro_point = head_of_mcros;
    strcpy(str_temp, current_line);
    portion = strtok(str_temp, " \t\n");
    while (strcmp(temp_macro_point->macro_name, portion))
        temp_macro_point = temp_macro_point->next;
    return temp_macro_point;
}

/*----------------------------------Data transportation------------------------------------*/

void insert_line_to_macro(char current_line[], macro_point last_macro_created) {
    int i;
    i = 0;
    if (last_macro_created->macro_strings == NULL) {
        last_macro_created->macro_strings = (char **) calloc(1, sizeof(char *));
        last_macro_created->macro_strings[i] = (char *) calloc(MAX_SIZE_OF_LINE, sizeof(char));
        strcpy(last_macro_created->macro_strings[i], current_line);
        return;
    }
    for (i = 0; last_macro_created->macro_strings[i] != NULL; i++);
    last_macro_created->macro_strings = (char **) realloc(last_macro_created->macro_strings,
                                                          (i + 5) * (sizeof(char *)));
    last_macro_created->macro_strings[i] = (char *) realloc(last_macro_created->macro_strings[i],
                                                            MAX_SIZE_OF_LINE * sizeof(char));
    strcpy(last_macro_created->macro_strings[i], current_line);
}


void insert_lines_to_file(macro_point called_macro, FILE *des_file) {
    int i;
    if (called_macro->macro_strings == NULL)
        return;
    for (i = 0; called_macro->macro_strings[i] != NULL; i++)
        fputs(called_macro->macro_strings[i], des_file);
}

/*----------------------------------Error identifying------------------------------------*/

int is_mcro_name_wrong(char *portion) {
    if (!(strcmp(portion, "mov")))return e_ON;
    if (!(strcmp(portion, "cmp")))return e_ON;
    if (!(strcmp(portion, "add")))return e_ON;
    if (!(strcmp(portion, "sub")))return e_ON;
    if (!(strcmp(portion, "lea")))return e_ON;
    if (!(strcmp(portion, "not")))return e_ON;
    if (!(strcmp(portion, "clr")))return e_ON;
    if (!(strcmp(portion, "inc")))return e_ON;
    if (!(strcmp(portion, "dec")))return e_ON;
    if (!(strcmp(portion, "jmp")))return e_ON;
    if (!(strcmp(portion, "bne")))return e_ON;
    if (!(strcmp(portion, "red")))return e_ON;
    if (!(strcmp(portion, "prn")))return e_ON;
    if (!(strcmp(portion, "jsr")))return e_ON;
    if (!(strcmp(portion, "rts")))return e_ON;
    if (!(strcmp(portion, "stop")))return e_ON;
    if (!(strcmp(portion, ".data")))return e_ON;
    if (!(strcmp(portion, ".string")))return e_ON;
    if (!(strcmp(portion, ".extern")))return e_ON;
    if (!(strcmp(portion, ".extern")))return e_ON;
    return e_OFF;

}

int rest_of_mcro_definition(char current_line[]) {
    char current_line_temp[MAX_SIZE_OF_LINE], *portion;
    strcpy(current_line_temp, current_line);
    portion = strtok(current_line_temp, " \t\n");
    if (!(strcmp(portion, "mcro"))) {
        portion = strtok(NULL, " \t\n");
        portion = strtok(NULL, " \t\n");
        if (portion == NULL)
            return e_OFF;
        else
            return e_ON;
    } else {
        portion = strtok(NULL, " \t\n");
        if (portion == NULL)
            return e_OFF;
        else
            return e_ON;
    }
}

