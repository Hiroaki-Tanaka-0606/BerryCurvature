# Algorithm of the calculation of Berry curvature
By diagonalizing the Hamiltonian matrix (function of wave vector **k**), we can get eigenvectors (also function of wave vector).
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\hat{H}(k)\psi_n(k)=\varepsilon_n\psi_n(k)"></p>

Berry curvature of wave vector **&Omega;<sub>n</sub>** is defined in the following equation:
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\Omega_n(k)=\text{i}\Bigl(\nabla_k\psi_n^*(k)\Bigr)\times\Bigl(\nabla_k\psi_n(k)\Bigr)"></p>

To calculate the Berry curvature, we need to calculate the wave vector gradient of eigenvector. Defining
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\chi_n(k)=\nabla_k\psi_n(k)"></p>
The above equation is transformed to
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\Omega_n(k)=\text{i}\chi_n^*(k)\times\chi_n(k)=\begin{pmatrix}-2\text{Im}(\chi_{n,y}^*(k)\chi_{n,z}(k))&space;\\&space;-2\text{Im}(\chi_{n,z}^*(k)\chi_{n,x}(k))&space;\\&space;-2\text{Im}(\chi_{n,x}^*(k)\chi_{n,y}(k))\end{pmatrix}"></p>

To calculate the wave vector gradient of eigenvector, we use differencial approximation. For the **x** derivative (center difference) case
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\chi_{n,x}(k)=\frac{\psi_n(k&plus;e_x\Delta&space;k)-\psi_n(k-e_x\Delta&space;k)}{2\Delta&space;k}&plus;\mathcal{O}(\Delta&space;k^2)"></p>

However, Both **&psi;<sub>n</sub>(k+e<sub>x</sub>&Delta;k)** and **&psi;<sub>n</sub>(k)** can not always be obtained by diagonalization process, because they have one degree of freedom in phase factor **exp(i&theta;)**. To get the smooth function **&psi;<sub>n</sub>(k)**, we need to adjust the phase. Here we show the algorithm to adjust the phase of **k+e<sub>x</sub>&Delta;k** to **k**.

By diagonalization process, we get eigenvectors with phase difference **&psi;<sub>n</sub>(k)** and **exp(i&theta;)&psi;<sub>n</sub>(k+e<sub>x</sub>&Delta;k)**. Using Taylor expansion in the first-order,
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;e^{\text&space;i\theta}\psi_n(k&plus;e_x\Delta&space;k)=e^{\text&space;i\theta}\left(\psi_n(k)&plus;\Delta&space;k\cdot\frac{\partial}{\partial&space;x}\psi_n(k)&space;\right&space;)&plus;\mathcal{O}(\Delta&space;k^2)"></p>

Taking the norm with **&psi;<sub>n</sub>(k)**,
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;z\equiv\psi_n^*(k)e^{\text&space;i\theta}\psi_n(k&plus;e_x\Delta&space;k)=e^{\text&space;i\theta}\left(1&plus;\Delta&space;k\cdot\psi_n^*(k)\frac{\partial}{\partial&space;x}\psi_n(k)&space;\right&space;)&plus;\mathcal{O}(\Delta&space;k^2)"></p>

If we neglect the first order of **&Delta;k**, we can get the phase factor **&theta;**
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\theta=\arg&space;z&plus;\mathcal{O}(\Delta&space;k)"></p>

Since the first order of **&Delta;k** is neglected, the calculation of the wave vector gradient by differencial approximation seems to result in the **O(1)** difference. However, you can prove that it actually results in **O(&Delta;K)** difference. You can see the proof in the following section.

By orthonormalization condition of the eigenvector **&psi;<sub>n</sub>(k)**,
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\frac{\partial}{\partial&space;x}\Bigl(\psi_n^*(k)\psi_n(k)\Bigr)=\psi_n^*(k)\frac{\partial}{\partial&space;x}\psi_n(k)&plus;\left(\psi_n^*(k)\frac{\partial}{\partial&space;x}\psi_n(k)\right)^*=0"></p>

Therefore, **&psi;<sub>n</sub><sup>*</sup>(k) d/dx &psi;<sub>n</sub>(k)** is a pure imaginal number. Defining
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\text&space;i\alpha_x=\psi_n^*(k)\frac{\partial}{\partial&space;x}\psi_n(k)\&space;\&space;(\alpha_x\in\mathbb{R})"></p>

and consider the argument of **z** in the first order of **&Delta;k**,
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\arg&space;z=\theta&plus;\alpha_x\Delta&space;k&plus;\mathcal{O}(\Delta&space;k^2)"></p>

Using **arg z** as an approximate value of **&theta;**,
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\begin{align*}&space;\psi_\text{approx}(k&plus;e_x\Delta&space;k)&\equiv&space;e^{-\text&space;i\arg&space;z}e^{\text&space;i\theta}\psi_n(k&plus;e_x\Delta&space;k)\\&space;&=(1-\text&space;i\alpha_x\Delta&space;k)\left(\psi_n(k)&plus;\Delta&space;k\frac{\partial}{\partial&space;x}\psi_n(k)\right)&plus;\mathcal{O}(\Delta&space;k^2)\\&space;&=\psi_n(k)&plus;\Delta&space;k\frac{\partial}{\partial&space;x}\psi_n(k)-\text&space;i\alpha_x\Delta&space;k\psi_n(k)&plus;\mathcal{O}(\Delta&space;k^2)&space;\end{align*}"></p>

In addition, we do the same calculation as **&psi;<sub>n</sub>(k+e<sub>x</sub>&Delta;k)** to **&psi;<sub>n</sub>(k-e<sub>x</sub>&Delta;k)**:
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;z^\prime\equiv\psi_n^*(k)e^{\text&space;i\theta^\prime}\psi_n(k-e_x\Delta&space;k),\&space;\&space;\arg&space;z^\prime=\theta^\prime-\alpha_x\Delta&space;k&plus;\mathcal{O}(\Delta&space;k^2)"></p>

and use **arg z'** as an approximate value of **&theta;'**,
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\begin{align*}&space;\psi_\text{approx}(k-e_x\Delta&space;k)&\equiv&space;e^{-\text&space;i\arg&space;z^\prime}e^{\text&space;i\theta^\prime}\psi_n(k-e_x\Delta&space;k)\\&space;&=\psi_n(k)-\Delta&space;k\frac{\partial}{\partial&space;x}\psi_n(k)&plus;\text&space;i\alpha_x\Delta&space;k\psi_n(k)&plus;\mathcal{O}(\Delta&space;k^2)&space;\end{align*}"></p>

insert in the center difference formula,
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\frac{\psi_\text{approx}(k&plus;e_x\Delta&space;k)-\psi_\text{approx}(k-e_x\Delta&space;k)}{2\Delta&space;k}=\frac{\partial}{\partial&space;x}\psi_n(k)&plus;\text&space;i\alpha_x\psi_n(k)&plus;\mathcal{O}(\Delta&space;k)"></p>

Finally, put it in the definition formula of Berry curvature then we can calculate **&Omega;<sub>n,x</sub>(k)**:
<p><img src="https://latex.codecogs.com/svg.latex?\fn_cm&space;\begin{align*}&space;\Omega_{n,x}(k)&=-2\text{Im}\Bigl(\chi_{n,y}^*(k)\chi_{n,z}(k)\Bigr)\\&space;&=-2\text{Im}\left[\left(\frac{\partial}{\partial&space;y}&space;\psi_n(k)-\text&space;i\alpha_y\psi_n(k)\right&space;)^*\left(\frac{\partial}{\partial&space;z}&space;\psi_n(k)-\text&space;i\alpha_z\psi_n(k)\right&space;)\right]\\&space;&=-2\text{Im}\left[\frac{\partial}{\partial&space;y}\psi_n^*(k)\frac{\partial}{\partial&space;z}\psi_n(k)&space;&plus;\text&space;i\alpha_y\psi_n^*(k)\frac{\partial}{\partial&space;z}\psi_n(k)-\text&space;i\alpha_z\psi_n(k)\frac{\partial}{\partial&space;y}\psi_n^*(k)&plus;\alpha_y\alpha_z\psi_n^*(k)\psi_n(k)\right&space;]\\&space;&=-2\text{Im}\left[\frac{\partial}{\partial&space;y}\psi_n^*(k)\frac{\partial}{\partial&space;z}\psi_n(k)-\alpha_y\alpha_z&space;\right&space;]\\&space;&=-2\text{Im}\left(\frac{\partial}{\partial&space;y}\psi_n^*(k)\frac{\partial}{\partial&space;z}\psi_n(k)&space;\right&space;)\&space;\&space;(\because&space;\alpha_y,\&space;\alpha_z\in\mathbb{R})&space;\end{align*}"></p>

It means that **&alpha;** doesn't affect the calculation of Berry curvature **&Omega;<sub>n</sub>**. Actually, **-&\alpha;** is **Berry connection**, and it has one gauge indefinitenss. Above argument correspond to setting **&alpha;** to zero near k.
