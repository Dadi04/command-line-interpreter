
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

- Interactive Mode

    Compile the project:
    Use a C++ compiler (with C++11 or higher support) to compile the source code. For example:
    ```bash
        g++ -std=c++11 -o cli Interpreter.cpp
    ```
    Run the executable:
    Launch the CLI interpreter by running:
    ```bash
        ./cli
    ```
    The interpreter will display a prompt (default is $), and you can start entering commands.

- Batch Mode

    To execute a list of commands stored in a text file, use the batch command followed by the file name:
    ```bash
        $ batch "commands.txt"
    ```
    The file commands.txt should contain one command per line. The interpreter will process these commands sequentially.

- Input/Output Redirection and Piping
    1. Redirection:
        You can redirect the output of a command to a file using the > operator:
        ```bash
            $ wc -w "Sample text" > output.txt
        ```
        
        Similarly, input redirection can be done using <:
        ```bash
            $ wc -w < input.txt
        ```
        
    2. Piping:
        Connect the output of one command directly into the input of another using the pipe symbol (|):
        ```bash
            $ time | tr ":" "." | wc -c > time.txt
         ```

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

The interpreter is designed to provide informative feedback when errors occur. Errors may arise during lexical analysis, syntax checking, or execution. Here are the main categories:

1. #### Lexical Errors:
If invalid characters or unexpected symbols are encountered in the command, the interpreter will print an error message indicating the position of the first invalid character. For example:
```bash
    $ wc& -w "Lorem ipsum" +?
    Error - unexpected characters:
    wc& -w "Lorem ipsum" +?
      ^
```

2. #### Uknown Commands: 
If the entered command is not recognized, the interpreter outputs a message such as:
```bash
    Unknown command: [command]
```

3. #### Syntax Errors:
If the command format does not adhere to the specified structure (e.g., missing quotes for arguments with spaces), an appropriate syntax error message is displayed.

4. #### Execution Errors:
Errors during execution (such as attempting to delete a non-existent file or issues with file permissions) are caught, and corresponding error messages are shown.

5. #### Redirection/Piping Conflicts:
If a command incorrectly combines arguments and redirections (or pipes), the interpreter will reject the command and print an error message, ensuring that the input or output stream is not ambiguously defined.
