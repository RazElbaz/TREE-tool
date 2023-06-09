#include "stree.h"


// Print indentation based on depth
void print_indentation(int depth) {
  for (int i = 0; i < depth; i++) {
    if (i == depth - 1) {
      printf(" ");
    } else {
      printf("   ");
    }
  }
}

// Print program usage information
void print_usage(const char *program_name) {
  fprintf(stderr, "Usage: %s directory-path\n", program_name);
}

// Print directory information
void print_directory_info(const counter_t *counter) {
  const char *print_directory = (counter->dirs - 1 == 1) ? "directory" : "directories";
  const char *print_file = (counter->files == 1) ? "file" : "files";

  printf("\n%zu %s, %zu %s\n", (counter->dirs > 0) ? (counter->dirs - 1) : 0, print_directory, counter->files, print_file);
}

// Print a directory name in colored text
void print_colored_directory(const char *directory) {
  printf("\033[1;34m%s\033[0m\n", directory);  // Blue color for the top-level directory
}

// Check if a file has a JPG, PNG, or GIF extension
int is_jpg_png_gif(const char* filename) {
  const char* extension = strrchr(filename, '.');
  if (extension != NULL) {
    if (strcmp(extension, ".jpg") == 0 ||
        strcmp(extension, ".png") == 0 ||
        strcmp(extension, ".gif") == 0) {
      return 1;
    }
  }
  return 0;
}

// Check if a file has a TAR.GZ or ZIP extension
int is_tar_gz_or_zip(const char* filename) {
  const char* extension = strrchr(filename, '.');
  if (extension != NULL) {
    if (strcmp(extension, ".gz") == 0 ||
        strcmp(extension, ".zip") == 0) {
      return 1;
    }
  }
  return 0;
}

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
      fprintf(stderr, "Cannot get file information for \"%s\"\n", full_path);
      free(full_path);
      free(current->name);
      free(current);
      continue;
    }

    current->is_dir = S_ISDIR(file_stat.st_mode);
    current->next = NULL;

    if (head == NULL) {
      head = current;
    } else if (strcmp(current->name, head->name) < 0) {
      current->next = head;
      head = current;
    } else {
      for (iter = head; iter->next && strcmp(current->name, iter->next->name) > 0; iter = iter->next);

      current->next = iter->next;
      iter->next = current;
    }

    size++;
  }

  closedir(dir_handle);
  if (!head) {
    return;
  }

  for (index = 0; index < size; index++) {
    if (index == size - 1) {
      pointer = "└── ";
      segment = "    ";
    } else {
      pointer = "├── ";
      segment = "│  ";
    }

    struct stat file_stat;
    full_path = malloc(strlen(directory) + strlen(head->name) + 2);
    sprintf(full_path, "%s/%s", directory, head->name);

    if (stat(full_path, &file_stat) == -1) {
      fprintf(stderr, "Cannot get file information for \"%s\"\n", full_path);
      free(full_path);
      continue;
    }

    int is_dir = head->is_dir;
    char *name = head->name;

    struct passwd *owner = getpwuid(file_stat.st_uid);
    char *owner_name = owner ? owner->pw_name : "";
    struct group *group = getgrgid(file_stat.st_gid);
    char *group_name = group ? group->gr_name : "";

    char permissions[11];
    sprintf(permissions, "%s%s%s%s%s%s%s%s%s%s",
            (S_ISDIR(file_stat.st_mode)) ? "d" : "-",
            (file_stat.st_mode & S_IRUSR) ? "r" : "-",
            (file_stat.st_mode & S_IWUSR) ? "w" : "-",
            (file_stat.st_mode & S_IXUSR) ? "x" : "-",
            (file_stat.st_mode & S_IRGRP) ? "r" : "-",
            (file_stat.st_mode & S_IWGRP) ? "w" : "-",
            (file_stat.st_mode & S_IXGRP) ? "x" : "-",
            (file_stat.st_mode & S_IROTH) ? "r" : "-",
            (file_stat.st_mode & S_IWOTH) ? "w" : "-",
            (file_stat.st_mode & S_IXOTH) ? "x" : "-");

printf("%s", prefix);
print_indentation(depth);
printf("%s[%s %-8s %-8s %10ld]  ", pointer, permissions, owner_name, group_name, file_stat.st_size);
if (is_dir) {
  printf("\033[1;34m%s\033[0m\n", name);  // Blue color for directories
} else {
  if (S_ISSOCK(file_stat.st_mode)) {
    printf("\033[1;93m%s\033[0m\n", name);  // Bright yellow color for socket files
  } else if (S_ISFIFO(file_stat.st_mode)) {
    printf("\033[0;33m%s\033[0m\n", name);  // Brown color for pipes
  } else if (file_stat.st_mode & S_IXUSR) {
    printf("\033[1;32m%s\033[0m\n", name);  // Green color for executable files
  } else if (S_ISBLK(file_stat.st_mode)) {
    printf("\033[1;33m%s\033[0m\n", name);  // Dark yellow color for block device files
  } else if (S_ISCHR(file_stat.st_mode)) {
    printf("\033[1;33m%s\033[0m\n", name);  // Dark yellow color for character device files
  } else if (is_jpg_png_gif(name)) {
    printf("\033[1;35m%s\033[0m\n", name);  // Purple color for jpg/png/gif files
  } else if (S_ISREG(file_stat.st_mode) && access(full_path, F_OK) == -1) {
    printf("\033[1;31m%s\033[0m\n", name);  // Red color for orphaned symbolic links
  } else if (is_tar_gz_or_zip(name)) {
    printf("\033[1;31m%s\033[0m\n", name);  // Red color for .tar.gz and .zip files
  } else {
    printf("%s\n", name);
  }
  
  counter->files++;
}




    if (is_dir) {
      next_prefix = malloc(strlen(prefix) + strlen(segment) + 1);
      sprintf(next_prefix, "%s%s", prefix, segment);

      walk(full_path, next_prefix, counter, depth + 1);
      free(next_prefix);
    }

    free(full_path);

    current = head;
    head = head->next;

    free(current->name);
    free(current);
  }
}



int main(int argc, char *argv[]) {
  if (argc != 2) {
    print_usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  const char* directory = (argc > 1) ? argv[1] : ".";
  print_colored_directory(directory);  // Print the top-level directory in blue

  counter_t counter = {0, 0};
  walk(directory, "", &counter, 0);

  print_directory_info(&counter);

  return 0;
}
