#ifndef WRAPPER_H
#define WRAPPER_H

char *wrap_text(char *str, int max_line_length);
void wrap_printf(const char *format, int max_line_length, ...);

#endif
