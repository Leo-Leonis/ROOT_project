#include "Particle.hpp"
#include "ParticleType.hpp"
#include "ResonanceType.hpp"

#include "TH1D.h"
#include "TMath.h"
#include "TRandom.h"

#include <stdexcept>

R__LOAD_LIBRARY(Particle_cpp.so)
R__LOAD_LIBRARY(ResonanceType_cpp.so)
R__LOAD_LIBRARY(ParticleType_cpp.so)

int main() {
  gRandom->SetSeed();
  Particle::AddParticleType("pi+", 0.13957, 1);       // index 0
  Particle::AddParticleType("pi-", 0.13957, -1);      // index 1
  Particle::AddParticleType("K+", 0.49367, 1);        // index 2
  Particle::AddParticleType("K-", 0.49367, -1);       // index 3
  Particle::AddParticleType("p+", 0.93827, 1);        // index 4
  Particle::AddParticleType("p-", 0.93827, -1);       // index 5
  Particle::AddParticleType("K*", 0.89166, 0, 0.050); // index 6

  int const nEvents = 10E5;      // total number of events
  int const NParticles = 100;    // number of initial particles
  int const NMaxParticles = 120; // max number of particles
  Particle EventParticles[NMaxParticles];

  TH1D *hIndex = new TH1D("partTypes", "Particle type distribution", 7, 0, 6);
  TH1D *hPhi =
      new TH1D("hphi", "Azimutal angle distribution", 100, 0, TMath::TwoPi());
  TH1D *hTheta =
      new TH1D("htheta", "Polar angle distribution", 100, 0, TMath::Pi());
  TH1D *hModule = new TH1D("hmodule", "Impulse module distribution", 100, 0, 1);
  /* TODO: implementare anche gli istrogrammi di:

    - impulso trasverso
    - energia particelle
    - massa invariante (tutti i casi)
  */

  // In each event...
  for (int n_ev = 0; n_ev != nEvents; n_ev++) {

    int n_decays = 0; // number of decays per event

    // For each particle generated...
    for (int i = 0; i != NParticles; i++) {

      double phi = gRandom->Uniform(0., TMath::TwoPi()); // Azimut angle
      double theta = gRandom->Uniform(0., TMath::Pi());  // Polar angle
      double imp_mod = gRandom->Exp(1);                  // Impulse module

      // Assign a random impulse vector
      EventParticles[i].SetP(imp_mod * TMath::Sin(theta) * TMath::Cos(phi),
                             imp_mod * TMath::Sin(theta) * TMath::Sin(phi),
                             imp_mod * TMath::Cos(theta));

      // Generate a random number...
      double n_judge = gRandom->Rndm();
      // ...which decides the type of the particle
      if (n_judge < 0.40) {
        EventParticles[i].SetIndex(0); // pi+
      } else if (n_judge >= 0.40 && n_judge < 0.80) {
        EventParticles[i].SetIndex(1); // pi-
      } else if (n_judge >= 0.80 && n_judge < 0.85) {
        EventParticles[i].SetIndex(2); // K+
      } else if (n_judge >= 0.85 && n_judge < 0.90) {
        EventParticles[i].SetIndex(3); // K-
      } else if (n_judge >= 0.90 && n_judge < .945) {
        EventParticles[i].SetIndex(4); // p+
      } else if (n_judge >= .945 && n_judge < 0.99) {
        EventParticles[i].SetIndex(5); // p-
      } else {
        EventParticles[i].SetIndex(6); // K*
        if (EventParticles[i].Decay2Body(
                EventParticles[NParticles + n_decays + 1],
                EventParticles[NParticles + n_decays + 2]) != 0) {
          throw std::runtime_error("Decay of a particle was unsuccessful.");
        } else {
          double random_n = gRandom->Rndm();
          if (random_n < 0.5) {
            EventParticles[NParticles + (n_decays * 2) + 1].SetIndex(0);
            EventParticles[NParticles + (n_decays * 2) + 2].SetIndex(3);
          } else {
            EventParticles[NParticles + (n_decays * 2) + 1].SetIndex(1);
            EventParticles[NParticles + (n_decays * 2) + 2].SetIndex(2);
          }
          n_decays++;
        }
      }
    }
  }

  return 0;
}
