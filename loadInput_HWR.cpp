#include <iostream>
#include <stdio.h>
#include <string.h>
#include "variables_ext.hpp"
#include "utils.hpp"
using namespace std;
void load_error_HWR(int row){	
	printf("Error in loading %d%s line\n",row,ordinalSuffix[ordinalSuffix_index(row)]);
}
void parse_error_HWR(int row){
	printf("Error in parsing %d%s line\n",row,ordinalSuffix[ordinalSuffix_index(row)]);
}


int loadInput_HWR(FILE* input){
	//load input from .HWR
	char line[buffer_length];
	char* fgets_status;
	int sscanf_status;
	int i,j;
	int row=1;

	//line 1: skip
	fgets_status=fgets(line, buffer_length, input);
	if(fgets_status==NULL){
		load_error_HWR(row);
		return -1;
	}
	row++;
	//line 2: N
	fgets_status=fgets(line, buffer_length, input);
	if(fgets_status==NULL){
		load_error_HWR(row);
		return -1;
	}
	sscanf_status=sscanf(line, "%*s %*s %*s %*s %d", &N);
	if(sscanf_status!=1){
		parse_error_HWR(row);
		return -1;
	}
	printf("#Renewed matrix size: %d\n", N);
	row++;
	//line 3: number of W-S supercell
	fgets_status=fgets(line, buffer_length, input);
	if(fgets_status==NULL){
		load_error_HWR(row);
		return -1;
	}
	sscanf_status=sscanf(line, "%*s %*s %*s %*s %d", &HWRSize);
	if(sscanf_status!=1){
		parse_error_HWR(row);
		return -1;
	}
	printf("#Number of Wigner-Seitz supercell: %d\n", HWRSize);
	row++;
	//line 4: skip
	fgets_status=fgets(line, buffer_length, input);
	if(fgets_status==NULL){
		load_error_HWR(row);
		return -1;
	}
	row++;
	//lines 5-7: lattice
	HWRLattice=alloc_dmatrix(3, 3);
	for(i=0; i<3; i++){
		fgets_status=fgets(line, buffer_length, input);
		if(fgets_status==NULL){
			load_error_HWR(row);
			return -1;
		}
		sscanf_status=sscanf(line, "%lf %lf %lf", &HWRLattice[i][0], &HWRLattice[i][1], &HWRLattice[i][2]);
		if(sscanf_status!=3){
			parse_error_HWR(row);
			return -1;
		}
		printf("#Lattice vector %d (Bohr): %10.5f %10.5f %10.5f\n", i+1, HWRLattice[i][0], HWRLattice[i][1], HWRLattice[i][2]);
		row++;
	}
	//lines 8, 9: skip
	for(i=0; i<2; i++){
		fgets_status=fgets(line, buffer_length, input);
		if(fgets_status==NULL){
			load_error_HWR(row);
			return -1;
		}
		row++;
	}
	//prepare HWRMatrix
	HWRMatrix=new complex<double>**[HWRSize];
	RCoordinate=alloc_imatrix(3, HWRSize);
	for(i=0; i<HWRSize; i++){
		HWRMatrix[i]=alloc_zmatrix(N, N);
		// first line: R
		fgets_status=fgets(line, buffer_length, input);
		if(fgets_status==NULL){
			load_error_HWR(row);
			return -1;
		}
		sscanf_status=sscanf(line, "%*s %*s %d %d %d", &RCoordinate[i][0], &RCoordinate[i][1], &RCoordinate[i][2]);
		if(sscanf_status!=3){
			parse_error_HWR(row);
			return -1;
		}
		printf("#Loading R = (%2d %2d %2d)\n", RCoordinate[i][0], RCoordinate[i][1], RCoordinate[i][2]);
		row++;
		// successive N*N lines: matrix elements
		for(j=0; j<N*N; j++){
			fgets_status=fgets(line, buffer_length, input);
			if(fgets_status==NULL){
				load_error_HWR(row);
				return -1;
			}
			int mat_m;
			int mat_n;
			double real_part;
			double imaginary_part;
			sscanf_status=sscanf(line, "%d %d %lf %lf", &mat_m, &mat_n, &real_part, &imaginary_part);
			if(sscanf_status!=4){
				parse_error_HWR(row);
				return -1;
			}
			HWRMatrix[i][mat_m-1][mat_n-1]=complex<double>(real_part, imaginary_part);
			row++;
		}
	}
	
	return 1;
}
