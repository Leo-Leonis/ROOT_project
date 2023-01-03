#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"

void analysis() {
  TFile *resultFile = new TFile("result.root");

	TH1I *hIndex = (TH1I*)resultFile->Get("hIndex");
	TH1D *hPhi = (TH1D*)resultFile->Get("hPhi");
	TH1D *hTheta = (TH1D*)resultFile->Get("hTheta");
	TH1D *hImpModule = (TH1D*)resultFile->Get("hImpModule");
	TH1D *hTransvImpModule = (TH1D*)resultFile->Get("hTransvImpModule");
}