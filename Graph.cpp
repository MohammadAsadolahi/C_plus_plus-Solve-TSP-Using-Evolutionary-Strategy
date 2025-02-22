/*
 * Graph.cpp
 */
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility> // pair
#include <time.h> // time
#include <stdlib.h> // srand, rand
#include <algorithm>
using namespace std;
class Graph
	{
	public:
		int vertexCount; // number of Vertices
		int EdgeCount; // number of total Edge
		int startVertex; // start vertex
		map<pair<int ,int >,int >edges; // map of the edges

//	public:
		Graph (int vertexCount ,int startVertex)
			{
				this->vertexCount = vertexCount;
				this->startVertex = startVertex;
				this->EdgeCount = 0;
			}
		void printGraph ( )
			{
				map<pair<int ,int >,int >::iterator it;
				for (it = edges.begin( ) ;it != edges.end( ) ;++it)
					{
						cout << it->first.first << " linked to vertex "
								<< it->first.second << " with weight "
								<< it->second << endl;
					}
			}
		void addEdge (int source ,int destination ,int weight) // add a edge
			{
				edges[make_pair(source ,destination)] = weight;// adds edge in the map
			}
	};
