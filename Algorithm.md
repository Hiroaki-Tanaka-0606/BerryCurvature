# Algorithm of the calculation of Berry curvature
By diagonalizing the Hamiltonian matrix (function of wave vector **k**), we can get eigenvectors (also function of wave vector).
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\hat{H}(k)\psi_n(k)=\varepsilon_n\psi_n(k)"></p>

Berry curvature of wave vector **&Omega;<sub>n</sub>** is defined in the following equation:
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\Omega_n(k)=\text{i}\Bigl(\nabla_k\psi_n^*(k)\Bigr)\times\Bigl(\nabla_k\psi_n(k)\Bigr)"></p>

To calculate the Berry curvature, we need to calculate the wave vector gradient of eigenvector. Defining
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\chi_n(k)=\nabla_k\psi_n(k)"></p>
The above equation is transformed to
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\Omega_n(k)=\text{i}\chi_n^*(k)\times\chi_n(k)=\begin{pmatrix}-2\text{Im}(\chi_{n,y}^*(k)\chi_{n,z}(k))&space;\\&space;-2\text{Im}(\chi_{n,z}^*(k)\chi_{n,x}(k))&space;\\&space;-2\text{Im}(\chi_{n,x}^*(k)\chi_{n,y}(k))\end{pmatrix}"></p>

To calculate the wave vector gradient of eigenvector, we use differencial approximation:
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\chi_n(k)=\frac{\psi_n(k&plus;\Delta&space;k)-\psi_n(k-\Delta&space;k)}{2\Delta&space;k}&plus;\mathcal{O}(\Delta&space;k)"></p>

However, Both **&psi;<sub>n</sub>(k+&Delta;k)** and **&psi;<sub>n</sub>(k-&Delta;k)** can not always be obtained by diagonalization process, because they have one degree of freedom in phase factor **exp(i&theta;)**. To get the smooth function **&\psi;<sub>n</sub>(k)**, we need to adjust the phase. Here we show the algorithm to adjust the phase of **k+&Delta;k** to **k**.

By diagonalization process, we get eigenvectors with phase difference **&psi;<sub>n</sub>(k)** and **exp(i&theta;)&psi;<sub>n</sub>(k+&Delta;k)**. Using Taylor expansion in the first-order,
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;e^{\text{i}\theta}\psi_n(k&plus;\Delta&space;k)=e^{\text{i}\theta}\Bigl(\psi_n(k)&plus;\Delta&space;k\cdot\nabla_k\psi_n(k)\Bigr)&plus;\mathcal{O}(\Delta&space;k^2)"></p>

Taking the norm with **&psi;<sub>n</sub>(k)**,
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\psi_n^*(k)e^{\text&space;i\theta}\psi_n(k&plus;\Delta&space;k)=e^{\text&space;i\theta}\Bigl(1&plus;\Delta&space;k\cdot\psi_n^*(k)\nabla_k\psi_n(k)\Bigr)&plus;\mathcal{O}(\Delta&space;k^2)"></p>

If we neglect the first order of **&Delta;k**, we can get the phase factor **&theta;**
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\theta=\arg\Bigl(\psi_n^*(k)e^{\text&space;i\theta}\psi_n(k&plus;\Delta&space;k)\Bigr)"></p>

To get less approximate value, we need to evaluate **&psi;<sub>n</sub><sup>*</sup>(k)&nabla;<sub>k</sub>&psi;<sub>n</sub>(k)**. By orthonormalization condition of eigenvector **&psi;<sub>n</sub>(k)**,
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\nabla_k\Bigl(&\psi_n^*(k)\psi_n(k)\Bigr)=\psi_n^*(k)\nabla_k\psi_n(k)&plus;\Bigl(\psi_n^*(k)\nabla_k\psi_n(k)\Bigr)^*=0"></img>

Therefore, **&psi;<sub>n</sub><sup>*</sup>(k)&nabla;<sub>k</sub>&psi;<sub>n</sub>(k)** is purely imaginal number.
