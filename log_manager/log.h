#ifndef LOG_H
#define LOG_H

#include <string>
#include <vector>
#include <cctype>
#include <algorithm>

using namespace std;

struct Log{
	Log(long long& timestamp_in, string& category_in, size_t entryID_in)
		: timestamp(timestamp_in), category(category_in), entryID(entryID_in) {}

	long long timestamp;
	string category;
	size_t entryID;
};

//comparator for strictly timestamps
struct timestamp_less{
	bool operator()(const Log& lhs, const Log& rhs){
		return lhs.timestamp < rhs.timestamp;
	}
};

//comparator for entire log entries
struct log_less{
	bool operator()(const Log& lhs, const Log& rhs){
		if(lhs.timestamp == rhs.timestamp){
			if(lhs.category.compare(rhs.category) == 0){
				return lhs.entryID < rhs.entryID;
			}
			return lhs.category.compare(rhs.category) < 0;
		}
		return lhs.timestamp < rhs.timestamp;
	}
};

//convert string timestamp to long long timestamp
void convert_timestamp(string& log_entry, long long& timestamp){
	size_t digit;
	long long multiplier = (long long)pow(10, 9);

	for(size_t i = 0; i < 14; ++i){
		if((i % 3) != 2){
			digit = log_entry[i] - '0';
			timestamp += digit*multiplier;
			multiplier /= 10;
		}
		if(log_entry[i] == '|'){
			cerr << "Invalid timestamp" << endl;
			exit(1);
		}
	}
}

//find word(s) from user input
void read_words(string& input){
	if(!input.empty()){
		input = input.substr(1, input.size()); //account for space after command
	}
}

//find category from log_entry
void find_category(string& log_entry, string& category){
	size_t end_index = 15;

	while(log_entry[end_index] != '|'){
		++end_index;
	}

	category = log_entry.substr(15, end_index - 15); //substr(index, str_length)
	transform(category.begin(), category.end(), category.begin(), ::tolower);
}

//find whole message from log_entry, used for finding keywords
void find_message(string& log_entry, string& message){
	message = log_entry.substr(15, log_entry.size() - 15);
}

//parse through message to find keywords
void parse_keywords(vector<string>& keywords, string master){
	size_t first_letter = 0;
	size_t size = master.size();
	keywords.clear();

	for(size_t i = 0; i < size; ++i){
		if(isalnum(master[i])){
			master[i] = (char)tolower(master[i]);
		}
		else if(i > first_letter){
			keywords.push_back(master.substr(first_letter, i - first_letter));
			first_letter = i + 1;
		}
		else{
			++first_letter;
		}
	}
	if(size - 1 >= first_letter){
		keywords.push_back(master.substr(first_letter, size - first_letter));
	}
}


#endif