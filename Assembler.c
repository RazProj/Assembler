#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "pre_assembler.h"
#include "syntax_pass.h"
#include "error_checker.h"
#include "second_pass.h"
#include "general.h"

/*
Culmination Project for the System Programming Lab course at the Open University of Israel.
The subject of this project is to construct an assembler for a 16-instruction assembly language,
designed for a 12-bit CPU with a memory capacity of 256 words. The creators of this project are @Raz_Solomon & @Malki_Jusewitz.
*/

int main(int argc, char* argv[]) {
    int i;
    for (i = 1; i < argc; i++) {
        process_filenames(argv[i]);
    }
    return 0;
}

