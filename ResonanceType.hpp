#ifndef RESONANCETYPE_HPP
#define RESONANCETYPE_HPP
#include "ParticleType.hpp"
#include <string>

class ResonanceType : public ParticleType
{
private:
  /// @brief The RESONANCE WIDTH associated .
  const double fWidth;

public:
  // constructor
  ResonanceType(std::string name, double mass, int charge, double width);

  // getters

  /// @brief Returns the resonance width of the particle.
  /// @return double fWidth
  double getWidth() const { return fWidth; }

  
  /// @brief Prints in the terminal a list of the parameter relative to the particle
  void Print() const;
};

#endif
