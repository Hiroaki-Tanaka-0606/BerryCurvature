#include <string.h>
#include <stdio.h>
#include <iostream>
#include <complex>
#define _USE_MATH_DEFINES
#include <math.h>

#include "variables.hpp"
#include "loadInput.hpp"
#include "loadInput_HWR.hpp"
#include "initialize.hpp"
#include "utils.hpp"
#include "interpretEquation.hpp"
#include "lapack.hpp"
using namespace std;

int main(int argc, const char** argv){

	//initial message
	cout << "#Berry curvature calculation from a .HWR file of OpenMX\n";
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
	int loadInput_status=loadInput(input,false);
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

	//evaluation of basis vectors and delta_k
	complex<double> evaluated_value;
	int i,j,k,l,m,n;
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
	cout << "#Evaluation of delta_k" << endl;
	interpretEquation_status=interpretEquation(delta_k_string,allParams_name,allParams_value,&evaluated_value);
	if(interpretEquation_status!=1){
		cout << "Error in evaluating delta_k" << endl;
		return -1;
	}else{
	  delta_k=evaluated_value.real();
		sprintf(evaluationOutput_format,"#delta_k=%s\n",realNumber_format);
		printf(evaluationOutput_format,delta_k);
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
	int Dimension=0;
	for(i=0;i<3;i++){
		if(k_split[i]>0){
			Dimension++;
		}
	}
	
	//eigenvector of k+Delta k, k, k-Delta k
	complex<double>** psi_plus=alloc_zmatrix(N,N);
	complex<double>** psi=alloc_zmatrix(N,N);
	complex<double>** psi_minus=alloc_zmatrix(N,N);
	//k gradient of psi(k) [xyz][eigenvalue order][vector index]
	complex<double>** grad_psi[3];
	for(i=0;i<3;i++){
		grad_psi[i]=alloc_zmatrix(N,N);
	}
	

	printf("#kx ky kz\t");
	for(i=0;i<N;i++){
		printf("Omega_x[%d] Omega_y[%d] Omega_z[%d]\t",i,i,i);
	}
	printf("\n");

	char berryCurvatureOutput_format[format_length];
	sprintf(berryCurvatureOutput_format,"%s %s %s\t",realNumber_format,realNumber_format,realNumber_format);
	//z, z^\prime in Algorithm.md
	complex<double> psi_norm, psi_norm_prime;
	//psi_(plus|minus) value without phase difference
	complex<double> psi_value_plus,psi_value_minus;

	complex<double> k_default;
	double phase_plus,phase_minus;
	double omega_x,omega_y,omega_z;
	
	for(i=0;i<=k_split[0];i++){
		k1=k1List[i];
		for(j=0;j<=k_split[1];j++){
			k2=k2List[j];
			for(k=0;k<=k_split[2];k++){
				k3=k3List[k];
				compositeK(k1,k2,k3);

				printf(realNumber_format,allParams_value[0].real());
				printf("\t");
				printf(realNumber_format,allParams_value[1].real());
				printf("\t");
				printf(realNumber_format,allParams_value[2].real());
				printf("\t");
				
				for(l=0;l<3;l++){
					//calculate k[l] derivative of psi(k)
				  k_default=allParams_value[l];
					compositeMatrix_HWR();
					zheev();
					if(INFO!=0){
						cout << "zheev error" << endl;
						return -1;
					}
					copy_zmatrix(psi,matrix,N,N);
					//k+Delta k
					allParams_value[l]=k_default+(complex<double>)delta_k;
					compositeMatrix_HWR();
					zheev();
					if(INFO!=0){
						cout << "zheev error" << endl;
						return -1;
					}
					copy_zmatrix(psi_plus,matrix,N,N);
					//k-Delta k
					allParams_value[l]=k_default-(complex<double>)delta_k;
					compositeMatrix_HWR();
					zheev();
					if(INFO!=0){
						cout << "zheev error" << endl;
						return -1;
					}
					copy_zmatrix(psi_minus,matrix,N,N);

					//reset k value
					allParams_value[l]=k_default;

					for(m=0;m<N;m++){
						psi_norm=zNorm(psi[m],psi_plus[m],N);
						psi_norm_prime=zNorm(psi[m],psi_minus[m],N);
					  phase_plus=arg(psi_norm);
					  phase_minus=arg(psi_norm_prime);
						for(n=0;n<N;n++){
							psi_value_plus=exp(-(complex<double>(0,1))*phase_plus)*psi_plus[m][n];
							psi_value_minus=exp(-(complex<double>(0,1))*phase_minus)*psi_minus[m][n];
							grad_psi[l][m][n]=(psi_value_plus-psi_value_minus)/((complex<double>(2,0))*delta_k);
						}
					}
				}

				//print Berry curvature
				for(m=0;m<N;m++){
					//Berry curvature of m-th eigenvector
					omega_x=-2*(zNorm(grad_psi[1][m],grad_psi[2][m],N).imag());
					omega_y=-2*(zNorm(grad_psi[2][m],grad_psi[0][m],N).imag());
					omega_z=-2*(zNorm(grad_psi[0][m],grad_psi[1][m],N).imag());
					printf(berryCurvatureOutput_format,omega_x,omega_y,omega_z);
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
