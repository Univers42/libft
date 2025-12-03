in 42 we use this format of map because the FDF project is basically a height map to parse, in a 2D array of points, and draw a 3D wireframe of it.

It's exactly the same format that:
- terrain engines
- GIS elevation maps
- voxel world generators
- procedural landscapes
use.

# The real structure behin d it:
Inside out program, files becomes:
2D array (matrix of points)
```c
typedef struct s_point {
    int z;
    int color;
} t_point;

t_point **map;
```

