/*
 * Graph.cpp
 */

#include "GeneticTspSolver.cpp"
using namespace std;

int main() {

	Graph *graph2 = new Graph(5, 3);
	graph2->addEdge(0, 1, 1);
	graph2->addEdge(1, 0, 1);
	graph2->addEdge(0, 2, 3);
	graph2->addEdge(2, 0, 3);
	graph2->addEdge(0, 3, 4);
	graph2->addEdge(3, 0, 4);
	graph2->addEdge(0, 4, 5);
	graph2->addEdge(4, 0, 5);
	graph2->addEdge(1, 2, 1);
	graph2->addEdge(2, 1, 1);
	graph2->addEdge(1, 3, 4);
	graph2->addEdge(3, 1, 4);
	graph2->addEdge(1, 4, 8);
	graph2->addEdge(4, 1, 8);
	graph2->addEdge(2, 3, 5);
	graph2->addEdge(3, 2, 5);
	graph2->addEdge(2, 4, 1);
	graph2->addEdge(4, 2, 1);
	graph2->addEdge(3, 4, 2);
	graph2->addEdge(4, 3, 2);

	//graph1->printGraph( );
	srand(time(NULL)); // random numbers
	Graph *graph1 = new Graph(9, 3);
	graph1->addEdge(0, 1, 1);
	graph1->addEdge(1, 0, 1);
	graph1->addEdge(0, 2, 3);
	graph1->addEdge(2, 0, 3);
	graph1->addEdge(0, 3, 4);
	graph1->addEdge(3, 0, 4);
	graph1->addEdge(0, 4, 5);
	graph1->addEdge(4, 0, 5);
	graph1->addEdge(1, 2, 1);
	graph1->addEdge(2, 1, 1);
	graph1->addEdge(1, 3, 4);
	graph1->addEdge(3, 1, 4);
	graph1->addEdge(1, 4, 8);
	graph1->addEdge(4, 1, 8);
	graph1->addEdge(2, 3, 5);
	graph1->addEdge(3, 2, 5);
	graph1->addEdge(2, 4, 1);
	graph1->addEdge(4, 2, 1);
	graph1->addEdge(3, 4, 2);
	graph1->addEdge(4, 3, 2);

	// creates the graph2
	graph1->addEdge(5, 6, 2);
	graph1->addEdge(6, 5, 2);
	graph1->addEdge(5, 7, 6);
	graph1->addEdge(7, 5, 6);
	graph1->addEdge(5, 8, 3);
	graph1->addEdge(8, 5, 3);
	graph1->addEdge(6, 8, 7);
	graph1->addEdge(8, 6, 7);
	graph1->addEdge(6, 7, 4);
	graph1->addEdge(7, 6, 4);
	graph1->addEdge(7, 8, 2);
	graph1->addEdge(8, 7, 2);

	graph1->addEdge(3, 5, 7);
	graph1->addEdge(5, 3, 7);

	//	graph2->printGraph();
	graph2->printGraph( );

	//GeneticTspSolver geneticTspSolverMin(graph2, 10, 100, 20);

	GeneticTspSolver geneticTspSolver(graph1, 30, 20, 5);

	const clock_t begin_time = clock();

	geneticTspSolver.solve( );
	//geneticTspSolverMin.solve();

	cout << "\n\nTime for to run the genetic algorithm: "
			<< float(clock() - begin_time) / CLOCKS_PER_SEC << " seconds.";

	return 0;
}
