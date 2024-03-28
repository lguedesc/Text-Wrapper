#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrapper.h"

#define MAX_PRINT_LEN 20

void line_length_visualization(char chr, size_t maxlength) {
    for (size_t i = 0; i < maxlength; i++) {
        printf("%c", chr);
    }
    printf("\n");
}

void fline_length_visualization(FILE* file, char chr, size_t maxlength) {
    for (size_t i = 0; i < maxlength; i++) {
        fprintf(file, "%c", chr);
    }
    fprintf(file, "\n");
}

int main(void) {
    // Test strings
    char *str_1 = "\x1b[33m=>\x1b[32m This is a very very loong message that needs to be inserted into this program to test the split function. Lets make this string really big to test it properly.\x1b[0m\n";
    char *str_2 = "\x1b[33m=>\x1b[32m File \x1b[35m'luaguedesc/data/in/DRDs/very_long_input_file.txt' (25 bytes)\x1b[32m successfully loaded! I have also another input file to be loaded \x1b[35m'luaguedesc/data/DRDs/input_files/very_long_input_file_2.txt' (27 bytes)\x1b[32m that was successfully loaded! AndAFinalVeryVeryBigWordWithManyCharactersAndNoSpaces.\x1b[0m\n";
    char *str_3 = "\033[33m=>\033[32m TestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTest\033[0m\n";

    // Call wrapper   
    printf("\nWRAP_TEXT TEST:\n");
    line_length_visualization('-', MAX_PRINT_LEN);
    char *new_str_1 = wrap_text(str_1, MAX_PRINT_LEN);
    printf("%s", new_str_1);
    
    line_length_visualization('-', MAX_PRINT_LEN);
    char *new_str_2 = wrap_text(str_2, MAX_PRINT_LEN);
    printf("%s", new_str_2);
    
    line_length_visualization('-', MAX_PRINT_LEN);
    char *new_str_3 = wrap_text(str_3, MAX_PRINT_LEN);
    printf("%s", new_str_3);

    printf("\nWRAP_PRINTF TEST:\n");
    line_length_visualization('-', MAX_PRINT_LEN);
    wrap_printf("%s\n", MAX_PRINT_LEN, str_1);
    line_length_visualization('-', MAX_PRINT_LEN);
    wrap_printf("%s", MAX_PRINT_LEN, str_2);
    line_length_visualization('-', MAX_PRINT_LEN);
    wrap_printf("%s", MAX_PRINT_LEN, str_3);

    // Free memory
    free(new_str_1);
    free(new_str_2);
    free(new_str_3);
}
