#include <complex>
#include <string.h>
#include <iostream>
#include "variables_ext.hpp"
#include "utils.hpp"
using namespace std;

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
}

int getValue(char* name, char** params_name, complex<double>* params_value, char** auxParameter_name, complex<double>* auxParameter_value, int auxParameter_next, complex<double>* value){
	//from params
	int i;
  for(i=0;i<N_params+4;i++){
		if(strcmp(name,params_name[i])==0){
			*value=params_value[i];
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
	int auxParameter_next=0;
	//copy equation
	char* equation_current=new char[strlen(equation)+1];
	strcpy(equation_current,equation);

	//initial step: if real number, set the value to the result
	//To avoid error in parsing "-1"
	if(isRealNumber(equation)){
		*result=(complex<double>)atof(equation);
		return 1;
	}
	
	//first step: resolve parentheses
	int start,end;
	int findParentheses_status;
	int i;
	findParentheses_status=findParentheses(equation_current,&start,&end);
	while(findParentheses_status==1){
		//calculate inside of the outermost parentheses and set to auxParemeter
		//set string inside of the outermost parentheses to equation_inParentheses
		char* equation_inParentheses=new char[end-start];
		strncpy(equation_inParentheses,&equation_current[start+1],end-start-1);
		equation_inParentheses[end-start-1]='\0';
		//calculate inside
		complex<double> result_inParentheses;
		int inParentheses_status=interpretEquation(equation_inParentheses, params_name, params_value, &result_inParentheses);
		if(inParentheses_status==1){
			//set to auxParameter
			auxParameter_name[auxParameter_next]=new char[auxParameter_length+2];
			setAuxParameterName((char**)auxParameter_name,auxParameter_next);
			auxParameter_value[auxParameter_next]=result_inParentheses;
			//replace the parentheses to new auxParameter
			int newlen=strlen(equation_current)-(end-start+1)+(1+auxParameter_length)+1;
			char* newEquation=new char[newlen];
			int newidx=0;
			//before start parenthesis
			for(i=0;i<start;i++){
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
			equation_current=(char*)newEquation;
			auxParameter_next++;
		}else{
			cout << "Error in interpretEquation" << endl;
			return -1;
		}
		findParentheses_status=findParentheses(equation_current,&start,&end);
	}

	//second step: calculate * and /
	//find first * or /
	int middle;
	char* leftName;
	char* rightName;
	int getValue_status;
	complex<double> leftValue, rightValue;
	complex<double> operatorResult;
	int findMulDivOperator_status;
	int newlen, newidx;
	char* newEquation;
	findMulDivOperator_status=findMulDivOperator(equation_current,&start,&middle,&end);
	while(findMulDivOperator_status==1){
		//extract left and right name (real number or parameter)
		leftName=new char[middle-start];
		rightName=new char[end-middle];
		strncpy(leftName,&equation_current[start+1],middle-start-1);
		leftName[middle-start-1]='\0';
		strncpy(rightName,&equation_current[middle+1],end-middle-1);
		rightName[end-middle-1]='\0';
		//get the value of left and right
		getValue_status=getValue(leftName,params_name,params_value,auxParameter_name,auxParameter_value,auxParameter_next,&leftValue);
		if(getValue_status!=1){
			cout << "Error in interpretEquation, getting value of " << leftName << "(left, */)" << endl;
			return -1;
		}
		getValue_status=getValue(rightName,params_name,params_value,auxParameter_name,auxParameter_value,auxParameter_next,&rightValue);
		if(getValue_status!=1){
			cout << "Error in interpretEquation, getting value of " << rightName << "(right, */)"  << endl;
			return -1;
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
		equation_current=(char*)newEquation;
		auxParameter_next++;
		
		findMulDivOperator_status=findMulDivOperator(equation_current,&start,&middle,&end);
	}

	//third step: calculate + and -
	//find first + or -
	int findAddSubOperator_status;
	findAddSubOperator_status=findAddSubOperator(equation_current,&start,&middle,&end);
	while(findAddSubOperator_status==1){
		//extract left and right name (real number or parameter)
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
			return -1;
		}
		getValue_status=getValue(rightName,params_name,params_value,auxParameter_name,auxParameter_value,auxParameter_next,&rightValue);
		if(getValue_status!=1){
			cout << "Error in interpretEquation, getting value of " << rightName << "(right, +-)" << endl;
			return -1;
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
		equation_current=(char*)newEquation;
		auxParameter_next++;
		
		findAddSubOperator_status=findAddSubOperator(equation_current,&start,&middle,&end);
	}
	
	//set the value to the result
	getValue_status=getValue(equation_current,params_name,params_value,auxParameter_name,auxParameter_value,auxParameter_next,result);
	if(getValue_status!=1){
		cout << "Error in interpretEquation, getting value of " << equation_current << "(last)" << endl;
		return -1;
	}
	//cout << "interpretEquation result: " << *result << endl;
	return 1;
}
