#include "Particle.hpp"
#include "ParticleType.hpp"
#include "ResonanceType.hpp"

#include "TCanvas.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TList.h"
#include "TMath.h"
#include "TRandom.h"

#include <iostream>
#include <stdexcept>

R__LOAD_LIBRARY(ParticleType_cpp.so)
R__LOAD_LIBRARY(ResonanceType_cpp.so)
R__LOAD_LIBRARY(Particle_cpp.so)

// int main () {
void program() {
  gRandom->SetSeed();

  Particle::AddParticleType("pi+", 0.13957, 1);                     // index 0
  Particle::AddParticleType("pi-", 0.13957, -1);                    // index 1
  Particle::AddParticleType("K+", 0.49367, 1);                      // index 2
  Particle::AddParticleType("K-", 0.49367, -1);                     // index 3
  Particle::AddParticleType("p+", 0.93827, 1); // index 4
  Particle::AddParticleType("p-", 0.93827, -1);                     // index 5
  Particle::AddParticleType("K*", 0.89166, 0, 0.050);               // index 6

  int const nEvents = 1E5;                // total number of events
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

  TH1D *hTotInvMass = new TH1D(
      "htotInvMass", "Invariant Mass of all particles distribution", 100, 0, 3);
  TH1D *hInvMass = new TH1D(
      "hinvMass", "Invariant Mass of same charge distribution", 100, 0, 3);
  TH1D *hInvMassOpp = new TH1D(
      "hinvMassOpp", "Invariant Mass of oppos. charge distr.", 100, 0, 3);
  TH1D *hInvMassKpi = new TH1D(
      "hinvMassKpi", "Invariant Mass of same charge between K and pi distr.",
      100, 0, 3);
  TH1D *hInvMassOppKpi = new TH1D(
      "hinvMassOppKpi",
      "Invariant Mass of oppos. charge between K and pi distr.", 100, 0, 3);
  TH1D *hInvMassControl = new TH1D(
      "hinvMassControl", "Invariant Mass control distribution", 100, 0, 2);

  TCanvas *canvas = new TCanvas("c1", "c1", 1080, 720);
  TCanvas *canvasInvMass = new TCanvas("c2", "c2", 1080, 720);

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
                EventParticles[NParticles + (n_decays * 2) + 1],
                EventParticles[NParticles + (n_decays * 2) + 2]) != 0) {
          throw std::runtime_error("Decay of a particle K* was unsuccessful.");
        } else {
          double random_n = gRandom->Rndm();
          if (random_n < 0.5) {
            // pi+ and K-
            EventParticles[NParticles + (n_decays * 2) + 1].SetIndex(0);
            EventParticles[NParticles + (n_decays * 2) + 2].SetIndex(3);
          } else {
            // pi- and K+
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
    // TODO: correggere il range del loop utilizzando il contatore di particelle
    // generate.
    for (int part_n = 0; part_n != NParticles + n_decays; part_n++) {

      // skip the fist iteration
      if (part_n == 0) {
        continue;
      }

      int curr_indx =
          EventParticles[part_n].GetIndex(); // current particle index
      int curr_charge =
          EventParticles[part_n].GetCharge(); // current particle charge

      // if the particle is a K*, fill the control histogram, otherwise skip
      if (curr_indx == 6) {
        hInvMassControl->Fill(
            EventParticles[NParticles + (nKstars * 2) + 1].InvMass(
                EventParticles[NParticles + (nKstars * 2) + 2]));
        nKstars++;
      } else {
        for (int sub_i = 0; sub_i != part_n; sub_i++) {

          int tobe_indx =
              EventParticles[sub_i].GetIndex(); // to be compared particle index
          int tobe_charge = EventParticles[sub_i]
                                .GetCharge(); // to be compared particle charge

          // if the particle TO BE COMPARED is a K*, skip...
          if (tobe_indx == 6) {
            continue;
          }
          // ...otherwise

          // fill hTotInvMass
          hTotInvMass->Fill(
              EventParticles[part_n].InvMass(EventParticles[sub_i]));

          // IF same charge fill InvMass, otherwise fill InvMassOpp
          if (curr_charge * tobe_charge == 1) {
            hInvMass->Fill(
                EventParticles[part_n].InvMass(EventParticles[sub_i]));
          } else {
            hInvMassOpp->Fill(
                EventParticles[part_n].InvMass(EventParticles[sub_i]));
          }

          // IF K and pi
          if (((curr_indx == 0 || curr_indx == 1) &&
               (tobe_indx == 2 || tobe_indx == 3)) ||
              ((curr_indx == 2 || curr_indx == 3) &&
               (tobe_indx == 0 || tobe_indx == 1))) {
            // IF same charge fill InvMassKpi, otherwise fill InvMassOppKpi
            if (curr_charge * tobe_charge == 1) {
              hInvMassKpi->Fill(
                  EventParticles[part_n].InvMass(EventParticles[sub_i]));
            } else {
              hInvMassOppKpi->Fill(
                  EventParticles[part_n].InvMass(EventParticles[sub_i]));
            }
          }
        }
      }
    } // end invariant mass calculation

    // print number of progression in order to give feedback
    std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
                 "\b\b\b\b\b\b\b\b\b\b\b\b"
              << "events generated: " << n_ev << '(' << 100 * n_ev / nEvents
              << "%)";
  } // end event generations loop

  std::cout << '\n';
  canvas->Divide(2, 3);
  canvasInvMass->Divide(2, 3);

  canvas->cd(1);
  hIndex->Draw();
  canvas->cd(2);
  hPhi->Draw();
  canvas->cd(3);
  hTheta->Draw();
  canvas->cd(4);
  hImpModule->Draw();
  canvas->cd(5);
  hTrasvImpModule->Draw();
  canvas->cd(6);
  hEnergy->Draw();

  canvasInvMass->cd(1);
  hTotInvMass->Draw();
  canvasInvMass->cd(2);
  hInvMass->Draw();
  canvasInvMass->cd(3);
  hInvMassOpp->Draw();
  canvasInvMass->cd(4);
  hInvMassKpi->Draw();
  canvasInvMass->cd(5);
  hInvMassOppKpi->Draw();
  canvasInvMass->cd(6);
  hInvMassControl->Draw();

  // return 0;
}
