#include "clipboard.h"

#ifdef __APPLE__
    #include <stdlib.h>
#elif __linux__
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <stdlib.h>
#endif

char* clipboard() {
    char* rawData = NULL;

#ifdef __APPLE__

    FILE* fp = popen("pbpaste", "r");
    if (fp == NULL) {
        perror("Failed to open pbpaste");
        return NULL;
    }

    rawData = (char*)malloc(BUFFER_SIZE);
    if (rawData == NULL) {
        perror("Memory allocation failed");
        fclose(fp);
        return NULL;
    }

    size_t bytesRead = fread(rawData, 1, BUFFER_SIZE - 1, fp);
    if (bytesRead > 0) {
        rawData[bytesRead] = '\0';  // Null-terminate the string
    } else {
        free(rawData);
        rawData = NULL;
    }

    fclose(fp);

#elif __linux__
    // Linux clipboard handling using X11
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        perror("Unable to open X display");
        return NULL;
    }

    Window window = DefaultRootWindow(display);
    Atom clipboardAtom = XInternAtom(display, "CLIPBOARD", False);
    Atom type;
    int format;
    unsigned long length, remaining;
    unsigned char *data = NULL;

    Atom property = XInternAtom(display, "PRIMARY", False);
    if (XGetWindowProperty(display, window, property, 0, (~0L), False, AnyPropertyType,
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

    XCloseDisplay(display);
#endif

    return rawData;
}