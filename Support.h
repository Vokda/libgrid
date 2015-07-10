#pragma once
#include <vector>
using namespace std;

/*
	This file contains additional members of Tile classes that are used with this library
*/

namespace gl
{
	template<class tile_type, class position_type>
	class Additional_Tile_Members
	{
	public:
		typedef vector<tile_type*> tile_neighbors;

		Additional_Tile_Members(int x_pos, int y_pos) :
			grid_position(x_pos, y_pos)
		{}

		Additional_Tile_Members(int x_pos, int y_pos, position_type pt) :
			grid_position(x_pos, y_pos), position(pt)
		{}

		Additional_Tile_Members() :
			grid_position(-1, -1)
		{
		}

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


		void add_neighbor(tile_type* neighbor)
		{
			neighbors.push_back(neighbor);
		}

		tile_neighbors& get_neighbors()
		{
			return neighbors;
		}
	protected:
		tile_neighbors neighbors;



		/*-----------------------------------------------------------------------------
			GRID POSITION
			This is the position in the grid. This cannot change when the tile is created it will be created.
		*/
	public:

		pair<int, int> get_grid_position() const
		{
			return grid_position;
		}


	protected:
		const pair<int, int> grid_position;

		/*----------------------------------------------------------------------------
			CUSTOM POSITION
			This is a position that may or may not relate to the grid position
		*/
	public:
		position_type get_position() const
		{
			return position;
		};
		
		virtual void set_position(position_type p)
		{
			position = p;
		}
	protected:
		position_type position;

	};
}
