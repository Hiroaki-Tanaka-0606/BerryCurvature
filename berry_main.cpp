#include <string.h>
#include <stdio.h>
#include <iostream>
#include <complex>
#define _USE_MATH_DEFINES
#include <math.h>

#include "variables.hpp"
#include "loadInput.hpp"
#include "initialize.hpp"
#include "utils.hpp"
#include "interpretEquation.hpp"
#include "lapack.hpp"
using namespace std;

int main(int argc, const char** argv){

	//initial message
	cout << "#Berry curvature calculation\n";
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
	int loadInput_status=loadInput(input,false);
	if(loadInput_status==1){
		cout << "Finished loading input" << endl;
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
	
	int i,j,k,l,m,n;
	double kx,ky,kz;

	//eigenvector of k+Delta k, k, k-Delta k
	complex<double>** psi_plus=alloc_zmatrix(N,N);
	complex<double>** psi=alloc_zmatrix(N,N);
	complex<double>** psi_minus=alloc_zmatrix(N,N);
	//k gradient of psi(k) [xyz][eigenvalue order][vector index]
	complex<double>*** grad_psi=new complex<double>**[3];
	for(i=0;i<3;i++){
		grad_psi[i]=alloc_zmatrix(N,N);
	}
	
	printf("#kx ky kz\t");
	for(i=0;i<N;i++){
		printf("Omega_x[%d] Omega_y[%d] Omega_z[%d]\t",i,i,i);
	}
	printf("\n");

	for(i=0;i<=k_split[0];i++){
		kx=kxList[i];
		allParams_value[0]=(complex<double>)kx;
		for(j=0;j<=k_split[1];j++){
			ky=kyList[j];
			allParams_value[1]=(complex<double>)ky;
			for(k=0;k<=k_split[2];k++){
				kz=kzList[k];
				allParams_value[2]=(complex<double>)kz;

				printf(realNumber_format,kx);
				printf(" ");
				printf(realNumber_format,ky);
				printf(" ");
				printf(realNumber_format,kz);
				printf("\t");
				
				for(l=0;l<3;l++){
					//calculate k[l] derivative of psi(k)
					complex<double> k_default=allParams_value[l];
					compositeMatrix();
					zheev();
					if(INFO!=0){
						cout << "zheev error" << endl;
						return -1;
					}
					copy_zmatrix(psi,matrix,N,N);
					//k+Delta k
					allParams_value[l]=k_default+(complex<double>)delta_k;
					compositeMatrix();
					zheev();
					if(INFO!=0){
						cout << "zheev error" << endl;
						return -1;
					}
					copy_zmatrix(psi_plus,matrix,N,N);
					//k-Delta k
					allParams_value[l]=k_default-(complex<double>)delta_k;
					compositeMatrix();
					zheev();
					if(INFO!=0){
						cout << "zheev error" << endl;
						return -1;
					}
					copy_zmatrix(psi_minus,matrix,N,N);

					//reset k value
					allParams_value[l]=k_default;

					//z, z^\prime in Algorithm.md
					complex<double> psi_norm, psi_norm_prime;
					//psi_(plus|minus) value without phase difference
					complex<double> psi_value_plus,psi_value_minus;
					for(m=0;m<N;m++){
						psi_norm=zNorm(psi[m],psi_plus[m],N);
						psi_norm_prime=zNorm(psi[m],psi_minus[m],N);
						double phase_plus=arg(psi_norm);
						double phase_minus=arg(psi_norm_prime);
						for(n=0;n<N;n++){
							psi_value_plus=exp(-(complex<double>(0,1))*phase_plus)*psi_plus[m][n];
							psi_value_minus=exp(-(complex<double>(0,1))*phase_minus)*psi_minus[m][n];
							grad_psi[l][m][n]=(psi_value_plus-psi_value_minus)/((complex<double>(2,0))*delta_k);
						}
					}
				}

				//print Berry curvature
				double omega_x,omega_y,omega_z;
				for(m=0;m<N;m++){
					//Berry curvature of m-th eigenvector
					omega_x=-2*(zNorm(grad_psi[1][m],grad_psi[2][m],N).imag());
					omega_y=-2*(zNorm(grad_psi[2][m],grad_psi[0][m],N).imag());
					omega_z=-2*(zNorm(grad_psi[0][m],grad_psi[1][m],N).imag());
					char berryCurvatureOutput_format[format_length];
					sprintf(berryCurvatureOutput_format,"%s %s %s\t",realNumber_format,realNumber_format,realNumber_format);
					printf(berryCurvatureOutput_format,omega_x,omega_y,omega_z);
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
