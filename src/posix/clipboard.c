#include "clipboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef CLIPBOARD_PBCOPY
#include <stdlib.h>

char* clipboard() {
    // For macOS, use pbpaste
    FILE *fp = popen("pbpaste", "r");
    if (fp == NULL) {
        perror("Failed to run pbpaste");
        return NULL;
    }

    // Find the size of the clipboard content
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Read the content from the clipboard
    char* result = (char*)malloc(size + 1);
    if (result != NULL) {
        fread(result, 1, size, fp);
        result[size] = '\0'; // Null-terminate
    }

    fclose(fp);
    return result;
}
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

char* clipboard() {
    Display* display = XOpenDisplay(NULL);
    if (display == NULL) {
        perror("Unable to open X display");
        return NULL;
    }

    Window root = DefaultRootWindow(display);
    Atom clipboardAtom = XInternAtom(display, "CLIPBOARD", False);
    Atom targetAtom = XInternAtom(display, "UTF8_STRING", False);
    Atom propertyAtom = XInternAtom(display, "CLIPBOARD", False);

    XConvertSelection(display, clipboardAtom, targetAtom, propertyAtom, root, CurrentTime);
    XFlush(display);

    XEvent event;
    XNextEvent(display, &event);

    if (event.xselection.property != None) {
        Atom type;
        int format;
        unsigned long len, bytesAfter;
        unsigned char* data = NULL;

        if (XGetWindowProperty(display, root, event.xselection.property, 0, ~0L, False, AnyPropertyType,
                               &type, &format, &len, &bytesAfter, &data) == Success) {
            if (data != NULL) {
                char* result = (char*)malloc(len + 1);
                if (result != NULL) {
                    memcpy(result, data, len);
                    result[len] = '\0';
                    XFree(data);
                    XCloseDisplay(display);
                    return result;
                }
            }
        }
    }

    XCloseDisplay(display);
    return NULL;
}
#endif
