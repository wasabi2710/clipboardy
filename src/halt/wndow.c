#include <windows.h>
#include <string.h>
#include <stdio.h>

char* getClipText() {
    char* capText = NULL;

    // initialize clipboard
    if(!OpenClipboard(NULL)) {
        printf("Cannot Open Clipboard\n");
        return NULL;
    }

    // checking if the global mem has cf_text format
    if (CountClipboardFormats() != 0) {
        if(IsClipboardFormatAvailable(CF_TEXT)) {
            //get the clipboard data
            HGLOBAL hglobal = GetClipboardData(CF_TEXT);
            if (hglobal != NULL) {
                char* ptext = (char*)GlobalLock(hglobal);
                if (ptext != NULL) {
                    // allocate the mem for copying
                    capText = (char*)malloc(strlen(ptext) + 1);
                    if (capText != NULL) {
                        strcpy(capText, ptext);
                    }
                    GlobalUnlock(hglobal);
                }
            }
        }
    }

    CloseClipboard();
    return capText;
}
 
int main() {

    char* capText = "";
    char* prevText = "";

    while (1) { 
        Sleep(100);
        capText = getClipText();
        
        if (capText && (!prevText || strcmp(capText, prevText) != 0)) {
            printf("Captured: %s\n", capText);
            prevText = capText;
            //free prevText to renew
            free(prevText);
        } else {
            // free the captext allocation
            free(capText);
        }
    }    

    return 0;
}