#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct File {
    char name[50];
    char content[1000];
} File;

typedef struct Directory {
    char name[50];
    struct Directory *parent;
    struct Directory *subdirectories[50];
    File *files[50];
    int num_subdirectories;
    int num_files;
} Directory;

typedef struct Path {
    char *name;
    struct Path *next;    
} Path;

Path *tokenize(const char *path);
Directory *initialize_directory(const char *name, Directory *parent);
void cd(Directory **current_directory, const char *path);
void mkdir(Directory *current_directory, const char *path);
void rmdir(Directory *current_directory, const char *path);
void ls(Directory *current_directory, const char *path);
void touch(Directory *current_directory, const char *filename);
void rm(Directory *current_directory, const char *filename);
void cat(Directory *current_directory, const char *filename);
void gedit(Directory *current_directory, const char *filename);
void pwd(Directory *current_directory);
void help();
void sortDirectory(Directory *current_directory);
int binarySearch(Directory *directories[], int low, int high, const char *name);
int searchDirectory(Directory *current_directory, char *name);

Directory *root_directory = NULL;
Directory *current_directory = NULL;

int main() {
    root_directory = initialize_directory("/", NULL);
    current_directory = root_directory;
    while (1) {
        char input[100];
        pwd(current_directory);
        printf(">");
        scanf("%[^\n]s", input); while (getchar() != '\n');
        
        char temp1[100], temp2[100];
        strcpy(temp1, input); strcpy(temp2, input);
        char *command = strtok(temp1, " ");
        char *path = strtok(temp2, " "); path = strtok(NULL, " ");
        
        if (path != NULL && strcmp(path, "/") == 0 && (strcmp(command, "mkdir") == 0 || strcmp(command, "rmdir") == 0 || strcmp(command, "touch") == 0 || strcmp(command, "cat") == 0 || strcmp(command, "rm") == 0 || strcmp(command, "gedit") == 0)) {
            printf("Cannot modify root directory.\n");
            continue;
        }
        Directory *current = current_directory;
        if (path != NULL && path[0] == '/' && path[1] != '\0') {
            current = root_directory;
            char pathcopy[100];
            int i = 0;
            for (i = 0; path[i] != '\0'; i++) {
                pathcopy[i] = path[i + 1];
            }
            pathcopy[++i] = '\0'; 
            strcpy(path, pathcopy);
        }
        if (strcmp(command, "cd") == 0) {
            if (path == NULL) {
                printf("No path specified.\n");
                continue;
        }
            cd(&current_directory, path);
        }
        else if (strcmp(command, "mkdir") == 0) {
            if (path == NULL) {
                printf("No path specified.\n");
                continue;
        }
            mkdir(current, path);
        }
        else if (strcmp(command, "rmdir") == 0) {
            if (path == NULL) {
                printf("No path specified.\n");
                continue;
        }
            rmdir(current, path);
        }
        else if (strcmp(command, "ls") == 0) {
            ls(current, path);
        }
        else if (strcmp(command, "touch") == 0) {   
            if (path == NULL) {
                printf("No path specified.\n");
                continue;
        }
            touch(current, path);
        }
        else if (strcmp(command, "rm") == 0) {
            if (path == NULL) {
                printf("No path specified.\n");
                continue;
        }   
            rm(current, path);
        }
        else if (strcmp(command, "cat") == 0) {
            if (path == NULL) {
                printf("No path specified.\n");
                continue;
        }
            cat(current, path);
        }
        else if (strcmp(command, "gedit") == 0) {   
            if (path == NULL) {
                printf("No path specified.\n");
                continue;
        }
            gedit(current, path);
        }
        else if (strcmp(input, "pwd") == 0) {
            pwd(current_directory);
            printf("\n");
        }
        else if (strcmp(input, "help") == 0) {
            help();
        }
        else if (strcmp(input, "exit") == 0) {
            break;
        }
        else {
            printf("\'%s\' is not a recognized command.\n", command);
            printf("Enter \'help\' to view list of available commands.\n");
        }
    }
    return 0;
}

Path *tokenize(const char *path) {
    char path_copy[100];
    strcpy(path_copy, path);
    char *token = strtok(path_copy, "/");
    if (path[0] == '.' && path[1] == '/') {
        token = strtok(NULL, "/");
    }
    Path *head = NULL;
    Path *ptr = NULL;
    while (token != NULL) {
        Path *new = (Path *) malloc(sizeof(Path));
        new->name = (char *) malloc(strlen(token) + 1);
        strcpy(new->name, token);
        new->next = NULL;
        if (head == NULL) {
            head = new;
            ptr = head;
        }
        else {
            ptr->next = new;
            ptr = ptr->next;
        
        }
        if (strcmp(token, ".") == 0) {
            token = strtok(NULL, "/");
        }
        token = strtok(NULL, "/");
    }
    return head;
}

// Function to initialize a directory
Directory *initialize_directory(const char *name, Directory *parent) {
    Directory *new_directory = (Directory *) malloc(sizeof(Directory));
    strcpy(new_directory->name, name);
    new_directory->parent = parent;
    new_directory->num_subdirectories = 0;
    new_directory->num_files = 0;
    return new_directory;
}

// Function to change directory
void cd(Directory **current_directory, const char *path) {
    char path_copy[100];
    if (strcmp(path, "/") == 0){
        *current_directory = root_directory;
        return;
    }
    strcpy(path_copy, path);
    Path *p = tokenize(path_copy);
    Directory *current = *current_directory;
    while (p != NULL) {
        int index = searchDirectory(current, p->name);
        if (index != -1) {
            current = current->subdirectories[index];
        }
        else if (strcmp(p->name, "..") == 0 && current->parent != NULL) {
            current = current->parent;
        }
        else if (index == -1) {
            printf("Directory \'%s\' not found.\n", p->name);
            return;
        }
        p = p->next;
    }
    *current_directory = current;
}

void mkdir(Directory *current_directory, const char *path) {
    char path_copy[100];
    strcpy(path_copy, path);
    Path *p = tokenize(path_copy);
    Path *q = p->next;
    Directory *current = current_directory;
    while (p->next != NULL) {
        int index = searchDirectory(current, p->name);
        if (index != -1 && q != NULL) {
            current = current->subdirectories[index];
        }
        else if (index == -1 && q != NULL) {
            printf("Directory \'%s\' not found.\n", p->name);
            return;
        }
        p = p->next;
        q = q->next;
    }
    if (current->num_subdirectories >= 50) {
        printf("Cannot create file. Maximum file limit reached.\n");
        return;
    }
    if (searchDirectory(current, p->name) != -1) {
        printf("Directory \'%s\' already exists.\n", p->name);
        return;
    }
    Directory *new_dir = initialize_directory(p->name, current);
    current->subdirectories[current->num_subdirectories++] = new_dir;
    current = new_dir;
    printf("Directory successfully created.\n");
    current_directory = current;
}

// Function to remove a directory
void rmdir(Directory *current_directory, const char *path) {
    char path_copy[100];
    strcpy(path_copy, path);
    Path *p = tokenize(path_copy);
    Directory *current = current_directory;
    int index;
    while (p != NULL) {
        index = searchDirectory(current, p->name);
        if (index != -1) {
            current = current->subdirectories[index];
            p = p->next;
        }
        else {
            printf("Directory \'%s\' not found.\n", p->name);
            return;
        }
    }
    
    current_directory = current->parent;
    Directory *new = current->parent;
    for (int j = index; j < new->num_subdirectories - 1; j++) {
        new->subdirectories[j] = new->subdirectories[j + 1];
    }
    current->parent->num_subdirectories--;
    free(current);
    printf("Directory successfully removed.\n");
}

// Function to list directory contents
void ls(Directory *current_directory, const char *path) {
    Directory *current = current_directory;
    if (path != NULL) { 
        char path_copy[100];
        strcpy(path_copy, path);
        Path *p = tokenize(path_copy);
        while (p != NULL) {
            int index = searchDirectory(current, p->name);
            if (index != -1) {
                current = current->subdirectories[index];
            }
            else {
                printf("Directory \'%s\' not found.\n", p->name);
                return;
            }
            p = p->next;
        }
    }
    printf("Directory listing for "); pwd(current); printf(":\n");
    if (current->num_subdirectories == 0) {
        printf("(no directories) ");
    }
    for (int i = 0; i < current->num_subdirectories; i++) {
        printf("\033[0;34m%s  ", current->subdirectories[i]->name);
    }
    printf("\033[0m");
    if (current->num_files == 0) {
        printf("(no files) ");
    }
    for (int i = 0; i < current->num_files; i++) {
        printf("%s  ", current->files[i]->name);
    }
    printf("\033[0m\n");
}

// Function to create a new file
void touch(Directory *current_directory, const char *path) {
    char path_copy[100];
    strcpy(path_copy, path);
    Path *p = tokenize(path_copy);
    Path *q = p->next;
    Directory *current = current_directory;
    while (p->next != NULL) {
        int index = searchDirectory(current, p->name);
        if (index != -1 && q != NULL) {
            current = current->subdirectories[index];
        }
        else if (index == -1 && q != NULL) {
            printf("Directory \'%s\' not found.\n", p->name);
            return;
        }
        p = p->next;
        q = q->next;
    }
    if (current->num_files >= 50) {
        printf("Cannot create file. Maximum file limit reached.\n");
        return;
    }
    char filename[20];
    strcpy(filename, p->name);
    for (int i = 0; i < current->num_files; i++) {
        if (strcmp(current->files[i]->name, filename) == 0) {
            printf("File \'%s\' already exists.\n", filename);
            return;
        }
    }

    File *new_file = (File *) malloc(sizeof(File));
    strcpy(new_file->name, filename);
    strcpy(new_file->content, "");
    current->files[current->num_files++] = new_file;
    printf("File created successfully.\n");
}

// Function to remove a file
void rm(Directory *current_directory, const char *path) {
    char path_copy[100];
    strcpy(path_copy, path);
    Path *p = tokenize(path_copy);
    Path *q = p->next;
    Directory *current = current_directory;
    while (p->next != NULL) {
        int index = searchDirectory(current, p->name);
        if (index != -1 && q != NULL) {
            current = current->subdirectories[index];
        }
        else if (index == -1 && q != NULL) {
            printf("Directory \'%s\' not found.\n", p->name);
            return;
        }
        p = p->next;
        q = q->next;
    }
    char filename[20];
    strcpy(filename, p->name);
    int found = 0, index;
    for (int i = 0; i < current->num_files; i++) {
        if (strcmp(current->files[i]->name, filename) == 0) {
            found = 1;
            index = i;
            break;
        }
    }
    if (found) {
        free(current->files[index]);
        for (int i = index; i < current->num_files - 1; i++) {
            current->files[i] = current->files[i + 1];
        }
        current->num_files--;
        printf("File removed successfully.\n");
    } else {
        printf("File \'%s\' not found.\n", filename);
    }
}

// Function to display the content of a file
void cat(Directory *current_directory, const char *path) {
    char path_copy[100];
    strcpy(path_copy, path);
    Path *p = tokenize(path_copy);
    Path *q = p->next;
    Directory *current = current_directory;
    while (p->next != NULL) {
        int index = searchDirectory(current, p->name);
        if (index != -1 && q != NULL) {
            current = current->subdirectories[index];
        }
        else if (index == -1 && q != NULL) {
            printf("Directory \'%s\' not found.\n", p->name);
            return;
        }
        p = p->next;
        q = q->next;
    }
    char filename[20];
    strcpy(filename, p->name);
    int found = 0;
    for (int i = 0; i < current->num_files; i++) {
        if (strcmp(current->files[i]->name, filename) == 0) {
            printf("Content of file \'%s\': %s\n", filename, current->files[i]->content);
            if (strcmp(current->files[i]->content, "") == 0) {
                printf("(file empty)\n");
            }
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("File \'%s\' not found.\n", filename);
    }
}

// Function to edit the content of a file
void gedit(Directory *current_directory, const char *path) {
    char path_copy[100];
    strcpy(path_copy, path);
    Path *p = tokenize(path_copy);
    Path *q = p->next;
    Directory *current = current_directory;
    while (p->next != NULL) {
        int index = searchDirectory(current, p->name);
        if (index != -1 && q != NULL) {
            current = current->subdirectories[index];
        }
        else if (index == -1 && q != NULL) {
            printf("Directory \'%s\' not found.\n", p->name);
            return;
        }
        p = p->next;
        q = q->next;
    }
    char filename[20];
    strcpy(filename, p->name);
    int found = 0;
    for (int i = 0; i < current->num_files; i++) {
        if (strcmp(current->files[i]->name, filename) == 0) {
            printf("Enter content of file: ");
            char cont[1000];
            scanf("%[^\n]", cont); while (getchar() != '\n');
            strcpy(current->files[i]->content, cont);
            found = 1;
            printf("Content written successfully.\n");
            break;
        }
    }
    if (!found) {
        touch(current, path);
        gedit(current, path);
    }
}

//Function to print present working directory
void pwd(Directory *current_directory) {
    char tpath[50][50]; 
    int i = 0, count = 0;
    if (current_directory == root_directory) {
        printf("/");
        return;
    }
    Directory *tmp = current_directory;
    while (tmp->parent != NULL && i < 50) { 
        strcpy(tpath[i++], tmp->name);
        tmp = tmp->parent;
        count++;
    }
    while (i > 0) {
        printf("/%s", tpath[--i]);
    }
}

//Function to display help message
void help() {
    printf("Following is the list of available commands and their uses:\n");
    printf("cd <path>     : change current working directory to specified path\n");
    printf("mkdir <path>  : create new directory in specified path\n");
    printf("rmdir <path>  : remove directory from specified path\n");
    printf("ls <path>(opt): list directories and files in specified path or current working directory\n");
    printf("touch <path>  : create new file in specified path\n");
    printf("rm <path>     : remove directory from specified path\n");
    printf("cat <path>    : display contents of file at specified path\n");
    printf("gedit <path>  : edit contents of file at specified path\n");
    printf("pwd           : display present working directory\n");
    printf("help          : display help information for terminal commands\n");
    printf("exit          : exit the terminal\n");
}

//Function to sort the directory
void sortDirectory(Directory *current_directory) {
    int i, j;
    for (i = 0; i < current_directory->num_subdirectories - 1; i++) {
        for (j = 0; j < current_directory->num_subdirectories - i - 1; j++) {
            if (strcmp(current_directory->subdirectories[j]->name, current_directory->subdirectories[j + 1]->name) > 0) {
                Directory *temp = current_directory->subdirectories[j];
                current_directory->subdirectories[j] = current_directory->subdirectories[j + 1];
                current_directory->subdirectories[j + 1] = temp;
            }
        }
    }
}

// Functions to search directory
int binarySearch(Directory *directories[], int low, int high, const char *name) {
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(directories[mid]->name, name);
        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
    }

int searchDirectory(Directory *current_directory, char *name) {
    sortDirectory(current_directory);
    int index = binarySearch(current_directory->subdirectories, 0, current_directory->num_subdirectories - 1, name);
    if (index != -1) {
        return index;
    } else {
        return -1;
    }
}
