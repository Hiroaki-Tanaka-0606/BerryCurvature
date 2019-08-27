#include <string.h>
#include <stdio.h>
#include <iostream>
#include <complex>
#include "variables.hpp"
#include "loadInput.hpp"
#include "initialize.hpp"
#include "utils.hpp"
#include "interpretEquation.hpp"
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
	int loadInput_status=loadInput(input,false);
	if(loadInput_status==1){
		cout << "Finished loading input" << endl;
	}else{
		cout << "Failed in loading input" << endl;
		return -1;
	}
	
	//close input file
	fclose(input);

	int i;
	//add kx, ky, kz, I to parameters
	char** allParams_name=new char*[N_params+4];
	complex<double>* allParams_value=new complex<double>[N_params+4];
	allParams_name[0]=(char*)"kx";
	allParams_name[1]=(char*)"ky";
	allParams_name[2]=(char*)"kz";
	allParams_name[3]=(char*)"I";
	allParams_value[0]=complex<double>(0,0);
	allParams_value[1]=complex<double>(0,0);
	allParams_value[2]=complex<double>(0,0);
	allParams_value[3]=complex<double>(0,0);
	for(i=0;i<N_params;i++){
		allParams_name[i+4]=params_name[i];
		allParams_value[i+4]=(complex<double>)params_value[i];
	}
	
	//interpretEquation test
	char* eq=(char*)"kx+I*(ky-2)*kz+((m/.1-b)*3.0)";
	complex<double> result;
	int interpret_status;
	interpret_status=interpretEquation(eq,allParams_name,allParams_value,&result);

	if(interpret_status==1){
		cout << "interpretEquation result: "<< result << endl;
	}
	return 0;
}
