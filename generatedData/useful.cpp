
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
using namespace std;

struct point{
	double x;
	double y;
};

int readInputFile(string path, vector<vector<double>> & v){
	ifstream in(path);
	if(!in.good()){
		cout << "File not found" << endl;
		exit(1);
	}
	int instanceSize, no;
	double a, b;
	vector<point> p;
	in >> instanceSize;
	while(in >> no >> a >> b)
		p.push_back({a, b});
	v.resize(p.size(), vector<double>(p.size(), 0));

	for (int i = 0; i < p.size(); ++i)
		for (int j = i+1; j < p.size(); ++j)
			v[i][j] = v[j][i] = sqrt(pow(p[i].x - p[j].x, 2) + pow(p[i].y - p[j].y, 2));
	in.close();
	return instanceSize;
}

vector<int> readSolutionFile(string path){
	ifstream in(path);
	vector<int> v;
	int a;
	while (in >> a) v.push_back(a-1);
	in.close();
	return v;
}

double result(vector<vector<double>> & e, vector<int> & seq){
	double res = 0;
	for (int i = 0; i < seq.size()-1; ++i)
		res += e[seq[i]][seq[i+1]];
	res += e[seq[seq.size()-1]][seq[0]];
	return res;
}

vector<int> greedy(vector<vector<double>>& e, int start) {
	vector<bool> vis;
	vector<int> sequence;
	double mini = numeric_limits<double>::max();
	int a = start, b;
	vis.resize(e.size(), false);
	sequence.push_back(start);
	vis[start] = true;
	for (int i = 0; i < e.size()-1; ++i) {
		for (int j = 0; j < e.size(); ++j) {
			if (!vis[j] && e[a][j] < mini) {
				b = j;
				mini = e[a][j];
			}
		}
		sequence.push_back(b);
		vis[b] = true;
		a = b;
		mini = numeric_limits<double>::max();
	}
	return sequence;
}

void bruteforce(vector<vector<double>> & e, vector<int> & seq){
	double miniRes = result(e,seq) - e[seq[seq.size()-1]][seq[0]];
	vector<int> seq2 = seq;
	do {
		if(result(e,seq2) - e[seq2[seq2.size()-1]][seq2[0]] < miniRes){
			seq = seq2;
			miniRes = result(e,seq) - e[seq[seq.size()-1]][seq[0]];
		}
	} while (next_permutation(seq2.begin()+1, seq2.end()-1));
}
