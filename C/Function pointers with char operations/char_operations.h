#ifndef _char_operations_H
#define _char_operations_H

typedef char (*func_ptr)(char);

char lower_to_upper(char text);
char upper_to_lower(char text);
char space_to_dash(char text);
char reverse_letter(char text);

char* letter_modifier(const char *text, func_ptr ptr);

#endif
