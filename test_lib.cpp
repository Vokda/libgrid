#include <iostream>
#include "grid.h"
#include "support.h"
#include <chrono>
#define STRESS_TEST false
#define SAVE_LOAD_TEST true
using namespace std;
using namespace gl;

	//tile class that is used in grid
	class Tile
	{
	public:
		Tile(){ letter = 'D';}


		Tile(const Tile& tile):
			_x(tile._x), _y(tile._y), letter(tile.letter)
		{}
		Tile(int x, int y, char c = 'D'):
			_x(x), _y(y), letter(c)  
		{

		}

		char get_letter() const
		{
			return letter;
		}

		void set_letter(char c){letter = c;}

		string save() const
		{
			stringstream ss;
			ss << _x << ' ' << _y << ' ';
			ss << letter << ' ';
			return ss.str();
		}

		virtual void load(const string& data)
		{
			istringstream iss(data);
			iss >> _x >> _y >> letter;
		}

		friend ostream& operator<<(ostream& out, const Tile& g)
		{
			return out << g.get_letter();
		}

		int get_x() const
		{
			return _x;
		}

		int get_y() const
		{
			return _y;
		}

	protected:
		int _x, _y;
		char letter;
	};

	class Tile_Inheriting_Members:
		public Tile, public gl::Additional_Tile_Members<Tile_Inheriting_Members>
	{
	public:

		Tile_Inheriting_Members(int x, int y)
			: Tile(x, y), Additional_Tile_Members(x, y)
		{}

		Tile_Inheriting_Members(int x, int y, char c)
			: Tile(x, y, c), Additional_Tile_Members(x, y)
		{
		}

		Tile_Inheriting_Members(int x, int y, char c, int offset)
			: Tile(x, y, c), Additional_Tile_Members(x, y)
		{
			position = pair<int, int>(x * offset, y * offset);
		}


		void set_position(int x, int y) 
		{
			position.first = x;
			position.second = y;
		}

		std::pair<int, int> get_position() const
		{
			return position;
		}

		string save() const
		{
			stringstream ss;
			ss << position.first << ' ' << position.second << ' ' << Tile::save() << gl::Additional_Tile_Members<Tile_Inheriting_Members>::save();
			return ss.str();
		}

		void load(const string data)
		{
			istringstream iss(data);
			iss >> position.first >> position.second;
			iss >> _x >> _y >> letter;
			string s;
			getline(iss, s);
			gl::Additional_Tile_Members<Tile_Inheriting_Members>::load(s);
		}

private:
	std::pair<int, int> position;
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
	for (auto& neighbor : tile11)
	{
		cout << "neighbor postion: x: " << neighbor.second->get_grid_position().first << ", ";
		cout << "y: " << neighbor.second->get_grid_position().second << endl;
	}

	cout << endl << "tile 2,2 neighbors (lower right corner tile) " << endl;
	for (auto& neighbor : tile22)
	{
		cout << "neighbor postion: x: " << neighbor.second->get_grid_position().first << ", ";
		cout << "y: " << neighbor.second->get_grid_position().second << endl;
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

	cout << "Function test completed." << endl;
	cout << "Starting copy test." << endl;
	//cout << "Copy constructing grid" << endl;
	gl::Grid<Tile> copy_grid(simple_grid);
	cout << "simple_grid:" <<endl;
	simple_grid.print();
	cout << "copy_grid:" << endl;
	copy_grid.print();

	gl::Grid<Tile_Inheriting_Members, char> copy_complex_grid(complex_grid_connected, complex_grid_connected.get_number_of_neighbors());
	cout << "complex_grid_connected" << endl;
	complex_grid_connected.print();
	cout << "copy_complex_grid" <<endl;
	copy_complex_grid.print();

	//cout << "Assignment test" << endl;
	//auto assignment_grid = gl::Grid<Tile_Inheriting_Members, char> (3, 3, 8, 'C');
	//assignment_grid.print();
	cout << "Copy test completed." << endl;

	/*
	 *******************TESTING LOAD AND SAVE FUNCTIONS*************************
	 */
	#if SAVE_LOAD_TEST

	cout << "Save/load test. " << endl;

	cout << "Save simple grid:" << endl;
	cout << simple_grid << endl;
	simple_grid.save("simple.grid");
	gl::Grid<Tile> load_grid("simple.grid");
	cout << "Load simple grid to a new grid:" << endl;
	cout << load_grid << endl;



	cout << "Save complex grid:" << endl;
	cout << complex_grid_connected << endl;
	complex_grid_connected.save("complex.grid");
	gl::Grid<Tile_Inheriting_Members> load_complex_grid("complex.grid", true);
	cout << "Load complex grid to a new grid:" << endl;
	//cout << load_complex_grid << endl;
	load_complex_grid.print();
	tile11 = load_complex_grid.get_tile(1, 1)->get_neighbors();

	cout << "tile 1,1 neighbors (center tile)" << endl;
	for (auto& neighbor : tile11)
	{
		cout << "neighbor postion: x: " << neighbor.second->get_grid_position().first << ", ";
		cout << "y: " << neighbor.second->get_grid_position().second << endl;
	}

	cout << "Save/load test completed." << endl;

	#endif
	/*
	 * *************************************STRESS TEST**********************************
	 */
	#if STRESS_TEST

	cout << "Stress test:" << endl;

	//simple large grid
	cout << "-----------------------------" << endl;
	for(int i = 100; i < 10000; i*=2)
	{
		chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
		gl::Grid<Tile> simple_grid_large(i, i);
		chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
		chrono::duration<double> time_taken = chrono::duration_cast<chrono::duration<double>>( t2 - t1);
		size = simple_grid_large.get_grid_size();
		cout << "Grid size: " << size.first << ", " << size.second << "." <<endl;
		cout << "Number of tiles " << i*i << endl;
		cout << "Creation took " << time_taken.count() << " seconds."<< endl;
		cout << "-----------------------------" << endl;
	}
	#endif
	return 0;
}

