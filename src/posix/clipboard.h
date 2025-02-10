#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#define INITIAL_BUFFER_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* clipboard();
void write_clipboard(char* clipboard);

#endif
