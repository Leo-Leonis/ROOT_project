#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TMath.h"
#include "TStyle.h"

#include <iostream>

/// @brief Check the entries of an histogram with an accepted range,
/// and prints out the results.
/// @param histo the histogram pointer
/// @param expEntries the number of expected entries
/// @param range the accepted error range
void check_entries(TH1 *histo, int expEntries, int range) {
  int entries = histo->GetEntries(); // number of entries

  if (range == 0) {
    if (entries == expEntries) {
      std::cout << "INFO: " << histo->GetName()
                << " has the correct number of entries (" << entries << ")\n";
    } else {
      std::cout << "WARNING: " << histo->GetName()
                << " does NOT have the correct number of entries (entries: "
                << entries << "; expected: " << expEntries << ")\n";
    }
  } else {
    if (entries >= expEntries - range && entries <= expEntries + range) {
      std::cout << "INFO: " << histo->GetName()
                << " has the correct number of entries (" << entries
                << "; expected: " << expEntries << ")\n";
    } else {
      std::cout << "WARNING: " << histo->GetName()
                << "'s entries are out of expected range (entries: " << entries
                << "; expected: " << expEntries << ", range: " << range
                << ")\n";
    }
  }
}

/// @brief Function that checks the number of entries of a specified bin.
/// @param histo the histogram pointer
/// @param bin the number associated to the bin
/// @param expEntries the number of expected entries
void check_bin_entries(TH1 *histo, int bin, int expEntries) {
  int entries = histo->GetBinContent(bin);
  std::cout << "INFO: checking " << histo->GetName() << ", bin " << bin << ": "
            << entries << " entries (expected: " << expEntries << ')' << '\n';
}

void analysis() {

  gStyle->SetOptStat(2200);
  gStyle->SetOptFit(1111);

  TFile *resultFile = new TFile("result.root");

  std::cout << '\n';
  std::cout << "CHECKING ENTRIES-------------------------------------\n";
  TH1I *hIndex = (TH1I *)resultFile->Get("hIndex");
  check_entries(hIndex, 1E7, 0);
  TH1D *hPhi = (TH1D *)resultFile->Get("hPhi");
  check_entries(hPhi, 1E7, 0);
  TH1D *hTheta = (TH1D *)resultFile->Get("hTheta");
  check_entries(hTheta, 1E7, 0);
  TH1D *hImpModule = (TH1D *)resultFile->Get("hImpModule");
  check_entries(hImpModule, 1E7, 0);
  TH1D *hTransvImpModule = (TH1D *)resultFile->Get("hTransvImpModule");
  check_entries(hTransvImpModule, 1E7, 0);
  TH1D *hEnergy = (TH1D *)resultFile->Get("hEnergy");
  check_entries(hEnergy, 1E7, 0);
  TH1D *hTotInvMass = (TH1D *)resultFile->Get("hTotInvMass");
  check_entries(hTotInvMass, 495000000, 1E5);
  TH1D *hInvMass = (TH1D *)resultFile->Get("hInvMass");
  check_entries(hInvMass, 247500000, 1E5);
  TH1D *hInvMassOpp = (TH1D *)resultFile->Get("hInvMassOpp");
  check_entries(hInvMassOpp, 247500000, 1E5);
  TH1D *hInvMassKpi = (TH1D *)resultFile->Get("hInvMassKpi");
  check_entries(hInvMassKpi, 41000000, 150000);
  TH1D *hInvMassOppKpi = (TH1D *)resultFile->Get("hInvMassOppKpi");
  check_entries(hInvMassOppKpi, 41000000, 150000);
  TH1D *hInvMassControl = (TH1D *)resultFile->Get("hInvMassControl");
  check_entries(hInvMassControl, 1E5, 1E3);

  std::cout << "\n\n";

  /////////////////////////////////1/4//////////////////////////////////////////
  std::cout << "CHECKING PARTICLE TYPES GENERATION------------------------\n";
  check_bin_entries(hIndex, 1, 4E6);    // pi+ (40%)
  check_bin_entries(hIndex, 2, 4E6);    // pi- (40%)
  check_bin_entries(hIndex, 3, 5E5);    // K+ (5%)
  check_bin_entries(hIndex, 4, 5E5);    // K- (5%)
  check_bin_entries(hIndex, 5, 450000); // p+ (4.5%)
  check_bin_entries(hIndex, 6, 450000); // p- (4.5%)
  check_bin_entries(hIndex, 7, 1E5);    // K* (1%)

  std::cout << "\n\n";

  ///////////////////////////////2/4//////////////////////////////////////////
  std::cout << "FITTING AND CHECKING ANGLE DISTRIBUTIONS------------------\n";

  TCanvas *canvas1 = new TCanvas("c1", "Canvas 1", 1440, 720);
  canvas1->Divide(2, 2);
  canvas1->cd(1);
  hIndex->GetXaxis()->SetTitle("Particle Type");
  hIndex->GetYaxis()->SetTitle("Entries");
  hIndex->Draw("H");
  hIndex->Draw("E, P, SAME");
  canvas1->cd(2);

  TF1 *fPhi = new TF1("fPhi", "pol 0", 0, TMath::TwoPi());
  hPhi->Fit(fPhi, "S, Q");
  TF1 *fTheta = new TF1("fTheta", "pol 0", 0, TMath::Pi());
  hTheta->Fit(fTheta, "S, Q");

  TF1 *fitPhiRes = hPhi->GetFunction("fPhi");
  std::cout << "INFO: Phi fit results..." << '\n';
  std::cout << "           Value: " << fitPhiRes->GetParameter(0) << " ± "
            << fitPhiRes->GetParError(0) << '\n'
            << "           Chi^2: " << fitPhiRes->GetChisquare() << '\n'
            << "             NDF: " << fitPhiRes->GetNDF() << '\n'
            << "       Chi^2/NDF: "
            << fitPhiRes->GetChisquare() / fitPhiRes->GetNDF() << '\n'
            << "     Probability: " << fitPhiRes->GetProb() << '\n'
            << '\n';

  TF1 *fitThetaRes = hTheta->GetFunction("fTheta");
  std::cout << "INFO: Theta fit results..." << '\n';
  std::cout << "           Value: " << fitThetaRes->GetParameter(0) << " ± "
            << fitThetaRes->GetParError(0) << '\n'
            << "           Chi^2: " << fitThetaRes->GetChisquare() << '\n'
            << "             NDF: " << fitThetaRes->GetNDF() << '\n'
            << "       Chi^2/NDF: "
            << fitThetaRes->GetChisquare() / fitThetaRes->GetNDF() << '\n'
            << "     Probability: " << fitThetaRes->GetProb() << '\n';

  std::cout << "\n\n";

  hPhi->GetXaxis()->SetTitle("Angle [rad]");
  hPhi->GetYaxis()->SetTitle("Entries");
  hPhi->Draw("H");
  hPhi->Draw("E, P, SAME");

  canvas1->cd(3);
  hTheta->GetXaxis()->SetTitle("Angle [rad]");
  hTheta->GetYaxis()->SetTitle("Entries");
  hTheta->Draw("H");
  hTheta->Draw("E, P, SAME");

  ///////////////////////////////3/4//////////////////////////////////////////
  std::cout << "FITTING AND CHECKING IMPULSE DISTRIBUTION-----------------\n";

  canvas1->cd(4);
  TF1 *fImpModule = new TF1("fImpModule", "expo", 0, 6);
  hImpModule->Fit(fImpModule, "S, Q");

  TF1 *fitImpModuleRes = hImpModule->GetFunction("fImpModule");

  std::cout << "INFO: Impulse module fit results..." << '\n';
  std::cout << "           Value: " << -fitImpModuleRes->GetParameter(1)
            << " ± " << fitImpModuleRes->GetParError(1) << '\n'
            << "           Chi^2: " << fitImpModuleRes->GetChisquare() << '\n'
            << "             NDF: " << fitImpModuleRes->GetNDF() << '\n'
            << "       Chi^2/NDF: "
            << fitImpModuleRes->GetChisquare() / fitImpModuleRes->GetNDF()
            << '\n'
            << "     Probability: " << fitImpModuleRes->GetProb() << '\n';

  std::cout << "\n\n";

  hImpModule->GetXaxis()->SetTitle("Impulse [GeV]");
  hImpModule->GetYaxis()->SetTitle("Entries");
  hImpModule->Draw("H");
  hImpModule->Draw("E, P, SAME");

  canvas1->Print("canvas1.png");
  canvas1->Print("canvas1.C");
  canvas1->Print("canvas1.root");

  ///////////////////////////////4/4//////////////////////////////////////////
  std::cout << "CHECKING INVARIANT MASS HISTOGRAMS------------------------\n";

  TCanvas *canvas2 =
      new TCanvas("c2", "Difference invariant mass histograms", 720, 720);
  canvas2->Divide(1, 3);
  canvas2->cd(1);

  TH1F *hSubAll = new TH1F("hSubAll",
                           "Difference between opposite charge particles from "
                           "same ones - Invariant mass distribuition",
                           100, 0, 3);
  TH1F *hSubKpi =
      new TH1F("hSubKPi",
               "Difference between opposite charge of pi and K particles from "
               "same ones - Invariant mass distribuition",
               100, 0, 3);

  hSubAll->Add(hInvMassOpp, hInvMass, 1, -1);
  hSubKpi->Add(hInvMassOppKpi, hInvMassKpi, 1, -1);

  TF1 *fSubAll = new TF1("fSubAll", "gaus", 0, 3);
  hSubAll->Fit(fSubAll, "S, Q", "", 0.5, 3);
  TF1 *fSubKpi = new TF1("fSubKpi", "gaus", 0, 3);
  hSubKpi->Fit(fSubKpi, "S, Q", "", 0.5, 3);

  TF1 *fitSubAllRes = hSubAll->GetFunction("fSubAll");
  std::cout << "INFO: Invariant Mass K* fit results (all particles)..." << '\n';
  std::cout << "  K* Mass (mean): " << fitSubAllRes->GetParameter(1) << " ± "
            << fitSubAllRes->GetParError(1) << '\n'
            << "   K* Width (SD): " << fitSubAllRes->GetParameter(2) << " ± "
            << fitSubAllRes->GetParError(2) << '\n'
            << "      gaus width: " << fitSubAllRes->GetParameter(0) << " ± "
            << fitSubAllRes->GetParError(0) << '\n'
            << "           Chi^2: " << fitSubAllRes->GetChisquare() << '\n'
            << "             NDF: " << fitSubAllRes->GetNDF() << '\n'
            << "       Chi^2/NDF: "
            << fitSubAllRes->GetChisquare() / fitSubAllRes->GetNDF() << '\n'
            << "     Probability: " << fitSubAllRes->GetProb() << '\n'
            << '\n';

  TF1 *fitSubKpiRes = hSubKpi->GetFunction("fSubKpi");
  std::cout << "INFO: Invariant Mass K* fit results (K and pi particles)..."
            << '\n';
  std::cout << "  K* Mass (mean): " << fitSubKpiRes->GetParameter(1) << " ± "
            << fitSubKpiRes->GetParError(1) << '\n'
            << "   K* Width (SD): " << fitSubKpiRes->GetParameter(2) << " ± "
            << fitSubKpiRes->GetParError(2) << '\n'
            << "      gaus width: " << fitSubKpiRes->GetParameter(0) << " ± "
            << fitSubKpiRes->GetParError(0) << '\n'
            << "           Chi^2: " << fitSubKpiRes->GetChisquare() << '\n'
            << "             NDF: " << fitSubKpiRes->GetNDF() << '\n'
            << "       Chi^2/NDF: "
            << fitSubKpiRes->GetChisquare() / fitSubKpiRes->GetNDF() << '\n'
            << "     Probability: " << fitSubKpiRes->GetProb() << '\n'
            << '\n';

  canvas2->cd(1);
  hSubAll->GetXaxis()->SetTitle("Invariant Mass [GeV/c^2]");
  hSubAll->GetYaxis()->SetTitle("Entries");
  hSubAll->Draw("H");
  hSubAll->Draw("E, P, SAME");

  canvas2->cd(2);
  hSubKpi->GetXaxis()->SetTitle("Invariant Mass [GeV/c^2]");
  hSubKpi->GetYaxis()->SetTitle("Entries");
  hSubKpi->Draw("H");
  hSubKpi->Draw("E, P, SAME");

  canvas2->cd(3);
  hInvMassControl->GetXaxis()->SetTitle("Invariant Mass [GeV/c^2]");
  hInvMassControl->GetYaxis()->SetTitle("Entries");
  hInvMassControl->Draw("H");
  hInvMassControl->Draw("E, P, SAME");

  canvas2->Print("canvas2.png");
  canvas2->Print("canvas2.C");
  canvas2->Print("canvas2.root");
}