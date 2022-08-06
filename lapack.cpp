#include <complex>
#include <math.h>
#include "variables_ext.hpp"
#include "utils.hpp"
using namespace std;

extern "C"{
	void zheev_(
							char* JOBZ, //'V' calculate eigenvalues and eigenvectors, 'N' only eigenvalues
							char* UPLO, //'U' upper triangular part is stored, 'L' lower triangular part is stored
							int* N, //order of the matrix A
							complex<double>* A, //N*N matrix, on exit eigenvectors are stored
							int* LDA, //the first dimension of A
							double* W, //N-dimensonal vector, on exit eigenvalues in ascending order
							complex<double>* WORK, //LWORK-dimensional workspace 
							int* LWORK, //dimension of WORK, when -1 calculate only optimal WORK size (stored in Real(WORK[0]))
							double* RWORK, //(3*N-2)-dimensional workspace
							int* INFO //on exit 0 when succeeded
							);
}
void zheevInit(){
	JOBZ='N';
	UPLO='U';
	matrix=alloc_zmatrix(N,N);
	W=alloc_dvector(N);
	WORK=alloc_zvector(1); //provisional
	LWORK=-1; //for WORK size calculation
	RWORK=alloc_dvector(3*N-2);

	zheev_(&JOBZ,&UPLO,&N,&matrix[0][0],&N,&W[0],&WORK[0],&LWORK,&RWORK[0],&INFO);
	if(INFO==0){
		LWORK=floor(WORK[0].real());
		WORK=alloc_zvector(LWORK);
	}
}
void zheev(){
	JOBZ='V';
	UPLO='U';
	zheev_(&JOBZ,&UPLO,&N,&matrix[0][0],&N,&W[0],&WORK[0],&LWORK,&RWORK[0],&INFO);
}
