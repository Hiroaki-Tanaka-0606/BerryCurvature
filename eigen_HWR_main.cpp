#include <string.h>
#include <stdio.h>
#include <iostream>
#include <complex>
#include "variables.hpp"
#include "loadInput.hpp"
#include "loadInput_HWR.hpp"
#include "initialize.hpp"
#include "utils.hpp"
#include "lapack.hpp"
#include "interpretEquation.hpp"
using namespace std;

int main(int argc, const char** argv){

	//initial message
	cout << "#Eigenvalue calculation from a .HWR file of OpenMX\n";
	initialize();
	
	//load input file
	char fileName[fileName_length];
	char HWRName[fileName_length];
	if(argc<3){
		printf("Usage: %s input HWR\n", argv[0]);
		return -1;
	}
	//with parameters -> input file name is argv[1]
	strcpy(fileName, argv[1]);

	
	cout << "#Input file: " << fileName << endl;
	FILE* input=fopen(fileName,"r");
	if(input==NULL){
		cout << "Error in opening the input file" << endl;
		return -1;
	}

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

	// HWR file
	strcpy(HWRName, argv[2]);
	cout << "#HWR file: " << HWRName << endl;
	FILE* HWR=fopen(HWRName, "r");
	if(HWR==NULL){
		cout << "Error in opening the HWR file" << endl;
		return -1;
	}

	loadInput_status=loadInput_HWR(HWR);
	if(loadInput_status==1){
		cout << "#Finished loading HWR" << endl;
	}else{
		cout << "Failed in loading HWR" << endl;
		return -1;
	}

	//add kx, ky, kz, I to parameters
	addKI();

	//evaluation of basis vectors
	//that of delta_k is only in berry_main.cpp
	complex<double> evaluated_value;
	int i,j,k,l;
	int interpretEquation_status;
	char evaluationOutput_format[format_length];
	cout << "#Evaluation of k basis vectors" << endl;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			interpretEquation_status=interpretEquation(k_bases_string[i][j],allParams_name,allParams_value,&evaluated_value);
			if(interpretEquation_status!=1){
				printf("Error in evaluating k_bases[%d][%d]",i,j);
				return -1;
			}else{
				k_bases[i][j]=evaluated_value.real();
				sprintf(evaluationOutput_format,"#k_bases[%%d][%%d]=%s\n",realNumber_format);
				printf(evaluationOutput_format,i,j,k_bases[i][j]);
			}
		}
	}
	cout << "#Evaluation succeeded" << endl;

	
	//make k coordinates list
	double* k1List;
	double* k2List;
	double* k3List;
	makeKList(&k1List,0);
	makeKList(&k2List,1);
	makeKList(&k3List,2);

	//eigenvalue calculation
	//initialization
  zheevInit();
	if(INFO!=0){
		cout << "zheevInit error" << endl;
		return -1;
	}
	
	double k1,k2,k3;
	printf("#kx\tky\tkz\teigenvalues\n");
	int Dimension=0;
	for(i=0;i<3;i++){
		if(k_split[i]>0){
			Dimension++;
		}
	}

	
	for(i=0;i<=k_split[0];i++){
		k1=k1List[i];
		for(j=0;j<=k_split[1];j++){
			k2=k2List[j];
			for(k=0;k<=k_split[2];k++){
				k3=k3List[k];
				compositeK(k1,k2,k3);

				compositeMatrix_HWR();
				zheev();
				if(INFO!=0){
					cout << "zheev error" << endl;
					return -1;
				}
				printf(realNumber_format,allParams_value[0].real());
				printf("\t");
				printf(realNumber_format,allParams_value[1].real());
				printf("\t");
				printf(realNumber_format,allParams_value[2].real());
				printf("\t");
				for(l=0;l<N;l++){
					printf(realNumber_format,W[l]);
					printf("\t");
				}
				printf("\n");
			}
			if(k_split[2]!=0 && Dimension>1){
				printf("\n");
			}
		}
		if(k_split[2]==0 && Dimension>1){
			printf("\n");
		}
	}
		

	return 0;
}
