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
void walk(const char* directory, const char* prefix, counter_t *counter, int depth) {
    file_info_t *head = NULL, *current, *iter;
    size_t size = 0, index;

    struct dirent *file_dirent;
    DIR *dir_handle;

    char *full_path, *segment, *pointer, *next_prefix;

    dir_handle = opendir(directory);
    if (!dir_handle) {
        fprintf(stderr, "Cannot open directory \"%s\"\n", directory);
        return;
    }

    counter->dirs++;

    while ((file_dirent = readdir(dir_handle)) != NULL) {
        if (file_dirent->d_name[0] == '.') {
            continue;
        }

        current = malloc(sizeof(file_info_t));
        current->name = strcpy(malloc(strlen(file_dirent->d_name) + 1), file_dirent->d_name);

        struct stat file_stat;
        full_path = malloc(strlen(directory) + strlen(current->name) + 2);
        sprintf(full_path, "%s/%s", directory, current->name);

        if (stat(full_path, &file_stat) == -1) {
            fprintf(stderr, "Failed to get stat for file \"%s\": %s\n", full_path, strerror(errno));
            free(full_path);
            free(current->name);
            free(current);
            continue;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            current->is_dir = 1;
            counter->dirs++;
        } else {
            current->is_dir = 0;
            counter->files++;
        }

        size += file_stat.st_blocks;

        if (head == NULL || strcmp(head->name, current->name) > 0) {
            current->next = head;
            head = current;
        } else {
            iter = head;
            while (iter->next != NULL && strcmp(iter->next->name, current->name) < 0) {
                iter = iter->next;
            }
            current->next = iter->next;
            iter->next = current;
        }

        free(full_path);
    }

    closedir(dir_handle);

    print_indentation(depth);
    printf("%s\n", prefix);

    for (iter = head; iter != NULL; iter = iter->next) {
        print_indentation(depth);
        printf("├── %s", iter->name);
        if (iter->is_dir) {
            printf("/");
        }
        printf("\n");

        if (iter->is_dir) {
            next_prefix = malloc(strlen(prefix) + strlen(iter->name) + 2);
            sprintf(next_prefix, "%s%s/", prefix, iter->name);
            walk(full_path, next_prefix, counter, depth + 1);
            free(next_prefix);
        }
    }

    while (head != NULL) {
        current = head;
        head = head->next;
        free(current->name);
        free(current);
    }
}

