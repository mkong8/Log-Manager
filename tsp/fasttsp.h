#ifndef FASTTSP
#define FASTTSP

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

#include "genfunc.h"

using namespace std;

class FAST_map{
public:
    FAST_map(istream& filein);

    void optswap(size_t a, size_t b);
    void two_opt(size_t factor);
    void nearestNeighbor();//test
    void greedy_alg();

    double get_cost();

    void print_adjmat();
    void print_path();

    void distance_check();

private:
    struct Vertex{
        int x;
        int y;
        bool visited = false;
    };

    size_t size;
    vector<Vertex> coordinates;
    vector<size_t> path;
    double tot_distance = 0;
};

FAST_map::FAST_map(istream& filein){
    Vertex vertex;

    filein >> size;
    while(filein >> vertex.x >> vertex.y){
        coordinates.push_back(vertex);
    }
}

//reverse the order of path [a, b)
//essentially swapping them to uncross paths
void FAST_map::optswap(size_t a, size_t b){
    reverse(path.begin() + a, path.begin() + b);
}

void FAST_map::two_opt(size_t factor){
    size_t begin = size - (size/factor);
    for(size_t i = begin; i < size - 1; ++i){
        for(size_t j = i + 2; j < size; ++j){
            //original edges
            double edge1 = calc_distance(coordinates[path[i]].x, coordinates[path[i]].y,
                        coordinates[path[i + 1]].x, coordinates[path[i + 1]].y);
            double edge2 = calc_distance(coordinates[path[j]].x, coordinates[path[j]].y,
                        coordinates[path[j + 1]].x, coordinates[path[j + 1]].y);
            //swapped vertices
            double edge3 = calc_distance(coordinates[path[i]].x, coordinates[path[i]].y,
                        coordinates[path[j]].x, coordinates[path[j]].y);
            double edge4 = calc_distance(coordinates[path[i + 1]].x, coordinates[path[i + 1]].y,
                        coordinates[path[j + 1]].x, coordinates[path[j + 1]].y);
            //check for crossed edges
            if((edge1 + edge2) > (edge3 + edge4)){
                optswap(i + 1, j + 1);
                tot_distance -= (edge1 + edge2);
                tot_distance += edge3 + edge4;
            }
        }
    }
}

void FAST_map::greedy_alg(){
    double dist_to_now;
    size_t now, min = 0;

    //nearest neighbor greedy algorithm
    now = 0;
    path.push_back(now);
    for(size_t i = 1; i < size; ++i){
        coordinates[now].visited = true;
        double dist_min = numeric_limits<double>::infinity();

        for(size_t j = 1; j < size; ++j){
            if(!coordinates[j].visited){
                dist_to_now = calc_distance(coordinates[now].x, coordinates[now].y,
                    coordinates[j].x, coordinates[j].y);
                if(dist_to_now < dist_min){
                    dist_min = dist_to_now;
                    min = j;
                } //nearest neighbor
            }
        }

        tot_distance += dist_min;
        now = min;
        path.push_back(now); //add to path for 2-opt later
    }
    path.push_back(0);
    tot_distance += calc_distance(coordinates[0].x, coordinates[0].y,
                        coordinates[now].x, coordinates[now].y);
    //add last node to first node for final, cyclic edge
}

double FAST_map::get_cost(){
    return tot_distance;
}

void FAST_map::print_adjmat(){
    cout << size << endl;
    for(size_t i = 0; i < size; ++i){
        cout << coordinates[i].x << "," << coordinates[i].y << "\t";
        cout << coordinates[i].visited << endl;
    }
}

void FAST_map::print_path(){
    cout << tot_distance << "\n";
    for(size_t i = 0; i < size - 1; ++i){
        cout << path[i] << " ";
    }
    cout << path[size - 1] << endl; //spec requires no space after last node
    //cout << "Calculated tour: " << tot_distance << endl;//test
}

void FAST_map::distance_check(){
    double dist = 0;

    for(size_t i = 0; i < path.size() - 1; ++i){
        dist += calc_distance(coordinates[path[i]].x, coordinates[path[i]].y,
                    coordinates[path[i + 1]].x, coordinates[path[i + 1]].y);
    }
    cout << "Actual tour: " << dist << endl;
}

#endif //FASTTSP