#include "clipboard.h"

#ifdef __APPLE__
    #include <stdlib.h>
#elif __linux__
    #include <unistd.h>
#endif

void write_clipboard(char* clipboard) {
#ifdef __linux__   
    FILE* xclip = popen("xclip -selection clipboard", "w");
    if (xclip == NULL) {
        perror("Failed to open xclip");
        return;
    }
    fprintf(xclip, "%s", clipboard);
    pclose(xclip);

#elif __APPLE__
    FILE* pipe = popen("pbcopy", "w");
    if (!pipe) {
        perror("Failed to open pbcopy");
        return;
    }
    fprintf(pipe, "%s", clipboard);
    pclose(pipe);
#endif
}

char* clipboard() {
    char* rawData = NULL;

#ifdef __APPLE__
    // macOS clipboard handling using pbpaste
    FILE* pipe = popen("pbpaste", "r");
    if (!pipe) {
        perror("Failed to open pbpaste");
        return NULL;
    }

    size_t bufferSize = INITIAL_BUFFER_SIZE;
    size_t totalBytesRead = 0;
    size_t bytesRead = 0;
    rawData = (char*)malloc(bufferSize);
    if (rawData != NULL) {
        while ((bytesRead = fread(rawData + totalBytesRead, 1, bufferSize - totalBytesRead - 1, pipe)) > 0) {
            totalBytesRead += bytesRead;
            if (totalBytesRead >= bufferSize - 1) {
                bufferSize *= 2;
                rawData = (char*)realloc(rawData, bufferSize);
                if (rawData == NULL) {
                    perror("Failed to allocate memory");
                    break;
                }
            }
        }
        if (rawData != NULL) {
            rawData[totalBytesRead] = '\0'; // null-terminate the string
        }
    }
    pclose(pipe);

#elif __linux__
    // Linux clipboard handling using xclip
    FILE* xclip = popen("xclip -selection clipboard -o", "r");
    if (!xclip) {
        perror("Failed to open xclip");
        return NULL;
    }

    size_t bufferSize = INITIAL_BUFFER_SIZE;
    size_t totalBytesRead = 0;
    size_t bytesRead = 0;
    rawData = (char*)malloc(bufferSize);
    if (rawData != NULL) {
        while ((bytesRead = fread(rawData + totalBytesRead, 1, bufferSize - totalBytesRead - 1, xclip)) > 0) {
            totalBytesRead += bytesRead;
            if (totalBytesRead >= bufferSize - 1) {
                bufferSize *= 2;
                rawData = (char*)realloc(rawData, bufferSize);
                if (rawData == NULL) {
                    perror("Failed to allocate memory");
                    break;
                }
            }
        }
        if (rawData != NULL) {
            rawData[totalBytesRead] = '\0'; // null-terminate the string
        }
    }
    pclose(xclip);

#endif

    return rawData;
}
