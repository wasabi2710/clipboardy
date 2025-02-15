#include "clipboard.h"

void writeToClipboard(char *clipboard) {
    // Check if the input string is valid; return immediately if NULL.
    if (!clipboard) {
        fprintf(stderr, "Error: Null string passed to writeToClipboard().\n");
        return;
    }

    // Attempt to open the clipboard to gain exclusive access.
    if (!OpenClipboard(NULL)) {
        fprintf(stderr, "Error: Failed to open clipboard.\n");
        return;
    }

    // Clear any existing content in the clipboard.
    // This ensures that the clipboard is in a clean state before setting new data.
    EmptyClipboard();

    // Determine the length of the input string, including the null terminator.
    size_t clipLen = strlen(clipboard) + 1; // +1 for the null terminator

    // Allocate a block of global memory that can be moved by the system.
    // GMEM_MOVEABLE allows Windows to move the memory if needed.
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, clipLen);
    if (!hMem) {
        fprintf(stderr, "Error: Failed to allocate global memory for clipboard.\n");
        CloseClipboard(); // Ensure clipboard is closed before returning.
        return;
    }

    // Lock the allocated memory to get a pointer to its contents.
    void *memPtr = GlobalLock(hMem);
    if (!memPtr) {
        fprintf(stderr, "Error: Failed to lock allocated memory.\n");
        GlobalFree(hMem); // Free memory since we failed to lock it.
        CloseClipboard();
        return;
    }

    // Copy the input string into the allocated memory.
    memcpy(memPtr, clipboard, clipLen);

    // Unlock the memory block to allow the system to manage it.
    GlobalUnlock(hMem);

    // Set the clipboard data. The clipboard now takes ownership of the memory.
    // We do not free hMem afterward, as the system will manage it.
    if (!SetClipboardData(CF_TEXT, hMem)) {
        fprintf(stderr, "Error: Failed to set clipboard data.\n");
        GlobalFree(hMem); // If SetClipboardData fails, we must manually free the memory.
    }

    // Close the clipboard to release exclusive access.
    CloseClipboard();
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