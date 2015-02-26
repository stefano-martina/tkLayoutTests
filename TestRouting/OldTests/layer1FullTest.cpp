void layer1FullTest(string fileName) {
  TFile* file = TFile::Open(fileName.c_str());
  TCanvas* origCanv = (TCanvas*) file->Get("matFull001");
  TPad* origPad = (TPad*) origCanv->GetPrimitive("overviewMaterialpixel_1");
  TProfile* origProf = (TProfile*) origPad->GetPrimitive("rfullvolume");
  
  TCanvas* outCanv = new TCanvas();
  origProf->Draw("BAR");

  std::stringstream funcString1("");
  std::stringstream funcString2("");
  std::stringstream funcString3("");
  std::stringstream funcString4("");
  std::stringstream funcString5("");
  funcString1 << "(0.1*12*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*40*0.1286*2)";
  funcString2 << "(0.1*12*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*185*0.1286*2)";
  funcString3 << "(0.1*12*(TMath::E()^(2*x)+1))/(TMath::Pi()*(165+115)*0.1286*(TMath::E()^(2*x)-1))";
  funcString4 << "(0.1*12*(TMath::E()^(2*x)+1))/(TMath::Pi()*(115+75)*0.1286*(TMath::E()^(2*x)-1))";
  funcString5 << "(0.1*12*(TMath::E()^(2*x)+1))/(TMath::Pi()*(70+35)*0.1286*(TMath::E()^(2*x)-1))";

  TF1* func1 = new TF1("func1", funcString1.str().c_str() , 0., 4.);
  TF1* func2 = new TF1("func2", funcString2.str().c_str() , 0., 4.);
  TF1* func3 = new TF1("func3", funcString3.str().c_str() , 0., 4.);
  TF1* func4 = new TF1("func4", funcString4.str().c_str() , 0., 4.);
  TF1* func5 = new TF1("func5", funcString5.str().c_str() , 0., 4.);

  TF1* funcComb1 = new TF1("funcComb1", "func1+func2+func3", 0., 4.);
  TF1* funcComb2 = new TF1("funcComb2", "func1+func2+func4", 0., 4.);
  TF1* funcComb3 = new TF1("funcComb3", "func1+func2+func5", 0., 4.);

  func2->SetLineColor(4);
  func3->SetLineColor(5);
  func4->SetLineColor(5);
  func5->SetLineColor(5);
  funcComb1->SetLineColor(3);
  funcComb2->SetLineColor(6);
  funcComb3->SetLineColor(9);

  func1->Draw("same");
  func2->Draw("same");
  func3->Draw("same");
  func4->Draw("same");
  func5->Draw("same");
  funcComb1->Draw("same");
  funcComb2->Draw("same");
  funcComb3->Draw("same");
}
