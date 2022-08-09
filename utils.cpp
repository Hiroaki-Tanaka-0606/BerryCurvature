#define _USE_MATH_DEFINES
#include "variables_ext.hpp"
#include <string.h>
#include <iostream>
#include <complex>
#include "interpretEquation.hpp"
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
void setAuxParameterName(char** auxParameter_name, int auxParameter_count){
	if(auxParameter_name[auxParameter_count]!=NULL){
		return;
	}
	auxParameter_name[auxParameter_count]=new char[auxParameter_length+2];
	int indices[auxParameter_length];
	int i;
	int index=auxParameter_count;
	int len=strlen(auxParameter_letters);
	for(i=0;i<auxParameter_length;i++){
		indices[i]=index%len;
		index/=len;
	}
	auxParameter_name[auxParameter_count][0]='_';
	for(i=0;i<auxParameter_length;i++){
		auxParameter_name[auxParameter_count][i+1]=auxParameter_letters[indices[auxParameter_length-i-1]];																											 
	}
	auxParameter_name[auxParameter_count][auxParameter_length+1]='\0';
}

void addKI(){
	allParams_name=new char*[N_params+3+mathConstants_count];
	allParams_value=new complex<double>[N_params+3+mathConstants_count];
	allParams_name[0]=(char*)"kx";
	allParams_name[1]=(char*)"ky";
	allParams_name[2]=(char*)"kz";
	allParams_name[3]=(char*)"I";
	allParams_name[4]=(char*)"PI";
	allParams_value[0]=complex<double>(0,0);
	allParams_value[1]=complex<double>(0,0);
	allParams_value[2]=complex<double>(0,0);
	allParams_value[3]=complex<double>(0,1);
	allParams_value[4]=complex<double>(M_PI,0);
	int i;
	for(i=0;i<N_params;i++){
		allParams_name[i+3+mathConstants_count]=params_name[i];
		allParams_value[i+3+mathConstants_count]=(complex<double>)params_value[i];
	}
}

void makeKList(double** kArray, int index){
	double start=k_start[index];
	double stop=k_stop[index];
	int split=k_split[index];
	double* kList=new double[split+1];
	int i;
	if(split==0){
		//only one point
		kList[0]=start;
	}else{
		double delta=(stop-start)/split;
		for(i=1;i<split;i++){
			kList[i]=start+delta*i;
		}
		kList[0]=start;
		kList[split]=stop;
	}
	for(i=0;i<=split;i++){
		char kListOutput_format[format_length];
		sprintf(kListOutput_format,"#k[%%d][%%d] = %s\n",realNumber_format);
		printf(kListOutput_format,index,i,kList[i]);
	}
	*kArray=kList;
}

complex<double>** alloc_zmatrix(int n, int m){
  int i;
	complex<double>** mat;
  mat = (complex<double>**)malloc((size_t)(m * sizeof(complex<double>*)));
  if (mat == NULL) {
    cout << "Error: allocation failed in alloc_zmatrix" << endl;
		exit(1);
  }
  mat[0] = (complex<double>*)malloc((size_t)(m * n * sizeof(complex<double>)));
  if (mat[0] == NULL) {
    cout << "Error: allocation failed in alloc_zmatrix" << endl;
    exit(1);
  }
  for (i=1;i<m;i++){
		mat[i] = mat[i-1] + n;
	}
  return mat;
}
double** alloc_dmatrix(int n, int m){
  int i;
  double** mat;
  mat = (double**)malloc((size_t)(m * sizeof(double*)));
  if (mat == NULL) {
    cout << "Error: allocation failed in alloc_dmatrix" << endl;
		exit(1);
  }
  mat[0] = (double*)malloc((size_t)(m * n * sizeof(double)));
  if (mat[0] == NULL) {
    cout << "Error: allocation failed in alloc_dmatrix" << endl;
    exit(1);
  }
  for (i=1;i<m;i++){
		mat[i] = mat[i-1] + n;
	}
  return mat;
}
int** alloc_imatrix(int n, int m){
  int i;
  int** mat;
  mat = (int**)malloc((size_t)(m * sizeof(int*)));
  if (mat == NULL) {
    cout << "Error: allocation failed in alloc_imatrix" << endl;
		exit(1);
  }
  mat[0] = (int*)malloc((size_t)(m * n * sizeof(int)));
  if (mat[0] == NULL) {
    cout << "Error: allocation failed in alloc_imatrix" << endl;
    exit(1);
  }
  for (i=1;i<m;i++){
		mat[i] = mat[i-1] + n;
	}
  return mat;
}

double* alloc_dvector(int n){
  double *vec;
  vec = (double*)malloc((size_t)(n * sizeof(double)));
  if (vec == NULL) {
    cout << "Error: allocation failed in alloc_dvector" << endl;
    exit(1);
  }
  return vec;
}

complex<double>* alloc_zvector(int n){
  complex<double> *vec;
  vec = (complex<double>*)malloc((size_t)(n * sizeof(complex<double>)));
  if (vec == NULL) {
    cout << "Error: allocation failed in alloc_zvector" << endl;
    exit(1);
  }
  return vec;
}

void compositeMatrix(){
	int i,j;
	int interpretEquation_status;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			//note: matrix[j][i] is ok because of the difference of two-dimensional array in fortran and C
			auxParameter_count=0;
			interpretEquation_status=interpretEquation(matrix_string[i][j],allParams_name,allParams_value,&matrix[j][i]);
			//cout << matrix[j][i] << endl;
			if(interpretEquation_status!=1){
				cout << "Error in compositeMatrix" << endl;
				exit(1);
			}
		}
	}
}

void compositeMatrix_HWR(){
	int i,j,r,s1,s2;
	complex<double> mat_element;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			matrix[j][i]=complex<double>(0,0);
		}
	}
	double R[3];
	double KR;
	for(r=0; r<HWRSize; r++){
		// (kx, ky, kz)=allParams_value[0, 1, 2]
		// R[i]=HWRLattice[j][i]*RCoordinate[r]
		for(s1=0; s1<3; s1++){
			R[s1]=0;
			for(s2=0; s2<3; s2++){
				R[s1]+=HWRLattice[s2][s1]*RCoordinate[r][s2];
			}
		}
		KR=0.0;
		for(s1=0; s1<3; s1++){
			KR+=R[s1]*allParams_value[s1].real();
		}
		complex<double> cisKR(cos(KR), sin(KR));
		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				//note: matrix[j][i] is ok because of the difference of two-dimensional array in fortran and C
				matrix[j][i]+=cisKR*HWRMatrix[r][i][j];
			}
		}
	}
}

void compositeK(double k1, double k2, double k3){
	int i;
	for(i=0;i<3;i++){
		allParams_value[i]=k1*k_bases[0][i]+k2*k_bases[1][i]+k3*k_bases[2][i];
	}			
}


void copy_zmatrix(complex<double>** dst, complex<double>** src, int N, int M){
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<M;j++){
			dst[i][j]=src[i][j];
		}
	}
}

complex<double> zNorm(complex<double>* a, complex<double>* b, int N){
	//return a^* \cdot b
	complex<double> norm=(complex<double>(0,0));
	int i;
	for(i=0;i<N;i++){
		norm+=conj(a[i])*b[i];
	}
	return norm;
}
