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
	char* line=new char[buffer_length];
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

	//(2+N_params)th line: matrix size N
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

	//(2+N_params+1)th to (2+N_params+N)th line: matrix
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
	
	//(3+N_params+N)th to (5+N_params+N)th line: k range for eigenvalu calculation (start, stop, split)
	//in Berry curvature calculation, (6+N_params+N)th to (8+N_params+N)th line
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

	//(9+N_params+N)th line: delta k
	fgets_status=fgets(line,buffer_length,input);
	if(fgets_status==NULL){
		load_error(row);
		return -1;
	}
	sscanf_status=sscanf(line, "%lf", &delta_k);
	if(sscanf_status!=1){
		parse_error(row);
		return -1;
	}
	char deltakOutput_format[format_length];
	sprintf(deltakOutput_format,"#Delta k: %s\n",realNumber_format);
	printf(deltakOutput_format,delta_k);
	row++;
	
	return 1;
}
