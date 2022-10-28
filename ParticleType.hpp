#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP

#include <string>

class ParticleType
{
private:
  /// @brief The NAME of the particle associated.
  const std::string fName;
  /// @brief the MASS of the particle associated.
  const double fMass;
  /// @brief The Charge of the particle associated.
  const int fCharge;

public:
  // constructor
  ParticleType(std::string name, double mass, int charge);

  // getters
  std::string GetName() const { return fName; }
  double GetMass() const { return fMass; }
  int GetCharge() const { return fCharge; }

  // misc
  virtual void Print() const;
};

#endif 