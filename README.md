# libgrid
This is a library for handling grids containing tiles. This can be used for creating a game using a grid of tiles, such as chess.

Quick note before continuing: This was compiled as a library (dll) under Windows, when I later moved to Debian I kept the files as is, so calling it a library may not be entirely correct. It's simply two template headers written in C++. This is my first attempt at making a "library". 

Files:

Grid.h - Where the magic happens. Include this to use it.

Support.h - Where additional magic can happen. There is functionality in Grid.h to automatically connect the tiles in the grid to one another. If you want to implement that yourself just skip this file.

test_lib.cpp - Contains some simple examples of grids and has been used to test the code.

How to use:
The goal is to make it simple for anyone to use, yet generic enough so it does not restrict you too much. 
To put it simple it is meant to be used as such:

gl::Grid\<YOUR_TILE_CLASS, ARGS_FOR_YOUR_TILE_CLASS\> myGrid(GRID_X_SIZE, GRID_Y_SIZE, YOUR_ARGS);

YOUR_TILE_CLASS is whatever class you want your tiles to consist of. YOUR_TILE_CLASS may inherit from the class Additional_Tile_Members found in Support.h that can be useful to have, but are entirely optional. 
ARGS_FOR_YOUR_TILE_CLASS can be an arbitrary number of types of arguments that your tiles need when they are created. 
GRID_X_SIZE & GRID_Y_SIZE determine the size of the grid.
YOUR_ARGS can be an arbitrary number of arguments that your tiles need when they are created.

Example:

gl::Grid\<Tile, char> exampleGrid(2, 3, 'c');
//the resulting grid would be

cc

cc

cc

Useful Functions:
for_each_tile(Func f); - takes a functor or function object as argument and applies to all tiles.
operator<<(ostream& o, Grid& g); /*and*/ print(); - prints your grid in the terminal, assuming your tile class implements that kind of functionality.
connect_tiles(int neighbors); - connects neighboring tiles to one another. neighbors can be 4 or 8. This assumes your tile class can handle neighbors and was implemented for tiles inheriting from Additional_Tile_Members. 
get_tile(int x, int y) - retrieves a tile at the specified coordinates. Returns nullptr if coordinates are outside grid.

Support.h

Contains the class Additional_Tile_Members. Optional to use. To make use of it simply let your tile class inherit from it. It allows for automatically connecting the tiles during creation of the grid.

Example:
class Tile: public Additional_Tile_Members<Tile>

ALTERNATIVE_POSITION could for example be std::pair<T, T> from the standard library.

Not Implemented:

Copying a grid.

Accessing tiles in a certain pattern such as in a line.

Connecting grids to one another.

Merging grids.

Different kinds of grids, like hexagonal. 
