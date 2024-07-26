# Transfer Cavity Lock: Thermal Properties
## Ivan Shalashilin and Jacob J. J. Edginton
## Last Updated: 26-07-2024

This document serves as a brief overview of the status of research on the thermal properties of the transfer cavity lock system in the Centre for Cold Matter. This work is primarily derived from John Barry's PhD thesis.

## Linear Theory
The basic theory relies on minimizing the equation:
$
\Delta L=\left(\alpha_{Q}L_{Q}+\alpha_{P}L_{P}+\alpha_{M}L_{M}-\alpha_{S}L_{S}\right)\cdot T
$
where Q,P,M,S refer the the quartz, PZT, mirror, and steel, respectively. To minimize the thermal expansion whilst also specifying a particular length of the cavity (e.g. a confocal configuration) the two simultaneous equations must be satisfied:
$
\alpha_{Q}L_{Q}+\alpha_{P}L_{P}+\alpha_{M}L_{M}	-\alpha_{S}L_{S}=0
L_{Q}+L_{P}+L_{M}-L_{S}	=L_{\text{cavity}}.
$
and 
$
\alpha_{Q}L_{Q}+\alpha_{P}L_{P}+\alpha_{M}L_{M}	-\alpha_{S}L_{S}=0
L_{Q}+L_{P}+L_{M}-L_{S}	=L_{\text{cavity}}.
$
Two equations with two unknowns: $L_{Q},L_{S}$.

## Nonlinear Theory
The equation $\Delta L=\alpha LT$ is a linear approximation to the full equation $L(T)=L_{0}e^{\alpha T}$. The same thermal constraint cannot be applied to the full version of the equation. Nonlinear contributions in this set-up are estimated to be on the order of $\Delta L/L\sim10^{-10}$.

## Machining Tolerance
The below table gives the approximate tolerances and thermal expansivities of the materials used.

|     Component    | Expansivity $$(10^{-6}\mathrm{K^{-1}})$$ | Approx. Tolerance $$(\mathrm{\mu m})$$ |
|:----------------:|:------------------------------------:|:----------------------------------:|
|    Quartz Tube   |                  0.4                 |                 50                 |
|    1020 Steel    |                 11.7                 |                 25                 |
| Optics (mirrors) |                 0.57                 |                 6.4                |
|       Piezo      |                  -1                  |                 50                 |

## Time-Dependence
It was found when using a step-change temperature the time for each material of the cavity became important. In the above equations T had to be replaced by T_{app}(t)=T_{max}\left(1-e^{-t/\tau_{H}}\right). A definitive model was not established. See [Opt. Express 23, 5134-5146 (2015)](https://doi.org/10.1364/OE.23.005134) and [J. Opt. Soc. Am. B 27, 914-919 (2010)](https://doi.org/10.1364/JOSAB.27.000914) for information on the methodology. We suggest a finite-element analysis is required to conclusively determine the time-temperature dependence