#include "clipboard.h"
#include <string.h>
#include <stdio.h>

#ifdef __APPLE__
    #include <CoreFoundation/CoreFoundation.h>
    #include <ApplicationServices/ApplicationServices.h>
#elif __linux__
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <stdlib.h>
#endif

char* clipboard() {
    char* rawData = NULL;

#ifdef __APPLE__
    // macOS clipboard handling using pbpaste
    CFDataRef clipboardData = NULL;
    CFStringRef clipboardString = NULL;

    // Create the pasteboard object
    PasteboardRef pasteboard;
    CFStringRef pasteboardName = CFSTR("com.apple.general.pasteboard");
    OSStatus err = PasteboardCreate(pasteboardName, &pasteboard);
    
    if (err == noErr) {
        PasteboardSynchronize(pasteboard);
        ItemCount itemCount;
        PasteboardGetItemCount(pasteboard, &itemCount);

        if (itemCount > 0) {
            PasteboardItemID itemID;
            err = PasteboardGetItemIdentifier(pasteboard, 1, &itemID);
            if (err == noErr) {
                CFTypeRef dataType;
                err = PasteboardCopyItemFlavorData(pasteboard, itemID, kPasteboardFlavorTypeText, &dataType);
                if (err == noErr) {
                    clipboardString = (CFStringRef) dataType;
                    if (clipboardString) {
                        rawData = (char*)malloc(CFStringGetLength(clipboardString) + 1);
                        if (rawData != NULL) {
                            CFStringGetCString(clipboardString, rawData, CFStringGetLength(clipboardString) + 1, kCFStringEncodingUTF8);
                        }
                    }
                }
            }
        }
    }
    
    if (rawData == NULL) {
        return NULL;
    }

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
