#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"

#include <iostream>

/// @brief Check the expected entries of an histogram with an accepted range,
/// and prints out the results.
/// @param histo the histogram pointer
/// @param expEntries the number of expected entries
/// @param range the maximum accepted distance of the number of entries from the
/// expected one
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

void analysis() {
  TFile *resultFile = new TFile("result.root");

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
  check_entries(hInvMassControl, 1E5, 2000);

	
}