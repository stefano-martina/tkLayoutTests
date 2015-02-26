void comparePixel(string oneFileName, string twoFileName) {
  TFile* oneFile = TFile::Open(oneFileName.c_str());
  TFile* twoFile = TFile::Open(twoFileName.c_str());

  TCanvas* oneCanvas = (TCanvas*) oneFile->Get("matTrack001");
  TCanvas* twoCanvas = (TCanvas*) twoFile->Get("matTrack001");
  TCanvas* materialBudgetComparisonCanvas =
     new TCanvas("comparisonCanvas", "Material Budget comparison", 600, 600);
  materialBudgetComparisonCanvas->Divide(2);
  TGraph* radComparison = new TGraph();
  TGraph* intComparison = new TGraph();
  radComparison->SetTitle(Form("Radiation length comparison;%s;%s", oneFileName.c_str(), twoFileName.c_str()));
  intComparison->SetTitle(Form("Interaction length comparison;%s;%s", oneFileName.c_str(), twoFileName.c_str()));

  TPad* onePad1 = (TPad*) oneCanvas->GetPrimitive("materialInTrackingVolumepixel_1");
  TPad* twoPad1 = (TPad*) twoCanvas->GetPrimitive("materialInTrackingVolumepixel_1");

  TProfile* oneProfile1 = (TProfile*) onePad1->GetPrimitive("rglobal");
  TProfile* twoProfile1 = (TProfile*) twoPad1->GetPrimitive("rglobal");
  std::cerr << "nbins first = " << oneProfile1->GetXaxis()->GetNbins() << std::endl;
  std::cerr << "nbins second = " << twoProfile1->GetXaxis()->GetNbins() << std::endl;
  if (oneProfile1->GetXaxis()->GetNbins() == twoProfile1->GetXaxis()->GetNbins()) {
     for (int i=1; i<=oneProfile1->GetXaxis()->GetNbins(); ++i) {
       radComparison->SetPoint(radComparison->GetN(),
                               oneProfile1->GetBinContent(i),
                               twoProfile1->GetBinContent(i));
     }
     if (radComparison->GetYaxis()->GetXmax() > radComparison->GetXaxis()->GetXmax()) {
       radComparison = new TGraph();
       radComparison->SetTitle(Form("Radiation length comparison;%s;%s", twoFileName.c_str(), oneFileName.c_str()));

       for (int i=1; i<=oneProfile1->GetXaxis()->GetNbins(); ++i) {
         radComparison->SetPoint(radComparison->GetN(),
                                 twoProfile1->GetBinContent(i),
                                 oneProfile1->GetBinContent(i));
       }
     }
     materialBudgetComparisonCanvas->cd(1);
     radComparison->SetMarkerStyle(7);
     radComparison->GetYaxis()->SetRangeUser(0, radComparison->GetXaxis()->GetXmax());

     radComparison->Draw("ap");
     radComparison->Fit("pol1");
  }

  TPad* onePad2 = (TPad*) oneCanvas->GetPrimitive("materialInTrackingVolumepixel_2");
  TPad* twoPad2 = (TPad*) twoCanvas->GetPrimitive("materialInTrackingVolumepixel_2");

  TProfile* oneProfile2 = (TProfile*) onePad2->GetPrimitive("iglobal");
  TProfile* twoProfile2 = (TProfile*) twoPad2->GetPrimitive("iglobal");
  if (oneProfile2->GetXaxis()->GetNbins() == twoProfile2->GetXaxis()->GetNbins()) {
     for (int i=1; i<=oneProfile2->GetXaxis()->GetNbins(); ++i) {
       intComparison->SetPoint(intComparison->GetN(),
                               oneProfile2->GetBinContent(i),
                               twoProfile2->GetBinContent(i));
     }
     if (intComparison->GetYaxis()->GetXmax() > intComparison->GetXaxis()->GetXmax()) {
       intComparison = new TGraph();
       intComparison->SetTitle(Form("Interaction length comparison;%s;%s", twoFileName.c_str(), oneFileName.c_str()));

       for (int i=1; i<=oneProfile1->GetXaxis()->GetNbins(); ++i) {
         intComparison->SetPoint(intComparison->GetN(),
                                 twoProfile1->GetBinContent(i),
                                 oneProfile1->GetBinContent(i));
       }
     }
     materialBudgetComparisonCanvas->cd(2);
     intComparison->SetMarkerStyle(7);
     intComparison->GetYaxis()->SetRangeUser(0, intComparison->GetXaxis()->GetXmax());

     intComparison->Draw("ap");
     intComparison->Fit("pol1");
  }

  oneProfile1->SetFillStyle(0);
  twoProfile1->SetFillStyle(0);
  oneProfile1->SetLineColor(kRed);
  oneProfile1->SetName(Form("%s_first", oneProfile1->GetName()));
  twoProfile1->SetName(Form("%s_second", oneProfile1->GetName()));

  oneProfile2->SetFillStyle(0);
  twoProfile2->SetFillStyle(0);
  oneProfile2->SetLineColor(kRed);
  oneProfile2->SetName(Form("%s_first", oneProfile2->GetName()));
  twoProfile2->SetName(Form("%s_second", oneProfile2->GetName()));

  TCanvas* out = new TCanvas("outPixel", "Compare");
  out->Divide(2,1);

  out->cd(1);
  oneProfile1->Draw("HIST C");
  twoProfile1->Draw("HIST C SAME");

  TLegend* leg1 = new TLegend(0.133568, 0.757698, 0.357559, 0.900018);
  leg1->AddEntry(oneProfile1, oneFileName.c_str(), "l");
  leg1->AddEntry(twoProfile1, twoFileName.c_str(), "l");
  leg1->Draw();

  out->cd(2);
  oneProfile2->Draw("HIST C");
  twoProfile2->Draw("HIST C SAME");

  TLegend* leg2 = new TLegend(0.133568, 0.757698, 0.357559, 0.900018);
  leg2->AddEntry(oneProfile2, oneFileName.c_str(), "l");
  leg2->AddEntry(twoProfile2, twoFileName.c_str(), "l");
  leg2->Draw();
}
