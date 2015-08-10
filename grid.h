#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "support.h"
using namespace std;

/*class Grid;
class Simple_square_grid;*/

namespace gl
{
	template<class tile_type, typename ... arg_types>
	class Grid
	{
	public:
		typedef vector<vector< unique_ptr<tile_type> >> matrix;

		//----------------CONSTRUCTORS-----------------

		explicit Grid(unsigned x, unsigned y, arg_types ... args) :
			x_size(x), y_size(y), _neighbors(0)
		{
			fill_tiles(args...);
		}

		explicit Grid(unsigned x, unsigned y, unsigned neighbors, arg_types ... args) :
			x_size(x), y_size(y), _neighbors(neighbors)
		{
			fill_tiles(args...);
			if (neighbors > 0)
				connect_tiles(neighbors);
		}

		/*template<class F>
		explicit Grid(unsigned x, unsigned y, unsigned neighbors, F function, arg_types ... args):
			x_size(x), y_size(y)
		{
			fill_tiles(function, function, args...);
			if(neighbors > 0)
				connect_tiles(neighbors);
		}*/

		explicit Grid(const Grid& g):
			x_size(g.get_grid_matrix().size()), 
			y_size(g.get_grid_matrix().back().size()), 
			_neighbors(0)
		{
			copy_grid(g);
		}

		explicit Grid(const Grid& g, unsigned neighbors):
			x_size(g.get_grid_matrix().size()),
		   	y_size(g.get_grid_matrix().back().size()),
		   	_neighbors(g.get_number_of_neighbors())
		{
			copy_grid(g);
			if (neighbors > 0)
				connect_tiles(neighbors);
		}

		//destructor

		virtual ~Grid(){};

		//------------FUNCTION MEMBERS-------------

		/*
		 * Copies grid m to new_grid and returns it. 
		 */
		void copy_grid(const Grid& g)
		{
			for (unsigned x = 0; x < x_size; ++x)
			{
				grid_matrix.push_back(vector<unique_ptr<tile_type>>());
				for (unsigned y = 0; y < y_size; ++y)
				{
					grid_matrix.back().push_back(unique_ptr<tile_type>(new tile_type(*g.get_tile(x, y))));
				}
			}
		}

		/* 
		 * retuns number of neighbors that each tile has.
		 * This is only ever useful if a neighbor nubmer is set in a constructor
		 */
		unsigned get_number_of_neighbors() const
		{
			return _neighbors;
		}
		
		//returns the grid
		matrix& get_grid_matrix()
		{
			return grid_matrix;
		}

		matrix& get_grid_matrix() const
		{
			return const_cast<matrix&>(grid_matrix);
		}
		/*
			returns nullptr if coordinates given are illegal
		*/
		tile_type* get_tile(int x, int y)
		{
			try
			{
				if (x < 0 || y < 0 || x >= (int)grid_matrix.size() || y >= (int)grid_matrix.front().size())
				{
					string error = "Illegal coordinates used: (" + to_string(x) + ", " + to_string(y) + "). Returning NULL POINTER.";
					throw domain_error(error);
					
				}	
				else
					return grid_matrix[x][y].get();
			}
			catch (domain_error& e)
			{
				cerr << e.what() << endl;
				return nullptr;
			}
		}

		tile_type* get_tile(int x, int y) const
		{
			try
			{
				if (x < 0 || y < 0 || x >= (int)grid_matrix.size() || y >= (int)grid_matrix.front().size())
				{
					string error = "Illegal coordinates used: (" + to_string(x) + ", " + to_string(y) + "). Returning NULL POINTER.";
					throw domain_error(error);
					
				}	
				else
					return grid_matrix[x][y].get();
			}
			catch (domain_error& e)
			{
				cerr << e.what() << endl;
				return nullptr;
			}
		}
		//returns the horizontal and vertical size of the grid 
		pair<int, int> get_grid_size()
		{
			return pair<int, int>(x_size, y_size);
		}

		/*
			functions manipulating the tiles in the grid_matrix
		*/

		template<class Function>
		void for_each_tile(Function func)
		{
			//vector in X axis
			for(auto& vector_x : grid_matrix)
			{
				//tile
				for(auto& tile : vector_x)
				{
					func(*tile);
				}
			}
		}

		/*
			Fills the grid with tiles with the arguments x and y coordinates. 
			This is so that the caller can make use of the x and y coords. 
		*/
		void fill_tiles()
		{
			//vector in X axis
			for (unsigned x = 0; x < x_size; ++x)
			{
				grid_matrix.push_back(vector<unique_ptr<tile_type>>());
				for (unsigned y = 0; y < y_size; ++y)
				{
					grid_matrix.back().push_back(unique_ptr<tile_type>(new tile_type()));
				}
			}
		}

		/*
			fill_tiles() with loop integers and caller arguments.
			The grid coordinates are called x and y. They will be given as the first arguments. Example:
			new tile_type(x, y, args...); //where args are the callers arguments.
		*/
		template<typename ... argument_types>
		void fill_tiles(argument_types ... args)
		{
			//vector in X axis
			for (unsigned x = 0; x < x_size; ++x)
			{
				grid_matrix.push_back(vector<unique_ptr<tile_type>>());
				for (unsigned y = 0; y < y_size; ++y)
				{
					grid_matrix.back().push_back(unique_ptr<tile_type>(new tile_type(x, y, args...)));
				}
			}
		}

		/*
		 * will fill tiles like template function above but with the addition of being able to send one argument to
		 * the constructed tile as a function of its grid coordinates (x & y). 
		 */
		/*
		template<class F, typename ... argument_types>
			void fill_tiles(F func, argument_types ... args)
			{
				//vector in X axis
				for (unsigned x = 0; x < x_size; ++x)
				{
					grid_matrix.push_back(vector<unique_ptr<tile_type>>());
					for (unsigned y = 0; y < y_size; ++y)
					{
						grid_matrix.back().push_back(unique_ptr<tile_type>(new tile_type(x, y, func(x, y),args...)));
					}
				}
			}*/



		/*
			connect the neighbors after the grid matrix has been created.
			arg neighbors is to indicate how many neighbors each tile will have (tiles at the edge will have fewer neighbors)
			will connect neighboring tiles in clockwise order:
			1st neighbor - north
			2nd neighbor - north east
			3rd neighbor - east etc...
		*/
		void connect_tiles(int neighbors)
		{
			try
			{
				if (neighbors == 4 || neighbors == 8)
				{
					for (unsigned i = 0; i < grid_matrix.size(); i++)
					{
						for (unsigned j = 0; j < grid_matrix.front().size(); j++)
						{
							if (j > 0)
								grid_matrix[i][j]->add_neighbor(grid_matrix[i][j - 1].get(), tile_type::N); //north ^

							if (j > 0 && i < grid_matrix.size() - 1 && neighbors == 8)
								grid_matrix[i][j]->add_neighbor(grid_matrix[i + 1][j - 1].get(), tile_type::NE); //north east  ^/

							if (i < grid_matrix.size() - 1)
								grid_matrix[i][j]->add_neighbor(grid_matrix[i + 1][j].get(), tile_type::E); //east >

							if (i < grid_matrix.size() - 1 && j < grid_matrix.front().size() - 1 && neighbors == 8)
								grid_matrix[i][j]->add_neighbor(grid_matrix[i + 1][j + 1].get(), tile_type::SE); //south east \v

							if (j < grid_matrix.front().size() - 1)
								grid_matrix[i][j]->add_neighbor(grid_matrix[i][j + 1].get(), tile_type::S); //south v

							if (j < grid_matrix.front().size() - 1 && i > 0 && neighbors == 8)
								grid_matrix[i][j]->add_neighbor(grid_matrix[i - 1][j + 1].get(), tile_type::SW); //south west v/

							if (i > 0)
								grid_matrix[i][j]->add_neighbor(grid_matrix[i - 1][j].get(), tile_type::W); //west <

							if (j > 0 && i > 0 && neighbors == 8)
								grid_matrix[i][j]->add_neighbor(grid_matrix[i - 1][j - 1].get(), tile_type::NW); //north west \<
						}
					}
				}
				else
				{
					string thrown_string ="ILLEGAL NUMBER OF NEIGHBORS: "; 
					thrown_string += to_string(neighbors);
					throw domain_error(thrown_string);
				}
			}
			catch (domain_error e)
			{
				cerr << e.what() << endl;
			}
		}


		//---------------PRINTING FUNCTIONS------------------

		void print()
		{
			for (auto& v : grid_matrix)
			{
				for (auto& tile : v)
				{
					cout << tile->get_letter();
				}
				cout << endl;
			}
		}

		friend ostream& operator<<(ostream& out, Grid& g)
		{
			for(auto& v : g.grid_matrix)
			{
				for(auto& tile : v)
				{
					out << *tile;
				}
				out << endl;
			}	
			return out;
		}

		
	private:

		matrix grid_matrix;
		Grid() = delete;
		//Grid(const Grid& g) = delete;
		const unsigned x_size, y_size; //size of grid
		const unsigned _neighbors;
	};
}
