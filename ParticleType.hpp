#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP

#include <string>

class ParticleType {
private:
  /// @brief The NAME of the particle associated.
  const std::string fName;
  /// @brief the MASS of the particle associated.
  const double fMass;
  /// @brief The CHARGE of the particle associated.
  const int fCharge;

public:
  /////////////////////////////////////////////
  /// @brief The constructor of class ParticleType.
  /// @param name The name of the particle type
  /// @param mass The mass of the particle type
  /// @param charge The charge of the particle type
  /////////////////////////////////////////////
  ParticleType(std::string name, double mass, int charge);

  std::string GetName() const { return fName; }
  double GetMass() const { return fMass; }
  int GetCharge() const { return fCharge; }
  virtual double GetWidth() const { return 0.; }

  /// @brief Prints in the terminal a list of the parameter relative to the
  /// particle type
  virtual void Print() const;
};

#endif