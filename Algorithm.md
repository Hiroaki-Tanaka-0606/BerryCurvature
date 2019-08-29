# Algorithm of the calculation of Berry curvature
By diagonalizing the Hamiltonian matrix (function of wave vector **k**), you can get eigenvectors (also function of wave vector).
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\hat{H}(k)\psi_n(k)=\varepsilon_n\psi_n(k)"></p>

Berry curvature of wave vector **&Omega;<sub>n</sub>** is defined in the following equation:
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\Omega_n(k)=\text{i}\Bigl(\nabla_k\psi_n^*(k)\Bigr)\times\Bigl(\nabla_k\psi_n(k)\Bigr)"></p>

To calculate the Berry curvature, you need to calculate the wave vector gradient of eigenvector. Defining
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\chi_n(k)=\nabla_k\psi_n(k)"></p>
The above equation is transformed to
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\Omega_n(k)=\text{i}\chi_n^*(k)\times\chi_n(k)=\begin{pmatrix}-2\text{Im}(\chi_{n,y}(k)\chi_{n,z}(k))&space;\\&space;-2\text{Im}(\chi_{n,z}(k)\chi_{n,x}(k))&space;\\&space;-2\text{Im}(\chi_{n,x}(k)\chi_{n,y}(k))\end{pmatrix}"></p>
