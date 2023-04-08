\sethlcolor{blue}
\begin{minted}[
frame=lines,
framesep=2mm,
baselinestretch=0.9,
bgcolor=bg,
%fontsize=\small,
fontfamily=tt,
linenos,
escapeinside=||,
]{cpp}
#include "ResonanceType.hpp"
#include <iostream>

ResonanceType::ResonanceType(std::string name, double mass, int charge,
                             double width)
    : ParticleType(name, mass, charge), fWidth{width} {}

void ResonanceType::Print() const {
  ParticleType::Print();
  std::cout << "  | width: " << fWidth << " GeV/c2" << '\n';
}
\end{minted}