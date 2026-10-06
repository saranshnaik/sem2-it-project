# File System Simulator

A basic command-line **File System Simulator** developed in C. The program simulates common terminal file and directory operations using dynamically allocated data structures.

## Features

- Create and remove directories
- Create and remove files
- Navigate between directories
- Display directory contents
- Display the current working directory
- Read file contents
- Edit file contents
- Search for directories
- Support for relative and absolute paths
- Built-in help command
- Alphabetical directory sorting
- Binary search for directories

## Available Commands

| Command | Description |
|---|---|
| `cd <path>` | Change the current working directory |
| `mkdir <path>` | Create a new directory |
| `rmdir <path>` | Remove a directory |
| `ls <path>` | List directories and files |
| `touch <path>` | Create a new file |
| `rm <path>` | Remove a file |
| `cat <path>` | Display the contents of a file |
| `gedit <path>` | Edit the contents of a file |
| `pwd` | Display the current working directory |
| `help` | Display available commands |
| `exit` | Exit the simulator |

These commands are implemented directly in the program's command loop and help system.

## Data Structures

The project uses several C data structures:

- **`File`** — stores file names and file contents.
- **`Directory`** — stores directory names, parent directories, subdirectories, and files.
- **`Path`** — represents path components as a linked list.

Directories are connected using parent pointers and arrays of subdirectories, forming a hierarchical file-system structure.

## Algorithms

The project demonstrates:

- Dynamic memory allocation using `malloc()` and `free()`
- Linked lists for path tokenization
- Hierarchical/tree-like directory structure
- Bubble sort for sorting directories
- Binary search for locating directories
- String manipulation and path parsing

Directory searches are performed after sorting the directory list and then using binary search.

## Technologies Used

- **C**
- **Standard C Library**
- **Dynamic Memory Allocation**
- **Data Structures & Algorithms**

## How to Run

Compile the program using a C compiler:

```bash
gcc main.c -o filesystem
```

Then run:

```bash
./filesystem
```

On Windows:

```bash
filesystem.exe
```

## Example

```text
/ > mkdir documents
/ > cd documents
/documents > touch notes.txt
/documents > gedit notes.txt
/documents > cat notes.txt
```

## Fun Fact

LLMs weren't as useful at the time, and yet just by looking at the code, the professor accused us of using an LLM to write it. So this is one project I'm really proud of, since my syntactic and formatting practices look very similar to professional LLM-generated code.

## Limitations

This is a simulated file system. Files and directories are maintained in memory using C data structures rather than being created as actual files and folders on the host operating system.

## Author

Saransh

## Purpose

This project was developed as an assignment submission for **C programming, pointers, dynamic memory allocation, linked lists, trees/hierarchical structures, string manipulation, sorting, and searching algorithms** for the course CS102 through a command-line file system simulation.
