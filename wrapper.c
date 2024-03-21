#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#define MAX_PRINT_LEN 20

void ptr_safety_check(void *ptr, char *ptr_name) {
    if (ptr == NULL) {
        if (ptr_name != NULL) {
            printf("'%s' is a NULL pointer, please check the code\n", ptr_name);
        } 
        else {
            printf("NULL pointer found, please check the code\n");
        }
        printf("Exiting program...\n");
        exit(EXIT_FAILURE);
    }
}

void line_length_visualization(size_t maxlength) {
    for (size_t i = 0; i < maxlength; i++) {
        printf("=");
    }
    printf("\n");
}

bool check_ANSI_start(char character, bool current_ANSI_status) {
    /* This function checks if the position of the string is the start of an ANSI escape character */
    bool inside_ANSI = current_ANSI_status;
    // Check if the character is the first character of an ANSI escape sequence  
    if (character == '\x1b') {
        inside_ANSI = true;
    }
    return inside_ANSI;
}

bool check_ANSI_end(char character, bool current_ANSI_status) {
    /* This function checks if the position of the string is the end of an ANSI escape character 
       (this function must be called after all checks for inside_ANSI variable) */
    bool inside_ANSI = current_ANSI_status;
    // Check if position is inside an ANSI escape sequence and if it is the last position of the sequence
    if ((inside_ANSI == true) && (character == 'm')) {
        inside_ANSI = false;
    }
    return inside_ANSI;
}

bool is_a_big_word(char *str, int max_line_length, int last_space_position) {
    unsigned int word_length = 0;           // Size of the analyzed word
    bool inside_ANSI = false;               // Flag to determine if it is within an ANSI character
    // Iterate over the string from the initial position (where it has the last space) until it finds another space or "\0"
    int i = 0; // Account for the case where spaces have not been found yet 
    if (last_space_position != 0) { 
        // Put the position of the string after the last space
        i = last_space_position + 1;    
    } 
    while ((str[i] != ' ') && (str[i] != '\0') && (str[i] != '\n')){
        // Check if it is inside an ANSI escape character
        inside_ANSI = check_ANSI_start(str[i], inside_ANSI);
        // If it is a normal character, increase word length
        if (inside_ANSI == false) {
            word_length++;
        }
        inside_ANSI = check_ANSI_end(str[i], inside_ANSI);
        // Advance one position in the string
        i++;
    }
    // Determine if it is a big word or not
    if (word_length >= max_line_length) {       // Check later if it should be ">" (greater) or "=>" (greater or equal)
        return true;
    }
    else {
        return false;
    }
}

void get_last_space_pos_and_length(char *str, size_t i, bool *inside_ANSI, int *last_space_position, int *length_counter) {
    /* This function updates the last space position of the string and the length counter to control the line breaks */
    // Check if position is a ANSI escape character
    (*inside_ANSI) = check_ANSI_start(str[i], (*inside_ANSI));
    // If in position of normal string
    if ((*inside_ANSI) == false) {
        // Check if it is space
        if (str[i] == ' ') {
            // If is a space, hold as last space position
            (*last_space_position) = i;
        }
        // Increase the length counter
        (*length_counter)++;
    }
    // If position is inside a ANSI escape sequence and is in the last position of the sequence flag
    (*inside_ANSI) = check_ANSI_end(str[i], (*inside_ANSI));   
}

void put_newline_in_big_word(char *new_str, size_t *current_pos, size_t *advances, int *last_space_pos, int *length_counter) {
    // Put a newline character at the end of the line
    new_str[(*current_pos)] = '\n';
    (*advances)++;
    // Define last space position as the end of the big word and advance one character
    (*last_space_pos) = (*current_pos) - (*advances);
    (*current_pos)++;
    // Reset the monitor of the length (1 because of the advanced j)
    (*length_counter) = 1;
}

void put_newline_in_last_space(char *new_str, size_t original_str_pos, int last_space_pos, size_t advances, int *length_counter) {
    // Put a newline character in the last space character added by the number of additional newlines for big words
    new_str[last_space_pos + advances] = '\n';
    (*length_counter) = original_str_pos - last_space_pos;
}

char *wrap_text(char *str, int max_line_length) {
    // Safety check 
    ptr_safety_check(str, "argument 'char *str'");
    // If there is any big words, allocate memory accordingly
    size_t str_len = strlen(str);
    size_t new_str_len = str_len + 1 + (str_len/max_line_length); // Length of the string + space for \0 + additional space for line breaks of big words
    char *new_str = malloc(new_str_len);                          // This new string must be freed after the function call
    // Safety check
    ptr_safety_check(new_str, "'char *new_str'");
    // Declare some variables 
    int last_space_position = 0;    // Monitor of the last space position
    int length_counter = 0;         // Counter to check max line length
    bool inside_ANSI = false;       // Flag to determine if it is within an ANSI character
    size_t j = 0;                   // Position of the new string
    size_t advances = 0;            // Monitor of how many advances in memory were made
    bool dont_copy = false;         // Flag to not copy the string for some reason
    bool big_word = false;          // Flag to determine if the current word is bigger than the max_line_length
    // Iterate over the string
    for (size_t i = 0; i < str_len + 1; i++) {
        // Determine current line length, if it is inside an ANSI sequence and the last space position
        get_last_space_pos_and_length(str, i, &inside_ANSI, &last_space_position, &length_counter);
        // If reaches the length limit
        if (length_counter == max_line_length + 1) {
            // Check if it is a big word 
            if (str[i] == ' ') {    
                // If the current character is a space, the function is_a_big_word(...) will begin to count the word after the space, this line counteracts this problem
                big_word = false;
            } 
            else {
                big_word = is_a_big_word(str, max_line_length, last_space_position);
            }          
            if (big_word == true) {
                put_newline_in_big_word(new_str, &j, &advances, &last_space_position, &length_counter);
            } else {
                put_newline_in_last_space(new_str, i, last_space_position, advances, &length_counter);
                // If the newline character is inserted in the current line, flag to not copy the string later as it would write the space on top of the newline
                if (last_space_position == i) {
                    dont_copy = true;
                }
            } 
        }
        if (dont_copy == false) {
            // Copy str character to new_str if the flag is false
            new_str[j] = str[i];
        }
        else {
            // Else, reset flag
            dont_copy = false;
        }
        // Increase the index of new_str
        j++;
    }
    return new_str;
}

int main(void) {
    // Test strings
    char *str_1 = "\x1b[33m=>\x1b[32m This is a very very loong message that needs to be inserted into this program to test the split function. Lets make this string really big to test it properly.\x1b[0m\n";
    char *str_2 = "\x1b[33m=>\x1b[32m File \x1b[35m'luaguedesc/data/in/DRDs/very_long_input_file.txt' (25 bytes)\x1b[32m successfully loaded! I haveeDA also another input file to be loaded \x1b[35m'luaguedesc/data/DRDs/input_files/very_long_input_file_2.txt' (27 bytes)\x1b[32m that was successfully loaded! AndAFinalVeryVeryBigWordWithManyCharactersAndNoSpaces.\x1b[0m\n";
    char *str_3 = "\x1b[33m=>\x1b[32m dhaisdhiasudhuasihdiusahdiusahdhasiudsiuhdsauihdsuihdsaiuhdsaihudsaiuhdsauihsaduhiasdhuadsiuhdasihudiuasduhisaiuhdasuihdasuihuidasiuhuhiadsiuhasduihdaiudas.\x1b[0m\n";
    char *str_4 = "\x1b[33m=>\x1b[32m Vamos testar uma string muito grande, com palavras genericas e algumas muito grandes tipo essa PALAVRAMUITOGIGANTEPALAVRAMUITOGIGANTEPALAVRAMUITOGIGANTE para testar melhor essa função de wrap text com PALAVRAGRANDEPALAVRAGRANDEPALAVRAGRANDEPALAVRAGRANDEPALAVRAGRANDE e esse é o fim.\x1b[0m\n";   

    // Call wrapper
    line_length_visualization(MAX_PRINT_LEN);
    char *new_str_1 = wrap_text(str_1, MAX_PRINT_LEN);
    printf("%s", new_str_1);
    
    line_length_visualization(MAX_PRINT_LEN);
    char *new_str_2 = wrap_text(str_2, MAX_PRINT_LEN);
    printf("%s", new_str_2);
    
    line_length_visualization(MAX_PRINT_LEN);
    char *new_str_3 = wrap_text(str_3, MAX_PRINT_LEN);
    printf("%s", new_str_3);

    line_length_visualization(MAX_PRINT_LEN);
    char *new_str_4 = wrap_text(str_4, MAX_PRINT_LEN);
    printf("%s", new_str_4);
    
    // Free memory
    free(new_str_1);
    free(new_str_2);
    free(new_str_3);
    free(new_str_4);
}
