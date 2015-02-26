void testRouting(string fileName, double numCables, double radius) {
  TFile* file = TFile::Open(fileName.c_str());
  TCanvas* origCanv = (TCanvas*) file->Get("matFull001");
  TPad* origPad = (TPad*) origCanv->GetPrimitive("overviewMaterialpixel_1");
  TProfile* origProf = (TProfile*) origPad->GetPrimitive("rfullvolume");
  
  TCanvas* outCanv = new TCanvas();
  origProf->Draw("BAR");

  std::stringstream funcString1("");
  std::stringstream funcString2("");
  funcString1 << "(0.1*" << numCables << "*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*" << radius << "*0.1286*2)";
  funcString2 << "(0.1*" << numCables << "*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*185*0.1286*2)";

  TF1* func1 = new TF1("func1", funcString1.str().c_str() , 0., 4.);
  TF1* func2 = new TF1("func2", funcString2.str().c_str() , 0., 4.);
  func2->SetLineColor(4);
  func1->Draw("same");
  func2->Draw("same");
}
  
