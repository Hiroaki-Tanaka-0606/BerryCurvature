//Berry_EDC.cpp: sum up berry curvatures of points with same eigenenergies
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
using namespace std;

int loadInput(FILE* input, char* eigenName, char* berryName, int* numBands, double* width, char* component);

int main (int argc, const char** argv){
	//initial message
	cout << "#Berry curvature EDC calculation" << endl;

	//variables and constants
	int fileName_length=256; // max length of filename
	const char* fileName_default="input_Berry_EDC.dat"; //default filename of configuration
	char fileName[fileName_length]; //filename of configuration
	int i, j;
	//values of the following variables are specified by configuration file
	char eigenName[fileName_length]; //filename of eigenvalues
	char berryName[fileName_length]; //filename of berry curvatures
	int numBands; //number of bands to be read (unnecessarily the same as number of all bands)
	double binWidth; //width of a energy bin
	char component; //which component of berry curvatures (x, y, or z) are analyzed

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
	int loadInput_status=loadInput(input, &eigenName[0], &berryName[0], &numBands, &binWidth, &component);
	if(loadInput_status==1){
		cout << "#Finished loading input" << endl;
	}else{
		cout << "Failed in loading input" << endl;
		return -1;
	}
	//close input file
	fclose(input);

	//count the number of points in eigenvalues file
	int numPoints_E=0;
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
			numPoints_E++;
		}
		//pass to the end of the line
		if(fgets(line, buffer_length, eigens)==NULL){
			//if this read become failed, it means that the cursor reached to the EOF
			break;
		}	
	}
	cout << "#Number of points in eigenvalues file: " << numPoints_E << endl;

	//go back to the top
	rewind(eigens);

	//load values
	double** eigenvalues=new double*[numPoints_E];
	int index_E=0;
	while(true){
		//load values
		eigenvalues[index_E]=new double[numBands];
		validRow=true;
		for(i=0; i<numBands+3;i++){
			fscanf_status=fscanf(eigens, "%lf", &loaded_value);
			if(fscanf_status!=1){
				//this row is invalid
				validRow=false;
				break;
			}
			if(i>=3){
				eigenvalues[index_E][i-3]=loaded_value;
			}
		}
		if(validRow){
			index_E++;
		}
		//pass to the end of the line
		if(fgets(line, buffer_length, eigens)==NULL){
			//if this read become failed, it means that the cursor reached to the EOF
			break;
		}	
	}
	cout << "#Number of loaded points in eigenvalues file: " << index_E << endl;
	if(index_E!=numPoints_E){
		cout << "Error in loading eigenvalues" << endl;
		return -1;
	}
	fclose(eigens);

	
	//count the number of points in berry curvatures file
	int numPoints_B=0;
	FILE* berrys=fopen(berryName, "r");
	if(berrys==NULL){
		cout << "Error in opening the berry curvatures file" << endl;
		return -1;
	}

	double loaded_valueX, loaded_valueY, loaded_valueZ;
	while(true){
		//check validity
		validRow=true;
		for(i=0; i<numBands*3+3;i++){
			//+3 is necessary to skip kx ky kz values
			fscanf_status=fscanf(berrys, "%lf", &loaded_value);
			if(fscanf_status!=1){
				//this row is invalid
				validRow=false;
				break;
			}
		}
		if(validRow){
			numPoints_B++;
		}
		//pass to the end of the line
		if(fgets(line, buffer_length, berrys)==NULL){
			//if this read become failed, it means that the cursor reached to the EOF
			break;
		}	
	}
	cout << "#Number of points in berry curvatures file: " << numPoints_B << endl;

	//go back to the top
	rewind(berrys);

	//load values
	double** berryvalues=new double*[numPoints_B];
	int index_B=0;
	while(true){
		//load values
		berryvalues[index_B]=new double[numBands];
		validRow=true;
		//skip kx, ky, and kz
		for(i=0; i<3;i++){
			fscanf_status=fscanf(berrys, "%lf", &loaded_value);
			if(fscanf_status!=1){
				//this row is invalid
				validRow=false;
				break;
			}
		}
		if(validRow){
			for(i=0; i<numBands; i++){
				fscanf_status=fscanf(berrys, "%lf%lf%lf", &loaded_valueX, &loaded_valueY, &loaded_valueZ);
				if(fscanf_status!=3){
					validRow=false;
					break;
				}
				if(component=='x'){
					berryvalues[index_B][i]=loaded_valueX;
				}else if(component=='y'){
					berryvalues[index_B][i]=loaded_valueY;
				}else if(component=='z'){
					berryvalues[index_B][i]=loaded_valueZ;
				}
			}
		}
		if(validRow){
			index_B++;
		}
		//pass to the end of the line
		if(fgets(line, buffer_length, berrys)==NULL){
			//if this read become failed, it means that the cursor reached to the EOF
			break;
		}	
	}
	cout << "#Number of loaded points in berry curvatures file: " << index_B << endl;
	if(index_B!=numPoints_B){
		cout << "Error in loading values" << endl;
		return -1;
	}
	fclose(berrys);

	if(numPoints_B!=numPoints_E){
		cout << "Error: numbers of points are different" << endl;
		return -1;
	}
	int numPoints=numPoints_B;
	
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

	double** EDC=new double*[binIndex_max+1];
	double** EDC_plus=new double*[binIndex_max+1];
	double** EDC_minus=new double*[binIndex_max+1];
	for(i=0; i<=binIndex_max; i++){
		EDC[i]=new double[numBands];
		EDC_plus[i]=new double[numBands];
		EDC_minus[i]=new double[numBands];
		for(j=0; j<numBands; j++){
			EDC[i][j]=0;
			EDC_plus[i][j]=0;
			EDC_minus[i][j]=0;
		}
	}

	//generate histogram
	int EDCIndex;
	for(i=0; i<numPoints; i++){
		for(j=0; j<numBands; j++){
			EDCIndex=floor(eigenvalues[i][j]/binWidth)-binIndex_offset;
		  EDC[EDCIndex][j]+=berryvalues[i][j];
			if(berryvalues[i][j]>0){
				EDC_plus[EDCIndex][j]+=berryvalues[i][j];
			}else{
				EDC_minus[EDCIndex][j]+=berryvalues[i][j];
			}
		}
	}


	
	//output
	cout << endl;
	cout << "#center min max ";
	for(i=0; i<numBands;i++){
		cout << "Plus" << (i+1) << " ";
		cout << "Minus" << (i+1) << " ";
		cout << "Total" << (i+1) << " ";
	}
	cout << "All_plus" << " ";
	cout << "All_minus" << " ";
	cout << "All_total" << " ";
	cout << "All_plus_integ" << " ";
	cout << "All_minus_integ" << " ";
	cout << "All_integ" << endl;
	double total, total_plus, total_minus;
	double total_integ=0;
	double total_plus_integ=0;
	double total_minus_integ=0;
	for(i=0; i<=binIndex_max; i++){
		printf("%12.5e ", (i+0.5+binIndex_offset)*binWidth);
		printf("%12.5e ", (i+binIndex_offset)*binWidth);
		printf("%12.5e\t", (i+1+binIndex_offset)*binWidth);
		total=0;
		total_plus=0;
		total_minus=0;
		for(j=0; j<numBands;j++){
			printf("%12.5e ", EDC_plus[i][j]);
			printf("%12.5e ", EDC_minus[i][j]);
			printf("%12.5e\t", EDC[i][j]);
			total+=EDC[i][j];
			total_plus+=EDC_plus[i][j];
			total_minus+=EDC_minus[i][j];
		}
		total_integ+=total;
		total_plus_integ+=total_plus;
		total_minus_integ+=total_minus_integ;
		printf("%12.5e ", total_plus);
		printf("%12.5e ", total_minus);
		printf("%12.5e ", total);
		printf("%12.5e ", total_plus_integ);
		printf("%12.5e ", total_minus_integ);
		printf("%12.5e\n", total_integ);
	}
	return 1;
}

void load_error(int row){	
	printf("Error in loading line %d\n",row);
}
void parse_error(int row){
	printf("Error in parsing line %d\n",row);
}
int loadInput(FILE* input, char* eigenName, char* berryName, int* numBands, double* binWidth, char* component){
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
	
	//line 2: berryfile
	fgets_status=fgets(line, buffer_length, input);
	if(fgets_status==NULL){
		load_error(row);
		return -1;
	}
	sscanf_status=sscanf(line, "%s", berryName);
	if(sscanf_status!=1){
		parse_error(row);
		return -1;
	}
	printf("#Berry curvatures file: %s\n", berryName);
	row++;

	//line 3: number of bands
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
	
	//line 4: bin width
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
		
	//line 5: component
	fgets_status=fgets(line, buffer_length, input);
	if(fgets_status==NULL){
		load_error(row);
		return -1;
	}
	sscanf_status=sscanf(line, "%c", component);
	if(sscanf_status!=1){
		parse_error(row);
		return -1;
	}
	if(*component=='x' || *component=='X'){
		*component='x';
	}else if(*component=='y' || *component=='Y'){
		*component='y';
	}else if(*component=='z' || *component=='z'){
		*component='z';
	}else{
		printf("Error: Invalid component %c\n", *component);
		return -1;
	}
	printf("#Component: %c\n", *component);
	row++;
	
	return 1;
}
