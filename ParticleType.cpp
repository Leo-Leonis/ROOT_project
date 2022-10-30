#include "ParticleType.hpp"
#include <iostream>

ParticleType::ParticleType(std::string name, double mass, int charge)
    : fName{name}, fMass{mass}, fCharge{charge} {}

void ParticleType::Print() const {
  std::cout << "  |  name: " << fName << '\n';
  std::cout << "  |  mass: " << fMass << " GeV/c2" << '\n';
  std::cout << "  |charge: " << fCharge << '\n';
}