void macroLayer2() {
  TFile* file = TFile::Open("origL2.root");
  TCanvas* origCanv = (TCanvas*) file->Get("matFull001");
  TPad* origPad = (TPad*) origCanv->GetPrimitive("overviewMaterialpixel_1");
  TProfile* origProf = (TProfile*) origPad->GetPrimitive("rfullvolume");
  
  double w = 1400;
  double h = 900;
  TCanvas* outCanv = new TCanvas("out", "Layer 2", w, h);
  outCanv->SetWindowSize(w + (w - outCanv->GetWw()), h + (h - outCanv->GetWh()));
  outCanv->Divide(2,1,0.01,0.01);
  outCanv->cd(1);

  origProf->Draw("BAR");

  //sections
  double z[8];
  double r[8];
  
  z[0] = 0.1; r[0] = 78.847;
  z[1] = 260.5; r[1] = 189.368;

  z[2] = 260.6; r[2] = 166.368;
  z[3] = 260.6; r[3] = 114.538;

  z[4] = 260.6; r[4] = 114.538;
  z[5] = 250.5; r[5] = 78.847;

  z[6] = 260.6; r[6] = 73.847;
  z[7] = 2632.87; r[7] = 189.368;


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


  //layer cylinder
  TF1* funcL = new TF1("funcL", "(0.1*24*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*78.847*0.1286*2)", 0., 4.);
  funcL->SetLineColor(2);
  funcL->Draw("same");

  //exiting cylinder
  TF1* funcE = new TF1("funcE", "(0.1*24*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*189.368*0.1286*2)", 0., 4.);
  funcE->SetLineColor(3);
  funcE->Draw("same");
  
  //disks
  TF1* funcV2 = new TF1("funcV2", "(0.1*24*(TMath::E()^(2*x)+1))/(TMath::Pi()*(114.538+73.847)*0.1286*(TMath::E()^(2*x)-1))", 0., 4.);
  funcV2->SetLineColor(5);
  funcV2->Draw("same");
  
  TF1* funcV3 = new TF1("funcV3", "(0.1*24*(TMath::E()^(2*x)+1))/(TMath::Pi()*(166.368+114.538)*0.1286*(TMath::E()^(2*x)-1))", 0., 4.);
  funcV3->SetLineColor(5);
  funcV3->Draw("same");

  //sums of effects
  TF1* funcS2 = new TF1("funcS2", "funcV2+funcL+funcE", 0., 4.);
  funcS2->SetLineColor(6);
  funcS2->Draw("same");

  TF1* funcS3 = new TF1("funcS3", "funcV3+funcL+funcE", 0., 4.);
  funcS3->SetLineColor(7);
  funcS3->Draw("same");

  outCanv->cd(2);

  TGraph* corr = new TGraph();
  double eta, expect, measure;
  double max=0.;

  for(int bin=1; bin<=origProf->GetNbinsX(); ++bin) {
    eta = origProf->GetXaxis()->GetBinCenter(bin);
    measure = origProf->GetBinContent(bin);
    
    if((eta >= etas[0]) && (eta <= etas[1])) {     //layer
      expect = funcL->Eval(eta,0,0);
    } else if((eta >= etas[2]) && (eta <= etas[3])) {  //layer+disk3
      expect = funcS3->Eval(eta,0,0);
    } else if((eta >= etas[4]) && (eta <= etas[5])) {  //layer+disk2
      expect = funcS2->Eval(eta,0,0);
    } else if((eta >= etas[6]) && (eta <= etas[7])) {  //exiting
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
