#include <string.h>
#include <stdio.h>
#include <iostream>
#include "variables.h"
#include "loadInput.h"
#include "initialize.h"
using namespace std;

int main(int argc, const char** argv){

	//initial message
	cout << "Berry curvature calculation\n";
	initialize();
	
	//load input file
	char fileName[fileName_length];
	if(argc==1){
		//only command -> input file name is default
		strcpy(fileName, fileName_default);
	}else{
		//with parameters -> input file name is argv[1]
		strcpy(fileName, argv[1]);
	}
	cout << "Input file: " << fileName << endl;
	FILE* input=fopen(fileName,"r");

	//load input
	int loadInput_status=loadInput(input);
	if(loadInput_status==1){
		cout << "Finished loading input" << endl;
	}else{
		cout << "Failed in loading input" << endl;
		return -1;
	}
	
	//close input file
	fclose(input);
	
	return 0;
}
