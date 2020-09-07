#include <vector>

#include "Vec2.hpp"

namespace lio
{
	template <typename T, typename U>
	class AStarPathfinder
	{
	public:

		class Node
		{
		private:

			friend class AStarPathfinder<T, U>;

			bool is_obstacle = false;
			bool is_visited = false;
			float h = FLT_MAX;
			float g = FLT_MAX;
			Node* child = nullptr;
			Node* parent = nullptr;

			std::vector<Node*> neighbours;
			Vec2<T> pos;

			Node() = default;
			Node(const Vec2<T>& pos, bool is_obstacle)
				: pos(pos), is_obstacle(is_obstacle)
			{
			}

		public:

			const Node* GetChild() const
			{
				return child;
			}
			const Node* GetParent() const
			{
				return parent;
			}
			const Vec2<T>& GetPos() const
			{
				return pos;
			}
		};

		AStarPathfinder (
		const std::vector<std::vector<U>>& map,
		const Vec2<T>& map_size, 
		const Vec2<T>& start, 
		const Vec2<T>& end)
		{
			// vars
			nodes = new Node[map_size.x * map_size.y];

			// init node grid
			for (auto x = 0u; x < map_size.x; ++x)
			{
				for (auto y = 0u; y < map_size.y; ++y)
				{
					nodes[y * map_size.x + x] = Node(Vec2<T>(x, y), map.at(y).at(x));
				}
			}

			for (auto x = 0u; x < map_size.x; ++x)
			{
				for (auto y = 0u; y < map_size.y; ++y)
				{
					if (y > 0)
						nodes[y * map_size.x + x].neighbours.push_back(&nodes[(y - 1) * map_size.x + (x + 0)]);
					if (y < map_size.y - 1)
						nodes[y * map_size.x + x].neighbours.push_back(&nodes[(y + 1) * map_size.x + (x + 0)]);
					if (x > 0)
						nodes[y * map_size.x + x].neighbours.push_back(&nodes[(y + 0) * map_size.x + (x - 1)]);
					if (x < map_size.x - 1)
						nodes[y * map_size.x + x].neighbours.push_back(&nodes[(y + 0) * map_size.x + (x + 1)]);
				}
			}

			// init start end nodes
			n_start = &nodes[start.y * map_size.x + start.x];
			n_end = &nodes[end.y * map_size.x + end.x];

			// solve
			Node* n_current = n_start;
			n_start->g = 0.f;
			n_start->h = n_start->pos.Distance(n_end->pos);

			std::list<Node*> not_tested_nodes;
			not_tested_nodes.push_back(n_start);

			while (!not_tested_nodes.empty() && n_current != n_end)
			{
				not_tested_nodes.sort([](const Node* lhs, const Node* rhs) { return lhs->h < rhs->h; });

				while (!not_tested_nodes.empty() && not_tested_nodes.front()->is_visited)
					not_tested_nodes.pop_front();

				if (not_tested_nodes.empty()) break;

				n_current = not_tested_nodes.front();
				n_current->is_visited = true;

				for (auto neighbour : n_current->neighbours)
				{
					if (!neighbour->is_visited && !neighbour->is_obstacle)
					{
						not_tested_nodes.push_back(neighbour);
					}

					auto lowest = n_current->g + n_current->pos.Distance(neighbour->pos);

					if (lowest < neighbour->g)
					{
						neighbour->parent = n_current;
						neighbour->g = lowest;
						neighbour->h = neighbour->g + neighbour->pos.Distance(n_end->pos);
					}
				}
			}

			// finish children
			auto p = n_end;
    		while(p->parent != nullptr)
			{
				p->parent->child = p;
				p = p->parent;
			}
		}
		~AStarPathfinder()
		{
			delete[] nodes;
		}

		const Node* GetEndNode() const
		{
			return n_end;
		}
		const Node* GetStartNode() const
		{
			return n_start;
		}

		std::vector<Node*>&& MakePathVector() const
		{
			std::vector<AStarPathfinder::Node*> path;
			Node* p = n_end;

			while (p->parent != nullptr)
			{
				path.push_back(p);
				p = p->parent;
			}

			return std::move(path);
		}

		bool IsSuccess() const
		{
			return n_end->parent;
		}

	private:

		Node* nodes = nullptr;
		Node* n_start;
		Node* n_end;
	};
}