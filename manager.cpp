#include <iostream>
#include <cmath>
#include <fstream>

#include "log.h"
#include "hash.h"

using namespace std;

int main(int argc, char* argv[]){
	ios_base::sync_with_stdio(false);

	vector<string> master;
	vector<Log> log_data;
	vector<size_t> buffer_list;
	vector<size_t> excerpt_list;
	vector<size_t> code;

	unordered_map<string, vector<size_t>> categories;
	unordered_map<string, vector<size_t>> keyword_map;

	char choice;
	string timestamps;
	bool searched = false;
	bool cat = true;
	bool key = true;

	if((string(argv[1]) == "-h") || (string(argv[1]) == "--help")){
		cout << "i helped." << endl;
		exit(0);
	}
	else{
		string file = argv[argc - 1];
		ifstream infile(file.c_str());

		read_in(infile, master, log_data, code);

		cout << "% ";
		while(cin >> choice){
			switch(choice){
				case 't':{
					buffer_list.clear();
					cin >> timestamps;
					timestamp_search(timestamps, log_data, buffer_list);
					searched = true;
					break;
				}
				case 'm':{
					buffer_list.clear();
					cin >> timestamps;
					matching_search(timestamps, log_data, buffer_list);
					searched = true;
					break;
				}
				case 'c': {
					if(cat){
						init_cat(log_data, categories);
						cat = false;
					}
					buffer_list.clear();
					string category;
					getline(cin, category);
					read_words(category);
					category_search(category, buffer_list, categories);
					searched = true;
					break;
				}
				case 'k': {
					if(key){
						init_key(master, log_data, keyword_map);
						key = false;
					}
					buffer_list.clear();
					string keyword;
					getline(cin, keyword);
					read_words(keyword);
					keyword_search(keyword, buffer_list, keyword_map);
					searched = true;
					break;
				}
				case 'a': {
					size_t logID;
					cin >> logID;
					append_log(logID, excerpt_list, code);
					break;
				}
				case 'r': {
					append_buffer(buffer_list, excerpt_list, searched);
					break;
				}
				case 'd': {
					size_t pos;
					cin >> pos;
					delete_log(pos, excerpt_list);
					break;
				}
				case 'b': {
					size_t pos;
					cin >> pos;
					move_begin(pos, excerpt_list);
					break;
				}
				case 'e': {
					size_t pos;
					cin >> pos;
					move_end(pos, excerpt_list);
					break;
				}
				case 's': {
					sort_excerpt(excerpt_list);
					break;
				}
				case 'l': {
					excerpt_list.clear();
					cout << "excerpt list cleared\n";
					break;
				}
				case 'g':{
					print_buffer(buffer_list, log_data, master);
					break;
				}
				case 'p':{
					print_excerpt(excerpt_list, log_data, master);
					break;
				}
				case 'q': exit(0);
				case '#': {
					string buffer;
					getline(cin, buffer);
					break;
				}
				default:{
					cerr << "Invalid command" << endl;
					break;
				}	
			}//end switch
			cout << "% ";
		}//end while
	}
	return 0;
}