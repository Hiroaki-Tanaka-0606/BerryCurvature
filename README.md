# BerryCurvature
Calculate energy eigenvalue and Berry curvature of a matrix Hamiltonian

## Usage
```
$ ./eigen.o INPUT_FILE
$ ./berry.o INPUT_FILE
```
The default name of the input file (used when missing the argument ```INPUT_FILE```) is **input.dat** (defined in **initialize.cpp**).

## Input file
Input file is a formatted text file, according to the following rules.
Several space letters are put between each value.
In each line, you can write down some comments after arguments.
No blank row should be put between each section.

### Parameters section (ll.1-(1+N_params))
The first line of this section represents the number of parameters (```N_params```) and successive ```N_params``` rows represent the name and value of the parameters.
You can't use, as names of parameters,**kx, ky, kz, I, PI, cos, sin, cis** and any string starting from **\_** (underscore) or including **+, -, \*, /, .** because they have specific meaning in interpretation of matrix elements.
```
2 #Number of parameters
a 3.0  #Name and value of the first parameter
b -2.0 #The second parameter
```

### Equations section (ll.(2+N_params)-(2+N_params+N_eqns))
The first line of this section represents the number of equations (```N_eqns```) and successive ```N_eqns``` rows represent the name and value of the equations.
You can use any representations other than the name of the equation itself.
Be careful **not to cause circular reference**.

Actually, the function of equations includes that of parameters.
```
2 #Number of equations
func1 kx+I*ky #Name and value of the first equation
func2 kx-I*ky #The second equation
```

### Matrix section (ll.(3+N_params+N_eqns)-(3+N_params+N_eqns+N))
The first line of this section represents the size of the Hamiltonian matrix (```N```) and successive ```N``` rows represent the rows of the matrix.
```
4 #Size of the Hamiltonian matrix
a+b     0         kz        kx-I*ky   #The first row
0       a-b       kx+I*ky   (-1)*kz   #The second row
kz      kx-I*ky   (-1)*a+b  0         #The third row
kx+I*ky (-1)*kz   0         (-1)*a-b  #The fourth row
```
- Several space letters are put between each matrix element and no space letter is put in one matrix element.
- You can use **parameters** (defined in the parameters section), **kx, ky, kz** (coordinates of a wave vector), **I** (the imaginary unit), **PI** (pi), real numbers (**x.yy** format is ok, **x.yye+01** is not), **+, -, \*, /** (binary operators), **(, )** (parentheses), **cos** (cosine), **sin** (sine), **cis** (cos+I\*sin), **sqrt** (square root) when you specity matrix elements.
  - When you use negative real numbers, you must enclose the number by parentheses to distinguish it from a minus operator (see the above example).
- Since Hamiltonian matrix should be Hermite, only upper triangular part and diagonal component are used in diagonalization procedure. However you probably should write down also lower trianglular part so that the Hamiltonian matrix is clearly Hermite.
  
### k range section for eigenvalue calculation (ll.(4+N_params+N_eqns+N)-(6+N_params+N_eqns+N))
These three lines in this section represent kx, ky, kz range of calculation.
```
0  0 0  #kx range (double start, double stop, int split)
-1 1 10 #ky range
-2 2 20 #kz range
```
This program split the range **[start, stop]** into ```split``` pieces and calculation is done in ```split```+1 points.
```
 [Index of k point] 1    2    3   --- n-1     n 
 [Index of piece]   | #1 | #2 |   ---  | #n-1 | 
 [Value of k point] i   i+d  i+2d --- f-d     f 
i=start, f=stop, d=(stop-start)/split, n=split+1
```
If ```split``` is zero, calculation is done in case of **k=start**.

### k range section for Berry curvature calculation (ll.(7+N_params+N_eqns+N)-(9+N_params+N_eqns+N))
The format is the same as above k range section.

### Delta k section (l.(10+N_params+N_eqns+N), only for Berry curvature calculation)
This section has only one row, representing **&Delta;k** (used in difference approximation).
```
1e-2 #Delta k
```
