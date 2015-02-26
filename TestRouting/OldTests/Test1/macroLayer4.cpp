void macroLayer4() {
  TFile* file = TFile::Open("origL4.root");
  TCanvas* origCanv = (TCanvas*) file->Get("matFull001");
  TPad* origPad = (TPad*) origCanv->GetPrimitive("overviewMaterialpixel_1");
  TProfile* origProf = (TProfile*) origPad->GetPrimitive("rfullvolume");
  
  double w = 1400;
  double h = 900;
  TCanvas* outCanv = new TCanvas("out", "Layer 4", w, h);
  outCanv->SetWindowSize(w + (w - outCanv->GetWw()), h + (h - outCanv->GetWh()));
  outCanv->Divide(2,1,0.01,0.01);
  outCanv->cd(1);

  origProf->Draw("BAR");

  //sections
  double z[4];
  double r[4];
  
  z[0] = 0.1; r[0] = 171.368;
  z[1] = 260.5; r[1] = 189.368;

  z[2] = 250.5; r[2] = 171.368;
  z[3] = 2632.87; r[3] = 189.368;


  TGraph* sect;
  double etas[4];
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


  //layer cylinder
  TF1* funcL = new TF1("funcL", "(0.1*52*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*171.368*0.1286*2)", 0., 4.);
  funcL->SetLineColor(2);
  funcL->Draw("same");

  //exiting cylinder
  TF1* funcE = new TF1("funcE", "(0.1*52*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*189.368*0.1286*2)", 0., 4.);
  funcE->SetLineColor(3);
  funcE->Draw("same");
  
  outCanv->cd(2);

  TGraph* corr = new TGraph();
  double eta, expect, measure;
  double max=0.;

  for(int bin=1; bin<=origProf->GetNbinsX(); ++bin) {
    eta = origProf->GetXaxis()->GetBinCenter(bin);
    measure = origProf->GetBinContent(bin);
    
    if((eta >= etas[0]) && (eta <= etas[1])) {     //layer
      expect = funcL->Eval(eta,0,0);
    } else if((eta >= etas[2]) && (eta <= etas[3])) {  //exiting
      expect = funcE->Eval(eta,0,0);
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
