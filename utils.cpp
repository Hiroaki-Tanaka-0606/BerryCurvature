#include "variables_ext.hpp"
#include <string.h>
#include <iostream>
using namespace std;

int ordinalSuffix_index(int n){
	if(n<0){
		n*=-1;
	}
	if(n==11 || n==12){
		return 3;
	}
	if(n%10==1){
		return 0;
	}
	if(n%10==2){
		return 1;
	}
	if(n%10==3){
		return 2;
	}
	return 3;
}
void setAuxParameterName(char** auxParameter_name, int auxParameter_next){
	int indices[auxParameter_length];
	int i;
	int index=auxParameter_next;
	int len=strlen(auxParameter_letters);
	for(i=0;i<auxParameter_length;i++){
		indices[i]=index%len;
		index/=len;
	}
	auxParameter_name[auxParameter_next][0]='_';
	for(i=0;i<auxParameter_length;i++){
		auxParameter_name[auxParameter_next][i+1]=auxParameter_letters[indices[auxParameter_length-i-1]];																											 
	}
	auxParameter_name[auxParameter_next][auxParameter_length+1]='\0';
}
