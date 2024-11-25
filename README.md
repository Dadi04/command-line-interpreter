
# Simple Command Line Interpreter

### Overview
This project implements a basic Command-Line Interpreter (CLI) as part of the requirements for the Object-Oriented Programming course at the Faculty of Electrical Engineering, University of Belgrade. The interpreter supports a subset of commands found in common operating systems, allowing users to execute commands interactively or in batch mode.

### Features
The CLI interpreter supports the following functionalities:

* Interactive Command Input: Users can enter commands interactively via the console.
* Batch Command Input: Commands can be loaded and executed from a text file.
* Redirection and Piping: Support for input and output redirection and command piping.
* Error Handling: Includes error handling for incorrect command syntax, unknown commands, and other execution-related errors.

### Commands
The interpreter currently supports the following commands:

* echo [argument]: Outputs the text provided as an argument.
* prompt argument: Changes the command prompt symbol.
* time: Displays the current system time.
* date: Displays the current system date.
* touch argument: Creates an empty file.
* truncate argument: Clears the contents of a specified file.
* rm argument: Deletes a specified file.
* wc [option] [argument]: Counts words (-w) or characters (-c) in a text.
* tr [argument] what [with]: Replaces or removes specific characters in a text.
* head -ncount [argument]: Displays the first few lines of a text.
* batch [argument]: Executes a series of commands from a file.

### Usage

todo

### Examples

1. Echo Command:
    ```bash
    $ echo "Hello, World!"
    ```
    Output:
    ```bash
    Hello, World!
    ```
2. Word Count (wc) Command:
    ```bash
    $ wc -w "Lorem ipsum dolor sit amet"
    ```
    Output:
    ```bash
    5
    ```
3. Redirection:
    ```bash
    $ wc -w "Sample text" > output.txt
    ```
4. Piping:
    ```bash
    $ time | tr ":" "." | wc -c > time.txt
    ```

### Error Handling

The interpreter provides feedback for incorrect commands or syntax errors. Some examples include:

todo
