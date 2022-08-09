#include <complex>
using namespace std;
//description of there variables are in variables.hpp
//constants
extern int buffer_length;
extern char* fileName_default;
extern char** ordinalSuffix;
extern int fileName_length;
extern int parameter_length;
extern int equation_length;
extern int matrixElement_length;
extern int format_length;
extern char* realNumber_format;
extern int auxParameter_length;
extern char* auxParameter_letters;
extern int auxParameter_maxCount;
extern int mathConstants_count;
//variables
extern int N, N_params, N_eqns;
extern char** params_name;
extern double* params_value;
extern char** eqns_name;
extern char** eqns_value;
extern char*** matrix_string;
extern char*** k_bases_string;
extern double** k_bases;
extern double* k_start;
extern double* k_stop;
extern int* k_split;
extern char* delta_k_string;
extern double delta_k;
extern char** allParams_name;
extern complex<double>* allParams_value;
extern char** auxParameter_name;
extern complex<double>* auxParameter_value;
extern int auxParameter_count;
//variables for zheev
extern char JOBZ;
extern char UPLO;
extern complex<double>** matrix;
extern double* W;
extern complex<double>* WORK;
extern int LWORK;
extern double* RWORK;
extern int INFO;
//variables for interpretEquation
extern char* equation_inParentheses;
extern char* newEquation;
//variables for HWR matrix
extern complex<double>*** HWRMatrix;
extern double** HWRLattice;
extern int** RCoordinate;
extern int HWRSize;
