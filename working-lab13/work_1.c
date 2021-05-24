#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

void get_links(const char *const path, int path_len, int *const rdepth) {
    char *new_path = (char *) malloc((path_len + 2) * sizeof(char));
    strcpy(new_path, path);
    if (new_path[path_len - 1] == 'z' || new_path[path_len - 1] == 'Z') {
        strcat(new_path, "a");
        path_len++;
    } else {
        new_path[path_len - 1]++;
    }
    if (symlink(path, new_path) == -1) {
        *rdepth = 0;
        return;
    }
    get_links(new_path, path_len, rdepth);
    (*rdepth)++;
    unlink(new_path);
    free(new_path);
}

int main() {
    FILE *f;
    int rdepth;
    printf("Calculating recursion depth...\n");
    if ((f = fopen("a", "w")) == NULL) {
        printf("Error occured while creating original file.");
        exit(-1);
    }
    if (fputs("Original file", f) == EOF) {
        printf("Error occured while writing to original file.");
        exit(-1);
    }
    get_links("a", 1, &rdepth);
    printf("Recursion depth is %d\n", rdepth);
    if (fclose(f) == EOF) {
        printf("Error occured while closing original file.");
        exit(-1);
    }
    return 0;
}
