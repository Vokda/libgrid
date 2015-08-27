#ifndef SUPPORT_H
#define SUPPORT_H
#include <map>
#include <sstream>
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

		string save() const
		{
			//save neighbors
			stringstream ss;
			/*for(auto& neighbor : neighbors)
			{
				int neighbor_x = neighbor.second->get_grid_position().first;
				int neighbor_y = neighbor.second->get_grid_position().second;
				// neighbor.first = neighbor direction, neighbor pointer is saved as neighbor's position
				ss << neighbor.first << ',' << neighbor_x << ',' << neighbor_y << ',';
			}*/

			//grid_position;
			ss << (grid_position.first) << ' ' << (grid_position.second) << ' ';
			return ss.str();
		}

		void load(const string& s)
		{
			stringstream ss(s);
			//int n_x, n_y, nd;
			//neighbor_direction nd;
			//ss >> nd >> n_x >> n_y;
			//auto neighbor = g->get_tile(n_x, n_y);
			//add_neighbor(neighbor, (neighbor_direction) nd);

			//position of this tile
			ss >> const_cast<int&> (grid_position.first) >> const_cast<int&>(grid_position.second);
		}

	protected:
		tile_neighbors neighbors;
		const pair<int, int> grid_position;


	};
}
#endif
