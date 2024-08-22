/**
*************************************************************
* @file     utils.h
* @author   Chatgpt & WATCHO Gabby
* @date     31/07/2024
* @brief
*************************************************************
*/

#ifndef UTILS_H
#define UTILS_H

#include "log.h"
#include "mesh.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <unistd.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

/**
 * @brief Creates a directory if it does not exist.
 *
 * @param path The path of the directory to create.
 * @return 0 if the directory exists or was created successfully, -1 otherwise.
 */
int createDirectory(const char *path);

/**
 * @brief Parses and checks command line arguments.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 if arguments are valid, -1 otherwise.
 */
meshType parseArguments(int argc, char *argv[]);

const char *getTypeName(meshType type);
void convertMeshToPolyVTK(const Mesh *mesh, const char *output_filename);
void convertMeshToGridVTK(const Mesh *mesh, const char *output_filename);

#endif // UTILS_H
