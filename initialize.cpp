#include "variables_ext.hpp"
#include <complex>
void initialize(){
	buffer_length=1024;
	fileName_default=(char*)"input.dat";
	fileName_length=256;
	format_length=256;
	matrixElement_length=128;
	parameter_length=32;
	equation_length=128;
	ordinalSuffix=new char*[4];
	ordinalSuffix[0]=(char*)"st";
	ordinalSuffix[1]=(char*)"nd";
	ordinalSuffix[2]=(char*)"rd";
	ordinalSuffix[3]=(char*)"th";
	realNumber_format=(char*)"%12.5e";
	auxParameter_length=4;
	auxParameter_letters=(char*)"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	auxParameter_maxCount=256;
	auxParameter_name=new char*[auxParameter_maxCount];
	auxParameter_value=new complex<double>[auxParameter_maxCount];
	mathConstants_count=2;
	equation_inParentheses=new char[equation_length];
	newEquation=new char[equation_length];
}
