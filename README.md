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
```
2 #Number of parameters
a 3.0  #Name and value of the first parameter
b -2.0 #The second parameter
```

### Matrix section (ll.(2+N_params)-(2+N_params+N))
The first line of this section represents the size of the Hamiltonian matrix (```N```) and successive ```N``` rows represent the rows of the matrix.
```
4 #Size of the Hamiltonian matrix
a+b     0         kz        kx-I*ky   #The first row
0       a-b       kx+I*ky   (-1)*kz   #The second row
kz      kx-I*ky   (-1)*a+b  0         #The third row
kx+I*ky (-1)*kz   0         (-1)*a-b  #The fourth row
```
- Several space letters are put between each matrix element and no space letter is put in one matrix element.
- You can use **parameters** (defined in the parameters section), **kx, ky, kz** (coordinates of a wave vector), **I** (the imaginary unit), real numbers (**x.yy** format is ok, **x.yye+01** is not), **+-\*/** (binary operators), **()** (parentheses) when you specity matrix elements.
  - When you use negative real numbers, you must enclose the number by parentheses to distinguish it from a minus operator (see the above example).
  
### k range section for eigenvalue calculation (ll.(3+N_params+N)-(5+N_params+N))
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
