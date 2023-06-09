#define _XOPEN_SOURCE 600 /* Get nftw() */
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
    char *name;
    int is_dir;
    struct file_info *next;
} file_info_t;

void print_indentation(int depth) {
    for (int i = 0; i < depth; i++) {
        if (i == depth - 1) {
            printf(" ");
        } else {
            printf("│   ");
        }
    }
}

int is_jpg_png_gif(const char* filename);
int is_tar_gz_or_zip(const char* filename);
void walk(const char* directory, const char* prefix, counter_t *counter, int depth);
