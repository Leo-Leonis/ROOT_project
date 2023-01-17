#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TMath.h"

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
/// @param range the accepted error range
void check_bin_entries(TH1 *histo, int bin, int expEntries, int range) {
  int entries = histo->GetBinContent(bin);
  std::cout << "INFO: checking " << histo->GetName() << ", bin " << bin
            << "... ";
  if (entries >= expEntries - range && entries <= expEntries + range) {
    std::cout << "correct! (" << entries << ')' << '\n';
  } else {
    std::cout << "value out of range! (" << entries
              << "; expected: " << expEntries << ')' << '\n';
  }
}

void analysis() {
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
  check_entries(hInvMassKpi, 41000000, 1E5);
  TH1D *hInvMassOppKpi = (TH1D *)resultFile->Get("hInvMassOppKpi");
  check_entries(hInvMassOppKpi, 41000000, 1E5);
  TH1D *hInvMassControl = (TH1D *)resultFile->Get("hInvMassControl");
  check_entries(hInvMassControl, 1E5, 1E3);

  std::cout << "\n\n";

  std::cout << "CHECKING PARTICLE TYPES GENERATION------------------------\n";
  check_bin_entries(hIndex, 1, 4E6, 1E5);    // pi+ (40%)
  check_bin_entries(hIndex, 2, 4E6, 1E5);    // pi- (40%)
  check_bin_entries(hIndex, 3, 5E5, 1E3);    // K+ (5%)
  check_bin_entries(hIndex, 4, 5E5, 1E3);    // K- (5%)
  check_bin_entries(hIndex, 5, 450000, 1E3); // p+ (4.5%)
  check_bin_entries(hIndex, 6, 450000, 1E3); // p- (4.5%)
  check_bin_entries(hIndex, 7, 1E5, 1E3);    // K* (1%)

  std::cout << "\n\n";

  std::cout << "FITTING AND CHECKING ANGLE DISTRIBUTIONS------------------\n";

  TF1 *unifPhi = new TF1("unifPhi", "pol 0", 0, TMath::TwoPi());
  hPhi->Fit(unifPhi, "S, 0, Q"); // "0" for no plotting, "Q" for quiet mode
  TF1 *unifTheta = new TF1("unifTheta", "pol 0", 0, TMath::Pi());
  hTheta->Fit(unifTheta, "S, 0, Q");

  TF1 *fitPhiRes = hPhi->GetFunction("unifPhi");
  std::cout << "INFO: Phi fit results..." << '\n';
  std::cout << "           Value: " << fitPhiRes->GetParameter(0) << " ± "
            << fitPhiRes->GetParError(0) << '\n'
            << "       Chi^2/NDF: "
            << fitPhiRes->GetChisquare() / fitPhiRes->GetNDF() << '\n'
            << "     Probability: " << fitPhiRes->GetProb() << '\n'
            << '\n';

  TF1 *fitThetaRes = hTheta->GetFunction("unifTheta");
  std::cout << "INFO: Theta fit results..." << '\n';
  std::cout << "           Value: " << fitThetaRes->GetParameter(0) << " ± "
            << fitThetaRes->GetParError(0) << '\n'
            << "       Chi^2/NDF: "
            << fitThetaRes->GetChisquare() / fitThetaRes->GetNDF() << '\n'
            << "     Probability: " << fitThetaRes->GetProb() << '\n';

  std::cout << "\n\n";

  std::cout << "FITTING AND CHECKING IMPULSE DISTRIBUTION-----------------\n";

  TF1 *expImpMod = new TF1("expImpMod", "expo", 0, 6);
  hImpModule->Fit(expImpMod, "S, 0, Q");
  TF1 *fitImpModRes = hImpModule->GetFunction("expImpMod");

  std::cout << "INFO: Impulse module fit results..." << '\n';
  std::cout << "           Value: " << -fitImpModRes->GetParameter(1) << " ± "
            << fitImpModRes->GetParError(1) << '\n'
            << "       Chi^2/NDF: "
            << fitImpModRes->GetChisquare() / fitImpModRes->GetNDF() << '\n'
            << "     Probability: " << fitImpModRes->GetProb() << '\n';

  std::cout << "\n\n";

  std::cout << "CHECKING INVARIANT MASS HISTOGRAMS------------------------\n";

  TH1F *hSubAll = new TH1F(
      "hSubAll", "Difference between opposite charge particles from same ones",
      100, 0, 3);
  TH1F *hSubKPi = new TH1F(
      "hSubKPi",
      "Difference between opposite charge Pi and K particles from same ones",
      100, 0, 3);
  hSubAll->Add(hInvMassOpp, hInvMass, 1, -1);
  hSubKPi->Add(hInvMassOppKpi, hInvMassKpi, 1, -1);

  TCanvas *canvas = new TCanvas();
  canvas->Divide(1, 3);
  canvas->cd(1);
  hSubAll->Draw();
  canvas->cd(2);
  hSubKPi->Draw();
  canvas->cd(3);
  hInvMassControl->Draw();

}