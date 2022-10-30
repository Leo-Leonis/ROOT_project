#include "Particle.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <cassert>
#include <stdexcept>

/// @brief Computes and returns a vectorial sum with another vector.
/// @param v1 first vector
/// @param v2 seconde vector
/// @return the vectorial sum of the two vectors.
vector3d ComputeSum(vector3d const &v1, vector3d const &v2) {
  return vector3d(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

/// @brief A static integer counter that gives the
/// current number of types of particles.
int Particle::fNParticleTypes = 0;

/// @brief A static vector of ParticleType pointers. Each
/// pointer points to a different type of particle.
std::vector<ParticleType *> Particle::fParticleType{};

int Particle::FindParticle(std::string part_name) {
  int i = 0;
  for (ParticleType* part_ptr : fParticleType) {
    if (part_name == part_ptr->GetName()) {
      return i;
      break;
    }
  }
  return -1;
}

Particle::Particle(std::string part_name, double Px, double Py, double Pz)
    : fImpulse(Px, Py, Pz) {
  int part_index = FindParticle(part_name);
  if (part_index == -1) {
    std::cout << "BREAKING NEWS: FOUND NEW PARTICLE!!!" << '\n';
  } else fIndex = part_index;
}

void Particle::AddParticleType(std::string part_name, double mass, int charge) {
  if (FindParticle(part_name) == -1 &&
      fNParticleTypes != fMaxNumParticleTypes) {
    fNParticleTypes++;
    ParticleType *toBeAddedPart = new ParticleType(part_name, mass, charge);
    fParticleType.push_back(toBeAddedPart);
  }
}

void Particle::AddParticleType(std::string part_name, double mass, int charge,
                               double width) {
  if (FindParticle(part_name) == -1 &&
      fNParticleTypes != fMaxNumParticleTypes + 1) {
    fNParticleTypes++;
    ParticleType *toBeAddedPart =
        new ResonanceType(part_name, mass, charge, width);
    fParticleType.push_back(toBeAddedPart);
  } else if (fNParticleTypes == fMaxNumParticleTypes + 1) {
    throw std::runtime_error("ERROR: maximum number of particles reached.");
  }
}

void Particle::SetIndex(int index) {
  // index avaibility check
  if (index > fNParticleTypes) {
    std::cout << "Particle index" << index << "not found." << '\n'
              << "Current particle types = " << fNParticleTypes << '\n';
    return;
  } else {
    fIndex = index; // index setting
  }
}

void Particle::SetIndex(std::string part_name) {
  int index = FindParticle(part_name);
  if (index == -1) { // index avaibility checking
    std::cout << "Particle index" << index << "not found." << '\n'
              << "Current particle types = " << fNParticleTypes << '\n';
    return;
  } else {
    fIndex = index; // index setting
  }
}

void Particle::PrintAllParticleTypes() {
  int i = 0;
  std::cout << "Printing the list of every parameteres of all particle types:"
            << '\n'
            << '\n';
  for (ParticleType *part_type_ptr : fParticleType) {
    std::cout << "PARTICLE INDEX: " << i << '\n';
    part_type_ptr->Print();
    i++;
  }
}

void Particle::PrintParticleData() const {
  std::cout << "Printing the data associated to this particle:" << '\n';
  std::cout << "  || Part. index: " << fIndex << '\n';
  std::cout << "  ||  Part. name: " << fParticleType[fIndex]->GetName() << '\n';
  std::cout << "  || Impulse:  x: " << fImpulse.x << " GeV" << '\n'
            << "  ||           y: " << fImpulse.y << " GeV" << '\n'
            << "  ||           z: " << fImpulse.z << " GeV" << '\n';
}

double Particle::GetEnergy() const {
  return std::sqrt(std::pow(fParticleType[fIndex]->GetMass(), 2) +
                   std::pow(fImpulse.module(), 2));
}

double Particle::InvMass(Particle &p) const {
  return std::sqrt(
      std::pow(this->GetEnergy() + p.GetEnergy(), 2) -
      std::pow((ComputeSum(fImpulse, p.GetImpulse())).module(), 2));
}

void Particle::SetP(double px, double py, double pz) {
  fImpulse.x = px;
  fImpulse.y = py;
  fImpulse.z = pz;
}