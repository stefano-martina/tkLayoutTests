void macro() {
  TFile* file = TFile::Open("orig.root");
  TCanvas* origCanv = (TCanvas*) file->Get("matFull001");
  TPad* origPad = (TPad*) origCanv->GetPrimitive("overviewMaterialpixel_1");
  TProfile* origProf = (TProfile*) origPad->GetPrimitive("rfullvolume");
  
  double w = 1400;
  double h = 900;
  TCanvas* outCanv = new TCanvas("out", "Disk 1", w, h);
  outCanv->SetWindowSize(w + (w - outCanv->GetWw()), h + (h - outCanv->GetWh()));
  outCanv->Divide(2,1,0.01,0.01);
  outCanv->cd(1);

  origProf->Draw("BAR");

  //sections
  double z[16];
  double r[16];
  
  z[0] = 308.97; r[0] = 170.09;
  z[1] = 321.97; r[1] = 159.99;

  z[2] = 321.97; r[2] = 159.99;
  z[3] = 321.97; r[3] = 110.655;

  z[4] = 321.97; r[4] = 110.655;
  z[5] = 321.97; r[5] = 44.733;

  z[6] = 321.97; r[6] = 44.733;
  z[7] = 2632.97; r[7] = 189.368;

  z[8] = 2632.97; r[8] = 189.368;
  z[9] = 2637.97; r[9] = 184.268;

  z[10] = 2637.97; r[10] = 184.268;
  z[11] = 2722.97; r[11] = 189.368;

  z[12] = 2722.97; r[12] = 189.368;
  z[13] = 2637.97; r[13] = 174.99;

  z[14] = 2637.97; r[14] = 174.99;
  z[15] = 2632.87; r[15] = 170.09;


  TGraph* sect;
  double etas[16];
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

  //disks
  TF1* funcD1 = new TF1("funcD1", "(24*0.1*(TMath::E()^(2*x)+1))/(TMath::Pi()*(44.733+110.655)*0.1286*(TMath::E()^(2*x)-1))", 0., 4.);
  funcD1->SetLineColor(5);
  funcD1->Draw("same");

  TF1* funcD2 = new TF1("funcD2", "(24*0.1*(TMath::E()^(2*x)+1))/(TMath::Pi()*(110.655+159.99)*0.1286*(TMath::E()^(2*x)-1))", 0., 4.);
  funcD2->SetLineColor(5);
  funcD2->Draw("same");

  //cylinders
  TF1* funcC1 = new TF1("funcC1", "(24*0.1*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*170.09*0.1286*2)", 0., 4.);
  funcC1->SetLineColor(2);
  funcC1->Draw("same");

  TF1* funcC2 = new TF1("funcC2", "(24*0.1*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*189.368*0.1286*2)", 0., 4.);
  funcC2->SetLineColor(3);
  funcC2->Draw("same");

  //conjunction disk
  TF1* funcDC = new TF1("funcDC", "(24*0.1*(TMath::E()^(2*x)+1))/(TMath::Pi()*(174.99+184.268)*0.1286*(TMath::E()^(2*x)-1))", 0., 4.);
  funcDC->SetLineColor(5);
  funcDC->Draw("same");

  //sums of effects
  TF1* funcS1 = new TF1("funcS1", "funcD2+funcC1", 0., 4.);
  funcS1->SetLineColor(4);
  funcS1->Draw("same");

  TF1* funcS2 = new TF1("funcS2", "funcD1+funcC1", 0., 4.);
  funcS2->SetLineColor(6);
  funcS2->Draw("same");

  TF1* funcS3 = new TF1("funcS3", "funcC1+funcC2", 0., 4.);
  funcS3->SetLineColor(7);
  funcS3->Draw("same");

  TF1* funcS4 = new TF1("funcS4", "funcC1+funcDC+funcC2", 0., 4.);
  funcS4->SetLineColor(8);
  funcS4->Draw("same");

  TF1* funcS5 = new TF1("funcS5", "funcC1+funcDC", 0., 4.);
  funcS5->SetLineColor(9);
  funcS5->Draw("same");

  outCanv->cd(2);

  TGraph* corr = new TGraph();
  double eta, expect, measure;
  double max=0.;

  for(int bin=1; bin<=origProf->GetNbinsX(); ++bin) {
    eta = origProf->GetXaxis()->GetBinCenter(bin);
    measure = origProf->GetBinContent(bin);
    
    if((eta >= etas[0]) && (eta <= etas[1])) {
      expect = funcC1->Eval(eta,0,0);
    } else if((eta >= etas[2]) && (eta <= etas[3])) {
      expect = funcS1->Eval(eta,0,0);
    } else if((eta >= etas[4]) && (eta <= etas[5])) {
      expect = funcS2->Eval(eta,0,0);
    } else if((eta >= etas[6]) && (eta <= etas[7])) {
      expect = funcC1->Eval(eta,0,0);
    } else if((eta >= etas[8]) && (eta <= etas[9])) {
      expect = funcS3->Eval(eta,0,0);
    } else if((eta >= etas[10]) && (eta <= etas[11])) {
      expect = funcS4->Eval(eta,0,0);
    } else if((eta >= etas[12]) && (eta <= etas[13])) {
      expect = funcS5->Eval(eta,0,0);
    } else if((eta >= etas[14]) && (eta <= etas[15])) {
      expect = funcC1->Eval(eta,0,0);
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
  corr->GetXaxis()->SetTitle("TkLayout estimate");
  corr->GetYaxis()->SetTitle("Computed");
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
