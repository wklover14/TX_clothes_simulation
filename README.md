# Cloth Simulation Project

## Description
This project implements a cloth simulation system in C. It models various types of cloth-like structures using a mesh of points connected by springs. The simulation can handle different scenarios such as curtains, tablecloths, and stretchable materials.

## Features
- Multiple mesh types: curtain, tablecloth, and soft (stretchable) material
- Spring-based physics simulation
- Customizable parameters for mesh properties, spring characteristics, and simulation settings
- Logging system for debugging and information output
- VTK file output for visualization

## Dependencies
- Standard C libraries (stdlib.h, stdio.h, stdbool.h, time.h, string.h, math.h)
- GCC compiler
- Make
- OpenMP for parrallelism
- Valgrind (optional, used for memory checking)

## Project Structure
The project consists of several source and header files:
- `src/main.c`: Entry point of the program
- `src/mesh.c` and `include/mesh.h`: Mesh structure and related functions
- `src/params.c` and `include/params.h`: Simulation parameters
- `src/space.c` and `include/space.h`: Vector and point operations
- `src/spring.c` and `include/spring.h`: Spring structure and related functions
- `include/log.c` and `include/log.h`: Logging utilities
- `include/utils.c` and `include/utils.h`: Utility functions

## Building the Project
To build the project, use the provided Makefile:

``` shell
make
```

This will compile the source files and create an executable named `app` in the `bin` directory.

## Running the Simulation
The Makefile provides several targets for running different mesh types:

- For curtain simulation:
  ```
  make run-rideau
  ```

- For tablecloth simulation:
  ```
  make run-nappe
  ```

- For soft (stretchable) material simulation:
  ```
  make run-tissus
  ```
- For flag simulation:
  ```
  make run-drapeau
  ```

- For all the simulation:
  ```
  make run-all
  ```

## Memory Checking
To run the simulation with Valgrind for memory checking:

- For curtain:
  ```
  make saferun-rideau
  ```

- For tablecloth:
  ```
  make saferun-nappe
  ```

- For soft material:
  ```
  make saferun-tissus
  ```
- For flag simulation:
  ```
  make saferun-drapeau
  ```
## Cleaning the Project
To remove all built files and VTK output:

```
make clean
```

## Configuration
The simulation can be configured by modifying the parameters in `src/params.c`. Key parameters include:
- Mesh dimensions (M, N)
- Spring properties (stiffness, energy threshold, damage threshold)
- Simulation settings (time step, number of updates, output frequency)

To modify parameters only for a certain type of cloth, use the `custom_param()` function in `mesh.c`.

## Creating a New Mesh Type
To create a new mesh type:

1. Add a new enum value to the `meshType` enum in `include/mesh.h`.
2. Modify the `isFixedPoint` function in `src/mesh.c` to define fixed points for your new mesh type.
3. Update the `initMesh` function in `src/mesh.c` to initialize the positions of points for your new mesh type.
4. If needed, add custom parameters for your mesh type in the `customs_params` function in `src/mesh.c`.
5. Implement any additional forces specific to your mesh type in the `computeAddForces` function in `src/mesh.c`.
6. Update the argument parsing in the `parseArguments` and `getTypeName` functions in `utils.c` to recognize the new mesh type from command-line arguments.
7. Add a new run target in the Makefile for the new mesh type.

Example: Adding a "dome" mesh type

```c
// In mesh.h
typedef enum {
    CURTAIN,
    TABLE_CLOTH,
    SOFT,
    DOME  // New mesh type
} meshType;

// In mesh.c
bool isFixedPoint(unsigned int i, unsigned int j, Mesh* mesh, meshType type) {
    // ...
    case DOME:
        return (i == 0 || i == mesh->n - 1 || j == 0 || j == mesh->m - 1);
    // ...
}

void initMesh(Mesh* mesh, meshType type) {
    // ...
    case DOME:
        mesh->P[i][j] = newVector(origin.x + i * SPACING, 
                                  origin.y + sqrt(RADIUS*RADIUS - pow(i*SPACING - (mesh->n-1)*SPACING/2, 2) - pow(j*SPACING - (mesh->m-1)*SPACING/2, 2)), 
                                  origin.z + j * SPACING);
    // ...
}

// In utils.c
meshType parseArguments(int argc, char *argv[]) {
    // ...
    else if (strcmp(argv[1], "dome")== 0)
        {
            return DOME;
        }
    // ...
}

//...
```

## Code specification

### Mesh
The mesh struct presents as follow :
```C
typedef struct Mesh {
  unsigned int n; // number of lines
  unsigned int m; // number of columns

  float t;    // the time at which position P are calculated
  Vector **P; // Coordinate in the space at t time, should be used for rendering
  Vector **V; // Velocity matrix n*m

  Vector **P0; // Initial position matrix

  Spring
      *springs; // list of springs of the mesh, refered as R in the litterature
  unsigned int n_springs; // number of non-break springs in the mesh

  unsigned int **
      *face_spring_indices; // 3D array of spring indices for each face
} Mesh;
```
At the initial time (t = 0), the positions P and P0 are identical.

Face are defined by the bottom-left point and others points are computed in a very short time.

The face_spring_indices is a 3D array that maps each face (i, j) in the position matrix P to the indices of the springs connected to that point. The integer k represents the index of a spring for a face associated with a given point (0 to 4 for structural springs), and the corresponding value in the array is the index of the spring in the springs array.

This allow for each face, to quickly identify breaked-springs.

### Springs
The springs are initialized and counted in a specific order. The process starts from the bottom-left point (0, 0) and proceeds by attempting to connect to other points by incrementing the indices `i` and `j` by `1` or `2` (e.g., (i, j+1), (i+1, j), (i+1, j+1), (i+2, j), (i, j+2)).

This method allows the springs to be computed simultaneously while initializing the matrix values for other points.

## Output
The simulation generates VTK files in the `vtk_poly_<mesh_type>` and `vtk_grid_<mesh_type>` directories for visualization. These can be viewed using appropriate VTK visualization software.

## License
Free to use

## Author
- WATCHO KEUGONG Gabby Pavel (gwathok@etu.utc.fr)

## Acknowledgments
- ChatGPT and code for boilerplate.
- Stack Overflow contribution for logging utilities : https://stackoverflow.com/a/23446001