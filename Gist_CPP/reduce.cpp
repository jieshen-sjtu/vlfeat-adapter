#include<stdio.h>
#include<string>
#include<iostream>
#include<map>
using namespace std;

int main(){
	string key;
	string value;
	
	while(cin>>key){
		cin>>value;

		cout<<key<<"\t"<<value<<endl;
	}

	return 0;
}
