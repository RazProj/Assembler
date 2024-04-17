#include "syntax_pass.h"
#include "error_checker.h"
#include "pre_assembler.h"

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

int syntax_pass(char* source_file_name) {
    char current_line[MAX_SIZE_OF_LINE], src_file_name[MAX_SIZE_OF_LINE];
    FILE* source_file;
    int error_flag, label_flag, line_number, ins_type, dir_type, operand_type_one, operand_type_two;
    strcpy(src_file_name, source_file_name);
    adding_am_to_the_file_name(src_file_name);
    source_file = fopen(src_file_name, "r");
    label_flag = OFF;
    error_flag = OFF;
    dir_type = OFF;
    operand_type_one = OFF;
    operand_type_two = OFF;
    line_number = 0;
    while (fgets(current_line, MAX_SIZE_OF_LINE, source_file)) {
        line_number++;
        adding_space_to_label_ins_line(current_line);
        if (is_line_exceed_length_limit(current_line, line_number)) {
            error_flag = ON;
            continue;
        }
        if (is_label_line(current_line)) {
            label_flag = ON;
            if ((ins_type = is_instructive_line(current_line, label_flag)) != 0) {
                if (two_operand_instruction(ins_type)) {
                    if ((operand_type_one = first_operand_checker(ins_type, current_line, label_flag)) != 0) {

                        if ((operand_type_two = second_operand_checker(ins_type, current_line, label_flag)) != 0) {
                            if (error_in_the_rest_of_the_row_of_instruction(current_line, line_number, ins_type,
                                label_flag)) {
                                error_flag = ON;
                            }
                            label_flag = OFF;
                            continue;
                        }
                        else {
                            error_in_second_operand(current_line, line_number, ins_type, label_flag);
                            error_flag = ON;
                            label_flag = OFF;
                            continue;
                        }
                    }
                    else {
                        error_in_first_operand(current_line, line_number, ins_type, label_flag);
                        error_flag = ON;
                        label_flag = OFF;
                        continue;
                    }
                }
                else if (one_operand_instruction(ins_type)) {

                    if ((operand_type_one = first_operand_checker(ins_type, current_line, label_flag)) !=
                        0) {/*do nothing*/
                    }
                    else {
                        error_in_first_operand(current_line, line_number, ins_type, label_flag);
                        error_flag = ON;
                        label_flag = OFF;
                        continue;
                    }
                    if (error_in_the_rest_of_the_row_of_instruction(current_line, line_number, ins_type,
                        label_flag)) {
                        error_flag = ON;
                    }
                    label_flag = OFF;
                    continue;
                }
                else {
                    if (error_in_the_rest_of_the_row_of_instruction(current_line, line_number, ins_type,
                        label_flag)) {
                        error_flag = ON;
                    }
                    label_flag = OFF;
                    continue;
                }
            }
            else if ((dir_type = is_directive_line(current_line, label_flag)) != 0) {
                if (dir_type == Data) {
                    if (data_value_checker(current_line, label_flag)) {
                        if (error_in_the_rest_of_the_row_of_directive(current_line, line_number, dir_type,
                            label_flag)) {
                            error_flag = ON;
                        }
                        label_flag = OFF;
                        continue;
                    }
                    else {
                        error_in_data_str_value(current_line, line_number, dir_type, label_flag);
                        error_flag = ON;
                        label_flag = OFF;
                        continue;
                    }
                }
                else {
                    if (string_value_checker(current_line, label_flag)) {
                        if (error_in_the_rest_of_the_row_of_directive(current_line, line_number, dir_type,
                            label_flag)) {
                            error_flag = ON;
                        }
                        label_flag = OFF;
                        continue;
                    }
                    else {
                        error_in_data_str_value(current_line, line_number, dir_type, label_flag);
                        error_flag = ON;
                        label_flag = OFF;
                        continue;
                    }
                }

            }
            else if (is_extern_line(current_line, label_flag)) {
                if (extern_entry_operand_checker(current_line, label_flag)) {
                    label_flag = OFF;
                    continue;
                }
                else {
                    error_in_extern_entry_operand(current_line, line_number, label_flag);
                    error_flag = ON;
                    label_flag = OFF;
                    continue;
                }
            }
            else if (is_entry_line(current_line, label_flag)) {
                if (extern_entry_operand_checker(current_line, label_flag)) {
                    label_flag = OFF;
                    continue;
                }
                else {
                    error_in_extern_entry_operand(current_line, line_number, label_flag);
                    error_flag = ON;
                    label_flag = OFF;
                    continue;
                }
            }
            else {
                error_in_the_ins_dir_word(current_line, line_number, label_flag);
                error_flag = ON;
                label_flag = OFF;
                continue;
            }

        }
        else if ((ins_type = is_instructive_line(current_line, label_flag)) != 0) {
            if (two_operand_instruction(ins_type)) {
                if ((operand_type_one = first_operand_checker(ins_type, current_line, label_flag)) != 0) {
                    if ((operand_type_two = second_operand_checker(ins_type, current_line, label_flag)) != 0) {
                        if (error_in_the_rest_of_the_row_of_instruction(current_line, line_number, ins_type,
                            label_flag)) {
                            error_flag = ON;
                        }
                        label_flag = OFF;
                        continue;
                    }
                    else {
                        error_in_second_operand(current_line, line_number, ins_type, label_flag);
                        error_flag = ON;
                        label_flag = OFF;
                        continue;
                    }
                }
                else {
                    error_in_first_operand(current_line, line_number, ins_type, label_flag);
                    error_flag = ON;
                    label_flag = OFF;
                    continue;
                }
            }
            else if (one_operand_instruction(ins_type)) {

                if ((operand_type_one = first_operand_checker(ins_type, current_line, label_flag)) != 0) {
                }
                else {
                    error_in_first_operand(current_line, line_number, ins_type, label_flag);
                    error_flag = ON;
                    label_flag = OFF;
                    continue;
                }
                if (error_in_the_rest_of_the_row_of_instruction(current_line, line_number, ins_type,
                    label_flag)) {
                    error_flag = ON;
                }
                label_flag = OFF;
                continue;
            }
            else {
                if (error_in_the_rest_of_the_row_of_instruction(current_line, line_number, ins_type,
                    label_flag)) {
                    error_flag = ON;
                }
                label_flag = OFF;
                continue;
            }

        }
        else if ((dir_type = is_directive_line(current_line, label_flag)) != 0) {
            if (dir_type == Data) {
                if (data_value_checker(current_line, label_flag)) {
                    if (error_in_the_rest_of_the_row_of_directive(current_line, line_number, dir_type,
                        label_flag)) {
                        error_flag = ON;
                    }
                    label_flag = OFF;
                    continue;
                }
                else {
                    error_in_data_str_value(current_line, line_number, dir_type, label_flag);
                    error_flag = ON;
                    label_flag = OFF;
                    continue;

                }
            }
            else {
                if (string_value_checker(current_line, label_flag)) {
                    if (error_in_the_rest_of_the_row_of_directive(current_line, line_number, dir_type,
                        label_flag)) {
                        error_flag = ON;
                    }
                    label_flag = OFF;
                    continue;
                }
                else {
                    error_in_data_str_value(current_line, line_number, dir_type, label_flag);
                    error_flag = ON;
                    label_flag = OFF;
                    continue;

                }
            }
        }
        else if (is_extern_line(current_line, label_flag)) {
            if (extern_entry_operand_checker(current_line, label_flag)) {
                label_flag = OFF;
                continue;
            }
            else {
                error_in_extern_entry_operand(current_line, line_number, label_flag);
                error_flag = ON;
                label_flag = OFF;
                continue;
            }
        }
        else if (is_entry_line(current_line, label_flag)) {
            if (extern_entry_operand_checker(current_line, label_flag)) {
                label_flag = OFF;
                continue;
            }
            else {
                error_in_extern_entry_operand(current_line, line_number, label_flag);
                error_flag = ON;
                label_flag = OFF;
                continue;
            }
        }
        else {
            error_in_the_first_word(current_line, line_number);
            error_flag = ON;
            label_flag = OFF;
            continue;
        }
    }
    fclose(source_file);
    return error_flag;
}

/*----------------------------------First Word AND Instructive\directive checking------------------------------------*/

int is_label_line(char* current_line) {
    char current_line_temp[MAX_SIZE_OF_LINE];
    int index, line_length;
    strcpy(current_line_temp, current_line);
    strtok(current_line_temp, " \t\n");
    line_length = strlen(current_line_temp) - 1;
    index = 0;
    if (isalpha(current_line_temp[index])) {
        for (index = 0; index < line_length; index++) {
            if ((isalpha(current_line_temp[index])) || (isdigit(current_line_temp[index])));
            else {
                return OFF;
            }
        }
        if (current_line_temp[index] == ':' && isspace(current_line[index + 1])) {
            return ON;

        }
    }
    return OFF;
}

int is_instructive_line(char* current_line, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    strcpy(current_line_temp, current_line);
    if (label_flag) {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, " \t\n");
    }
    else {
        portion = strtok(current_line_temp, " \t\n");
    }
    /*if the word equal to one the instruction,the return from strcmp will be 0 and the
     * ! in the beginning turn it into True which will cause entering the if statement,
     * and return the exact instruction. */
    if (!(strcmp(portion, "mov")))return Mov;
    if (!(strcmp(portion, "cmp")))return Cmp;
    if (!(strcmp(portion, "add")))return Add;
    if (!(strcmp(portion, "sub")))return Sub;
    if (!(strcmp(portion, "lea")))return Lea;
    if (!(strcmp(portion, "not")))return Not;
    if (!(strcmp(portion, "clr")))return Clr;
    if (!(strcmp(portion, "inc")))return Inc;
    if (!(strcmp(portion, "dec")))return Dec;
    if (!(strcmp(portion, "jmp")))return Jmp;
    if (!(strcmp(portion, "bne")))return Bne;
    if (!(strcmp(portion, "red")))return Red;
    if (!(strcmp(portion, "prn")))return Prn;
    if (!(strcmp(portion, "jsr")))return Jsr;
    if (!(strcmp(portion, "rts")))return Rts;
    if (!(strcmp(portion, "stop")))return Stop;
    return OFF;
}

int is_directive_line(char* current_line, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    strcpy(current_line_temp, current_line);
    if (label_flag) {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, " \t\n");
    }
    else {
        portion = strtok(current_line_temp, " \t\n");
    }
    if (!(strcmp(portion, ".data")))return Data;
    if (!(strcmp(portion, ".string")))return String;
    return OFF;
}

int is_extern_line(char* current_line, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    strcpy(current_line_temp, current_line);
    if (label_flag) {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, " \t\n");
        if (!(strcmp(portion, ".extern")))return ON;
        return OFF;
    }
    else {
        portion = strtok(current_line_temp, " \t\n");
        if (!(strcmp(portion, ".extern")))return ON;
        return OFF;
    }
}

int is_entry_line(char* current_line, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    strcpy(current_line_temp, current_line);
    if (label_flag) {
        portion = strtok(current_line_temp, " \t\n");
        portion = strtok(NULL, " \t\n");
        if (!(strcmp(portion, ".entry")))return ON;
        return OFF;
    }
    else {
        portion = strtok(current_line_temp, " \t\n");
        if (!(strcmp(portion, ".entry")))return ON;
        return OFF;
    }
}


/*----------------------------------operands OR data/string checking------------------------------------*/

int first_operand_checker(int ins_type, char* current_line, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    int operand_type, second_operand_flag;
    second_operand_flag = OFF;
    strcpy(current_line_temp, current_line);
    if (error_comma_first_letter_in_the_first_operand(current_line, label_flag)) { return OFF; }
    if (error_comma_last_letter_in_the_last_operand(current_line)) { return OFF; }
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
        if ((operand_type = groupOne_operand_checker(portion, second_operand_flag)) != 0)
            return operand_type;
        else
            return OFF;
    }
    else if (ins_type == Cmp) {
        if ((operand_type = groupTwo_operand_checker(portion, second_operand_flag)))
            return operand_type;
        else
            return OFF;
    }
    else if (ins_type == Not || ins_type == Clr || ins_type == Inc || ins_type == Jmp || ins_type == Bne ||
        ins_type == Red || ins_type == Jsr) {
        if ((operand_type = (groupThree_operand_checker(portion, second_operand_flag))))
            return operand_type;
        else
            return OFF;
    }
    else if (ins_type == Lea) {
        if ((operand_type = (groupFour_operand_checker(portion, second_operand_flag))))
            return operand_type;
        else
            return OFF;
    }
    else if (ins_type == Prn) {
        if ((operand_type = (groupFive_operand_checker(portion, second_operand_flag))))
            return operand_type;
        else
            return OFF;
    }
    else {
        /*The last group has no operands, */
        return ON;
    }

}

int second_operand_checker(int ins_type, char* current_line, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    int operand_type, second_operand_flag;
    second_operand_flag = ON;
    strcpy(current_line_temp, current_line);
    if (error_comma_last_letter_in_the_last_operand(current_line)) { return OFF; }
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
        if ((operand_type = groupOne_operand_checker(portion, second_operand_flag)) != 0)
            return operand_type;
        else
            return OFF;
    }
    else if (ins_type == Cmp) {
        if ((operand_type = groupTwo_operand_checker(portion, second_operand_flag)))
            return operand_type;
        else
            return OFF;
    }
    else if (ins_type == Not || ins_type == Clr || ins_type == Inc || ins_type == Jmp || ins_type == Bne ||
        ins_type == Red || ins_type == Jsr) {
        if ((operand_type = (groupThree_operand_checker(portion, second_operand_flag))))
            return operand_type;
        else
            return OFF;
    }
    else if (ins_type == Lea) {
        if ((operand_type = (groupFour_operand_checker(portion, second_operand_flag))))
            return operand_type;
        else
            return OFF;
    }
    else if (ins_type == Prn) {
        if ((operand_type = (groupFive_operand_checker(portion, second_operand_flag))))
            return operand_type;
        else
            return OFF;
    }
    else {
        /*The last group has no operands, */
        return ON;
    }
}

int data_value_checker(char* current_line, int label_flag) {
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
    if (portion == NULL)return OFF;
    if (!(isspace(portion[index])) && !(isdigit(portion[index])) && portion[index] != '+' && portion[index] != '-')
        return OFF;
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
        else
            return OFF;
    }
    if (!(isspace(portion[index - 1])) && !(isdigit(portion[index - 1])))
        return OFF;
    return ON;


}

int string_value_checker(char* current_line, int label_flag) {
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
    if (portion == NULL)return OFF;
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
        else
            return OFF;
    }
    if (end == OFF || not_empty_string == OFF)
        return OFF;
    else
        return ON;
}

int extern_entry_operand_checker(char* current_line, int label_flag) {
    char current_line_temp[MAX_SIZE_OF_LINE], * portion;
    strcpy(current_line_temp, current_line);
    if (error_comma_first_letter_in_the_first_operand(current_line, label_flag)) { return OFF; }
    if (error_comma_last_letter_in_the_last_operand(current_line)) { return OFF; }
    if (label_flag) {
        portion = strtok(current_line_temp, " \t\n"); /*portion = label */
        portion = strtok(NULL, " ,\t\n"); /*portion = .extern\.entry */
        portion = strtok(NULL, " ,\t\n"); /*portion = suppose the first operand */
        if (portion == NULL)
            return OFF;
        while (is_operand_is_label(portion)) {
            portion = strtok(NULL, " ,\t\n"); /*portion = to the next operand*/
        }
        if (portion == NULL)
            return ON;
        else
            return OFF;
    }
    else {
        portion = strtok(current_line_temp, " \t\n"); /*portion = .extern\.entry */
        portion = strtok(NULL, " ,\t\n"); /*portion = suppose the first operand */
        if (portion == NULL)
            return OFF;
        while (is_operand_is_label(portion)) {
            portion = strtok(NULL, " ,\t\n"); /*portion = to the next operand*/

        }
        if (portion == NULL)
            return ON;
        else
            return OFF;
    }
}

/*----------------------------------instruction type------------------------------------*/

int two_operand_instruction(int ins_type) {
    if (ins_type == Mov || ins_type == Cmp || ins_type == Add || ins_type == Sub || ins_type == Lea)
        return ON;
    else
        return OFF;
}

int one_operand_instruction(int ins_type) {
    if (ins_type == Not || ins_type == Clr || ins_type == Inc || ins_type == Dec || ins_type == Jmp ||
        ins_type == Bne || ins_type == Red || ins_type == Prn || ins_type == Jsr)
        return ON;
    else
        return OFF;
}

/*----------------------------------sub operands checking------------------------------------*/
/* Group one - instructions : mov,add,sub */
int groupOne_operand_checker(char* portion, int second_operand_flag) {
    /* If it is a second operand, the checking is different. */
    if (portion == NULL)return OFF;
    if (second_operand_flag) {
        if (is_operand_is_label(portion))
            return Label;
        else if (is_operand_is_register(portion))
            return Register;
        else
            return OFF;
    }
    /*It is a first operand. */
    else {
        if (is_operand_is_label(portion))
            return Label;
        else if (is_operand_is_register(portion))
            return Register;
        else if (is_operand_is_number(portion))
            return Number;
        else
            return OFF;
    }
}

/* Group two - instructions : cmp */
int groupTwo_operand_checker(char* portion, int second_operand_flag) {
    /* The source and destination operand is the same and all the possibilities .*/
    if (portion == NULL)return OFF;
    if (is_operand_is_label(portion))
        return Label;
    else if (is_operand_is_register(portion))
        return Register;
    else if (is_operand_is_number(portion))
        return Number;
    else
        return OFF;
}

/* Group three - instructions : not,clr,inc,dec,jmp,bne,red,jsr */
int groupThree_operand_checker(char* portion, int second_operand_flag) {
    /*there is no second operand. */
    if (portion == NULL)return OFF;
    if (is_operand_is_label(portion))
        return Label;
    else if (is_operand_is_register(portion))
        return Register;
    else
        return OFF;
}

/* Group four - instructions : lea */
int groupFour_operand_checker(char* portion, int second_operand_flag) {
    if (portion == NULL)return OFF;
    if (second_operand_flag) {
        if (is_operand_is_label(portion))
            return Label;
        else if (is_operand_is_register(portion))
            return Register;
        else
            return OFF;
    }
    /*It is a first operand. */
    else {
        if (is_operand_is_label(portion))
            return Label;
        else
            return OFF;
    }
}

/* Group five - instructions : prn */
int groupFive_operand_checker(char* portion, int second_operand_flag) {
    if (portion == NULL)return OFF;
    if (is_operand_is_label(portion))
        return Label;
    else if (is_operand_is_register(portion))
        return Register;
    else if (is_operand_is_number(portion))
        return Number;
    else
        return OFF;
}

/*----------------------------------identifies and check each operand------------------------------------*/

int is_operand_is_label(char* portion) {
    int index;
    index = 0;
    if (portion == NULL)return OFF;
    if (!(isalpha(portion[index])))
        return OFF;
    while (portion[index] != '\0') {
        if ((!(isalpha(portion[index]))) && !(isdigit(portion[index])))
            return OFF;
        else
            index++;
    }
    return ON;
}

int is_operand_is_register(char* portion) {
    int index = 0;
    if (portion == NULL)return OFF;
    if (portion[index] != '@')return OFF;
    index++;
    if (portion[index] != 'r')return OFF;
    index++;
    if (portion[index] < '0' || portion[index] > '7')return OFF;
    return ON;
}

int is_operand_is_number(char* portion) {
    int index = 0;
    if (portion == NULL)return OFF;
    if (portion[index] == '-' || portion[index] == '+')
        index++;
    while (portion[index] != '\0') {
        if (isdigit(portion[index]))
            index++;
        else
            return OFF;
    }
    return ON;
}

/*----------------------------------Utils------------------------------------*/

void adding_space_to_label_ins_line(char* current_line) {
    char temp_one, temp_two;
    int index = 0;
    while (current_line[index] != ':' && current_line[index] != '\0')
        index++;
    if (current_line[index] == '\0')
        return;
    else if (current_line[index] == ':' && current_line[index + 1] == '.')
        return;
    else {
        index++;
        temp_one = current_line[index]; /*saving the current char */
        current_line[index] = ' ';
        index++;
        while (temp_two != '\0') {
            temp_two = current_line[index];
            current_line[index] = temp_one;
            temp_one = temp_two;
            index++;
        }
        current_line[index] = '\0';
    }

}

int is_line_exceed_length_limit(char* current_line, int line_number) {
    int length = strlen(current_line);
    if (length > MAX_SIZE_OF_LINE) {
        printf("Line exceeded length limit - line %d.\n", line_number);
        return ON;
    }
    else {
        return OFF;
    }
}



