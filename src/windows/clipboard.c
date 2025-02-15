#include "clipboard.h"

void writeToClipboard(char *clipboard) {
    if (!OpenClipboard(NULL)) {
        fprintf(stderr, "Failed OpenClipboard\n");
        return;
    }
    if (clipboard) {
        size_t clipLen = strlen(clipboard) + 1;
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, clipLen);
        if (!hMem) return;
        void* memPtr = GlobalLock(hMem);
        if (!memPtr) {
            GlobalFree(hMem);
            return;
        }
        memcpy(memPtr, clipboard, clipLen);
        GlobalUnlock(hMem);
        SetClipboardData(CF_TEXT, hMem);
    }
}

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