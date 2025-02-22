/*
 * GeneticTspSolver.cpp
 */
#include "Graph.cpp"
class Gene
	{
	public:
		vector<int >route;
		int cost;
		Gene (vector<int >route ,int cost)
			{
				this->route = route;
				this->cost = cost;
			}
		bool operator== (Gene const &gene)
			{
				return this->route == gene.route;
			}
		bool operator< (const Gene &other) const
			{
				return cost < other.cost;
			}
	};
class GeneticTspSolver
	{
	public:
		Graph *graph; // the graph
		vector<Gene >population;
		vector<Gene >newGeneration;
		int populationSize; // size of population
		int generations; // amount of generations
		int mutationRate; // mutation rate
//	public:
		GeneticTspSolver (Graph *graph ,int populationSize ,int generations ,
				int mutationRate)
			{
				this->populationSize = populationSize;
				this->generations = generations;
				this->mutationRate = mutationRate;
				this->graph = graph;
				initialPopulation( );
			}

		int getCost (vector<int >&path)
			{
				int cost = 0;
				for (int index = 1 ;index < graph->vertexCount ;index++)
					{
						int node = path[(index - 1)];
						int nextNode = path[index];
						if (graph->edges.find(make_pair(node ,nextNode))
								== graph->edges.end( ))
							{
								return -1;
							}
						cost +=
								(graph->edges.find(make_pair(node ,nextNode)))->second;
					}
				return cost;
			}

		bool isPathAlreadyExists (Gene path)
			{
				for (auto i = population.begin( ) ;i != population.end( ) ;i++)
					{
						if (path == (*i))
							return true;
					}
				return false;
			}

		void initialPopulation ( )
			{
				srand((unsigned) time(0));
				do
					{
						vector<int >route;
						for (;route.size( ) < (graph->vertexCount) ;)
							{
								int random =
										(rand( ) % this->graph->vertexCount);
								vector<int >::iterator it = std::find(
										route.begin( ) ,route.end( ) ,random);
								if (it == route.end( ))
									route.push_back(random);
							}
						int cost = getCost(route);
						Gene newGene(route ,cost);
						if ((!(isPathAlreadyExists(newGene))) && (cost != -1))
							population.push_back(newGene);
					} while (population.size( ) <= populationSize);
			}
		void sortPopulation ( )
			{
				std::sort(population.begin( ) ,population.end( ) ,
						[ ] (const Gene &g1 ,const Gene &g2)
							{
								return g1.cost < g2.cost;
							});
			}
		void printPopulation ( )
			{
				for (auto i = population.begin( ) ;i != population.end( ) ;i++)
					{
						for (auto pathIndex = i->route.begin( ) ;
								pathIndex != i->route.end( ) ;pathIndex++)
							{
								cout << (*pathIndex) << " -> ";
							}
						cout << " with cost of: " << i->cost << endl;
					}
			}
		void printGene (int index)
			{
				for (auto pathIndex = population[index].route.begin( ) ;
						pathIndex != population[index].route.end( ) ;
						pathIndex++)
					{
						cout << (*pathIndex) << " -> ";
					}
				cout << " with cost of: " << population[index].cost << endl;

			}
		void printGene (Gene gene)
			{
				for (auto pathIndex = gene.route.begin( ) ;
						pathIndex != gene.route.end( ) ;pathIndex++)
					{
						cout << (*pathIndex) << " -> ";
					}
				cout << " with cost of: " << gene.cost << endl;

			}
		void crossOver (Gene &parent1 ,Gene &parent2 ,int crossOverIndex)
			{
				Gene tempChild = parent1;

				for (int index = 0 ;index < crossOverIndex ;index++)
					{
						parent1.route[index] = parent2.route[index];
					}
				for (int index = 0 ;crossOverIndex < parent1.route.size( ) ;
						crossOverIndex++ ,index++)
					{
						parent2.route[index] = tempChild.route[index];
					}

				int stats[parent1.route.size( )];// create an array to keep the statistics of routes

				// trying to repair the first gene after crossover
				for (int index = 0 ;index < parent1.route.size( ) ;index++)
					{
						auto count3 = count(parent1.route.begin( ) ,
								parent1.route.end( ) ,index);
						stats[index] = count3;
					}
				for (int index = 0 ;index < parent1.route.size( ) ;index++)
					{
						for (;stats[index] > 1 ;)
							{
								for (int i = 0 ;i < parent1.route.size( ) ;i++)
									if (stats[i] == 0)
										{
											auto count3 = find(
													parent1.route.begin( ) ,
													parent1.route.end( ) ,
													index);
											(*count3) = i;
											--stats[index];
											stats[i] = 1;
											break;
										}
							}
					}
				parent1.cost = getCost(parent1.route);
				// then we repair second gene
				for (int index = 0 ;index < parent2.route.size( ) ;index++)
					{
						auto count3 = count(parent2.route.begin( ) ,
								parent2.route.end( ) ,index);
						stats[index] = count3;
					}
				for (int index = 0 ;index < parent2.route.size( ) ;index++)
					{
						for (;stats[index] > 1 ;)
							{
								for (int i = 0 ;i < parent2.route.size( ) ;i++)
									if (stats[i] == 0)
										{
											auto count3 = find(
													parent2.route.begin( ) ,
													parent2.route.end( ) ,
													index);
											(*count3) = i;
											--stats[index];
											stats[i] = 1;
											break;
										}
							}
					}
				parent2.cost = getCost(parent2.route);
			}
		void applyMutation (int percent)
			{

				for (int i = 0 ;i < ((mutationRate * populationSize) / 100) ;
						i++)
					{
						int mutationIndex = (rand( ) % populationSize);
						do
							{
								mutationIndex = (rand( ) % populationSize);
							} while (mutationIndex
								<= (10 * populationSize / 100));
						do
							{
								int position1;
								int position2;
								do
									{
										position1 = (rand( )
												% graph->vertexCount);
										position2 = (rand( )
												% graph->vertexCount);

									} while (position1 == position2);
								int tempNumber =
										population[mutationIndex].route[position1];
								population[mutationIndex].route[position1] =
										population[mutationIndex].route[position2];
								population[mutationIndex].route[position2] =
										tempNumber;
								population[mutationIndex].cost = getCost(
										population[mutationIndex].route);
							} while (population[mutationIndex].cost == -1);
					}
			}
		void applyMutation (Gene &gene)
			{
				do
					{
						int position1;
						int position2;
						do
							{
								position1 = (rand( ) % graph->vertexCount);
								position2 = (rand( ) % graph->vertexCount);

							} while (position1 == position2);
						int tempNumber = gene.route[position1];
						gene.route[position1] = gene.route[position2];
						gene.route[position2] = tempNumber;

						gene.cost = getCost(gene.route);
					} while (gene.cost == -1);
			}
		void solve ( )
			{
				for (int generationIndex = 1 ;generationIndex <= generations ;
						generationIndex++)
					{
						printPopulation( );
						cout
								<< "*******************************************************************\n";
						sort(population.begin( ) ,population.end( ));
						cout << "generation: " << generationIndex
								<< " best of generation: ";
						printGene(population[0]);
						cout
								<< "----------------------------------------------------\n";

						for (int index = (10 * populationSize / 100) ;
								index < populationSize ;)
							{
								crossOver(population[index] ,
										population[(index + 1)] ,
										population[index].route.size( ) / 2);
								if (population[index].cost == -1)
									applyMutation(population[index]);
								if (population[index + 1].cost == -1)
									applyMutation(population[index + 1]);
								index = index + 2;
							}
						applyMutation(mutationRate);

					}
				cout << "\n the best solution is:";
				printGene(population[0]);

			}
	};

