# Simple Shell

This is a simple shell implementation in C language. It supports basic commands like changing directories (cd), listing directory contents (ls), and running any other command available in the system's PATH environment variable. Additionally, it keeps track of the command history and allows the user to view previous commands by entering the 'history' command.

## Getting Started

### Prerequisites
The following tools are required to build and run this project:
* GCC (GNU Compiler Collection) 
* Make

### Installation
To install the project, run the following command in the terminal:
```
$ git clone https://github.com/[username]/simple-shell.git
```

### Building the project
To build the project, navigate to the project's root directory and run the following command:
```
$ make
```

### Running the project
To run the project, navigate to the project's root directory and run the following command:
```
$ ./shell [optional arguments]
```
The optional arguments are the directories to be added to the PATH environment variable.

## Features
* Run basic commands like cd, ls, and any other command available in the system's PATH environment variable.
* Keep track of command history.
* View previous commands by entering the 'history' command.
