#include <complex>
using namespace std;
int ordinalSuffix_index(int n);
void setAuxParameterName(char** auxParameter_name, int auxParameter_next);
void addKI();
void makeKList(double** kArray, int index);
complex<double>** alloc_zmatrix(int n, int m);
double* alloc_dvector(int n);
complex<double>* alloc_zvector(int n);
void compositeMatrix();
void compositeK(double k1, double k2, double k3);
void copy_zmatrix(complex<double>** dst, complex<double>** src, int N, int M);
complex<double> zNorm(complex<double>* a, complex<double>* b, int N);
