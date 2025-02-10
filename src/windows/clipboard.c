#include "clipboard.h"
#include <string.h>
#include <windef.h>
#include <windows.h>
#include <wingdi.h>

char* clipboard() {

    // init clipboard
    if(!OpenClipboard(NULL)) {
        perror("Failed OpenClibpoard");
        return NULL;
    }

    char* rawData = NULL;

    if (CountClipboardFormats() != 0) {
        //CF_TEXT
        if (IsClipboardFormatAvailable(CF_TEXT) != 0) {
            HGLOBAL clipboardData = GetClipboardData(CF_TEXT);
            if (clipboardData != NULL) {
                char* lockedClipData = (char *)GlobalLock(clipboardData); //lock the data
                if (lockedClipData != NULL) {
                    rawData = (char *)malloc(strlen(lockedClipData) + 1);
                    if (rawData != NULL) {
                        strcpy(rawData, lockedClipData);
                    }
                    GlobalUnlock(clipboardData); //unlock the mem
                }
            }
        } 
    }

    //cleanup
    CloseClipboard();
    return rawData;
}