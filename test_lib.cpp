#include <iostream>
#include "Grid.h"
#include "Support.h"
using namespace std;
using namespace gl;

//tile class that is used in grid
class Tile
{
public:
	Tile(){ letter = 'D';}
	Tile(int x, int y, char c):
		_x(x), _y(y), letter(c)  
	{

	}

	char get_letter() const
	{
		return letter;
	}

	void set_letter(char c){letter = c;}

	friend ostream& operator<<(ostream& out, const Tile& g)
	{
		return out << g.get_letter();
	}
	int _x, _y;
private:
	char letter;
	
};

class Tile_Inheriting_Members
	:	public Tile, public gl::Additional_Tile_Members<Tile_Inheriting_Members, std::pair<int, int>>

	/*
		public grid_lib::Required_Tile_Members<Tile_Inheriting_Members>,
		public grid_lib::Optional_Tile_Members < std::pair<int, int> >*/
{
public:

	Tile_Inheriting_Members(int x, int y, char c)
		: Tile(x, y, c), Additional_Tile_Members(x, y)
	{
	}

	Tile_Inheriting_Members(int x, int y, char c, int offset)
		: Tile(x, y, c), Additional_Tile_Members(x, y, pair<int, int>(x * offset, y * offset))
	{
	}

	void set_position(int x, int y) 
	{
		position.first = x;
		position.second = y;
	}
};

//functor
class Set_Tile_Content
{
public:
	Set_Tile_Content(char S):letter(S){}
	void operator()(Tile& tile)
	{
		tile.set_letter(letter);
	}

private:
	char letter;
};

ostream& operator<<(ostream& o, pair<int, int>& i)
{
	return o << i.first << ", " << i.second;
}

int main()
{
	//simple grid
	gl::Grid<Tile> simple_grid(10,20);
	auto size = simple_grid.get_grid_size();
	cout << "grid size: " << size.first << ", " << size.second << "." <<endl;
	cout << "default simple_grid:" << endl;
	cout << simple_grid << endl;

	simple_grid.for_each_tile<Set_Tile_Content>(Set_Tile_Content('S'));
	cout << "simple_grid modified with functor" << endl << simple_grid << endl;

	simple_grid.for_each_tile(
		[](Tile& tile)
		{
			tile.set_letter('L');
		});
	cout << "simple_grid modified with lambda expression" << endl << simple_grid << endl;

	
	
	//complex grid test grid modifying functions
	//							x   y   n tile_type  arguments
	gl::Grid<Tile, char> complex_grid(10, 20, 'Q');
	size = complex_grid.get_grid_size();
	cout << "grid size: " << size.first << ", " << size.second << "." << endl;
	cout << "default complex_grid:" << endl;
	cout << complex_grid << endl;

	complex_grid.for_each_tile<Set_Tile_Content>(Set_Tile_Content('W'));
	cout << "complex_grid modified with functor" << endl << complex_grid << endl;

	complex_grid.for_each_tile(
		[](Tile& tile)
	{
		tile.set_letter('E');
	});
	cout << "complex_grid modified with lambda expression" << endl << complex_grid << endl;

	
	cout << "modifying specific tiles: (9, 19), (0, 10)" << endl;
	complex_grid.get_tile(9, 19)->set_letter('M');
	complex_grid.get_tile(0, 10)->set_letter('N');
	cout << complex_grid << endl;
	
	cout << "Trying to obtain tile (-1, -1) which should be illegal." << endl;
	Tile* tile = complex_grid.get_tile(-1, -1);
	if (tile != nullptr)
		tile->set_letter('I');
	cout << "Trying to obtain tile (10, 20) which should be illegal." << endl;
	tile = complex_grid.get_tile(10, 20);
	if (tile != nullptr)
		tile->set_letter('I');

	
	//complex grid with connections
	
	
	gl::Grid<Tile_Inheriting_Members, char> complex_grid_connected(3, 3, 8, 'C');
	size = complex_grid_connected.get_grid_size();
	cout << "grid size: " << size.first << ", " << size.second << "." << endl;
	cout << "complex_grid_connected:" << endl;
	
	cout << complex_grid_connected << endl;
	
	auto tile11 = complex_grid_connected.get_tile(1, 1)->get_neighbors();
	auto tile22 = complex_grid_connected.get_tile(2, 2)->get_neighbors();

	cout << "tile 1,1 neighbors (center tile)" << endl;
	for (auto neighbor : tile11)
	{
		cout << "neighbor postion: x: " << neighbor->get_grid_position().first << ", ";
		cout << "y: " << neighbor->get_grid_position().second << endl;
	}

	cout << endl << "tile 2,2 neighbors (lower right corner tile) " << endl;
	for (auto neighbor : tile22)
	{
		cout << "neighbor postion: x: " << neighbor->get_grid_position().first << ", ";
		cout << "y: " << neighbor->get_grid_position().second << endl;
	}

	
	//testing with fill_with_for_args

	gl::Grid<Tile_Inheriting_Members, char, int> complex_grid_connected_position(3, 3, 8, 'F', 10);
	complex_grid_connected_position.for_each_tile([](Tile_Inheriting_Members& tile)
	{
		cout << "grid_position: " << "x: " << tile.get_grid_position().first << ", y: " << tile.get_grid_position().second << endl;
		
	});

	complex_grid_connected_position.for_each_tile([](Tile_Inheriting_Members& tile)
	{
		cout << "position: " << "x: " << tile.get_position().first << ", y: " << tile.get_position().second << endl;
	});

	//copying grids not implemented at the moment 
	
	//cout << "Copy constructing grid" << endl;
	//auto copy_grid(complex_grid_connected);
	//copy_grid.print();

	//cout << "Assignment test" << endl;
	//auto assignment_grid = gl::Grid<Tile_Inheriting_Members, char> (3, 3, 8, 'C');
	//assignment_grid.print();
	
	cout << "Test completed." << endl;
	return 0;
}

