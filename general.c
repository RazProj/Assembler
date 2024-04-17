#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/*--------- Headers ---------*/
#include "second_pass.h"
#include "tree.h"
#include "general.h"


/*---------------------------------*/
char** read_file_lines(const char* filename, char suffix[], int* line_count, int* error_flag) {
    char suf_file[MAX_SIZE];
    char line[MAX_LINE_SIZE];
    char** lines = NULL;
    int count = 0;
    int i = 0, len = 0;
    FILE* file;

    strncpy(suf_file, filename, sizeof(suf_file) - 1);
    suf_file[sizeof(suf_file) - 1] = '\0';
    strcat(suf_file, suffix);

    file = fopen(suf_file, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open %s file\n", suf_file);
        fclose(file);
        *error_flag = 1;
        return NULL;
    }

    /* Count the number of lines in the file */
    count = line_cnt(suf_file, error_flag);

    if (count < 1) { /* Empty file */
        fprintf(stderr, "File is empty\n");
        fclose(file);
        *error_flag = 1;
        return NULL;
    }
    count++;

    /* Create the dynamic array */
    lines = (char**)malloc((count) * sizeof(char*));
    if (lines == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        fclose(file);
        *error_flag = 1;
        return NULL;
    }

    /* Initialize each element of the array to NULL */
    for (i = 0; i < (count); i++) {
        lines[i] = NULL;
    }

    i = 0;

    /* Read each line and store it in the array */
    while (fgets(line, sizeof(line), file) != NULL) {
        len = (int)strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0'; /* Remove the newline character */
            len--;
        }

        if (is_empty_comment_line(line))
            count--;

        else {
            delete_leading_whitespace(line);
            len++;
            lines[i] = (char*)malloc((len) * sizeof(char));
            if (lines[i] == NULL) {
                fprintf(stderr, "Failed to allocate memory\n");
                fclose(file);
                free_lines(lines, i);/* Free memory allocated so far */
                *error_flag = 1;
                return NULL;
            }
            strcpy(lines[i], line);
            i++;
        }
    }

    /*lines[i] = NULL;  Set the last element to NULL for termination */
    fclose(file);

    /* Update the line count and return the array */
    *line_count = count - 1;

    return lines;
}

/*---------------------------------*/
int is_empty_comment_line(const char line[]) {
    int line_length, white_line = 1, i = 0;

    line_length = (int)strlen(line);
    if (line_length == 0)
        return 1; /* Empty line */

    /* Check if the line contains non-whitespace characters */
    for (i = 0; i < line_length; i++) {
        if (line[i] == ';') {
            return 1; /* Comment line */
        }
        if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n') {
            white_line = 0;
            return 0;
        }
    }
    return white_line;
}

/*---------------------------------*/
int line_cnt(const char filename[], int* error_flag) {
    int ch, count = 0;

    FILE* file;
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        *error_flag = 1;
        return 0;
    }

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    rewind(file);
    fclose(file);

    return count;
}

/*---------------------------------*/
void free_lines(char** lines, int line_count) {
    int j;
    for (j = 0; j < line_count; j++) {
        free(lines[j]);
    }
    free(lines);
    /*lines = NULL;*/
}

/*---------------------------------*/
int is_valid_ent_label(label* label_arr, int labels_cnt) {
    int i, flag = ON;
    for (i = 0; i <= labels_cnt; i++) {
        if (get_label_type(label_arr, i) == ent && get_label_source(label_arr, i) == -1) {
            fprintf(stderr, "ERROR: .entry label %s declared but not initialized.\n", get_label_name(label_arr, i));
            flag = OFF;
        }
    }
    return flag;
}

/*---------------------------------*/
int is_valid_labels_dec(lex_tree* lex_arr, int lex_size) {
    int i = 0;
    for (i = 0; i < lex_size; i++) {
        if (is_error(lex_arr, i) == ON) {
            fprintf(stderr, "%s: %d\n", get_error(lex_arr, i), i + 1);
            return 0;
        }
    }
    return 1;
}

/*---------------------------------*/
void clear_all_whites(char* line) {
    int i = 0;
    int j = 0;
    while (line[i] != '\0') {
        if ((line[i] != ' ') && (line[i] != '\t') && (line[i] != '\n')) {
            line[j] = line[i];
            j++;
        }
        i++;
    }
    line[j] = '\0';
}

/*---------------------------------*/
void delete_pas(char* line, int i) {
    size_t len = strlen(line);
    if ((size_t)i >= len) {
        line[0] = '\0'; /* If i is greater than or equal to the length of the word, delete the entire word. */
    }
    else {
        memmove(line, line + i, len - (size_t)i + 1); /* Shift characters to the left by i positions. */
    }
    delete_leading_whitespace(line);
}

/*---------------------------------*/
char* my_strdup(const char* str) {
    size_t len = strlen(str);
    char* copy = (char*)malloc(len + 1); /* +1 for the null terminator */
    if (copy != NULL) {
        strcpy(copy, str);
    }
    return copy;
}

/*---------------------------------*/
int is_num(char* line) {
    int len = strlen(line);
    int i = 0;
    int sign = 1;
    int result = 0;

    /* Check for optional sign */
    if (line[i] == '-') {
        sign = -1;
        i++;
    }
    else if (line[i] == '+') {
        i++;
    }

    /* Process digits */
    while (i < len && isdigit(line[i])) {
        result = result * 10 + (line[i] - '0');
        i++;
    }

    /* Check if there are non-digit characters after the number */
    while (i < len && isspace(line[i])) {
        i++;
    }

    if (i != len) {
        return 0;  /* Not a valid number */
    }
    return result * sign;
}

/*---------------------------------*/
void delete_leading_whitespace(char* current_line) {
    int i = 0, j = 0;
    if (current_line == NULL || current_line[i] == '\0') {
        return;
    }
    while (isspace((unsigned char)current_line[i])) {
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
