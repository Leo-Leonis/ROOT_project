#include "Particle.hpp"
#include "ParticleType.hpp"
#include "ResonanceType.hpp"
#include <iostream>
#include <vector>
//#include <algorithm>

int main() {
  /* ParticleType * test = new ParticleType("e", 1., -1); */
  Particle::AddParticleType("e-", 1., -1);
  Particle::AddParticleType("e+", 1., 1);
  Particle::AddParticleType("pi+", 100., +1);
  Particle::AddParticleType("K*", 0.89166, 0, 0.050);
  Particle::PrintAllParticleTypes();
  Particle *savage = new Particle("e-", 1., -0.5, 0.7);
  Particle *love = new Particle("e+", 3., -0.5, 0.7);
  Particle *intruder = new Particle("K+", 2.2, 3.5, -12.);
  savage->PrintParticleData();
  love->PrintParticleData();
  std::cout << "energy: " << savage->GetEnergy() << '\n';
  std::cout << "inv. mass. 0 and 1: " << savage->InvMass(*love) << '\n';
  std::cout << "index: " << savage->GetIndex() << '\n';
}
