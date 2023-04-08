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
#ifndef RESONANCETYPE_HPP
#define RESONANCETYPE_HPP
#include "ParticleType.hpp"
#include <string>

class ResonanceType : public ParticleType {
private:
  // The RESONANCE WIDTH associated.
  const double fWidth;

public:
  // Constructor of class ResonanceType.
  ResonanceType(std::string name, double mass, int charge, double width);

  double GetWidth() const { return fWidth; }

  // Prints in the terminal a list of the parameter relative to the
  // particle
  void Print() const;
};

#endif

\end{minted}