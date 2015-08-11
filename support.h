#pragma once
#include <map>
using namespace std;

/*
	This file contains additional members of Tile classes that are used with this library
*/

namespace gl
{
	template<class tile_type>
	class Additional_Tile_Members
	{
	public:
		enum neighbor_direction{N, NE, E, SE, S, SW, W, NW};
		typedef map<neighbor_direction, tile_type*> tile_neighbors;

		Additional_Tile_Members(int x_pos, int y_pos) :
			grid_position(x_pos, y_pos)
		{}


		Additional_Tile_Members() :
			grid_position(-1, -1)
		{}

		virtual ~Additional_Tile_Members(){};


		/*Required_Tile_Members(unsigned number_of_neighbors)
		{
		neighbors.resize(number_of_neighbors);
		}*/

		//Required_Tile_Members() = default;

		/*void set_number_of_neighbors(int n)
		{
		//neighbors.resize(n);
		}*/


		void add_neighbor(tile_type* neighbor, neighbor_direction nd)
		{
			neighbors[nd] = neighbor;
		}

		tile_neighbors& get_neighbors()
		{
			return neighbors;
		}

		tile_neighbors& get_neighbors() const
		{
			return neighbors;
		}

		tile_type* get_neighbor(neighbor_direction nd)
		{
			return neighbors[nd];
		}

		pair<int, int> get_grid_position() const
		{
			return grid_position;
		}


	protected:
		tile_neighbors neighbors;
		const pair<int, int> grid_position;


	};
}
