#ifndef STREE_H
#define STREE_H

#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

typedef struct counter {
    size_t dirs;   // Number of directories
    size_t files;  // Number of files
} counter_t;

typedef struct file_info {
    char* name;                // Name of the file
    int is_dir;                // Flag indicating if it's a directory
    struct file_info* next;    // Pointer to the next file_info struct
} file_info_t;

// Print indentation based on depth
void print_indentation(int depth);

// Check if a file has a JPG, PNG, or GIF extension
int is_jpg_png_gif(const char* filename);

// Check if a file has a TAR.GZ or ZIP extension
int is_tar_gz_or_zip(const char* filename);

// Recursively walk through the directory tree and count directories/files
void walk(const char* directory, const char* prefix, counter_t* counter, int depth);

// Print program usage information
void print_usage(const char* program_name);

// Print directory information
void print_directory_info(const counter_t* counter);

// Print a directory name in colored text
void print_colored_directory(const char* directory);

#endif /* STREE_H */
