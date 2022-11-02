#include "Particle.hpp"
#include "ParticleType.hpp"
#include "ResonanceType.hpp"

#include "Rtypes.h"
#include "TMath.h"
#include "TRandom.h"

R__LOAD_LIBRARY(Particle_cpp.so)
R__LOAD_LIBRARY(ResonanceType_cpp.so)
R__LOAD_LIBRARY(ParticleType_cpp.so)

int main() {
  gRandom->SetSeed();
  Particle::AddParticleType("pi+", 0.13957, 1);
  Particle::AddParticleType("pi-", 0.13957, -1);
  Particle::AddParticleType("K+", 0.49367, 1);
  Particle::AddParticleType("K-", 0.49367, -1);
  Particle::AddParticleType("p+", 0.93827, 1);
  Particle::AddParticleType("p-", 0.93827, -1);
  Particle::AddParticleType("K*", 0.89166, 0, 0.050);

  int const NParticles = 120;      // number of initial particles
  int const NFinalParticles = 120; // size of array
  Particle EventParticles[NFinalParticles];

  for (int n_ev = 0; n_ev != 10E5; n_ev++) {
    // In each event
    for (int i = 0; i != NParticles; i++) {
      // For each particle generated
      double phi = gRandom->Uniform(0., TMath::TwoPi()); // azimut angle
      double theta = gRandom->Uniform(0., TMath::Pi());  // polar angle
      double imp_mod = gRandom->Exp(1);                  // impulse module

      EventParticles[i].SetP(imp_mod * TMath::Sin(theta) * TMath::Cos(phi),
                             imp_mod * TMath::Sin(theta) * TMath::Sin(phi),
                             imp_mod * TMath::Cos(theta));

      double n_judge = gRandom->Rndm();

      // judgement of particle
      if (n_judge < 0.40) {
        EventParticles[i].SetIndex(0);
      } else if (n_judge < 0.80 && n_judge >= 0.40) {
        EventParticles[i].SetIndex(1);
      } else if (n_judge < 0.85 && n_judge >= 0.80) {
        EventParticles[i].SetIndex(2);
      } else if (n_judge < 0.90 && n_judge >= 0.85) {
        EventParticles[i].SetIndex(3);
      } else if (n_judge < 0.945 && n_judge >= 0.90) {
        EventParticles[i].SetIndex(4);
      } else if (n_judge < 0.99 && n_judge >= 0.945) {
        EventParticles[i].SetIndex(5);
      } else {
        EventParticles[i].SetIndex(6);
      }

      if (EventParticles[i].GetIndex() == 6) {
        if (EventParticles[i].Decay2Body(EventParticles[NParticles + 1],
                                     EventParticles[NParticles + 2]) != 0)
          {
            
          }

      }
    }
  }

  return 0;
}
