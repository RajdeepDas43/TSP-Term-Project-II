
/*
This code is a genetic algorithm implementation for solving a specific optimization problem. Let's go through the code step by step to understand its functionality:

1. The code includes the header file "useful.cpp", which presumably contains some useful functions or definitions for the algorithm.

2. The code defines a macro `pdvi` as a pair of a double and a vector of integers. This is used to represent a solution chromosome in the genetic algorithm.

3. The code defines two enums: `selectionType` and `mutationType`, which are used to specify the type of selection and mutation methods used in the genetic algorithm.

4. Several global variables are declared and initialized, such as `numOfEpochs` (number of iterations), `generationSize` (number of individuals in each generation), `tournamentSize` (number of individuals selected in tournament selection), `mutationRate` (probability of mutation), `elitism` (flag indicating whether elitism is applied), `st` (selection type), and `mt` (mutation type).

5. Some global data structures are declared, including `e` (a 2D vector representing some data), and `generation` (a multimap storing the chromosomes and their corresponding fitness values).

6. The code defines several functions:
   - `startrandomGeneration()` initializes the `generation` multimap with randomly generated chromosomes.
   - `startGreedyGeneration()` initializes the `generation` multimap using a greedy heuristic.
   - `swapMutate()` applies swap mutation to a given chromosome.
   - `reverseMutate()` applies reverse mutation to a given chromosome.
   - `sumOfFitnesses()` calculates the sum of fitness values of all chromosomes in the `generation` multimap.
   - `proportional()` selects a chromosome from the `generation` multimap using proportional selection.
   - `tournament()` selects a chromosome from the `generation` multimap using tournament selection.
   - `rankBased()` selects a chromosome from the `generation` multimap using rank-based selection.
   - `crossover()` performs crossover between two parent chromosomes to generate a new child chromosome.
   - `nextGeneration()` generates the next generation of chromosomes by applying selection, crossover, and mutation operations.
   - `partialBrute()` performs a partial brute-force search by trying different combinations of elements in the best solution found so far.
   - `sigint()` is a signal handler for the SIGINT signal (interrupt signal) that performs some operations and prints the current best solution when the program is interrupted.

7. The `main()` function is the entry point of the program:
   - It sets up the necessary configurations, such as disabling synchronization with C I/O functions, setting up signal handling for SIGINT, and seeding the random number generator.
   - It reads input data from a file specified by the command-line argument and stores it in the `e` matrix.
   - It initializes the initial generation of chromosomes using either random generation or greedy generation, depending on the commented line.
   - It sets the initial best solution and its fitness value.
   - It performs the genetic algorithm iterations for the specified number of epochs, printing the best fitness value in each generation.
   - If the program is interrupted (SIGINT signal), it calls the `sigint()` function to perform some operations and print the current best solution.

Overall, the code implements a genetic algorithm to solve an optimization problem, where the goal is to find the best chromosome (solution) that maximizes the fitness value. The algorithm involves generating an initial population of chromosomes, performing selection, crossover, and mutation operations to create new generations, and iteratively improving the solution over a certain number of epochs.
*/
#include "useful.cpp"
#define pdvi pair<double, vector<int> >
using namespace std;

enum selectionType{tourSel, rankSel, propSel};
enum mutationType{swapMut, revMut};

int numOfEpochs = 1000;
int generationSize = 1000;
int tournamentSize = 2;
double mutationRate = 0.007;
bool elitism = 0;
selectionType st = tourSel;
mutationType mt = swapMut;

int instanceSize;
vector<int> theBestEver;
double theBestResult;
vector<vector<double>> e;
multimap<double,vector<int>> generation;

void startrandomGeneration() {
  vector<int> chromosome;
  for (int j = 0; j < e.size(); ++j) chromosome.push_back(j);
  while (generation.size() < generationSize) {
    random_shuffle(chromosome.begin(), chromosome.end());
    generation.emplace(result(e, chromosome), chromosome);
  }
}

void startGreedyGeneration(){
  vector<int> seq;
  for (int i = 0; i < e.size(); ++i){
    seq = greedy(e, i);
    generation.emplace(result(e, seq), seq);
  }
  vector<int> chromosome;
  for (int j = 0; j < e.size(); ++j) chromosome.push_back(j);
  while (generation.size() < generationSize) {
    random_shuffle(chromosome.begin(), chromosome.end());
    generation.emplace(result(e,chromosome),chromosome);
  }
}

void swapMutate(vector<int> &v) {
  for (int i = 0; i < v.size(); ++i)
    if (rand() / (double)RAND_MAX < mutationRate) {
      int a = rand() % (int)v.size();
      swap(v[i], v[a]);
    }
}

void reverseMutate(vector<int> &v) {
  if (rand() / (double)RAND_MAX < mutationRate) {
    int a = rand() % (int)v.size();
    int b = rand() % (int)v.size();
    if(a > b) swap(a, b);
    for (int i = 0; i <= (b-a)/2; ++i){
      swap(v[a+i], v[b-i]);
    }
  }
}

double sumOfFitnesses(){
  double sum = 0;
  auto ptr = generation.begin();
  for (; ptr != generation.end(); ++ptr){
    sum += ptr->first;
  }
  return sum;
}

vector<int> proportional() {
  double sum;
  double r = rand() / RAND_MAX;
  sum = sumOfFitnesses();
  auto ptr = generation.begin();
  for (; r > 0; ++ptr) r -= ptr->first / sum;
  return ptr->second;
}

vector<int> tournament() {
  auto item = generation.begin();
  auto minitem = generation.begin();
  for (int i = 0; i < tournamentSize; ++i) {
    item = generation.begin();
    advance(item, rand() % (int)generation.size()); //przesun do przodu o r el.
    if(item->first < minitem->first)
      minitem = item;
  }
  return minitem->second;
}

vector<int> rankBased() {
  double base = 1 / (((generationSize+1) * generationSize) / 2);
  double r = rand() / (double)RAND_MAX;
  for (int i = 0; i < generationSize; ++i){
    r -= base * (generationSize-i);
    if(r <= 0){
      auto ptr = generation.begin();
      advance(ptr,i);
      return ptr->second;
    }
  }
  return generation.begin()->second;
}

vector<int> crossover(vector<int> &a, vector<int> &b) {
  vector<int> newChromosome;
  int start = rand() % (int)e.size();
  int end = rand() % (int)e.size();
  for (int i = start; i != end; ++i, i %= (int)e.size())
    newChromosome.push_back(a[i]);
  for (int i = 0; i < b.size(); ++i)
    if (find(newChromosome.begin(), newChromosome.end(), b[i]) ==
        newChromosome.end())
      newChromosome.push_back(b[i]);
  return newChromosome;
}

void nextGeneration() {
  multimap<double, vector<int>> newGeneration;
  vector<int> a, b, ab;
  if (elitism) newGeneration.insert(*generation.begin());
  while (newGeneration.size() < generationSize) {
    if (st == tourSel)
      a = tournament(), b = tournament();
    else if (st == rankSel)
      a = rankBased(), b = rankBased();
    else if (st == propSel)
      a = proportional(), b = proportional();

    ab = crossover(a, b);

    if (mt == swapMut)
      swapMutate(ab);
    else if(mt == revMut)
      reverseMutate(ab);
    newGeneration.emplace(result(e, ab), ab);
  }
  generation = newGeneration;
  if(generation.begin()->first < theBestResult){
    theBestResult = generation.begin()->first;
    theBestEver = generation.begin()->second;
  }

}

void partialBrute(){
  int n = 10;
  int step = 4;
  int count = 1000;
  float progress = 0.0;
  int barWidth = 70;
  vector<int> tmpvec;
  for (int i = 0; i < count; i++){
    if(i%5 == 0){
    progress = (float)i/(float)count;
    cout << "[";
    int pos = int((float)barWidth * progress);
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << int(progress * 100.0) << " %\r";
    cout.flush();
  }
    for (int j = 0; j < n; ++j)
      tmpvec.push_back(theBestEver[(i*step + j) % instanceSize]);
    bruteforce(e, tmpvec);
    for (int j = 0; j < n; ++j)
      theBestEver[(i*step + j) % instanceSize] = tmpvec[j];
    tmpvec.clear();
  }
  theBestResult = result(e, theBestEver);
  cout<<endl;
}

void sigint(int a) {
  partialBrute();
  cout<<"najlepszy: "<<endl;
  for (int i = 0; i < instanceSize; ++i)
    cout<<theBestEver[i] + 1<<" ";
  cout<<endl;
  cout<<"best ever: "<<theBestResult<<endl;
  cout<<"NoE: "<<numOfEpochs<<endl;
  cout<<"genSize: "<<generationSize<<endl;
  cout<<"tourSize: "<<tournamentSize<<endl;
  cout<<"mutRate: "<<mutationRate<<endl;
  cout<<"elitism: "<<elitism<<endl;
  cout<<"selType: "<<st<<endl;
  cout<<"mutType: "<<mt<<endl;
  exit(0);
}

int main(int argc, char const *argv[]) {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  signal(SIGINT, sigint);
  srand(int(time(NULL))); rand();
  instanceSize = readInputFile(argv[1], e);
  // startrandomGeneration();
  startGreedyGeneration();
  theBestEver = generation.begin()->second;
  theBestResult = generation.begin()->first;
  cout<<theBestResult<<endl;
  for (int epoch = 0; epoch < numOfEpochs; ++epoch) {
    cout << epoch << "-> best in generation: " << result(e, generation.begin()->second) << endl;
    nextGeneration();
  }
  sigint(0);

  return 0;
}
