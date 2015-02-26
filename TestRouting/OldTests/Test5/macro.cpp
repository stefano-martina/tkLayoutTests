void macro() {
  TFile* file = TFile::Open("orig.root");
  TCanvas* origCanv = (TCanvas*) file->Get("matFull001");
  TPad* origPad = (TPad*) origCanv->GetPrimitive("overviewMaterialpixel_1");
  TProfile* origProf = (TProfile*) origPad->GetPrimitive("rfullvolume");
  
  double w = 1400;
  double h = 900;
  TCanvas* outCanv = new TCanvas("out", "Modules and rod Layer 1", w, h);
  outCanv->SetWindowSize(w + (w - outCanv->GetWw()), h + (h - outCanv->GetWh()));
  outCanv->Divide(2,1,0.01,0.01);
  outCanv->cd(1);

  origProf->Draw("BAR");

  //sections
  double z[10];
  double r[10];
  
  z[0] = 0.1; r[0] = 40.837;
  z[1] = 35.5; r[1] = 40.837;
  
  z[2] = 35.5; r[2] = 40.837;
  z[3] = 105.5; r[3] = 40.837;

  z[4] = 105.5; r[4] = 40.837;
  z[5] = 175.5; r[5] = 40.837;

  z[6] = 175.5; r[6] = 40.837;
  z[7] = 245.5; r[7] = 40.837;

  z[8] = 245.5; r[8] = 40.837;
  z[9] = 250.5; r[9] = 40.837;

  TGraph* sect;
  double etas[10];
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

  //rod effect
  TF1* funcR = new TF1("funcR", "(0.15*12*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*40.837*0.1286*2)", 0., 4.);
  funcR->SetLineColor(7);
  funcR->Draw("same");

  //cylinders
  TF1* func1 = new TF1("func1", "(0.1*12*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*40.837*0.1286*2)", 0., 4.);
  // func1->SetLineColor(5);
  // func1->Draw("same");

  TF1* func2 = new TF1("func2", "(0.1*(12+12)*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*40.837*0.1286*2)", 0., 4.);
  // func2->SetLineColor(5);
  // func2->Draw("same");

  TF1* func3 = new TF1("func3", "(0.1*(12+12+12)*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*40.837*0.1286*2)", 0., 4.);
  // func3->SetLineColor(5);
  // func3->Draw("same");

  TF1* func4 = new TF1("func4", "(0.1*(12+12+12+12)*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*40.837*0.1286*2)", 0., 4.);
  // func4->SetLineColor(5);
  // func4->Draw("same");

  //effect sums
  TF1* funcS1 = new TF1("funcS1", "funcR+func1", 0., 4.);
  funcS1->SetLineColor(2);
  funcS1->Draw("same");

  TF1* funcS2 = new TF1("funcS2", "funcR+func2", 0., 4.);
  funcS2->SetLineColor(3);
  funcS2->Draw("same");

  TF1* funcS3 = new TF1("funcS3", "funcR+func3", 0., 4.);
  funcS3->SetLineColor(4);
  funcS3->Draw("same");

  TF1* funcS4 = new TF1("funcS4", "funcR+func4", 0., 4.);
  funcS4->SetLineColor(6);
  funcS4->Draw("same");


  outCanv->cd(2);

  TGraph* corr = new TGraph();
  double eta, expect, measure;
  double max=0.;

  for(int bin=1; bin<=origProf->GetNbinsX(); ++bin) {
    eta = origProf->GetXaxis()->GetBinCenter(bin);
    measure = origProf->GetBinContent(bin);
    
    if((eta >= etas[0]) && (eta <= etas[1])) {
      expect = funcR->Eval(eta,0,0);
    } else if((eta >= etas[2]) && (eta <= etas[3])) {
      expect = funcS1->Eval(eta,0,0);
    } else if((eta >= etas[4]) && (eta <= etas[5])) {
      expect = funcS2->Eval(eta,0,0);
    } else if((eta >= etas[6]) && (eta <= etas[7])) {
      expect = funcS3->Eval(eta,0,0);
    } else if((eta >= etas[8]) && (eta <= etas[9])) {
      expect = funcS4->Eval(eta,0,0);
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
