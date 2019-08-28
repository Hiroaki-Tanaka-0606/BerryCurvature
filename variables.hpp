#include <complex>
using namespace std;
//constants
int buffer_length; //length of buffer when reading one line from input file
char* fileName_default; //default name of input file
char** ordinalSuffix; //"st","nd","rd","th"
int fileName_length; //max length of file name (including '\0')
int matrixElement_length; //max length of matrix element string
int parameter_length; //max length of paremeter name
int format_length; //max length of log output format
char* realNumber_format; //output format of real number
int auxParameter_length; //length of name of auxiliary parameter used in interpretation of matrix elements (without underscore)
char* auxParameter_letters; //letters used in the name of auxiliary parameter
int auxParameter_count; //max number of auxiliary parameter
//variables
int N; //matrix size
int N_params; //number of parameters
char** params_name; //array of names of parameters
double* params_value; //array of values of parameters
char*** matrix_string; //Hamiltonian matrix (string-form)
double* k_start; //start of k range
double* k_stop; //end of k range
int* k_split; //how many pieces k range is split into
double delta_k; //delta k in difference approximation
char** allParams_name; //params_name + kx,ky,kz,I
complex<double>* allParams_value; //params_value + kx,ky,kz,I
//variables for zheev
char JOBZ;
char UPLO;
complex<double>** matrix;
double* W;
complex<double>* WORK;
int LWORK;
double* RWORK;
int INFO;
