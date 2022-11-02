#ifndef RESONANCETYPE_HPP
#define RESONANCETYPE_HPP
#include "ParticleType.hpp"
#include <string>

class ResonanceType : public ParticleType {
private:
  /// @brief The RESONANCE WIDTH associated .
  const double fWidth;

public:
  /////////////////////////////////////////////
  /// @brief Constructor of class ResonanceType.
  /// @param name The particle type name
  /// @param mass The mass of the particle
  /// @param charge The charge of the particle
  /// @param width The resonance width of the particle
  /////////////////////////////////////////////
  ResonanceType(std::string name, double mass, int charge, double width);

  double GetWidth() const { return fWidth; }

  /// @brief Prints in the terminal a list of the parameter relative to the
  /// particle
  void Print() const;
};

#endif
