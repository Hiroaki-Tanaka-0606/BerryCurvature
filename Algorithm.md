# Algorithm of the calculation of Berry curvature
By diagonalizing the Hamiltonian matrix (function of wave vector **k**), we can get eigenvectors (also function of wave vector).
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\hat{H}(k)\psi_n(k)=\varepsilon_n\psi_n(k)"></p>

Berry curvature of wave vector **&Omega;<sub>n</sub>** is defined in the following equation:
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\Omega_n(k)=\text{i}\Bigl(\nabla_k\psi_n^*(k)\Bigr)\times\Bigl(\nabla_k\psi_n(k)\Bigr)"></p>

To calculate the Berry curvature, we need to calculate the wave vector gradient of eigenvector. Defining
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\chi_n(k)=\nabla_k\psi_n(k)"></p>
The above equation is transformed to
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\Omega_n(k)=\text{i}\chi_n^*(k)\times\chi_n(k)=\begin{pmatrix}-2\text{Im}(\chi_{n,y}^*(k)\chi_{n,z}(k))&space;\\&space;-2\text{Im}(\chi_{n,z}^*(k)\chi_{n,x}(k))&space;\\&space;-2\text{Im}(\chi_{n,x}^*(k)\chi_{n,y}(k))\end{pmatrix}"></p>

To calculate the wave vector gradient of eigenvector, we use differencial approximation. For the **x** derivative case
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\chi_{n,x}(k)=\frac{\psi_n(k&plus;e_x\Delta&space;k)-\psi_n(k)}{\Delta&space;k}&plus;\mathcal{O}(\Delta&space;k)"></p>

However, Both **&psi;<sub>n</sub>(k+e<sub>x</sub>&Delta;k)** and **&psi;<sub>n</sub>(k)** can not always be obtained by diagonalization process, because they have one degree of freedom in phase factor **exp(i&theta;)**. To get the smooth function **&psi;<sub>n</sub>(k)**, we need to adjust the phase. Here we show the algorithm to adjust the phase of **k+e<sub>x</sub>&Delta;k** to **k**.

By diagonalization process, we get eigenvectors with phase difference **&psi;<sub>n</sub>(k)** and **exp(i&theta;)&psi;<sub>n</sub>(k+e<sub>x</sub>&Delta;k)**. Using Taylor expansion in the first-order,
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;e^{\text&space;i\theta}\psi_n(k&plus;e_x\Delta&space;k)=e^{\text&space;i\theta}\left(\psi_n(k)&plus;\Delta&space;k\cdot\frac{\partial}{\partial&space;x}\psi_n(k)&space;\right&space;)&plus;\mathcal{O}(\Delta&space;k^2)"></p>

Taking the norm with **&psi;<sub>n</sub>(k)**,
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;z\equiv\psi_n^*(k)e^{\text&space;i\theta}\psi_n(k&plus;e_x\Delta&space;k)=e^{\text&space;i\theta}\left(1&plus;\Delta&space;k\cdot\psi_n(k)\frac{\partial}{\partial&space;x}\psi_n(k)&space;\right&space;)&plus;\mathcal{O}(\Delta&space;k^2)"></p>

If we neglect the first order of **&Delta;k**, we can get the phase factor **&theta;**
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\theta=\arg&space;z&plus;\mathcal{O}(\Delta&space;k)"></p>

However, since the first order of **&Delta;k** is neglected, the calculation of the wave vector gradient by differencial approximation result in the **O(1)** difference. To calculate the differenccial approximation in **O(&Delta;k)**, we need to get the phase factor **&theta;** with difference of **O(&Delta;k<sup>2</sup>)**.

By orthonormalization condition of the eigenvector **&psi;<sub>n</sub>(k)**,
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\frac{\partial}{\partial&space;x}\Bigl(\psi_n^*(k)\psi_n(k)\Bigr)=\psi_n^*(k)\frac{\partial}{\partial&space;x}\psi_n(k)&plus;\left(\psi_n^*(k)\frac{\partial}{\partial&space;x}\psi_n(k)\right)^*=0"></p>

Therefore, **&psi;<sub>n</sub><sup>*</sup>(k) d/dx &psi;<sub>n</sub>(k)** is a pure imaginal number. Defining
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\text&space;i\alpha=\psi_n^*(k)\frac{\partial}{\partial&space;x}\psi_n(k)\&space;\&space;(\alpha\in\mathbb{R})"></p>

and consider the argument of **z** in the first order of **&Delta;k**,
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\arg&space;z=\theta&plus;\alpha\Delta&space;k&plus;\mathcal{O}(\Delta&space;k^2)"></p>

In addition, we do the same calculation as **&psi;<sub>n</sub>(k+e<sub>x</sub>&Delta;k)** to **&psi;<sub>n</sub>(k-e<sub>x</sub>&Delta;k)**:
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;z^\prime\equiv\psi_n^*(k)e^{\text&space;i\theta^\prime}\psi_n(k-e_x\Delta&space;k),\&space;\&space;\arg&space;z^\prime=\theta^\prime-\alpha\Delta&space;k&plus;\mathcal{O}(\Delta&space;k^2)"></p>

We can also calculate the differential approximation from **&psi;<sub>n</sub>(k-e<sub>x</sub>&Delta;k)** and **&psi;<sub>n</sub>(k)**
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\frac{\psi_n(k-e_x\Delta&space;k)-\psi_n(k)}{-\Delta&space;k}=\frac{\psi_n(k&plus;e_x\Delta&space;k)-\psi_n(k)}{\Delta&space;k}&plus;\mathcal{O}(\Delta&space;k)"></p>
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\therefore&space;\psi_n(k&plus;e_x\Delta&space;k)&plus;\psi_n(k-e_x\Delta&space;k)=2\psi_n(k)&plus;\mathcal{O}(\Delta&space;k^2)"></p>

From these above equations **&theta;+&theta;'=arg z+arg z'+O(&Delta; k<sup>2</sup>)**, then
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\begin{align*}&space;\psi_n(k&plus;e_x\Delta&space;k)\psi_n(k-e_x\Delta&space;k)&=e^{-\text&space;i(\theta&plus;\theta^\prime)}\Bigl(e^{\text&space;i\theta}\psi_n(k&plus;e_x\Delta&space;k)\Bigr)\Bigl(e^{\text&space;i\theta^\prime}\psi_n(k-e_x\Delta&space;k)\Bigr)\\&space;&=e^{-\text&space;i(\arg&space;z&plus;\arg&space;z^\prime)}\Bigl(e^{\text&space;i\theta}\psi_n(k&plus;e_x\Delta&space;k)\Bigr)\Bigl(e^{\text&space;i\theta^\prime}\psi_n(k-e_x\Delta&space;k)\Bigr)&plus;\mathcal{O}(\Delta&space;k^2)\\&space;&=\left(\frac{zz^\prime}{|z||z^\prime|}\right)^{-1}\Bigl(e^{\text&space;i\theta}\psi_n(k&plus;e_x\Delta&space;k)\Bigr)\Bigl(e^{\text&space;i\theta^\prime}\psi_n(k-e_x\Delta&space;k)\Bigr)&plus;\mathcal{O}(\Delta&space;k^2)&space;\end{align*}"></p>
