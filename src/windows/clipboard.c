#include "clipboard.h"

void writeToClipboard(char *clipboard) {
    if (!clipboard) {
        fprintf(stderr, "Error: Null string passed to writeToClipboard().\n");
        return;
    }

    if (!OpenClipboard(NULL)) {
        fprintf(stderr, "Error: Failed to open clipboard.\n");
        return;
    }

    EmptyClipboard();

    size_t clipLen = strlen(clipboard) + 1; // +1 for the null terminator

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, clipLen);
    if (!hMem) {
        fprintf(stderr, "Error: Failed to allocate global memory for clipboard.\n");
        CloseClipboard();
        return;
    }

    void *memPtr = GlobalLock(hMem);
    if (!memPtr) {
        fprintf(stderr, "Error: Failed to lock allocated memory.\n");
        GlobalFree(hMem);
        CloseClipboard();
        return;
    }

    memcpy(memPtr, clipboard, clipLen);

    GlobalUnlock(hMem);

    if (!SetClipboardData(CF_TEXT, hMem)) {
        fprintf(stderr, "Error: Failed to set clipboard data.\n");
        GlobalFree(hMem);
    }

    CloseClipboard();
}

char* clipboard() {
    if(!OpenClipboard(NULL)) {
        perror("Failed OpenClipboard");
        return NULL;
    }

    char* rawData = NULL;
    if (CountClipboardFormats() != 0) {
        if (IsClipboardFormatAvailable(CF_TEXT) != 0) {
            HGLOBAL clipboardData = GetClipboardData(CF_TEXT);
            if (clipboardData != NULL) {
                char* lockedClipData = (char *)GlobalLock(clipboardData);
                if (lockedClipData != NULL) {
                    rawData = (char *)malloc(strlen(lockedClipData) + 1);
                    if (rawData != NULL) {
                        strcpy(rawData, lockedClipData);
                    }
                    GlobalUnlock(clipboardData);
                }
            }
        }
    }

    CloseClipboard();
    return rawData;
}
