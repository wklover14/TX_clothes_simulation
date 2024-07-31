#include "utils.h"

int createDirectory(const char *path) {
    struct stat st = {0};

    if (stat(path, &st) == -1) {
        if (MKDIR(path) == 0) {
            log_info("Directory created: %s", path);
            return 0; // Success
        } else {
            log_error("Error creating directory");
            return -1; // Failure
        }
    } else {
        log_debug("Directory already exists: %s", path);
        return 0; // Success (directory already exists)
    }
}

int parseArguments(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        return -1; // Failure
    }

    for (int i = 1; i < argc; i++) {
        if (strlen(argv[i]) == 0) {
            fprintf(stderr, "Invalid argument: empty string\n");
            return -1; // Failure
        }
    }

    return 0; // Success
}
