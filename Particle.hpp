#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "ParticleType.hpp"
#include "ResonanceType.hpp"
#include <cmath>
#include <string>
#include <vector>

/// @brief A type representing a 3d vector.
class Vector3d {
public:
  double x;
  double y;
  double z;

  /// @brief Default constructor of Vector3d.
  Vector3d() {};

  /// @brief Constructor of Vector3d.
  Vector3d(double i, double j, double k) : x{i}, y{j}, z{k} {}

  /// @brief Computes and returns the module of the vector.
  double GetModule() const { return std::sqrt(x * x + y * y + z * z); }
};

/// @brief It describes the type of the particle and holds
/// the cinematic properties of the particle.
class Particle {
private:
  /// @brief A static vector of ParticleType pointers. Each
  /// pointer points to a different type of particle.
  static std::vector<ParticleType *> fParticleType;

  /// @brief The static value indicates the max number
  /// of types of particle allowed.
  static const int fMaxNumParticleTypes = 10;

  /// @brief A static integer counter that gives the
  /// current number of types of particles.
  static int fNParticleTypes;

  /// @brief The integer that indicates the type of the
  /// relative particle.
  int fIndex;

  /// @brief A 3d vector corresponding to the impulse vector.
  Vector3d fImpulse;

  /// @brief This function searches for the corresponding
  /// number type of the particle through all the types present
  /// in fParticleType and returns the index as an integer.
  /// @param part_name Name of the particle
  /// @return The integer corrisponding to the particle type.
  int static FindParticle(std::string part_name);

  /// @brief Function used in Decay2Body to simulate a Lorentz transfomation
  /// @param bx Component x
  /// @param by Component y
  /// @param bz Component z
  void Boost(double bx, double by, double bz);

public:
  /////////////////////////////////////////////
  /// @brief Default constructor of class Particle.
  ///////////////// ////////////////////////////
  Particle() {};

  /////////////////////////////////////////////
  /// @brief Constructor of class Particle.
  /// @param part_name The name type of the particle
  /// @param Px The x component of the impulse
  /// @param Py The y component of the impulse
  /// @param Pz The z component of the impulse
  ///////////////// ////////////////////////////
  Particle(std::string part_name, double Px, double Py, double Pz);

  /// @brief This function add particles types to fParticleType.
  /// @param part_name The name of the new particle
  /// @param mass The mass of the new particle
  /// @param charge The charge of the new particle
  static void AddParticleType(std::string part_name, double mass, int charge);

  /// @brief This function add particles types to fParticleType.
  /// @param part_name The name of the new particle
  /// @param mass The mass of the new particle
  /// @param charge The charge of the new particle
  /// @param width The resonance width of the new particle (if it exists)
  static void AddParticleType(std::string part_name, double mass, int charge,
                              double width);

  /// @brief Checks and sets fIndex as the value in input.
  /// @param index The particle type index to be set
  void SetIndex(int index);

  /// @brief Checks and sets fIndex as the particle type's name as in input
  /// @param index The particle type name to be set
  void SetIndex(std::string part_name);

  /// @brief Prints all the data of all types of particles found.
  static void PrintAllParticleTypes();

  /// @brief Prints all the data of the current particle.
  void PrintParticleData() const;

  int GetIndex() const { return fIndex; }
  double GetxImpulse() const { return fImpulse.x; }
  double GetyImpulse() const { return fImpulse.y; }
  double GetzImpulse() const { return fImpulse.z; }
  Vector3d GetImpulse() const { return fImpulse; }
  double GetMass() const { return fParticleType[fIndex]->GetMass(); }
  double GetCharge() const { return fParticleType[fIndex]->GetCharge(); }

  /// @brief Computes and returns the energy stored in the particle.
  /// @return A double containing the energy of the particle.
  double GetEnergy() const;

  /// @brief Computes and returns the invariant mass between two particles.
  /// @param p The second particle to be compared to
  /// @return A double containing the invariant mass.
  double InvMass(Particle &p) const;

  /// @brief Manually sets the impulse vector.
  /// @param px The x component of the new impulse
  /// @param py The y component of the new impulse
  /// @param pz The z component of the new impulse
  void SetP(double px, double py, double pz);

  /// @brief This function simulates through a random generator the cinematics
  /// of the decay of the particle into other two.
  /// @param dau1 The first particle it decays into
  /// @param dau2 The seconde particle it decays into
  /// @return Returns 0 if it was successful, any other value if
  /// not successful.
  int Decay2Body(Particle &dau1, Particle &dau2) const;
};

#endif