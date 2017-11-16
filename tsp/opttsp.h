#ifndef OPTTSP
#define OPTTSP

#include <deque>

#include "mst.h"
#include "fasttsp.h"

class OPT_map{
public:
	OPT_map(istream& filein);

	double find_upper();
	double find_lower();
	bool promising(double& dist_now);
	void find_path();

	void print_path();
	void print_unused();

private:
	struct Vertex{
        int x;
        int y;
        bool visited = false;
        double distance = numeric_limits<double>::infinity();
    };

	size_t size;
	deque<size_t> unused;
	deque<size_t> path, best_path;
	vector<Vertex> coordinates;
	double best_cost;
	double current_best = 0;
	double upper = 0; //upper bound calculated from FASTTSP
};

OPT_map::OPT_map(istream& filein){
	Vertex vertex;

	filein >> size;
	while(filein >> vertex.x >> vertex.y){
		coordinates.push_back(vertex);
	}

	path.push_back(0); //path has to start at vertex 0
	//initialize unused in find_upper as fasttsp path minus the initial 0
	//finds good paths faster for better pruning
	best_cost = find_upper(); //set best_cost to upper bound of FASTTSP
}

//modified greedy algorithm for upper bound
double OPT_map::find_upper(){
	double dist_to_now;
    size_t now, min = 0;

    //nearest neighbor greedy algorithm
    now = 0;
    best_path.push_back(now);
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

        upper += dist_min;
        now = min;
        best_path.push_back(now);
    }
    best_path.push_back(0);
    upper += calc_distance(coordinates[0].x, coordinates[0].y,
                        coordinates[now].x, coordinates[now].y);

    //one pass through of two opt for more pruning
    for(size_t j = 0; j < 2; ++j){
	    for(size_t i = j; i < size - 1; ++i){
	        for(size_t j = i + 2; j < size; ++j){
	            //original edges
	            double edge1 = calc_distance(coordinates[best_path[i]].x, coordinates[best_path[i]].y,
	                        coordinates[best_path[i + 1]].x, coordinates[best_path[i + 1]].y);
	            double edge2 = calc_distance(coordinates[best_path[j]].x, coordinates[best_path[j]].y,
	                        coordinates[best_path[j + 1]].x, coordinates[best_path[j + 1]].y);
	            //swapped vertices
	            double edge3 = calc_distance(coordinates[best_path[i]].x, coordinates[best_path[i]].y,
	                        coordinates[best_path[j]].x, coordinates[best_path[j]].y);
	            double edge4 = calc_distance(coordinates[best_path[i + 1]].x, coordinates[best_path[i + 1]].y,
	                        coordinates[best_path[j + 1]].x, coordinates[best_path[j + 1]].y);
	            //check for crossed edges
	            if((edge1 + edge2) > (edge3 + edge4)){
	                reverse(best_path.begin() + i + 1, best_path.begin() + j + 1);
	                upper -= (edge1 + edge2);
	                upper += edge3 + edge4;
	            }
	        }
	    }
	}
    best_path.pop_back();
    //initialize unused as fasttsp path for faster - therefore better - pruning
    unused = best_path;
    unused.pop_front();

    return upper;
}

//modified MST for simplicity. Returns smallest estimate to remainder of path
double OPT_map::find_lower(){
	double dist_to_now;
	double lower = 0;
	size_t now, min;
	vector<Vertex> estimate;
 	
	for(size_t i = 0; i < unused.size(); ++i){
		coordinates[unused[i]].visited = false;
		estimate.push_back(coordinates[unused[i]]);
	}

	now = 0;
	coordinates[now].distance = 0;
	for(size_t j = 1; j < unused.size(); ++j){
		estimate[now].visited = true;
		double dist_min = numeric_limits<double>::infinity();

		for(size_t i = 1; i < unused.size(); ++i){
			if(!estimate[i].visited){
				dist_to_now = calc_distance(estimate[now].x, estimate[now].y, 
					estimate[i].x, estimate[i].y);
				if(dist_to_now < estimate[i].distance){
					estimate[i].distance = dist_to_now;
				}
				else{
					dist_to_now = estimate[i].distance;
				}
				if(dist_to_now < dist_min){
					dist_min = dist_to_now;
					min = i;
				} //track closest node
			}
		}
		lower += dist_min; //update lower bound
		now = min;
	}
	return lower;
}

//check if path seems promising -- is it possible according to lower bound?
bool OPT_map::promising(double& current_best){
	if(size < 6){
		return true;
	}

	double min_to_start = numeric_limits<double>::infinity();
	double min_to_end = numeric_limits<double>::infinity();
	double lower = current_best;

	lower += find_lower();

	for(size_t i = 0; i < unused.size(); ++i){
		double now_to_start = calc_distance(coordinates[unused[i]].x, coordinates[unused[i]].y,
								coordinates[0].x, coordinates[0].y);
		double now_to_end = calc_distance(coordinates[unused[i]].x, coordinates[unused[i]].y,
					coordinates[path[path.size() - 1]].x, coordinates[path[path.size() - 1]].y);
		if(now_to_start < min_to_start){
			min_to_start = now_to_start;
		}
		if(now_to_end < min_to_end){
			min_to_end = now_to_end;
		}
	}
	
	lower = lower + min_to_end + min_to_start;

	if(lower < best_cost){
		return true;
	}
	return false;
}

void OPT_map::find_path(){
	if(unused.empty()){
		double comp_best = 0;
		comp_best = current_best + calc_distance(coordinates[0].x, coordinates[0].y,
			coordinates[path[size - 1]].x, coordinates[path[size - 1]].y);
		if(comp_best < best_cost){
			best_cost = comp_best;
			best_path = path;
		}
		//print_path();
		return;
	}

	if(!promising(current_best)){
		return;
	}

	for(size_t i = 0; i != unused.size(); ++i){
		double dist_now = calc_distance(coordinates[path.back()].x, coordinates[path.back()].y,
			coordinates[unused.front()].x, coordinates[unused.front()].y);
		current_best += dist_now;
		path.push_back(unused.front());
		unused.pop_front();
		find_path();
		current_best -= dist_now;
		unused.push_back(path.back());
		path.pop_back();
	}
}

void OPT_map::print_path(){
    cout << best_cost << "\n";
    for(size_t i = 0; i < best_path.size() - 1; ++i){
        cout << best_path[i] << " ";
    }
    //cout << path.size() << endl;
    cout << best_path[best_path.size() - 1] << endl; //spec requires no space after last node
    //cout << "Calculated tour: " << tot_distance << endl;//test
}

void OPT_map::print_unused(){
    for(size_t i = 0; i < unused.size() - 1; ++i){
        cout << unused[i] << " ";
    }
    cout << unused[unused.size() - 1] << endl; //spec requires no space after last node
    //cout << "Calculated tour: " << tot_distance << endl;//test
}

#endif //OPTTSP