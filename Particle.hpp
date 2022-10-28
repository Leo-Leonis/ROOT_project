#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "ParticleType.hpp"
#include "ResonanceType.hpp"
#include <cmath>
#include <vector>
#include <string>

/// @brief A type representing a 3d vector.
class vector3d
{
public:
  double x;
  double y;
  double z;

  /// @brief Constructor of vector3d.
  vector3d(double i, double j, double k) : x{i}, y{j}, z{k} {}

  /// @brief Computes and returns the module of the vector.
  double module() { return std::sqrt(x * x + y * y + z * z); }
};

/// @brief It describes the type of the particle and holds
/// the cinematic parameters of the particle.
class Particle
{
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
  vector3d const fImpulse;

  /// @brief This function searches for the corresponding
  /// number type of the particle through all the types present
  /// in fParticleType and returns the index as an integer.
  /// @param part_name Name of the particle
  /// @return The integer corrisponding to the particle type.
  int static FindParticle(std::string part_name);

public:
  /// @brief Constructor of class Particle.
  /// @param part_name The name type of the particle
  /// @param Px The x component of the impulse
  /// @param Py The y component of the impulse
  /// @param Pz The z component of the impulse
  Particle(std::string part_name, double Px = 0., double Py = 0., double Pz = 0.);

  /// @brief Returns the index corresponding to the type
  /// of the particle.
  /// @return int fIndex
  int GetIndex() const { return fIndex; }

  /// @brief This function add particles types to fParticleType.
  /// @param part_name The name of the new particle
  /// @param mass The mass of the new particle
  /// @param charge The charge of the new particle
  /// @param width The resonance width of the new particle (if it exists)
  static void AddParticleType(std::string part_name, double mass, int charge, double width);
};

#endif