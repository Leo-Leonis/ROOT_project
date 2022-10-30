#include "ResonanceType.hpp"
#include <iostream>

ResonanceType::ResonanceType(std::string name, double mass, int charge, double width)
    : ParticleType(name, mass, charge), fWidth{width}
{
}

void ResonanceType::Print() const
{
  ParticleType::Print();
  std::cout << "  | width: " << fWidth << " GeV/c2" << '\n';
}