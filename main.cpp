#include "Particle.hpp"
#include "ParticleType.hpp"
#include "ResonanceType.hpp"

#include "TCanvas.h"
#include "TH1D.h"
#include "TList.h"
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

  int const nEvents = 10E5;               // total number of events
  int const NParticles = 100;             // number of initial particles
  int const NMaxParticles = 120;          // max number of particles
  Particle EventParticles[NMaxParticles]; // array of generated particles

  TH1I *hIndex = new TH1I("hpartTypes", "Particle type distribution", 7, 0, 7);
  TH1D *hPhi =
      new TH1D("hphi", "Azimutal angle distribution", 100, 0, TMath::TwoPi());
  TH1D *hTheta =
      new TH1D("htheta", "Polar angle distribution", 100, 0, TMath::Pi());
  TH1D *hImpModule =
      new TH1D("himpModule", "Impulse module distribution", 100, 0, 6);
  TH1D *hTrasvImpModule = new TH1D(
      "htrasvImpModule", "Trasverse impulse module distribution", 100, 0, 6);
  TH1D *hEnergy =
      new TH1D("henergy", "Particle energy distribution", 100, 1, 5);

  TList *listInvMass = new TList();
  TH1D *hTotInvMass = new TH1D(
      "htotInvMass", "Invariant Mass of all particles distribution", 100, 0, 3);
  listInvMass->Add(hTotInvMass);
  TH1D *hInvMass = new TH1D(
      "hinvMass", "Invariant Mass of same charge distribution", 100, 0, 3);
  listInvMass->Add(hInvMass);
  TH1D *hInvMassOpp = new TH1D(
      "hinvMassOpp", "Invariant Mass of oppos. charge distr.", 100, 0, 3);
  listInvMass->Add(hInvMassOpp);
  TH1D *hInvMassKpi = new TH1D(
      "hinvMassKpi", "Invariant Mass of same charge between K and pi distr.",
      100, 0, 3);
  listInvMass->Add(hInvMassKpi);
  TH1D *hInvMassOppKpi = new TH1D(
      "hinvMassOppKpi",
      "Invariant Mass of oppos. charge between K and pi distr.", 100, 0, 3);
  listInvMass->Add(hInvMassOppKpi);
  TH1D *hInvMassControl = new TH1D(
      "hinvMassControl", "Invariant Mass control distribution", 100, 0, 2);
  listInvMass->Add(hInvMassControl);

  TCanvas *maicanvas = new TCanvas();

  // In each event...
  for (int n_ev = 0; n_ev != nEvents; n_ev++) {

    int n_decays = 0; // number of decays per event

    // clear resonance generated particles
    for (int i = NParticles; i != NMaxParticles; i++) {
      EventParticles[i] = Particle();
    }

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
      } // end K* if-statement
      hIndex->Fill(EventParticles[i].GetIndex());
      hPhi->Fill(phi);
      hTheta->Fill(theta);
      hImpModule->Fill(imp_mod);
      hTrasvImpModule->Fill(TMath::Sin(theta) * imp_mod);
      hEnergy->Fill(EventParticles[i].GetEnergy());
    } // end particle generations loop per event

    // current number of the particle in EventParticles[]
    int part_n = 0;

    // current number of K* in EventParticles[]
    int nKstars = 0;

    // invariant mass calculations
    for (Particle p : EventParticles) {

      // skip the fist iteration
      if (part_n = 0) {
        part_n++;
        continue;
      }

      // if the particle is a K*, fill the controll histogram, otherwise skip
      if (p.GetIndex() == 6) {
        hInvMassControl->Fill(EventParticles[NParticles + nKstars + 1].InvMass(
            EventParticles[NParticles + nKstars + 2]));
        nKstars += 2;
      } else {
        for (int sub_i = 0; sub_i != part_n; sub_i++) {

          // if the particle TO BE COMPARED is a K*, skip...
          if (EventParticles[sub_i].GetIndex() == 6) {
            continue;
          }
          // ...otherwise
          
          // fill hTotInvMass
          hTotInvMass->Fill(p.InvMass(EventParticles[sub_i]));

          // IF same charge fill InvMass, otherwise fill InvMassOpp
          if (p.GetCharge() * EventParticles[sub_i].GetCharge() == 1) {
            hInvMass->Fill(p.InvMass(EventParticles[sub_i]));
          } else {
            hInvMassOpp->Fill(p.InvMass(EventParticles[sub_i]));
          }

          // IF K and pi
          if (((p.GetIndex() == 0 || p.GetIndex() == 1) &&
               (EventParticles[sub_i].GetIndex() == 2 ||
                EventParticles[sub_i].GetIndex() == 3)) ||
              ((p.GetIndex() == 2 || p.GetIndex() == 3) &&
               (EventParticles[sub_i].GetIndex() == 0 ||
                EventParticles[sub_i].GetIndex() == 1))) {
            // IF same charge fill InvMassKpi, otherwise fill InvMassOppKpi
            if (p.GetCharge() * EventParticles[sub_i].GetCharge() == 1) {
              hInvMassKpi->Fill(p.InvMass(EventParticles[sub_i]));
            } else {
              hInvMassOppKpi->Fill(p.InvMass(EventParticles[sub_i]));
            }
          }
        }
      }
      // next particle
      part_n++;
    } // end invariant mass calculation
  }   // end event generatiorns loop
  return 0;
}
