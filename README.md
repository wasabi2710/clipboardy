# Clipboardy - A Clipboard Broadcasting Application

Clipboardy is a simple cross-platform application that monitors clipboard changes and broadcasts the clipboard data over a network. 

## Features

- Monitors clipboard changes and relays data over the network.
- Uses UDP to broadcast clipboard data to other devices on the network.

## Build Requirements
- Cmake >= 3.25.x
  
### Windows
- MSVC and Universal C Runtime Standard Library

### POSIX
- GNU Compiler Collection
- Clipboard Commandline Utilities (pbcopy/pbpaste, xclip)

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/wasabi2710/clipboardy.git
2. Run cmake build command or the provided scripts

## Notes and Considerations

1. It is recommended to specify the direct broadcast address for your desired subnet in the server.h header file, rather than using a general broadcast address.
2. Please note that encryption is not implemented, so proceed with caution!
3. There is only one implemented format (CF_TEXT) for Windows
