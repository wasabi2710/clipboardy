#include "clipboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
    #include <stdlib.h>
#elif __linux__
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>
    #include <X11/Xutil.h>
    #include <unistd.h>
#endif

char* clipboard() {
    char* rawData = NULL;

#ifdef __APPLE__
    // macOS clipboard handling using pbpaste
    FILE* pipe = popen("pbpaste", "r");
    if (!pipe) {
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
    // Linux clipboard handling using X11
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        perror("Unable to open X display");
        return NULL;
    }

    Window window = DefaultRootWindow(display);
    Atom clipboardAtom = XInternAtom(display, "CLIPBOARD", False);
    Atom utf8StringAtom = XInternAtom(display, "UTF8_STRING", False);
    Atom type;
    int format;
    unsigned long length, remaining;
    unsigned char *data = NULL;

    Atom property = XInternAtom(display, "XSEL_DATA", False);
    if (XConvertSelection(display, clipboardAtom, utf8StringAtom, property, window, CurrentTime) == Success) {
        XFlush(display);
        if (XGetWindowProperty(display, window, property, 0, (~0L), False, utf8StringAtom,
                               &type, &format, &length, &remaining, &data) == Success) {
            if (data) {
                rawData = (char *)malloc(length + 1);
                if (rawData != NULL) {
                    memcpy(rawData, data, length);
                    rawData[length] = '\0';
                }
                XFree(data);
            }
        }
    }

    XCloseDisplay(display);
#endif

    return rawData;
}
