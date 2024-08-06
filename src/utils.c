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

meshType parseArguments(int argc, char *argv[]) {
    if (argc != 2) { // only one argument
        log_error("Usage: %s [curtain] | [table-cloth] | [soft] ", argv[0]);
        exit(EXIT_FAILURE);
    }

    if( strcmp(argv[1], "curtain") == 0 )
    {   
        return CURTAIN;
    } else if (strcmp(argv[1], "table-cloth") == 0)
    {
        return TABLE_CLOTH;
    } else if (strcmp(argv[1], "soft")== 0)
    {
        return SOFT;
    } else
    {
        log_error("the requested arguments doesn't exists");
        log_error("Usage: %s [curtain] | [table-cloth] ", argv[0]);
        exit(EXIT_FAILURE);
    }
}
