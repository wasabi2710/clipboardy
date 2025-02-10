#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <windows.h>
#include <string.h>
#include <stdio.h>

char* clipboard();
void writeToClipboard(char* clipboard);

#endif

