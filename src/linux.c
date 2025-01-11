#include <X11/Xlib.h>  // Core X11 functions for interacting with the display server
#include <X11/Xatom.h> // Atom-related functions for managing X11 properties
#include <stdio.h>     // Standard I/O functions
#include <stdlib.h>    // Standard library functions (e.g., malloc, free)
#include <string.h>    // String manipulation functions (e.g., strdup, strcmp)
#include <unistd.h>    // For the sleep() function

// Function to get clipboard text from the X11 server
char* get_clipboard_text(Display* display) {
    // Define the atoms we need
    Atom clipboard = XInternAtom(display, "CLIPBOARD", False);    // The X11 clipboard atom
    Atom utf8_string = XInternAtom(display, "UTF8_STRING", False); // Format: UTF-8 encoded text
    Atom target_property = XInternAtom(display, "XSEL_DATA", False); // Temporary property to hold clipboard data

    // Get the root window for the display
    Window window = XDefaultRootWindow(display);

    // Request clipboard content from the clipboard owner
    XConvertSelection(display, clipboard, utf8_string, target_property, window, CurrentTime);

    // Wait for the request to complete
    XSync(display, False);

    // Variables to retrieve clipboard data
    Atom actual_type;         // The actual data type returned
    int actual_format;        // The format of the returned data
    unsigned long nitems;     // Number of items in the returned data
    unsigned long bytes_after; // Bytes remaining after the requested data
    unsigned char* data = NULL; // Pointer to the returned data

    // Retrieve the property where the clipboard data is stored
    XGetWindowProperty(display, window, target_property, 0, (~0L), False,
                       AnyPropertyType, &actual_type, &actual_format,
                       &nitems, &bytes_after, &data);

    // If no data is returned, return NULL
    if (actual_type == None) {
        return NULL;
    }

    // Copy the clipboard data to a new string (so we can manage its memory independently)
    char* result = NULL;
    if (data) {
        result = strdup((char*)data); // Duplicate the data into a null-terminated string
        XFree(data);                 // Free the X11-allocated memory for the clipboard data
    }

    return result; // Return the clipboard text
}

int main() {
    // Open a connection to the X11 display server
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Unable to open X display\n"); // Print error if display can't be opened
        return 1; // Exit with error code
    }

    char* prev_text = NULL; // Store the last clipboard text to detect changes

    while (1) {
        sleep(1); // Poll the clipboard every second

        // Get the current clipboard text
        char* text = get_clipboard_text(display);

        // If new clipboard text is different from the previous one, print it
        if (text && (!prev_text || strcmp(text, prev_text) != 0)) {
            printf("Captured: %s\n", text);

            free(prev_text); // Free the memory for the previous clipboard text
            prev_text = text; // Update the previous text to the new clipboard text
        } else {
            free(text); // Free the memory for the newly fetched text (not used)
        }
    }

    // Close the connection to the X11 display server (unreachable in this infinite loop)
    XCloseDisplay(display);
    return 0;
}
