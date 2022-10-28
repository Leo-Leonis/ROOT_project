#include "Particle.hpp"
#include <algorithm>
#include <iostream>

int Particle::fNParticleTypes = 0;

int Particle::FindParticle(std::string part_name)
{
  for (int i = 0; i != fParticleType.size(); i++)
  {
    if (part_name == fParticleType[i]->GetName())
    {
      return i;
      break;
    }
  }
}

Particle::Particle(std::string part_name, double Px = 0., double Py = 0., double Pz = 0.)
    : fImpulse(Px, Py, Pz)
{
  int temp_ind = FindParticle(part_name);
  if (temp_ind != fNParticleTypes)
  {
    fNParticleTypes++;
    std::cout << "BREAKING NEWS: FOUND NEW PARTICLE!!!" << '\n';
  }
  fIndex = FindParticle(part_name);
}

void Particle::AddParticleType(std::string part_name, double mass, int charge, double width)
{
  if (FindParticle(part_name) == fNParticleTypes)
  {
    fParticleType.push_back(&ParticleType(part_name, mass, charge));
  }
}