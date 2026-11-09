#include <stdio.h>
#include <dirent.h>
#include <string.h>

void print_file(const char *name, int is_dir, int indent, int new_folder) {
    if (indent)
        printf("%*c", indent * 2, ' ');
    
    if(new_folder)
        printf("╰");

    if(is_dir)
        printf("\e[1;34m%s\e[0m\n", name);
    else {
        printf("%s\n", name);
    }
}

int walk(const char *path, int indent, int new_folder) {
    DIR *dir = opendir(path);

    if(!dir){
        perror("opendir");
        return 1;
    }

    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) {

        const char *name = dp->d_name;
        
        if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0 || name[0] == '.')
            continue;
    
        if (dp->d_type == DT_DIR) {
            print_file(name, 1, indent, new_folder);

            size_t len = strlen(path) + strlen(name) + 2;
            char sub_path[len];
            snprintf(sub_path, len, "%s/%s", path, name);
            new_folder = 1;
            walk(sub_path, indent + 2, new_folder);
        } else {
            print_file(name, 0, indent, new_folder);
        }
    }  
    closedir(dir);
    return 0;
}

int main(int argc, char **argv) {
    const char *path = ".";

    if (argc > 1) {
        path = argv[1];
    }

    int err = walk(path, 0, 0);

    return err;
}
