#include <iostream>
#include <stdio.h>
#include <string.h>
#include "variables_ext.hpp"
#include "utils.hpp"
using namespace std;
void load_error(int row){	
	printf("Error in loading %d%s line\n",row,ordinalSuffix[ordinalSuffix_index(row)]);
}
void parse_error(int row){
	printf("Error in parsing %d%s line\n",row,ordinalSuffix[ordinalSuffix_index(row)]);
}

int loadInput(FILE* input, bool isEigen){
	//load input
	char line[buffer_length];
	char* fgets_status;
	int sscanf_status;
	int row=1;
	int i,j;

	//first line: number of paremeters N_params
	fgets_status=fgets(line,buffer_length,input);
	if(fgets_status==NULL){
		load_error(row);
		return -1;
	}
	sscanf_status=sscanf(line, "%d", &N_params);
	if(sscanf_status!=1){
		parse_error(row);
		return -1;
	}
	printf("#Number of parameters: %d\n",N_params);

	//second to (1+N_params)th line: parameters name and value
	params_name=new char*[N_params];
	params_value=new double[N_params];
	for(i=0;i<N_params;i++){
		fgets_status=fgets(line,buffer_length,input);
		if(fgets_status==NULL){
			load_error(row);
			return -1;
		}
		params_name[i]=new char[parameter_length];
		sscanf_status=sscanf(line,"%s%lf",params_name[i],&params_value[i]);
		if(sscanf_status!=2){
			parse_error(row);
			return -1;
		}
		char parameterOutput_format[format_length];
		sprintf(parameterOutput_format,"#%%d%%s parameter: %%s = %s\n",realNumber_format);
		printf(parameterOutput_format,i+1,ordinalSuffix[ordinalSuffix_index(i+1)],params_name[i],params_value[i]);
		row++;
	}

	//(2+N_params)th line: number of equations N_eqns
	fgets_status=fgets(line,buffer_length,input);
	if(fgets_status==NULL){
		load_error(row);
		return -1;
	}
	sscanf_status=sscanf(line, "%d", &N_eqns);
	if(sscanf_status!=1){
		parse_error(row);
		return -1;
	}
	row++;
	printf("#Number of equations: %d\n",N_eqns);

	//(2+N_params+1)th to (2+N_params+N_eqns)th line: equations
	eqns_name=new char*[N_eqns];
	eqns_value=new char*[N_eqns];
	for(i=0;i<N_eqns;i++){
		fgets_status=fgets(line,buffer_length,input);
		if(fgets_status==NULL){
			load_error(row);
			return -1;
		}
	  eqns_name[i]=new char[parameter_length];
		eqns_value[i]=new char[equation_length];
		sscanf_status=sscanf(line,"%s%s",eqns_name[i],eqns_value[i]);
		if(sscanf_status!=2){
			parse_error(row);
			return -1;
		}
		printf("#%d%s equation: %s = %s\n",i+1,ordinalSuffix[ordinalSuffix_index(i+1)],eqns_name[i],eqns_value[i]);
		row++;
	}
	
	
	//(3+N_params+N_eqns)th line: matrix size N
	fgets_status=fgets(line,buffer_length,input);
	if(fgets_status==NULL){
		load_error(row);
		return -1;
	}
	sscanf_status=sscanf(line, "%d", &N);
	if(sscanf_status!=1){
		parse_error(row);
		return -1;
	}
	row++;
	printf("#Matrix size: %d\n",N);

	//(3+N_params+N_eqns+1)th to (3+N_params+N_eqns+N)th line: matrix
	matrix_string=new char**[N];
	for(i=0;i<N;i++){
		matrix_string[i]=new char*[N];
		for(j=0;j<N;j++){
			matrix_string[i][j]=new char[matrixElement_length];
			sscanf_status=fscanf(input,"%s",matrix_string[i][j]);
			if(sscanf_status!=1){
				parse_error(row);
				return -1;
			}
			printf("#Matrix[%d][%d] = %s\n",i,j,matrix_string[i][j]);
		}
		//pass to the end of the line
		fgets_status=fgets(line,buffer_length,input);
		if(fgets_status==NULL){
			load_error(row);
			return -1;
		}
		row++;
	}

	//(4+N_params+N_eqns+N)th to (6+N_params+N_eqns+N)th line: k basis vectors (equation-form)
	k_bases_string=new char**[3];
	k_bases=new double*[3];
	for(i=0;i<3;i++){
		k_bases_string[i]=new char*[3];
		for(j=0;j<3;j++){
			k_bases_string[i][j]=new char[equation_length];
		}
		k_bases[i]=new double[3];
	}
		
	for(i=0;i<3;i++){
		fgets_status=fgets(line,buffer_length,input);
		if(fgets_status==NULL){
			load_error(row);
			return -1;
		}
		sscanf_status=sscanf(line, "%s%s%s", k_bases_string[i][0], k_bases_string[i][1], k_bases_string[i][2]);
		if(sscanf_status!=3){
			parse_error(row);
			return -1;
		}
		for(j=0;j<3;j++){
			printf("#k_bases[%d][%d] = %s\n", i,j,k_bases_string[i][j]);
		}
		row++;
	}

	//if isEigen==false (calculation of Berry curvature), skip 3 rows (k range of eigenvalue calculation)
	if(isEigen==false){
		for(i=0;i<3;i++){
			fgets_status=fgets(line,buffer_length,input);
			if(fgets_status==NULL){
				load_error(row);
				return -1;
			}
			row++;
		}
	}
	
	//(7+N_params+N_eqns+N)th to (9+N_params+N_eqns+N)th line: k range for eigenvalu calculation (start, stop, split)
	//in Berry curvature calculation, (10+N_params+N_eqns+N)th to (12+N_params+N_eqns+N)th line
	//number of k points = split+1
	//n-th k point = start+(stop-start)*(n-1)/split = (start*(split+1-n)+stop*(n-1))/split
	//n=1,2,...,split+1
	k_start=new double[3];
	k_stop=new double[3];
	k_split=new int[3];
	for(i=0;i<3;i++){
		fgets_status=fgets(line,buffer_length,input);
		if(fgets_status==NULL){
			load_error(row);
			return -1;
		}
		sscanf_status=sscanf(line,"%lf%lf%d",&k_start[i],&k_stop[i],&k_split[i]);
		if(sscanf_status!=3){
			parse_error(row);
			return -1;
		}
		//k_split[i] should be positive or zero
		if(k_split[i]<0){
			printf("Error: k[%d] must be positive or zero\n",i);
			return -1;
		}
		char krangeOutput_format[format_length];
		sprintf(krangeOutput_format,"#k[%%d] axis: %s to %s, %%d points\n",realNumber_format,realNumber_format);
		printf(krangeOutput_format,i,k_start[i],k_stop[i],k_split[i]+1);
		row++;
	}

	//if isEigen==true (calculation of eigenvalue), end of the input (delta_k is not necessary in eigenvalue calculation)
	if(isEigen==true){
		return 1;
	}

	//(13+N_params+N_eqns+N)th line: delta k (equation-form)
	delta_k_string=new char[equation_length];
	fgets_status=fgets(line,buffer_length,input);
	if(fgets_status==NULL){
		load_error(row);
		return -1;
	}
	sscanf_status=sscanf(line, "%s", delta_k_string);
	if(sscanf_status!=1){
		parse_error(row);
		return -1;
	}
	printf("#Delta k: %s\n", delta_k_string);
	row++;
	
	return 1;
}
