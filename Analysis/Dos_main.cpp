//Dos.cpp: generate histrogram of eigenvalues (=density of states)
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
using namespace std;

int loadInput(FILE* input, char* eigenName, int* numBands, double* width);

int main (int argc, const char** argv){
	//initial message
	cout << "#Dos calculation" << endl;

	//variables and constants
	int fileName_length=256; // max length of filename
	const char* fileName_default="input_Dos.dat"; //default filename of configuration
	char fileName[fileName_length]; //filename of configuration
	int i, j;
	//values of the following variables are specified by configuration file
	char eigenName[fileName_length]; //filename of eigenvalues
	int numBands; //number of bands to be read (unnecessarily the same as number of all bands)
	double binWidth; //width of a bin

	//open input file
	if(argc==1){
		//only command -> input file name is default
		strcpy(fileName, fileName_default);
	}else{
		//with parameters -> input file name is argv[1]
		strcpy(fileName, argv[1]);
	}
	cout << "#Input file: " << fileName << endl;
	FILE* input=fopen(fileName, "r");
	if(input==NULL){
		cout << "Error in opening the input file" << endl;
		return -1;
	}

	//load input file
	int loadInput_status=loadInput(input, &eigenName[0], &numBands, &binWidth);
	if(loadInput_status==1){
		cout << "#Finished loading input" << endl;
	}else{
		cout << "Failed in loading input" << endl;
		return -1;
	}
	//close input file
	fclose(input);
	
	//count the number of points
	int numPoints=0;
	FILE* eigens=fopen(eigenName, "r");
	if(eigens==NULL){
		cout << "Error in opening the eigenvalues file" << endl;
		return -1;
	}

	int buffer_length=1024;
	char line[buffer_length];
	int fscanf_status;
	double loaded_value;
	bool validRow;
	while(true){
		//check validity
		validRow=true;
		for(i=0; i<numBands+3;i++){
			fscanf_status=fscanf(eigens, "%lf", &loaded_value);
			if(fscanf_status!=1){
				//this row is invalid
				validRow=false;
				break;
			}
		}
		if(validRow){
			numPoints++;
		}
		//pass to the end of the line
		if(fgets(line, buffer_length, eigens)==NULL){
			//if this read become failed, it means that the cursor reached to the EOF
			break;
		}	
	}
	cout << "#Number of points: " << numPoints << endl;

	//go back to the top
	rewind(eigens);

	//load values
	double** eigenvalues=new double*[numPoints];
	int index=0;
	while(true){
		//load values
		eigenvalues[index]=new double[numBands];
		validRow=true;
		for(i=0; i<numBands+3;i++){
			fscanf_status=fscanf(eigens, "%lf", &loaded_value);
			if(fscanf_status!=1){
				//this row is invalid
				validRow=false;
				break;
			}
			if(i>=3){
				eigenvalues[index][i-3]=loaded_value;
			}
		}
		if(validRow){
			index++;
		}
		//pass to the end of the line
		if(fgets(line, buffer_length, eigens)==NULL){
			//if this read become failed, it means that the cursor reached to the EOF
			break;
		}	
	}
	cout << "#Number of loaded points: " << index << endl;
	if(index!=numPoints){
		cout << "Error in loading eigenvalues" << endl;
		return -1;
	}
	//cout << eigenvalues[0][0] << " " << eigenvalues[0][1] << endl;
	//cout << eigenvalues[1][0] << " " << eigenvalues[1][1] << endl;
	//cout << eigenvalues[2][0] << " " << eigenvalues[2][1] << endl;
	//cout << eigenvalues[3][0] << " " << eigenvalues[3][1] << endl;
	fclose(eigens);

	//find minimum and maximum eigenvalues
	double minEigen=eigenvalues[0][0];
	double maxEigen=eigenvalues[0][0];
	for(i=0; i<numPoints; i++){
		for(j=0; j<numBands; j++){
			if(eigenvalues[i][j]<minEigen){
				minEigen=eigenvalues[i][j];
			}
			if(eigenvalues[i][j]>maxEigen){
				maxEigen=eigenvalues[i][j];
			}
		}
	}
  printf("#Minimum eigenvalue: %12.5e\n", minEigen);
	printf("#Maximum eigenvalue: %12.5e\n", maxEigen);

	//binIndex_offset: floor(minEigen/binWidth)
	//so that the binIndex of minEigen, calculated by floor(minEigen/binWidth)-binIndex_offset becomes zero
	int binIndex_offset=floor(minEigen/binWidth);

	//binIndex_max: the binindex of maxEigen
	int binIndex_max=floor(maxEigen/binWidth)-binIndex_offset;

	int** histogram=new int*[binIndex_max+1];
	for(i=0; i<=binIndex_max; i++){
		histogram[i]=new int[numBands];
		for(j=0; j<numBands; j++){
			histogram[i][j]=0;
		}
	}

	//generate histogram
	int histogramIndex;
	for(i=0; i<numPoints; i++){
		for(j=0; j<numBands; j++){
			histogramIndex=floor(eigenvalues[i][j]/binWidth)-binIndex_offset;
			histogram[histogramIndex][j]++;
		}
	}

	//output
	cout << endl;
	cout << "#center min max ";
	for(i=0; i<numBands;i++){
		cout << "Dos" << (i+1) << " ";
	}
	cout << "Total_Dos" << endl;
	int totalDos;
	for(i=0; i<=binIndex_max; i++){
		printf("%12.5e ", (i+0.5+binIndex_offset)*binWidth);
		printf("%12.5e ", (i+binIndex_offset)*binWidth);
		printf("%12.5e\t", (i+1+binIndex_offset)*binWidth);
		totalDos=0;
		for(j=0; j<numBands;j++){
			printf("%d\t", histogram[i][j]);
			totalDos+=histogram[i][j];
		}
		printf("%d\n", totalDos);
	}
		
	
	return 1;
}

void load_error(int row){	
	printf("Error in loading line %d\n",row);
}
void parse_error(int row){
	printf("Error in parsing line %d\n",row);
}
int loadInput(FILE* input, char* eigenName, int* numBands, double* binWidth){
	int buffer_length=1024;
	char line[buffer_length];
	char* fgets_status;
	int sscanf_status;
	int row=1;
	int i;

	//line 1: eigenfile
	fgets_status=fgets(line, buffer_length, input);
	if(fgets_status==NULL){
		load_error(row);
		return -1;
	}
	sscanf_status=sscanf(line, "%s", eigenName);
	if(sscanf_status!=1){
		parse_error(row);
		return -1;
	}
	printf("#Eigenvalues file: %s\n", eigenName);
	row++;

	//line 2: number of bands
	fgets_status=fgets(line, buffer_length, input);
	if(fgets_status==NULL){
		load_error(row);
		return -1;
	}
	sscanf_status=sscanf(line, "%d", numBands);
	if(sscanf_status!=1){
		parse_error(row);
		return -1;
	}
	printf("#Number of bands: %d\n", *numBands);
	row++;
	
	//line 3: bin width
	fgets_status=fgets(line, buffer_length, input);
	if(fgets_status==NULL){
		load_error(row);
		return -1;
	}
	sscanf_status=sscanf(line, "%lf", binWidth);
	if(sscanf_status!=1){
		parse_error(row);
		return -1;
	}
	printf("#bin width: %.2e\n", *binWidth);
	row++;
	
	return 1;
}
