#include <iostream>
#include <fstream>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <limits>
#include <sstream>
#include <iomanip>
using namespace std;

struct city {
	/// Constructor.
	city(int id, double x, double y) : id(id), x(x), y(y)
	{
	}
	int id;
	double x;
	double y;
};

struct instanceInput {
	int n;
	vector<city> cities;
};
struct solution {
	int start;
	double value = 0;
	vector<int> tour;
};

struct permutacion {
	int i;
	int j;
	vector<int> ordenPiezas;
};

//Evalua un tour, requiere del tour y de la matriz de distancias asociada. Devuelve un strcut solution {start; value; tour}
double evaluateTour(vector<int> tourPosible, vector<vector<double>> instanceMatrix){
	double result = 0;
	int i = 0;
	while(i < (int) tourPosible.size() - 1){
		//cout << "Iteracion: " << i << " // Ciudades: " << tourPosible[i] << ", " << tourPosible[i+1] << endl;
		result += instanceMatrix[tourPosible[i]-1][tourPosible[i+1]-1];
		i++;
	}
	return result;
}

vector<int> slicing(vector<int>& arr, int X, int Y) 
{ 
    // Starting and Ending iterators 
    auto start = arr.begin() + X; 
    auto end = arr.begin() + Y + 1; 
    if (X < 0) start = arr.begin();
    if (Y < 0) end = arr.end(); 
    // To store the sliced vector 
    vector<int> result(end - start); 
    // Copy vector using copy function() 
    copy(start, end, result.begin()); 
    // Return the final sliced vector 
    return result; 
}
solution tabuSearch2Opt(solution solucionInicial, vector<vector<double>> instanceMatrix, int tabuListLength, int maxIterations, int maxTime){
	pair<int, int> cuttedEdges;
	pair<int, int> bestCuttedEdges;

	solution tabuSearchResult = solucionInicial;
	solution bestIterationSolution;
	double actualTourValue;
	double bestTour;

	vector<pair<int, int>> tabuList;
	

	vector<int> tourInicial = solucionInicial.tour;
	vector<int> actualTour, slice1, slice2, slice3;

	int n = solucionInicial.tour.size() - 1; /// Resto uno porque ciudad inicial se repite al final
	int failedIterations = 0;

	clock_t start, end;
	cout << "Initial result: " << tabuSearchResult.value << endl;
	
	start = clock();
	while(failedIterations < maxIterations)
	{
		/// Search vicinity
		bestTour  = numeric_limits<double>::infinity();

		for (int i = 0; i < n-2; ++i)
		{
			for (int j = i+2; j < n && j-i != n - 1; ++j)
			{
				cuttedEdges = make_pair (i, j);
				if (find(tabuList.begin(), tabuList.end(), cuttedEdges) == tabuList.end()) /// Si el swap esta en TL no hacemos nada
				{
					actualTour = tourInicial;
					slice1 = slicing(actualTour, -1, i);
					slice2 = slicing(actualTour, i+1, j);
					slice3 = slicing(actualTour, j+1, -1);

					reverse(slice2.begin(), slice2.end());

					slice1.insert( slice1.end(), slice2.begin(), slice2.end() );
					slice1.insert( slice1.end(), slice3.begin(), slice3.end() );

					actualTour = slice1;
					
					actualTourValue = evaluateTour(actualTour, instanceMatrix); /// Obtenemos valor del tour

					if (actualTourValue < bestTour) /// Si el valor del tour actual es mejor que el best, actualizamos.
					{
						bestIterationSolution.start = actualTour[0];
						bestIterationSolution.value = actualTourValue;	/// Se actualiza mejor solucion
						bestIterationSolution.tour = actualTour;

						bestCuttedEdges = cuttedEdges;				/// Se actualiza el mejor par de cortes
						bestTour = actualTourValue;
					}
				}
			}
		}
		
		/// TL & BEST SOLUTION UPDATE ///
		if (tabuList.size() < tabuListLength)	///
		{										///
			tabuList.push_back(bestCuttedEdges);///
		}										/// Se actualiza Tabu List dependiendo del largo actual
		else									///
		{										///
			tabuList.erase(tabuList.begin());	///
			tabuList.push_back(bestCuttedEdges); 		///
		}										///
		//cout << "Mejor tour value de la iteracion: " << bestIterationSolution.value << endl;
		if (tabuSearchResult.value > bestIterationSolution.value)	///
		{															///
			tabuSearchResult.start = bestIterationSolution.start;	/// Se actualiza la mejor solucion encontrada de todas las iteraciones
			tabuSearchResult.value = bestIterationSolution.value;	///	solo si la de la iteracion actual es mejor.
			tabuSearchResult.tour = bestIterationSolution.tour;		///	Si ese son es el caso, se suma 1 a las iteraciones fallidas.	

			failedIterations = 0;
			cout << "Mejor tour value de momento: " << tabuSearchResult.value << endl;	
		}															
		else failedIterations++;	
		end = clock();
		double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
		if (time_taken >= maxTime)
		{
			break;
		}
		tourInicial = bestIterationSolution.tour;

	}

	return tabuSearchResult;
}

instanceInput getInstance(string path){
	instanceInput instance;
	ifstream instance_file (path);
	string line, a, b, c;
	int dimension, id;
	double x, y;
	if (instance_file.is_open())
	{
		getline(instance_file, line);
		istringstream iss;
		iss.str(line);
		iss >> a >> b >> c;
		cout << "Procesando instancia: " << c << endl;
		getline(instance_file, line);
		getline(instance_file, line);
		getline(instance_file, line);
		iss.clear();
		iss.str(line);
		iss >> a >> b >> dimension;
		cout << setprecision(10) << fixed <<  "Tamaño instancia: " << dimension << endl;
		getline(instance_file, line);
		getline(instance_file, line);
		iss.clear();

		for (int i = 0; i < dimension; i++)
		{
			getline(instance_file, line);
			iss.str(line);
			iss >> id >> x >> y;
			//cout << "datos: " << id << ", "<< x << ", " << y << endl;
			instance.cities.push_back(city(id, x, y));
			iss.clear();
		}
	}
	instance.n = dimension;
	return instance;
}

double calculateDistance(double x1, double y1, double x2, double y2){
	return sqrt(pow((x2-x1), 2.0) + pow((y2-y1), 2.0));
}

void eraseValue(vector<int> &array, int value){
	array.erase(remove(array.begin(), array.end(),value), array.end());
}

void generateOutput(solution greedy, string file_name, int seed, double time){
	vector<int> tour = greedy.tour;

	ofstream myfile;
	myfile.open ("Outputs_TSP/" + file_name + ".output", std::ios_base::app);
	if (myfile.is_open())
	{
		myfile << "Seed Value: " << seed << endl;
		myfile << "Tiempo de Ejecución: " << time << " segundos"<< endl;
		myfile << "Costo del tour: " << greedy.value << endl << "Tour encontrado:" << endl << endl;
 		for (auto content: tour)
		{
			myfile << content << " ";
			//cout << content << " ";
		}
		myfile << endl << endl << "--------------------------------------------------------------------------------------------------" << endl << endl;
	}
	myfile.close();
		
	
}

solution greedySolution(vector<vector<double>> instanceMatrix){
	solution greedy;
	int dimension = (int) instanceMatrix.size();
	int next, start = rand()%dimension; //obtenemos una ciudad inicial random
	int first = start;
	double minimalDistance;

	vector<int> remainingCities;
	for (int i = 0; i < dimension; ++i) //Creamos una lista con las ciudades
	{
		remainingCities.push_back(i);
	}
	eraseValue(remainingCities, start);
	greedy.tour.push_back(start+1);

	for (int i = 0; i < dimension-1; ++i)
	{
		minimalDistance = numeric_limits<double>::infinity();
		for (auto candidate: remainingCities)
		{
			if (instanceMatrix[start][candidate] < minimalDistance)	//buscamos la ciudad de menor costo
			{
				minimalDistance = instanceMatrix[start][candidate];
				next = candidate;
			}
		}
		eraseValue(remainingCities, next);	//La borramos de los candidatos y agregamos al tour con su costo
		start = next;
		greedy.tour.push_back(start+1);
		greedy.value += minimalDistance;	
	}
	greedy.tour.push_back(first+1); //cerramos el tour ciclico
	greedy.value += instanceMatrix[start][first]; //le sumamos el ultimo camino de vuelta
	greedy.start = first+1;

	//cout << "Comenzando test..." << endl;
	//solution test = evaluateTour(greedy.tour, instanceMatrix);

	//cout << "Resultado greedy: " << greedy.value << endl;
	//cout << "Resultado test: " << test.value << endl;

	return greedy;
}

solution greedySolutionBest(vector<vector<double>> instanceMatrix){
	solution greedy;
	int dimension = (int) instanceMatrix.size();
	int next, init;
	
	double minimalDistance, best = numeric_limits<double>::infinity();

	for (int ciudad = 0; ciudad < dimension; ++ciudad)
	{
		int start = ciudad;
		int first = start;
		double value = 0;
		vector<int> tour;
		vector<int> remainingCities;
		for (int i = 0; i < dimension; ++i) //Creamos una lista con las ciudades
		{
			remainingCities.push_back(i);
		}
		eraseValue(remainingCities, start);
		tour.push_back(start+1);

		for (int i = 0; i < dimension-1; ++i)
		{
			minimalDistance = numeric_limits<double>::infinity();
			for (auto candidate: remainingCities)
			{
				if (instanceMatrix[start][candidate] < minimalDistance)	//buscamos la ciudad de menor costo
				{
					minimalDistance = instanceMatrix[start][candidate];
					next = candidate;
				}
			}
			eraseValue(remainingCities, next);	//La borramos de los candidatos y agregamos al tour con su costo
			start = next;
			tour.push_back(start+1);
			value += minimalDistance;	
		}
		tour.push_back(first+1); //cerramos el tour ciclico
		value += instanceMatrix[start][first]; //le sumamos el ultimo camino de vuelta
		init = first+1;
		if (value < best)
		{
			best = value;
			greedy.tour = tour;
			greedy.value = value;
			greedy.start = init;
		}

	}

	//cout << "Comenzando test..." << endl;
	//solution test = evaluateTour(greedy.tour, instanceMatrix);

	//cout << "Resultado greedy: " << greedy.value << endl;
	//cout << "Resultado test: " << test.value << endl;

	return greedy;
}

solution greedySolutionWorst(vector<vector<double>> instanceMatrix){
	solution greedy;
	int dimension = (int) instanceMatrix.size();
	int next, init;
	
	double minimalDistance, worst = 0; //numeric_limits<double>::infinity();

	for (int ciudad = 0; ciudad < dimension; ++ciudad)
	{
		int start = ciudad;
		int first = start;
		double value = 0;
		vector<int> tour;
		vector<int> remainingCities;
		for (int i = 0; i < dimension; ++i) //Creamos una lista con las ciudades
		{
			remainingCities.push_back(i);
		}
		eraseValue(remainingCities, start);
		tour.push_back(start+1);

		for (int i = 0; i < dimension-1; ++i)
		{
			minimalDistance = numeric_limits<double>::infinity();
			for (auto candidate: remainingCities)
			{
				if (instanceMatrix[start][candidate] < minimalDistance)	//buscamos la ciudad de menor costo
				{
					minimalDistance = instanceMatrix[start][candidate];
					next = candidate;
				}
			}
			eraseValue(remainingCities, next);	//La borramos de los candidatos y agregamos al tour con su costo
			start = next;
			tour.push_back(start+1);
			value += minimalDistance;	
		}
		tour.push_back(first+1); //cerramos el tour ciclico
		value += instanceMatrix[start][first]; //le sumamos el ultimo camino de vuelta
		init = first+1;
		if (value > worst)
		{
			worst = value;
			greedy.tour = tour;
			greedy.value = value;
			greedy.start = init;
		}

	}

	//cout << "Comenzando test..." << endl;
	//solution test = evaluateTour(greedy.tour, instanceMatrix);

	//cout << "Resultado greedy: " << greedy.value << endl;
	//cout << "Resultado test: " << test.value << endl;

	return greedy;
}

vector<string> list_dir(const char *path) {
	struct dirent *entry;
	DIR *dir = opendir(path);
	vector<string> instancesArray; 

	if (dir == NULL) {
		cout << "Error abriendo directorio de instancias..." << endl;
		exit(0);
 	}
	while ( ((entry = readdir(dir)) != NULL) ){
		if ((strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) && strcmp(entry->d_name, ".DS_Store"))
		{
			instancesArray.push_back(entry->d_name);
		}
		
	}
	closedir(dir);
	return instancesArray;
}

vector<double> makeIntervalArray(double min, double max, int cant)
{
	vector<double> arr;
	arr.push_back(min);
	if (min == max)
	{
		return arr;
	}
	double num = (max - min)/(cant - 1);
	
	for (int i = 1; i < cant-1; ++i)
	{
		arr.push_back(min + num * i);
	}
	arr.push_back(max);
	return arr;
}

void reportResult(string file_name, double value, double TLLengthMultiplier, int mode){
	
	ofstream myfile;
	myfile.open ("TLLControl_" + file_name + ".result", std::ios_base::app);
	if (myfile.is_open())
	{
		if (mode == 0)
		{
			myfile << TLLengthMultiplier << ";" << value << endl;	
		}
		else
		{
			myfile << endl << "Best TLLMultiplier: " << TLLengthMultiplier << " | with value: " << value << endl << "*************\n";	
		}
	}
	myfile.close();

}

int main(int argc, char * argv[])
{
	int maxIterations = strtol(argv[1], nullptr, 0);
	int maxTime = strtol(argv[2], nullptr, 0);
	double intervalMin = strtod(argv[3], nullptr);
	double intervalMax = strtod(argv[4], nullptr);
	int intervalLength = strtol(argv[5], nullptr, 0);

	vector<double> TLLMultiplierArray = makeIntervalArray(intervalMin, intervalMax, intervalLength);
	vector<string> instancesArray = list_dir("Instancias_TSP_3");

	

	
		
	for (int k = 6; k < argc; ++k)
	{
		for (unsigned i = 0; i < instancesArray.size(); ++i)
		{
			instanceInput instanceData = getInstance("Instancias_TSP_3/" + instancesArray[i] );
			int dimension = instanceData.n;
			//generating matrix
			double distance;
			double x1, y1, x2, y2;
			vector<vector<double>> instanceMatrix(dimension, vector<double>(dimension,0.0));
			for (int i = 0; i < dimension; ++i)
			{
				for (int j = i+1; j < dimension; ++j)
				{
					x1 = instanceData.cities[i].x;
					y1 = instanceData.cities[i].y;

					x2 = instanceData.cities[j].x;
					y2 = instanceData.cities[j].y;

					//cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;

					distance = calculateDistance(x1, y1, x2, y2);
					instanceMatrix[i][j] = distance;
					instanceMatrix[j][i] = distance;

					//cout << "camino: | " << i << " -> " << j << " | posee costo: " << instanceMatrix[i][j] << endl;
				}
			}
			double bestMultiplier, bestValue = 999999;
			for (auto TLLengthMultiplier:TLLMultiplierArray)
			{
				int seed = strtol(argv[k], nullptr, 0);
				srand(seed);
				clock_t start, end;

				start = clock();
				
				solution greedy = greedySolutionBest(instanceMatrix);

				cout << "TLLengthMultiplier: " << TLLengthMultiplier << endl;

				solution ts = tabuSearch2Opt(greedy, instanceMatrix, dimension*TLLengthMultiplier, maxIterations, maxTime); //_,_,TLLength,maxIterations, time [s] 
				end = clock();
				double time_taken = double(end - start) / double(CLOCKS_PER_SEC);

				cout << "Tiempo de ejecucion: " << time_taken << " segundos" << endl;

				cout << "inicio: " << ts.start << " - costo: " << ts.value << endl << endl;
				generateOutput(ts, instancesArray[i], seed, time_taken);
				reportResult(instancesArray[i], ts.value, TLLengthMultiplier, 0);
				if (ts.value < bestValue)
				{
					bestValue = ts.value;
					bestMultiplier = TLLengthMultiplier;
				}
			}
			reportResult(instancesArray[i], bestValue, bestMultiplier, 1);
		}		
	}
	return 0;
}