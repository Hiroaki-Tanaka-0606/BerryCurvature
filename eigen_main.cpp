#include <string.h>
#include <stdio.h>
#include <iostream>
#include <complex>
#include "variables.hpp"
#include "loadInput.hpp"
#include "initialize.hpp"
#include "utils.hpp"
#include "lapack.hpp"
using namespace std;

int main(int argc, const char** argv){

	//initial message
	cout << "#Eigenvalue calculation\n";
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
	cout << "#Input file: " << fileName << endl;
	FILE* input=fopen(fileName,"r");

	//load input
	int loadInput_status=loadInput(input,true);
	if(loadInput_status==1){
		cout << "#Finished loading input" << endl;
	}else{
		cout << "Failed in loading input" << endl;
		return -1;
	}
	
	//close input file
	fclose(input);

	//add kx, ky, kz, I to parameters
	addKI();

	//make k coordinates list
	double* kxList;
	double* kyList;
	double* kzList;
	makeKList(&kxList,0);
	makeKList(&kyList,1);
	makeKList(&kzList,2);

	//eigenvalue calculation
	//initialization
  zheevInit();
	if(INFO!=0){
		cout << "zheevInit error" << endl;
		return -1;
	}
	
	int i,j,k,l;
	double kx,ky,kz;
	printf("#kx\tky\tkz\teigenvalues\n");
	for(i=0;i<=k_split[0];i++){
		kx=kxList[i];
		allParams_value[0]=(complex<double>)kx;
		for(j=0;j<=k_split[1];j++){
			ky=kyList[j];
			allParams_value[1]=(complex<double>)ky;
			for(k=0;k<=k_split[2];k++){
				kz=kzList[k];
				allParams_value[2]=(complex<double>)kz;

				compositeMatrix();
				zheev();
				if(INFO!=0){
					cout << "zheev error" << endl;
					return -1;
				}
				printf(realNumber_format,kx);
				printf("\t");
				printf(realNumber_format,ky);
				printf("\t");
				printf(realNumber_format,kz);
				printf("\t");
				for(l=0;l<N;l++){
					printf(realNumber_format,W[l]);
					printf("\t");
				}
				printf("\n");
			}
			if(k_split[2]!=0){
				printf("\n");
			}
		}
		if(k_split[2]==0){
			printf("\n");
		}
	}
		

	return 0;
}
