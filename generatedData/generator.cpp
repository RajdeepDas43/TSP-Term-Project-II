#include  <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cstring>
using namespace std;

int main(int argc, char* argv[]){
	srand(time(NULL));

	int numOfFiles = atoi(argv[1]);
	int numOfExamples = atoi(argv[2]);
	int min = atoi(argv[3]);
	int max = atoi(argv[4]);

	for (int i = 1; i <= numOfFiles; ++i) {
		string fileName = "test" + to_string(i) + ".tsp";
		ofstream out;
		out.open(fileName.c_str());
		for (int j = 1; j <= numOfExamples; j++){
			double x = (max - min)*((double)rand()/(double)RAND_MAX) + min;
			double y = (max - min)*((double)rand()/(double)RAND_MAX) + min;
			out<<j<<" "<<x<<" "<<y<<"\n";
		}
		out.close();
	}


	return 0;
}