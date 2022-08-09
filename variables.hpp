#include <complex>
using namespace std;
//constants
int buffer_length; //length of buffer when reading one line from input file
char* fileName_default; //default name of input file
char** ordinalSuffix; //"st","nd","rd","th"
int fileName_length; //max length of file name (including '\0')
int matrixElement_length; //max length of matrix element string
int parameter_length; //max length of parameter or equation name
int equation_length; //max length of equation value
int format_length; //max length of log output format
char* realNumber_format; //output format of real number
int auxParameter_length; //length of name of auxiliary parameter used in interpretation of matrix elements (without underscore)
char* auxParameter_letters; //letters used in the name of auxiliary parameter
int auxParameter_maxCount; //max number of auxiliary parameter
int mathConstants_count; //number of math constants
//variables
int N; //matrix size
int N_params; //number of parameters
int N_eqns; //number of equations
char** params_name; //array of names of parameters
double* params_value; //array of values of parameters
char** eqns_name; //array of names of equations
char** eqns_value; //array of values of equations
char*** matrix_string; //Hamiltonian matrix (string-form)
char*** k_bases_string; // basis vectors (string-form)
double** k_bases; // basis vectors for k range
double* k_start; //start of k range
double* k_stop; //end of k range
int* k_split; //how many pieces k range is split into
char* delta_k_string; // delta k (string-form)
double delta_k; //delta k in difference approximation
char** allParams_name; //params_name + kx,ky,kz,I
complex<double>* allParams_value; //params_value + kx,ky,kz,I
char** auxParameter_name; //names of auxiary parameters
complex<double>* auxParameter_value; //values of auxiary parameters
int auxParameter_count; // number of auxiary parameters
//variables for zheev
char JOBZ;
char UPLO;
complex<double>** matrix;
double* W;
complex<double>* WORK;
int LWORK;
double* RWORK;
int INFO;
//variables for interpertEquation
char* equation_inParentheses; // equation inside of the largest parentheses
char* newEquation; // new equation generated by replacing some part into an auxiary parameter
//variables for HWR matrix
complex<double>*** HWRMatrix; // H_mn(R) (Hartree)
double** HWRLattice; // Lattice vector (Bohr)
int** RCoordinate; // R position
int HWRSize; // number of Wigner-Seitz supercell
