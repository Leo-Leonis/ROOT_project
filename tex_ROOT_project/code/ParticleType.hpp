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
#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP

#include <string>

class ParticleType {
private:
  // The NAME of the particle associated.
  const std::string fName;
  // The MASS of the particle associated.
  const double fMass;
  // The CHARGE of the particle associated.
  const int fCharge;

public:
  // The constructor of class ParticleType.
  ParticleType(std::string name, double mass, int charge);

  std::string GetName() const { return fName; }
  double GetMass() const { return fMass; }
  int GetCharge() const { return fCharge; }
  virtual double GetWidth() const { return 0.; }

  // Prints in the terminal a list of the parameter relative to the
  // particle type
  virtual void Print() const;
};

#endif
\end{minted}