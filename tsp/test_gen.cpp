#include <cstdlib>
#include <iostream>

using namespace std;

int main(){
	int n = 36;
	cout << n << endl;
	for(int i = 0; i < n; ++i){
		cout << rand() % 100 << " " << rand() % 100 << endl;
	}
}