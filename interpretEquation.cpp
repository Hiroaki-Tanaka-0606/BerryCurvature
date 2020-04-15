#include <complex>
#include <string.h>
#include <iostream>
#include <cmath>
#include "variables_ext.hpp"
#include "utils.hpp"
using namespace std;
int interpretEquation(char* equation, char** params_name, complex<double>* params_value, complex<double>* result);

int findParentheses(char* equation, int* start, int* end){
	//find outermost parentheses
	//return 1 -> found
	//return 0 -> not found
	//return -1 -> error (mismatch)
	int i;
	int len=strlen(equation);
	int depth=0;
	*start=-1;
	const char openParenthesis='(';
	const char closeParenthesis=')';
	for(i=0;i<len;i++){
		if(equation[i]==openParenthesis){
			if(depth==0){
				//start of outermost parentheses
				*start=i;
			}
			depth++;
		}
		if(equation[i]==closeParenthesis){
			if(depth<1){
				//error
				return -1;
			}
			if(depth==1){
				//end of outermost parentheses
				*end=i;
				return 1;
			}
			depth--;
		}
	}
	if(*start==-1){
		return 0;
	}else{
		return -1;
	}
}

bool isBinaryOperator(char letter){
	return letter=='+' || letter=='-' || letter=='*' || letter=='/';
}
bool isMulDivOperator(char letter){
	return letter=='*' || letter=='/';
}
bool isAddSubOperator(char letter){
	return letter=='+' || letter=='-';
}
int findMulDivOperator(char* equation, int* start, int* middle, int* end){
	//find * or /
	//return 1 -> found
	//return 0 -> not found
	//return -1 -> error (may not happen)
	//start: index of the operator just before * or / (should be + or -)
	//middle: index of * or /
	//end: index of the operator just after * or / (should be + or -)
	int i,j,k;
	int len=strlen(equation);
	*start=-1;
	for(i=0;i<len;i++){
		//find * or /
		if(isMulDivOperator(equation[i])){
			*middle=i;
			//find operator just before * or /
			*start=-1;
			for(j=i-1;j>=0;j--){
				if(isBinaryOperator(equation[j])){
					*start=j;
					break;
				}
			}
			//find operator just after * or /
			*end=len;
			for(j=i+1;j<len;j++){
				if(isBinaryOperator(equation[j])){
					*end=j;
					break;
				}
			}
			return 1;
		}
	}
	return 0;
}
int findAddSubOperator(char* equation, int* start, int* middle, int* end){
	//find - or +
	//return 1 -> found
	//return 0 -> not found
	//return -1 -> error (may not happen)
	//start: index of the operator just before + or - (should be -1)
	//middle: index of * or /
	//end: index of the operator just after + or - (should be + or -)
	int i,j,k;
	int len=strlen(equation);
	*start=-1;
	for(i=0;i<len;i++){
		//find + or -
		if(isAddSubOperator(equation[i])){
			*middle=i;
			//find operator just before + or -
			*start=-1;
			for(j=i-1;j>=0;j--){
				if(isBinaryOperator(equation[j])){
					*start=j;
					break;
				}
			}
			//find operator just after + or -
			*end=len;
			for(j=i+1;j<len;j++){
				if(isBinaryOperator(equation[j])){
					*end=j;
					break;
				}
			}
			return 1;
		}
	}
	return 0;
}

bool isRealNumber(char* name){
	//only (-)xx.yyy type
	int len=strlen(name);
	int i;
	bool beforePoint=true;
	for(i=0;i<len;i++){
		if(i==0 && name[i]=='-'){
			continue;
		}
		if(name[i]=='.'){
			if(beforePoint==true){
				beforePoint==false;
				continue;
			}else{
				//error (multiple '.'s)
				return false;
			}
		}
		if(name[i]=='0' || name[i]=='1' || name[i]=='2' || name[i]=='3' ||
			 name[i]=='4' || name[i]=='5' || name[i]=='6' || name[i]=='7' ||
			 name[i]=='8' || name[i]=='9'){
			continue;
		}else{
			//error (letter other than [-.0-9])
			return false;
		}
	}
	return true;
}

int getValue(char* name, char** params_name, complex<double>* params_value, char** auxParameter_name, complex<double>* auxParameter_value, int auxParameter_next, complex<double>* value){
	//from params
	int i;
  for(i=0;i<N_params+3+mathConstants_count;i++){
		if(strcmp(name,params_name[i])==0){
			*value=params_value[i];
			return 1;
		}
	}
	//from equations
	for(i=0;i<N_eqns;i++){
		if(strcmp(name,eqns_name[i])==0){
			interpretEquation(eqns_value[i],params_name,params_value,value);
			return 1;
		}
	}
	//from auxParams
	for(i=0;i<auxParameter_next;i++){
		if(strcmp(name,auxParameter_name[i])==0){
			*value=auxParameter_value[i];
			return 1;
		}
	}
	//real number?
	if(isRealNumber(name)){
		*value=(complex<double>)atof(name);
		return 1;
	}else{
		return -1;
	}
}

int interpretEquation(char* equation, char** params_name, complex<double>* params_value, complex<double>* result){
	//cout << equation << endl;
	char* auxParameter_name[auxParameter_count];
	complex<double> auxParameter_value[auxParameter_count];
	
	//arguments (to be deleted)
	//for first step
	//auxParameter_name[i] is also to be deleted
	char* equation_current=NULL;
	char* equation_inParentheses=NULL;
	char* newEquation=NULL;
	//for second step
	char* leftName=NULL;
	char* rightName=NULL;

	//need to be initialized before goto statement
	complex<double> leftValue, rightValue;
	complex<double> operatorResult;

	//return status
	//when error occurs, status is changed to -1
	int status=1;
	
	int auxParameter_next=0;
	//copy equation
	equation_current=new char[strlen(equation)+1]; //no need to use delete[] because of its first time to use "new"
	strcpy(equation_current,equation);
	
	//initial step: if real number, set the value to the result
	//To avoid error in parsing "-1"
	if(isRealNumber(equation)){
		*result=(complex<double>)atof(equation);
		goto FINALIZATION;
	}
	
	//first step: resolve parentheses
	int start,end;
	int findParentheses_status;
	int i;
	findParentheses_status=findParentheses(equation_current,&start,&end);
	while(findParentheses_status==1){
		//calculate inside of the outermost parentheses and set to auxParemeter
		//set string inside of the outermost parentheses to equation_inParentheses
		delete[] equation_inParentheses;
		equation_inParentheses=new char[end-start];
		strncpy(equation_inParentheses,&equation_current[start+1],end-start-1);
		equation_inParentheses[end-start-1]='\0';
		//calculate inside
		complex<double> result_inParentheses;
		int inParentheses_status=interpretEquation(equation_inParentheses, params_name, params_value, &result_inParentheses);
		if(inParentheses_status==1){
			int length_beforeOpenParenthesis=0;
			//if the letters before open parenthesis is (+-*/)(cos|sin|cis) or $(cos|sin|cis), interpret these mathematical function
			//3-long math function
			if((start>4 && isBinaryOperator(equation_current[start-4])) || start==3){
				char mathFunc3[4];
				strncpy(mathFunc3,&equation_current[start-3],3);
				double result_realPart=result_inParentheses.real();
				if(strncmp(mathFunc3,"cos",3)==0){
					length_beforeOpenParenthesis=3;
					result_inParentheses=complex<double>(cos(result_realPart),0);
				}else if(strncmp(mathFunc3,"sin",3)==0){
					length_beforeOpenParenthesis=3;
					result_inParentheses=complex<double>(sin(result_realPart),0);
				}else if(strncmp(mathFunc3,"cis",3)==0){
					length_beforeOpenParenthesis=3;
					result_inParentheses=complex<double>(cos(result_realPart),sin(result_realPart));
				}else if(strncmp(mathFunc3,"exp",3)==0){
					length_beforeOpenParenthesis=3;
					result_inParentheses=complex<double>(exp(result_realPart),0);
				}
			}
			//4-long math function
			if((start>5 && isBinaryOperator(equation_current[start-5])) || start==4){
				char mathFunc4[5];
				strncpy(mathFunc4,&equation_current[start-4],4);
				double result_realPart=result_inParentheses.real();
				double result_imagPart=result_inParentheses.imag();
				if(strncmp(mathFunc4,"sqrt",4)==0){
					length_beforeOpenParenthesis=4;
					result_inParentheses=complex<double>(sqrt(result_realPart),0);
				}else if(strncmp(mathFunc4,"cosh",4)==0){
					length_beforeOpenParenthesis=4;
					result_inParentheses=complex<double>(cosh(result_realPart),0);
				}else if (strncmp(mathFunc4,"sinh",4)==0){
					length_beforeOpenParenthesis=4;
					result_inParentheses=complex<double>(sinh(result_realPart),0);
				} if (strncmp(mathFunc4,"conj",4)==0){
					length_beforeOpenParenthesis=4;
					result_inParentheses=complex<double>(result_realPart,-result_imagPart);
				}

			}
			
			//set to auxParameter
			auxParameter_name[auxParameter_next]=new char[auxParameter_length+2]; //first time
			setAuxParameterName((char**)auxParameter_name,auxParameter_next);
			auxParameter_value[auxParameter_next]=result_inParentheses;
			//replace the parentheses to new auxParameter
			int newlen=strlen(equation_current)-(end-start+length_beforeOpenParenthesis+1)+(1+auxParameter_length)+1;
			delete[] newEquation;
			newEquation=new char[newlen];
			int newidx=0;
			//before start parenthesis
			for(i=0;i<start-length_beforeOpenParenthesis;i++){
				newEquation[newidx]=equation_current[i];
				newidx++;
			}
			//inside parentheses -> new auxParameter
			for(i=0;i<=auxParameter_length;i++){
				newEquation[newidx]=auxParameter_name[auxParameter_next][i];
				newidx++;
			}
			//after end parenthesis
			for(i=end+1;i<=strlen(equation_current);i++){
				newEquation[newidx]=equation_current[i];
				newidx++;
			}
			newEquation[newidx]='\0';
			
			delete[] equation_current;
			equation_current=new char[newlen];
			strcpy(equation_current,newEquation);
			auxParameter_next++;
		}else{
			cout << "Error in interpretEquation" << endl;
			status=-1;
			goto FINALIZATION;
		}
		findParentheses_status=findParentheses(equation_current,&start,&end);
	}

	//second step: calculate * and /
	//find first * or /
	int middle;
	int getValue_status;
	int findMulDivOperator_status;
	int newlen, newidx;
	findMulDivOperator_status=findMulDivOperator(equation_current,&start,&middle,&end);
	while(findMulDivOperator_status==1){
		//extract left and right name (real number or parameter)
		delete[] leftName;
		delete[] rightName;
		leftName=new char[middle-start];
		rightName=new char[end-middle];
		strncpy(leftName,&equation_current[start+1],middle-start-1);
		leftName[middle-start-1]='\0';
		strncpy(rightName,&equation_current[middle+1],end-middle-1);
		rightName[end-middle-1]='\0';
		//get the value of left and right
		getValue_status=getValue(leftName,params_name,params_value,auxParameter_name,auxParameter_value,auxParameter_next,&leftValue);
		if(getValue_status!=1){
			cout << "Error in interpretEquation, getting value of " << leftName << " (left, */)" << endl;
			status=-1;
			goto FINALIZATION;
		}
		getValue_status=getValue(rightName,params_name,params_value,auxParameter_name,auxParameter_value,auxParameter_next,&rightValue);
		if(getValue_status!=1){
			cout << "Error in interpretEquation, getting value of " << rightName << " (right, */)"  << endl;
			status=-1;
			goto FINALIZATION;
		}

		//cout << leftName << "=" << leftValue << "," << rightName << "=" << rightValue << endl;

		//calculation
		if(equation_current[middle]=='*'){
			operatorResult=leftValue*rightValue;
		}else{
			operatorResult=leftValue/rightValue;
		}
		
		//set to auxParameter
		auxParameter_name[auxParameter_next]=new char[auxParameter_length+2];
		setAuxParameterName((char**)auxParameter_name,auxParameter_next);
		auxParameter_value[auxParameter_next]=operatorResult;
		//replace the calculated region to new auxParameter
		newlen=strlen(equation_current)-(end-start-1)+(1+auxParameter_length)+1;
		delete[] newEquation;
		newEquation=new char[newlen];
		newidx=0;
		//before start operator
		for(i=0;i<=start;i++){
			newEquation[newidx]=equation_current[i];
			newidx++;
		}
		//calculated region -> new auxParameter
		for(i=0;i<=auxParameter_length;i++){
			newEquation[newidx]=auxParameter_name[auxParameter_next][i];
			newidx++;
		}
		//after end operator
		for(i=end;i<=strlen(equation_current);i++){
			newEquation[newidx]=equation_current[i];
			newidx++;
		}
		newEquation[newidx]='\0';
		delete[] equation_current;
		equation_current=new char[newlen];
		strcpy(equation_current,newEquation);
		auxParameter_next++;
		
		findMulDivOperator_status=findMulDivOperator(equation_current,&start,&middle,&end);
	}

	//third step: calculate + and -
	//find first + or -
	int findAddSubOperator_status;
	findAddSubOperator_status=findAddSubOperator(equation_current,&start,&middle,&end);
	while(findAddSubOperator_status==1){
		//extract left and right name (real number or parameter)
		delete[] leftName;
		delete[] rightName;
		leftName=new char[middle-start];
		rightName=new char[end-middle];
		strncpy(leftName,&equation_current[start+1],middle-start-1);
		leftName[middle-start-1]='\0';
		strncpy(rightName,&equation_current[middle+1],end-middle-1);
		rightName[end-middle-1]='\0';
		//get the value of left and right
		getValue_status=getValue(leftName,params_name,params_value,auxParameter_name,auxParameter_value,auxParameter_next,&leftValue);
		if(getValue_status!=1){
			cout << "Error in interpretEquation, getting value of " << leftName << "(left, +-)" << endl;
			status=-1;
			goto FINALIZATION;
		}
		getValue_status=getValue(rightName,params_name,params_value,auxParameter_name,auxParameter_value,auxParameter_next,&rightValue);
		if(getValue_status!=1){
			cout << "Error in interpretEquation, getting value of " << rightName << "(right, +-)" << endl;
			status=-1;
			goto FINALIZATION;
		}

		//cout << leftName << "=" << leftValue << "," << rightName << "=" << rightValue << endl;

		//calculation
		if(equation_current[middle]=='+'){
			operatorResult=leftValue+rightValue;
		}else{
			operatorResult=leftValue-rightValue;
		}
		
		//set to auxParameter
		auxParameter_name[auxParameter_next]=new char[auxParameter_length+2];
		setAuxParameterName((char**)auxParameter_name,auxParameter_next);
		auxParameter_value[auxParameter_next]=operatorResult;
		//replace the calculated region to new auxParameter
		newlen=strlen(equation_current)-(end-start-1)+(1+auxParameter_length)+1;
		delete[] newEquation;
		newEquation=new char[newlen];
		newidx=0;
		//before start operator
		for(i=0;i<=start;i++){
			newEquation[newidx]=equation_current[i];
			newidx++;
		}
		//calculated region -> new auxParameter
		for(i=0;i<=auxParameter_length;i++){
			newEquation[newidx]=auxParameter_name[auxParameter_next][i];
			newidx++;
		}
		//after end operator
		for(i=end;i<=strlen(equation_current);i++){
			newEquation[newidx]=equation_current[i];
			newidx++;
		}
		newEquation[newidx]='\0';
		delete[] equation_current;
		equation_current=new char[newlen];
		strcpy(equation_current,newEquation);
		auxParameter_next++;
		findAddSubOperator_status=findAddSubOperator(equation_current,&start,&middle,&end);
	}
	
	//set the value to the result
	getValue_status=getValue(equation_current,params_name,params_value,auxParameter_name,auxParameter_value,auxParameter_next,result);
	if(getValue_status!=1){
		cout << "Error in interpretEquation, getting value of " << equation_current << "(last)" << endl;
		status=-1;
		goto FINALIZATION;
	}
	//cout << "interpretEquation result: " << *result << endl;

	//finalization: delete[]
 FINALIZATION:
	for(i=0;i<auxParameter_next;i+=1){
		delete[] auxParameter_name[i];
	}
	delete[] equation_current;
	delete[] equation_inParentheses;
	delete[] newEquation;
	delete[] leftName;
	delete[] rightName;
	return status;
	
}
