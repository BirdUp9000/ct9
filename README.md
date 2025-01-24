# CT9 - Prefix Tree Text Autocomplete
CT9 is a project that implements a prefix tree (also known as a trie) data structure to provide text autocomplete functionality. The project utilizes the X11 library interface to display a text input field where users can interact with the autocomplete feature.

## Features
- Prefix tree (trie) data structure for efficient text autocomplete.
- X11 library interface for displaying the text input field.
- Autocomplete suggestions based on user input.
- Ability to test and experiment with the autocomplete functionality.

## Usage
To use the CT9 project, simply run the provided application. Users can then type into the text input field, and the application will display autocomplete suggestions based on the prefix of the user's input.

## Building
##### To build the CT9 project, follow these steps: <br>
Run CMake to generate the build files : <br> 
`cd build && cmake ..` <br>
Build the project <br>
`make` <br>
Run the application <br>
`./ct9`

## Requirements
- C++17 or newer
- CMake version 3.25 or higher
- X11 development libraries: Make sure the libX11 library is installed.
