#ifndef MST
#define MST

#include <iostream>
#include <vector>
#include <limits>

#include "genfunc.h"

using namespace std;

class MST_map{
public:
	MST_map(istream& filein);

	void prim_alg();

	void print_adjmat();
	void print_path();

private:
	struct Vertex{
	    int x;
	    int y;
	    bool visited = false;
	    short cage; //safe = 2, fenced = 1, or wild = 0
	    double distance = numeric_limits<double>::infinity();
	    size_t prev;
	};

	size_t size;
	vector<Vertex> coordinates;
	double tot_distance = 0;
};

MST_map::MST_map(istream& filein){
	Vertex vertex;
	bool possible = false;
	bool wild_only = true;

	filein >> size;
	while(filein >> vertex.x >> vertex.y){
		//fence is only negative and zero coordinate, positives don't count
		if((vertex.x == 0 && vertex.y < 0) || (vertex.y == 0 && vertex.x <0)){
			vertex.cage = 1;
			possible = 1;
		}
		else if(vertex.x < 0 && vertex.y < 0){
			vertex.cage = 0;
		}
		else{
			vertex.cage = 2;
			wild_only = false; //only wild cages creates valid path
		}
		coordinates.push_back(vertex);
	}

	if(!possible && !wild_only){
		cerr << "Cannot construct MST" << endl;
		exit(1);
	}
}

void MST_map::prim_alg(){
	double dist_to_now;
	size_t now, min;

	now = 0;
	coordinates[now].distance = 0;
	for(size_t j = 1; j < size; ++j){
		coordinates[now].visited = true;
		double dist_min = numeric_limits<double>::infinity();

		for(size_t i = 1; i < size; ++i){
			if(!coordinates[i].visited){
				dist_to_now = calc_distance(coordinates[now].x, coordinates[now].y, 
					coordinates[i].x, coordinates[i].y);
				if(dist_to_now < coordinates[i].distance &&
					abs(coordinates[now].cage - coordinates[i].cage) != 2){
					coordinates[i].distance = dist_to_now;
					coordinates[i].prev = now;
				}
				else{
					dist_to_now = coordinates[i].distance;
				}
				if(dist_to_now < dist_min){
					dist_min = dist_to_now;
					min = i;
				} //track closest node
			}
		}
		tot_distance += dist_min; //update total distance of path
		now = min;
	}	
}

void MST_map::print_adjmat(){
	cout << size << endl;
	for(size_t i = 0; i < size; ++i){
		cout << coordinates[i].x << ", " << coordinates[i].y << "\t";
		cout << coordinates[i].visited << "\t" << coordinates[i].distance;
		cout << "\t" << coordinates[i].prev << "\t\t" << coordinates[i].cage << endl;
	}
}

void MST_map::print_path(){
	cout << tot_distance << "\n";
	for(size_t i = 1; i < size; ++i){
		size_t previous = coordinates[i].prev;
		if(i < previous){
			cout << i << " " << previous << "\n";
		}
		else{
			cout << previous << " " << i << "\n";
		}
	}
}

#endif //MST