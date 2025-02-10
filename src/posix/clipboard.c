#include "clipboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

char* clipboard() {
    // Open the X11 display
    Display* display = XOpenDisplay(NULL);
    if (display == NULL) {
        perror("Unable to open X display");
        return NULL;
    }

    // Get the root window and clipboard selection
    Window root = DefaultRootWindow(display);
    Atom clipboardAtom = XInternAtom(display, "CLIPBOARD", False);
    Atom targetAtom = XInternAtom(display, "UTF8_STRING", False);
    Atom propertyAtom = XInternAtom(display, "CLIPBOARD", False);

    // Request clipboard content
    XConvertSelection(display, clipboardAtom, targetAtom, propertyAtom, root, CurrentTime);
    XFlush(display);

    // Wait for the clipboard content to be available
    XEvent event;
    XNextEvent(display, &event);

    // If clipboard content is available, fetch the data
    if (event.xselection.property != None) {
        Atom type;
        int format;
        unsigned long len, bytesAfter;
        unsigned char* data = NULL;

        // Get the clipboard data
        if (XGetWindowProperty(display, root, event.xselection.property, 0, ~0L, False, AnyPropertyType,
                               &type, &format, &len, &bytesAfter, &data) == Success) {
            if (data != NULL) {
                // Allocate space for the clipboard content and copy it
                char* result = (char*)malloc(len + 1);
                if (result != NULL) {
                    memcpy(result, data, len);
                    result[len] = '\0'; // Null-terminate the string
                    XFree(data); // Free the X11 data
                    XCloseDisplay(display); // Close the X11 display
                    return result;
                }
            }
        }
    }

    // Cleanup and return NULL if clipboard content isn't found
    XCloseDisplay(display);
    return NULL;
}
