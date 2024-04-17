#include "error_checker.h"
#include "syntax_pass.h"

enum flag {
    OFF, ON
};

enum instruction_type {
    Mov = 100, Cmp, Add, Sub, Lea, Not, Clr, Inc, Dec, Jmp, Bne, Red, Prn, Jsr, Rts, Stop
};

enum direction_type {
    Data = 200, String
};

enum ins_operand_type {
    Label = 300, Register, Number
};


/*----------------------------------operands error identifier------------------------------------*/

int error_in_first_operand(char* current_line, int line_number, int ins_type, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    int second_operand_flag;
    second_operand_flag = OFF;
    strcpy(current_line_temp, current_line);
    if (error_comma_first_letter_in_the_first_operand(current_line, label_flag)) {
        fprintf(stderr,"Invalid first instruction operand - line %d.\n", line_number);
        return 0;
    }
    if (label_flag) {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, " \t\n");
        portion = strtok(NULL, " ,\t\n");
    }
    else {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, " ,\t\n");
    }
    if (ins_type == Mov || ins_type == Add || ins_type == Sub) {
        if (portion == NULL) {
            fprintf(stderr,"Missing instruction operand - line %d.\n", line_number);
            return 0;
        }
        else {
            fprintf(stderr,"Invalid first instruction operand - line %d.\n", line_number);
            return 0;
        }
    }
    else if (ins_type == Cmp) {
        if (portion == NULL) {
            fprintf(stderr,"Missing instruction operand - line %d.\n", line_number);
            return 0;
        }
        else {
            fprintf(stderr,"Invalid first instruction operand - line %d.\n", line_number);
            return 0;
        }

    }
    else if (ins_type == Not || ins_type == Clr || ins_type == Inc || ins_type == Jmp || ins_type == Bne ||
        ins_type == Red || ins_type == Jsr) {
        if (groupThree_error_checker(portion, second_operand_flag)) {
            fprintf(stderr,"This instruction cannot receive this operand - line %d.\n", line_number);
            return 0;
        }
        else {
            if (portion == NULL) {
                fprintf(stderr,"Missing instruction operand - line %d.\n", line_number);
                return 0;
            }
            else {
                fprintf(stderr,"Invalid instruction operand - line %d.\n", line_number);
                return 0;
            }
        }
    }
    else if (ins_type == Lea) {
        if (groupFour_error_checker(portion, second_operand_flag)) {
            fprintf(stderr,"This instruction cannot receive this operand - line %d.\n", line_number);
            return 0;
        }
        else {
            if (portion == NULL) {
                fprintf(stderr,"Missing instruction operand - line %d.\n", line_number);
                return 0;
            }
            else {
                fprintf(stderr,"Invalid first instruction operand - line %d.\n", line_number);
                return 0;
            }
        }
    }
    else if (ins_type == Prn) {
        if (portion == NULL) {
            fprintf(stderr,"Missing instruction operand - line %d.\n", line_number);
            return 0;
        }
        else {
            fprintf(stderr,"Invalid first instruction operand - line %d.\n", line_number);
            return 0;
        }
    }
    if (error_comma_last_letter_in_the_last_operand(current_line)) {
        fprintf(stderr,"Invalid last instruction operand - line %d.\n", line_number);
        return 0;
    }
    return 0;
}

int error_in_second_operand(char* current_line, int line_number, int ins_type, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    int second_operand_flag;
    second_operand_flag = ON;
    strcpy(current_line_temp, current_line);
    if (error_comma_last_letter_in_the_last_operand(current_line)) {
        fprintf(stderr,"Invalid last instruction operand - line %d.\n", line_number);
        return 0;
    }
    if (label_flag) {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, " \t\n");
        portion = strtok((NULL), ",\n");
        portion = strtok(NULL, " \t\n");
    }
    else {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, ",\n");
        portion = strtok(NULL, " \t\n");
    }
    if (ins_type == Mov || ins_type == Add || ins_type == Sub) {
        if (groupOne_error_checker(portion, second_operand_flag)) {
            fprintf(stderr,"This instruction cannot receive this operand - line %d.\n", line_number);
            return 0;
        }
        else {
            if (portion == NULL) {
                fprintf(stderr,"Missing instruction operand - line %d.\n", line_number);
                return 0;
            }
            else {
                fprintf(stderr,"Invalid second instruction operand - line %d.\n", line_number);
                return 0;
            }
        }
    }
    else if (ins_type == Cmp) {
        if (portion == NULL) {
            fprintf(stderr,"Missing instruction operand - line %d.\n", line_number);
            return 0;
        }
        else {
            fprintf(stderr,"Invalid second instruction operand - line %d.\n", line_number);
            return 0;
        }
    }
    else if (ins_type == Lea) {
        if (groupFour_error_checker(portion, second_operand_flag)) {
            fprintf(stderr,"This instruction receives only one operand - line %d.\n", line_number);
            return 0;
        }
        else {
            if (portion == NULL) {
                fprintf(stderr,"Missing instruction operand - line %d.\n", line_number);
                return 0;
            }
            else {
                fprintf(stderr,"Invalid instruction operand - line %d.\n", line_number);
                return 0;
            }
        }
    }
    return 0;
}

int error_in_data_str_value(char* current_line, int line_number, int dir_type, int label_flag) {
    if (dir_type == Data) {
        data_error_identifier(current_line, line_number, label_flag);
        return 0;
    }
    else {
        string_error_identifier(current_line, line_number, label_flag);
        return 0;
    }
}

int error_in_extern_entry_operand(char* current_line, int line_number, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    strcpy(current_line_temp, current_line);
    if (error_comma_first_letter_in_the_first_operand(current_line, label_flag)) {
        fprintf(stderr,"Invalid first instruction operand - line %d.\n", line_number);
        return 0;
    }
    if (error_comma_last_letter_in_the_last_operand(current_line)) {
        fprintf(stderr,"Invalid last instruction operand - line %d.\n", line_number);
        return 0;
    }
    if (label_flag) {
        portion = strtok(current_line_temp, " \t\n"); /*portion = label */
        portion = strtok(NULL, " ,\t\n"); /*portion = .extern\.entry */
        portion = strtok(NULL, " ,\t\n"); /*portion = suppose the first operand */
        if (portion == NULL) {
            fprintf(stderr,"Missing instruction operand - line %d.\n", line_number);
            return 0;
        }
        while (is_operand_is_label(portion)) {
            portion = strtok(NULL, " ,\t\n"); /*portion = to the next operand*/
        }
        if (is_operand_is_number(portion)) {
            fprintf(stderr,"This directive cannot receive this operand - line %d.\n", line_number);
            return 0;
        }
        else if (is_operand_is_register(portion)) {
            fprintf(stderr,"This directive cannot receive this operand - line %d.\n", line_number);
            return 0;
        }
        else {
            fprintf(stderr,"Invalid operand - line %d.\n", line_number);
            return 0;
        }
    }
    else {
        portion = strtok(current_line_temp, " \t\n"); /*portion = .extern\.entry */
        portion = strtok(NULL, " ,\t\n"); /*portion = suppose the first operand */
        if (portion == NULL) {
            fprintf(stderr,"Missing instruction operand - line %d.\n", line_number);
            return 0;
        }
        while (is_operand_is_label(portion)) {
            portion = strtok(NULL, " ,\t\n"); /*portion = to the next operand*/
        }
        if (is_operand_is_number(portion)) {
            fprintf(stderr,"This directive cannot receive this operand - line %d.\n", line_number);
            return 0;
        }
        else if (is_operand_is_register(portion)) {
            fprintf(stderr,"This directive cannot receive this operand - line %d.\n", line_number);
            return 0;
        }
        else {
            fprintf(stderr,"Invalid operand - line %d.\n", line_number);
            return 0;
        }
    }
}


/*----------------------------------general error------------------------------------*/

int error_in_the_ins_dir_word(char* current_line, int line_number, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    strcpy(current_line_temp, current_line);
    portion = strtok(current_line_temp, " \t\n");
    portion = strtok(NULL, " \t\n");
    if (portion == NULL) {
        fprintf(stderr,"Missing instruction/direction word - line %d.\n", line_number);
        return 0;
    }
    else {
        fprintf(stderr,"Undefined instruction/direction word - line %d.\n", line_number);
        return 0;
    }
}

int error_in_the_first_word(char* current_line, int line_number) {
    fprintf(stderr,"Invalid first word - line %d.\n", line_number);
    return 0;
}

/*----------------------------------others inside error checking------------------------------------*/

int error_comma_first_letter_in_the_first_operand(char* current_line, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE];
    int space, index;
    index = 0;
    space = 0;
    strcpy(current_line_temp, current_line);
    if (label_flag) {
        while (space < 2) {
            if ((isspace(current_line_temp[index])) && (!(isspace(current_line_temp[index + 1]))) && current_line_temp[index] != '\0') {
                space++;
                index++;
            }
            else
                index++;
        }
    }
    else {
        while (space < 1) {
            if ((isspace(current_line_temp[index])) && (!(isspace(current_line_temp[index + 1]))) && current_line_temp[index] != '\0') {
                space++;
                index++;
            }
            else
                index++;
        }

    }
    if (current_line_temp[index] == ',')
        return ON;
    else
        return OFF;
}

int error_comma_last_letter_in_the_last_operand(char* current_line) {
    char current_line_temp[MAX_SIZE_OF_LINE];
    int index_length;
    strcpy(current_line_temp, current_line);
    index_length = strlen(current_line_temp) - 1;
    while (isspace(current_line_temp[index_length]))
        index_length--;
    if (current_line_temp[index_length] == ',')
        return ON;
    else
        return OFF;
}

/*----------------------------------rest of the row error------------------------------------*/


int error_in_the_rest_of_the_row_of_instruction(char* current_line, int line_number, int ins_type, int label_flag) {
    if (ins_type == Mov || ins_type == Cmp || ins_type == Add || ins_type == Sub || ins_type == Lea) {
        if (error_in_the_rest_of_the_row_of_two_operand_instruction(current_line, line_number, label_flag))
            return ON;
        else
            return OFF;

    }
    else if (ins_type == Stop || ins_type == Rts) {
        if (error_in_the_rest_of_the_row_of_non_operand_instruction(current_line, line_number, label_flag))
            return ON;
        else
            return OFF;
    }
    else {
        if (error_in_the_rest_of_the_row_of_one_operand_instruction(current_line, line_number, label_flag))
            return ON;
        else
            return OFF;
    }
}

int error_in_the_rest_of_the_row_of_directive(char* current_line, int line_number, int dir_type, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    if (label_flag) {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, " \t\n");
        portion = strtok(NULL, "\n");
    }
    else {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, "\n");
    }
    if (portion == NULL)
        return OFF;
    else {
        fprintf(stderr,"Redundant writing - line %d.\n", line_number);
        return ON;
    }
}

/*----------------------------------identifies each instruction------------------------------------*/

int groupOne_error_checker(char* portion, int second_operand_flag) {
    if (is_operand_is_number(portion))
        return ON;
    else
        return OFF;
}

int groupThree_error_checker(char* portion, int second_operand_flag) {
    if (second_operand_flag) {
        if (is_operand_is_number(portion))
            return ON;
        else if (is_operand_is_label(portion))
            return ON;
        else if (is_operand_is_register(portion))
            return ON;
        else
            return OFF;
    }
    else {
        if (is_operand_is_number(portion))
            return ON;
        else
            return OFF;
    }
}

int groupFour_error_checker(char* portion, int second_operand_flag) {
    if (portion == NULL)return OFF;
    if (second_operand_flag) {
        if (is_operand_is_number(portion))
            return ON;
        else
            return OFF;
    }
    else {
        if (is_operand_is_number(portion))
            return ON;
        else if (is_operand_is_register(portion))
            return ON;
        else
            return OFF;
    }
}

/*----------------------------------identifies each error in directive------------------------------------*/

int data_error_identifier(char* current_line, int line_number, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion, current_char;
    int index, sign, num, start;
    index = 0;
    sign = OFF;
    num = OFF;
    start = OFF;
    strcpy(current_line_temp, current_line);
    if (label_flag) {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, " \t\n");
        portion = strtok(NULL, "\n");
    }
    else {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, "\n");
    }
    if (portion == NULL) {
        fprintf(stderr,"Missing data - line %d.\n", line_number);
        return 0;
    }
    if (!(isspace(portion[index])) && !(isdigit(portion[index])) && portion[index] != '+' && portion[index] != '-') {
        fprintf(stderr,"Undefined data integer - line %d.\n", line_number);
        return 0;
    }
    while (portion[index] != '\0') {
        current_char = portion[index];
        if ((isspace(current_char)) && sign == OFF) {
            index++;
        }
        else if ((current_char == '+' || current_char == '-') && num == OFF && sign == OFF) {
            sign = ON;
            index++;
        }
        else if ((current_char == ',') && num == ON && sign == OFF) {
            start = OFF;
            num = OFF;
            index++;
        }
        else if (isdigit(current_char)) {
            if (start == OFF) {
                num = ON;
                start = ON;
                index++;
                sign = OFF;
            }
            else {
                index++;
            }
        }
        else if ((isspace(current_char)) && sign == ON) {
            fprintf(stderr,"Invalid space after sign - line %d.\n", line_number);
            return 0;
        }
        else if ((current_char == '+' || current_char == '-') && num == ON && sign == OFF) {
            fprintf(stderr,"Wrong sign location - line %d.\n", line_number);
            return 0;
        }
        else if ((current_char == '+' || current_char == '-') && num == OFF && sign == ON) {
            fprintf(stderr,"Multiple consecutive signs - line %d.\n", line_number);
            return 0;
        }
        else if ((current_char == '+' || current_char == '-') && num == ON && sign == ON) {
            fprintf(stderr,"Wrong sign location - line %d.\n", line_number);
            return 0;
        }
        else if ((current_char == ',') && num == OFF && sign == OFF) {
            fprintf(stderr,"Wrong comma location - line %d.\n", line_number);
            return 0;
        }
        else {
            index++;
        }

    }
    fprintf(stderr,"Undefined data integer - line %d.\n", line_number);
    return 0;

}

int string_error_identifier(char* current_line, int line_number, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion, current_char;
    int index, start, end, not_empty_string;
    index = 0;
    start = OFF;
    end = OFF;
    not_empty_string = OFF;
    strcpy(current_line_temp, current_line);
    if (label_flag) {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, " \t\n");
        portion = strtok(NULL, "\n");
    }
    else {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, "\n");
    }
    if (portion == NULL) {
        fprintf(stderr,"Missing string - line %d.\n", line_number);
        return 0;
    }
    while (portion[index] != '\0') {
        current_char = portion[index];
        if (isspace(current_char) && (start == OFF || end == ON)) {
            index++;
        }
        else if (isspace(current_char) && start == ON && end == OFF) {
            index++;
            not_empty_string = ON;
        }
        else if (current_char == '"' && start == OFF) {
            start = ON;
            index++;
        }
        else if (current_char == '"' && end == OFF) {
            end = ON;
            index++;
        }
        else if (current_char == '"' && end == ON) {
            index++;
            not_empty_string = ON;
        }
        else if (!(isspace(current_char)) && current_char != '"' && start == ON && end == OFF) {
            index++;
            not_empty_string = ON;
        }
        else if (!(isspace(current_char)) && current_char != '"' && end == ON) {
            end = OFF;
            index++;
        }
        else {
            index++;
        }

    }
    if (end == OFF && start == ON) {
        fprintf(stderr,"Missing quote sign - line %d.\n", line_number);
        return 0;
    }
    else if (not_empty_string == OFF) {
        fprintf(stderr,"There are not chars in the string - line %d.\n", line_number);
        return 0;
    }
    else {
        fprintf(stderr,"Undefined string - line %d.\n", line_number);
        return 0;
    }

}

/*----------------------------------rest of the row specific instruction type------------------------------------*/

int error_in_the_rest_of_the_row_of_two_operand_instruction(char* current_line, int line_number, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    strcpy(current_line_temp, current_line);
    if (label_flag) {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, " \t\n");
        portion = strtok((NULL), ",\n");
        portion = strtok(NULL, " \t\n");
        portion = strtok(NULL, "\n");
    }
    else {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, ",\n");
        portion = strtok(NULL, " \t\n");
        portion = strtok(NULL, "\n");
    }
    if (portion == NULL)
        return OFF;
    else {
        fprintf(stderr,"Redundant writing - line %d.\n", line_number);
        return ON;
    }
}

int error_in_the_rest_of_the_row_of_one_operand_instruction(char* current_line, int line_number, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    strcpy(current_line_temp, current_line);
    if (label_flag) {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, " \t\n");
        portion = strtok(NULL, " ,\t\n");
        portion = strtok(NULL, "\n");
    }
    else {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, " ,\t\n");
        portion = strtok(NULL, "\n");
    }
    if (portion == NULL)
        return OFF;
    else {
        fprintf(stderr,"Redundant writing - line %d.\n", line_number);
        return ON;
    }

}

int error_in_the_rest_of_the_row_of_non_operand_instruction(char* current_line, int line_number, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    strcpy(current_line_temp, current_line);
    if (label_flag) {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, " \t\n");
        portion = strtok(NULL, "\n");
    }
    else {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, "\n");
    }
    if (portion == NULL)
        return OFF;
    else {
        fprintf(stderr,"Redundant writing - line %d.\n", line_number);
        return ON;
    }
}

