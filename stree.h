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
    size_t dirs;
    size_t files;
} counter_t;

typedef struct file_info {
    char* name;
    int is_dir;
    struct file_info* next;
} file_info_t;

void print_indentation(int depth);
int is_jpg_png_gif(const char* filename);
int is_tar_gz_or_zip(const char* filename);
void walk(const char* directory, const char* prefix, counter_t* counter, int depth);
void print_usage(const char* program_name);
void print_directory_info(const counter_t* counter);
void print_colored_directory(const char* directory);

#endif /* STREE_H */
