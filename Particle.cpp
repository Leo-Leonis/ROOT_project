#include "Particle.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>

/// @brief Computes and returns a vectorial sum with another vector.
/// @param v1 first vector
/// @param v2 seconde vector
/// @return the vectorial sum of the two vectors.
Vector3d ComputeSum(Vector3d const &v1, Vector3d const &v2) {
  return Vector3d(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

/// @brief A static integer counter that gives the
/// current number of types of particles.
int Particle::fNParticleTypes = 0;

/// @brief A static vector of ParticleType pointers. Each
/// pointer points to a different type of particle.
std::vector<ParticleType *> Particle::fParticleType{};

int Particle::FindParticle(std::string part_name) {
  int i = 0;
  for (ParticleType *part_ptr : fParticleType) {
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
  } else
    fIndex = part_index;
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
                   std::pow(fImpulse.GetModule(), 2));
}

double Particle::InvMass(Particle &p) const {
  return std::sqrt(
      std::pow(this->GetEnergy() + p.GetEnergy(), 2) -
      std::pow((ComputeSum(fImpulse, p.GetImpulse())).GetModule(), 2));
}

void Particle::SetP(double px, double py, double pz) {
  fImpulse.x = px;
  fImpulse.y = py;
  fImpulse.z = pz;
}

int Particle::Decay2Body(Particle &dau1, Particle &dau2) const {
  if (GetMass() == 0.0) {
    printf("Decay cannot be performed if mass is zero\n");
    return 1;
  }

  double massMot = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();

  if (fIndex > -1) { // add width effect

    // gaussian random numbers

    float x1, x2, w, y1, y2;

    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;
    y2 = x2 * w;

    massMot += fParticleType[fIndex]->GetWidth() * y1;
  }

  if (massMot < massDau1 + massDau2) {
    printf("Decayment cannot be preformed because mass is too low in this "
           "channel\n");
    return 2;
  }

  double pout =
      sqrt(
          (massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2)) *
          (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2))) /
      massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.SetP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi),
            pout * cos(theta));
  dau2.SetP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi),
            -pout * cos(theta));

  double energy = sqrt(std::pow(fImpulse.x, 2) + std::pow(fImpulse.y, 2) +
                       std::pow(fImpulse.z, 2) + massMot * massMot);

  double bx = fImpulse.x / energy;
  double by = fImpulse.y / energy;
  double bz = fImpulse.z / energy;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  return 0;
}

void Particle::Boost(double bx, double by, double bz) {

  double energy = GetEnergy();

  // Boost this Lorentz vector
  double b2 = bx * bx + by * by + bz * bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx * fImpulse.x + by * fImpulse.y + bz * fImpulse.z;
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  fImpulse.x += gamma2 * bp * bx + gamma * bx * energy;
  fImpulse.y += gamma2 * bp * by + gamma * by * energy;
  fImpulse.z += gamma2 * bp * bz + gamma * bz * energy;
}