\sethlcolor{blue}
\begin{minted}[
frame=lines,
framesep=2mm,
baselinestretch=0.9,
bgcolor=bg,
%fontsize=\small,
fontfamily=tt,
linenos,
escapeinside=||,
]{cpp}
#include "Particle.hpp"
#include "ParticleType.hpp"
#include "ResonanceType.hpp"

#include "TCanvas.h"
#include "TFile.h"
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

void program() {
  gRandom->SetSeed();
  TFile *resultFile = new TFile("result.root", "RECREATE");

  Particle::AddParticleType("pi+", 0.13957, 1);       // index 0
  Particle::AddParticleType("pi-", 0.13957, -1);      // index 1
  Particle::AddParticleType("K+", 0.49367, 1);        // index 2
  Particle::AddParticleType("K-", 0.49367, -1);       // index 3
  Particle::AddParticleType("p+", 0.93827, 1);        // index 4
  Particle::AddParticleType("p-", 0.93827, -1);       // index 5
  Particle::AddParticleType("K*", 0.89166, 0, 0.050); // index 6

  int const nEvents = 1E5;       // total number of events
  int const NParticles = 100;    // number of initial particles
  int const NMaxParticles = 120; // max number of particles allowed per event
  Particle EventParticles[NMaxParticles]; // main array of generated particles

  TH1I *hIndex = new TH1I("hIndex", "Particle type distribution", 7, 0, 7);
  TH1D *hPhi =
      new TH1D("hPhi", "Azimutal angle distribution", 500, 0, TMath::TwoPi());
  TH1D *hTheta =
      new TH1D("hTheta", "Polar angle distribution", 500, 0, TMath::Pi());
  TH1D *hImpModule =
      new TH1D("hImpModule", "Impulse module distribution", 100, 0, 6);
  TH1D *hTransvImpModule = new TH1D(
      "hTransvImpModule", "Transverse impulse module distribution", 100, 0, 6);
  TH1D *hEnergy =
      new TH1D("hEnergy", "Particle energy distribution", 100, 0, 5);

  TH1D *hTotInvMass = new TH1D(
      "hTotInvMass", "Invariant Mass of all particles distribution", 100, 0, 3);
  hTotInvMass->Sumw2();
  TH1D *hInvMass = new TH1D(
      "hInvMass", "Invariant Mass of same charge distribution", 100, 0, 3);
  hInvMass->Sumw2();
  TH1D *hInvMassOpp = new TH1D(
      "hInvMassOpp", "Invariant Mass of oppos. charge distr.", 100, 0, 3);
  hInvMassOpp->Sumw2();
  TH1D *hInvMassKpi = new TH1D(
      "hInvMassKpi", "Invariant Mass of same charge between K and pi distr.",
      100, 0, 3);
  hInvMassKpi->Sumw2();
  TH1D *hInvMassOppKpi = new TH1D(
      "hInvMassOppKpi",
      "Invariant Mass of oppos. charge between K and pi distr.", 100, 0, 3);
  hInvMassOppKpi->Sumw2();
  TH1D *hInvMassControl = new TH1D(
      "hInvMassControl", "Invariant Mass control distribution", 100, 0, 3);
  hInvMassControl->Sumw2();

  TCanvas *canvas =
      new TCanvas("c1", "General purpose data histograms", 1080, 720);
  TCanvas *canvasInvMass =
      new TCanvas("c2", "Invariant mass histograms", 1080, 720);

  // In each event...
  for (int n_ev = 0; n_ev != nEvents; n_ev++) {

    int n_decays = 0; // number of decays of the current event

    // clear all the particles generated by K* in the previous event
    for (int i = NParticles; i != NMaxParticles; i++) {
      EventParticles[i] = Particle();
    }

    // For each particle generated...
    for (int i = 0; i != NParticles; i++) {

      // assign the cinematic parameters
      double phi = gRandom->Uniform(0., TMath::TwoPi()); // Azimut angle
      double theta = gRandom->Uniform(0., TMath::Pi());  // Polar angle
      double imp_mod = gRandom->Exp(1);                  // Impulse module

      // set the impulse vector
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

        // number that decides the fate of the generated particles
        double random_n = gRandom->Rndm();

        if (random_n < 0.5) {
          // pi+ and K-
          EventParticles[NParticles + (n_decays * 2)].SetIndex(0);
          EventParticles[NParticles + (n_decays * 2) + 1].SetIndex(3);
        } else {
          // pi- and K+
          EventParticles[NParticles + (n_decays * 2)].SetIndex(1);
          EventParticles[NParticles + (n_decays * 2) + 1].SetIndex(2);
        }

        // decay the particle and check the result
        if (EventParticles[i].Decay2Body(
                EventParticles[NParticles + (n_decays * 2)],
                EventParticles[NParticles + (n_decays * 2) + 1]) != 0) {
          throw std::runtime_error("Decay of a particle K* was unsuccessful.");
        }

        n_decays++;

      } // end K* if-statement

      hIndex->Fill(EventParticles[i].GetIndex());
      hPhi->Fill(phi);
      hTheta->Fill(theta);
      hImpModule->Fill(imp_mod);
      hTransvImpModule->Fill(TMath::Sin(theta) * imp_mod);
      hEnergy->Fill(EventParticles[i].GetEnergy());
    } // end particle generations loop per event

    // current number of K* in EventParticles[]
    int nKstars = 0;

    // invariant mass calculations
    for (int part_n = 0; part_n != NParticles + n_decays; part_n++) {

      int curr_indx =
          EventParticles[part_n].GetIndex(); // current particle index
      int curr_charge =
          EventParticles[part_n].GetCharge(); // current particle charge

      // if the particle is a K*, fill the control histogram
      if (curr_indx == 6) {
        hInvMassControl->Fill(
            EventParticles[NParticles + (nKstars * 2)].InvMass(
                EventParticles[NParticles + (nKstars * 2) + 1]));
        nKstars++;
      } else { // in any other particle...
        for (int sub_i = 0; sub_i != part_n; sub_i++) {

          int tobe_indx =
              EventParticles[sub_i].GetIndex(); // to be compared particle index
          int tobe_charge = EventParticles[sub_i]
                                .GetCharge(); // to be compared particle charge

          // if the particle TO BE COMPARED is a K*, skip
          if (tobe_indx == 6) {
          } else // otherwise
          {
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
  hTransvImpModule->Draw();
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
  hInvMassControl->Fit("gaus", "S, Q"); // fit the control histogram
  hInvMassControl->Draw();


  resultFile->Write();
}

\end{minted}