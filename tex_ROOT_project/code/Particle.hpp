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
#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "ParticleType.hpp"
#include "ResonanceType.hpp"
#include <cmath>
#include <string>
#include <vector>

// A type representing a 3d vector.
class Vector3d {
public:
  double x;
  double y;
  double z;

  // Default constructor of Vector3d.
  Vector3d() {};

  // Constructor of Vector3d.
  Vector3d(double i, double j, double k) : x{i}, y{j}, z{k} {}

  // Computes and returns the module of the vector.
  double GetModule() const { return std::sqrt(x * x + y * y + z * z); }
};

// It describes the type of the particle and holds
// the cinematic properties of the particle.
class Particle {
private:
  // A static vector of ParticleType pointers. Each
  // pointer points to a different type of particle.
  static std::vector<ParticleType *> fParticleType;

  // The static value indicates the max number
  // of types of particle allowed.
  static const int fMaxNumParticleTypes = 10;

  // A static integer counter that gives the
  // current number of types of particles.
  static int fNParticleTypes;

  // The integer that indicates the type of the
  //relative particle.
  int fIndex;

  // A 3d vector corresponding to the impulse vector.
  Vector3d fImpulse;

  // This function searches for the corresponding
  // number type of the particle through all the types present
  // in fParticleType and returns the index as an integer.
  int static FindParticle(std::string part_name);

  // Function used in Decay2Body to simulate a Lorentz transfomation
  void Boost(double bx, double by, double bz);

public:
  // Default constructor of class Particle.
  Particle() {};

  // Constructor of class Particle.
  Particle(std::string part_name, double Px, double Py, double Pz);

  // Adds particles types to fParticleType.
  static void AddParticleType(std::string part_name, double mass, int charge);

  // Adds particles types to fParticleType.
  static void AddParticleType(std::string part_name, double mass, int charge,
                              double width);

  // Checks and sets fIndex as the value in input.
  void SetIndex(int index);

  // Checks and sets fIndex as the particle type's name as in input
  void SetIndex(std::string part_name);

  // Prints all the data of all types of particles found.
  static void PrintAllParticleTypes();

  // Prints all the data of the current particle.
  void PrintParticleData() const;

  int GetIndex() const { return fIndex; }
  double GetxImpulse() const { return fImpulse.x; }
  double GetyImpulse() const { return fImpulse.y; }
  double GetzImpulse() const { return fImpulse.z; }
  Vector3d GetImpulse() const { return fImpulse; }
  double GetMass() const { return fParticleType[fIndex]->GetMass(); }
  double GetCharge() const { return fParticleType[fIndex]->GetCharge(); }

  // Computes and returns the energy stored in the particle.
  double GetEnergy() const;

  // Computes and returns the invariant mass between two particles.
  double InvMass(Particle &p) const;

  // Manually sets the impulse vector.
  void SetP(double px, double py, double pz);

  // Simulates through a random generator the cinematics of the decay 
  // of the particle into other two.
  int Decay2Body(Particle &dau1, Particle &dau2) const;
};

#endif
\end{minted}