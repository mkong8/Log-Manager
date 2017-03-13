#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <unordered_map>
#include <sstream>
#include <set>
#include <iterator>

#include "log.h"

using namespace std;

void read_in(istream& in,
	vector<string>& master,
	vector<Log>& log_data,
	vector<size_t>& code){

	long long timestamp = 0;
	size_t count = 0;
	string log_entry, category;

	while(getline(in, log_entry)){
		if(log_entry[0] != '#'){
			convert_timestamp(log_entry, timestamp);
			find_category(log_entry, category);
			Log log_in(timestamp, category, count);
			master.push_back(log_entry);
			log_data.push_back(log_in);

			++count;
			timestamp = 0;
		}
	}
	
	code.resize(count);

	log_less comp;
	sort(log_data.begin(), log_data.end(), comp);

	//initializing unordered maps for category and keyword
	for(size_t i = 0; i < code.size(); ++i){
		size_t index = log_data[i].entryID;
		code[index] = i;
	}

	cout << count << " entries read\n";
}

void init_cat(vector<Log>& log_data,
	unordered_map<string, vector<size_t>>& categories){

	for(auto it = log_data.begin(); it != log_data.end(); ++it){
		categories[it->category].push_back((size_t)(it - log_data.begin()));
	}
}

void init_key(vector<string>& master,
	vector<Log>& log_data,
	unordered_map<string, vector<size_t>>& keyword_map){

	vector<string> keywords;
	string message;
	
	for(auto it = log_data.begin(); it != log_data.end(); ++it){
		size_t masterID = it->entryID;

		find_message(master[masterID], message);
		parse_keywords(keywords, message);
		set<string> clean(keywords.begin(), keywords.end());
		
		for(auto it2 = clean.begin(); it2 != clean.end(); ++it2){
			keyword_map[*it2].push_back((size_t)(it - log_data.begin()));
		}
	}
}

void timestamp_search(string& timestamps,
	vector<Log>& log_data,
	vector<size_t>& buffer_list){

	long long timestamp1(0), timestamp2(0);
	timestamp_less comp;
	size_t entryID_buffer = 0;
	string category_buffer = "";
	size_t count = 0;

	convert_timestamp(timestamps, timestamp1);
	timestamps = timestamps.substr(15, 14);
	convert_timestamp(timestamps, timestamp2);

	Log t1(timestamp1, category_buffer, entryID_buffer);
	Log t2(timestamp2, category_buffer, entryID_buffer);

	auto lower = lower_bound(log_data.begin(), log_data.end(), t1, comp);
	auto upper = upper_bound(log_data.begin(), log_data.end(), t2, comp);

	for(auto it = lower; it != upper; ++it){
		buffer_list.push_back((size_t)(it - log_data.begin()));
		++count;
	}

	cout << "Timestamps search: " << count << " entries found\n";
}

void matching_search(string& timestamp_in,
	vector<Log>& log_data,
	vector<size_t>& buffer_list){

	long long timestamp = 0;
	timestamp_less comp;
	size_t entryID_buffer = 0;
	string category_buffer = "";
	size_t count = 0;

	convert_timestamp(timestamp_in, timestamp);

	Log t(timestamp, category_buffer, entryID_buffer);

	auto lower = lower_bound(log_data.begin(), log_data.end(), t, comp);
	auto upper = upper_bound(log_data.begin(), log_data.end(), t, comp);

	for(auto it = lower; it != upper; ++it){
		buffer_list.push_back(it->entryID);
		++count;
	}

	cout << "Timestamp search: " << count << " entries found\n";
}

void category_search(string& category, 
	vector<size_t>& buffer_list,
	unordered_map<string, vector<size_t>>& categories){

	transform(category.begin(), category.end(), category.begin(), ::tolower);

	if(categories.count(category) != 0){
		buffer_list = categories[category];
	}
	
	cout << "Category search: " << buffer_list.size() << " entries found\n";
}

void keyword_search(string& keyword, 
	vector<size_t>& buffer_list,
	unordered_map<string, vector<size_t>>& keyword_map){

	vector<string> input;
	vector<size_t> master_list, current, updated;
	
	parse_keywords(input, keyword);

	if(input.size() == 1){
		buffer_list = keyword_map[input[0]];
	}
	else if(keyword_map.count(input[0]) != 0){
		master_list = keyword_map[input[0]];

		for(size_t i = 1; i < input.size(); ++i){	
			if(keyword_map.count(input[i]) == 0){
				master_list.clear();
				break;
			}
			current = keyword_map[input[i]];
			set_intersection(master_list.begin(), master_list.end(),
				current.begin(), current.end(), back_inserter(updated));
			master_list = updated;
			updated.clear();
		}
		set<size_t> clean(master_list.begin(), master_list.end());
		buffer_list.insert(buffer_list.begin(), clean.begin(), clean.end());
	}
	
	cout << "Keyword search: " << buffer_list.size() << " entries found\n";
}

void append_log(size_t logID, vector<size_t>& excerpt_list, vector<size_t>& code){
	if((size_t)logID >= code.size()){
		return;
	}
	else{
		excerpt_list.push_back(code[logID]);

		cout << "log entry " << logID << " appended\n";	
	}
}

void append_buffer(vector<size_t>& buffer_list, 
	vector<size_t>& excerpt_list,
	bool searched){

	if(searched){
		excerpt_list.insert(excerpt_list.end(), buffer_list.begin(), buffer_list.end());

		cout << buffer_list.size() << " log entries appended\n";
	}	
}

void delete_log(size_t pos, vector<size_t>& excerpt_list){
	if((size_t)pos >= excerpt_list.size()){
		return;
	}
	else{
		excerpt_list.erase(excerpt_list.begin() + pos);

		cout << "Deleted excerpt list entry " << pos << "\n";
	}
}

void move_begin(size_t pos, vector<size_t>& excerpt_list){
	if((size_t)pos >= excerpt_list.size()){
		return;
	}
	else{
		if(excerpt_list.size() > 0){
			size_t value = excerpt_list[pos];
			excerpt_list.erase(excerpt_list.begin() + pos);
			excerpt_list.insert(excerpt_list.begin(), value);
		}
		
		cout << "Moved excerpt list entry " << pos << "\n";
	}
}

void move_end(size_t pos, vector<size_t>& excerpt_list){
	if((size_t)pos >= excerpt_list.size()){
		return;
	}
	else{
		if(excerpt_list.size() > 0){
			size_t value = excerpt_list[pos];
			excerpt_list.erase(excerpt_list.begin() + pos);
			excerpt_list.push_back(value);
		}
		
		cout << "Moved excerpt list entry " << pos << "\n";
	}
}

void sort_excerpt(vector<size_t>& excerpt_list){
	sort(excerpt_list.begin(), excerpt_list.end());

	cout << "excerpt list sorted\n";
}

void print_buffer(vector<size_t>& list, 
	vector<Log>& log_data, 
	vector<string>& master){

	if(list.empty()){
		return;
	}
	else{
		for(size_t i = 0; i < list.size(); ++i){
			size_t index = list[i];
			index = log_data[index].entryID;
			cout << index << '|' << master[index] << endl;
		}
	}
}

void print_excerpt(vector<size_t>& list, 
	vector<Log>& log_data,
	vector<string>& master){
	for(size_t i = 0; i < list.size(); ++i){
		size_t index = list[i];
		index = log_data[index].entryID;
		cout << i << '|' << index << '|' << master[index] << endl;
	}
}


#endif