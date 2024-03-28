#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#define RESET_STYLE        "\x1b[0m"
#define COLOR_BLACK        "\x1b[30m"
#define COLOR_RED          "\x1b[31m"
#define COLOR_GREEN        "\x1b[32m"
#define COLOR_YELLOW       "\x1b[33m"
#define COLOR_BLUE         "\x1b[34m"
#define COLOR_MAGENTA      "\x1b[35m"
#define COLOR_CYAN         "\x1b[36m"
#define COLOR_WHITE        "\x1b[37m"
#define BACKGROUND_BLACK   "\x1b[40m"
#define BACKGROUND_RED     "\x1b[41m"
#define BACKGROUND_GREEN   "\x1b[42m"
#define BACKGROUND_YELLOW  "\x1b[43m"
#define BACKGROUND_BLUE    "\x1b[44m"
#define BACKGROUND_MAGENTA "\x1b[45m"
#define BACKGROUND_CYAN    "\x1b[46m"
#define BACKGROUND_WHITE   "\x1b[47m"
#define STYLE_BOLD         "\x1b[1m"
#define STYLE_ITALIC       "\x1b[3m"
#define STYLE_UNDERLINE    "\x1b[4m"

unsigned int count_big_words(char *str, int max_line_length) {
    unsigned int word_length = 0;        // Length of the word
    unsigned int big_words = 0;          // Number of big words
    bool inside_ANSI = false;            // Flag to check if position is within an ANSI escape sequence
    // Iterate over the original string to check how many line breaks "\n" will be inserted and for long words 
    size_t str_len = strlen(str);
    for (size_t i = 0; i < str_len + 1; i++) {
        inside_ANSI = check_ANSI_start(str[i], inside_ANSI);
        // If is not inside ANSI sequence, perform the counting of big words
        if (inside_ANSI != true) {
            // Increase word_length if it don't find a space, a newline or the null terminator
            if ((str[i] != ' ') && (str[i] != '\n') && (str[i] != '\0')) {
                word_length++;
            } 
            else {
                // If a word_length is bigger than the limit, increase the number of big_words
                if (word_length > max_line_length) {
                    big_words++;
                }
                // Reset the word length
                word_length = 0;
            }   
        }
        else {
            // Check if reaches ANSI escape end
            inside_ANSI = check_ANSI_end(str[i], inside_ANSI);
        }
    }
    // Check the last word
    if (word_length > max_line_length) {
        big_words++;
    }

    return big_words;
}

char *wrap_text_with_small_words(char *str, int max_line_length) {
    // Get the size of the original string
    size_t str_len = strlen(str);
    // Make a duplicate of the string
    char *new_str = strdup(str);  // Size of the string + potential
    int last_space_position = 0;    // Monitor of the last space position
    int length_counter = 0;         // Counter to check max line length
    bool inside_ANSI = false;
    // Iterate over the original string to check how many line breaks "\n" will be inserted            
    for (size_t i = 0; i < str_len + 1; i++) {
        get_last_space_pos_and_length(str, i, &inside_ANSI, &last_space_position, &length_counter);
        // Check if length_counter reaches the limit of max_line_length and if it founds any space
        if (length_counter == max_line_length + 1 && last_space_position != 0) {
            // If reaches max_line_length, substitute the last space position with a new line char
            new_str[last_space_position] = '\n';
            // Reset monitor of the length counter to the position of the word after the space
            length_counter = i - last_space_position;
        }
    }

    return new_str;
}

void wrap_fprintf(FILE *file, const char *format, int max_line_length, ...) {
    // Declare list of variable arguments and start variable argument list
    va_list args;
    va_start(args, max_line_length); 
    //Determine the size needed for the output string
    int size = vsnprintf(NULL, 0, format, args) + 1;
    // Allocate memory to pass the formatted string
    char *buffer = malloc(size);
    ptr_safety_check(buffer, "char *buffer at wrap_printf(...) function");
    // Format the string
    vsnprintf(buffer, size, format, args);
    // End the variable argument list
    va_end(args);
    // Wrap the text
    char *wrapped_text = wrap_text(buffer, max_line_length);
    // Output the wrapped text
    fprintf(file, "%s", wrapped_text);
    // Free allocated memory
    free(buffer);
    free(wrapped_text);
}