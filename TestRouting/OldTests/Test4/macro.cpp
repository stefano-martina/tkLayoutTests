void macro() {
  TFile* file = TFile::Open("orig.root");
  TCanvas* origCanv = (TCanvas*) file->Get("matFull001");
  TPad* origPad = (TPad*) origCanv->GetPrimitive("overviewMaterialpixel_1");
  TProfile* origProf = (TProfile*) origPad->GetPrimitive("rfullvolume");
  
  double w = 1400;
  double h = 900;
  TCanvas* outCanv = new TCanvas("out", "Modules Layer 1", w, h);
  outCanv->SetWindowSize(w + (w - outCanv->GetWw()), h + (h - outCanv->GetWh()));
  outCanv->Divide(2,1,0.01,0.01);
  outCanv->cd(1);

  origProf->Draw("BAR");

  //sections
  double z[8];
  double r[8];
  
  z[0] = 35.5; r[0] = 40.837;
  z[1] = 105.5; r[1] = 40.837;

  z[2] = 105.5; r[2] = 40.837;
  z[3] = 175.5; r[3] = 40.837;

  z[4] = 175.5; r[4] = 40.837;
  z[5] = 245.5; r[5] = 40.837;

  z[6] = 245.5; r[6] = 40.837;
  z[7] = 250.5; r[7] = 40.837;

  TGraph* sect;
  double etas[8];
  for (int i=0; i<(sizeof(z)/sizeof(z[0])); i++) {
    etas[i] = - log(sqrt((sqrt(z[i]*z[i] + r[i]*r[i])-z[i])/(sqrt(z[i]*z[i] + r[i]*r[i])+z[i])));
    
    sect = new TGraph();
    sect->SetPoint(sect->GetN(), etas[i], origProf->GetBinContent(origProf->GetMaximumBin()) /20);
    sect->SetPoint(sect->GetN(), etas[i], 0.);
    if(i%2) 
      sect->SetPoint(sect->GetN(), etas[i]-0.1, 0.);
    else
      sect->SetPoint(sect->GetN(), etas[i]+0.1, 0.);
    sect->SetLineColor(2);
    sect->SetLineWidth(5);
    sect->Draw("same");
  }


  //cylinders
  TF1* func1 = new TF1("func1", "(0.1*12*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*40.837*0.1286*2)", 0., 4.);
  func1->SetLineColor(2);
  func1->Draw("same");

  TF1* func2 = new TF1("func2", "(0.1*(12+12)*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*40.837*0.1286*2)", 0., 4.);
  func2->SetLineColor(3);
  func2->Draw("same");

  TF1* func3 = new TF1("func3", "(0.1*(12+12+12)*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*40.837*0.1286*2)", 0., 4.);
  func3->SetLineColor(4);
  func3->Draw("same");

  TF1* func4 = new TF1("func4", "(0.1*(12+12+12+12)*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*40.837*0.1286*2)", 0., 4.);
  func4->SetLineColor(6);
  func4->Draw("same");

  outCanv->cd(2);

  TGraph* corr = new TGraph();
  double eta, expect, measure;
  double max=0.;

  for(int bin=1; bin<=origProf->GetNbinsX(); ++bin) {
    eta = origProf->GetXaxis()->GetBinCenter(bin);
    measure = origProf->GetBinContent(bin);
    
    if((eta >= etas[0]) && (eta <= etas[1])) {
      expect = func1->Eval(eta,0,0);
    } else if((eta >= etas[2]) && (eta <= etas[3])) {
      expect = func2->Eval(eta,0,0);
    } else if((eta >= etas[4]) && (eta <= etas[5])) {
      expect = func3->Eval(eta,0,0);
    } else if((eta >= etas[6]) && (eta <= etas[7])) {
      expect = func4->Eval(eta,0,0);
    } else { //ignore intermedies
      continue;
    }

    if(measure > max)
      max = measure;
    if(expect > max)
      max = expect;
    
    corr->SetPoint(corr->GetN(), measure, expect);
  }

  corr->SetTitle("Correlation");
  corr->GetXaxis()->SetTitle("Measured");
  corr->GetYaxis()->SetTitle("Expected");
  corr->GetYaxis()->SetTitleOffset(1.5);
  corr->SetMarkerStyle(7);
  corr->SetMarkerColor(2);
  corr->Draw("AP");

  TGraph* line = new TGraph();
  line->SetPoint(corr->GetN(), 0., 0.);
  line->SetPoint(corr->GetN(), max, max);
  line->SetLineColor(4);
  line->Draw("same");
}
