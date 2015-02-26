void testRoutingHoriz(string fileName, int numCables, int numRadiuses, double* radiuses) {
  TFile* file = TFile::Open(fileName.c_str());
  TCanvas* origCanv = (TCanvas*) file->Get("matFull001");
  TPad* origPad = (TPad*) origCanv->GetPrimitive("overviewMaterialpixel_1");
  TProfile* origProf = (TProfile*) origPad->GetPrimitive("rfullvolume");
  
  TCanvas* outCanv = new TCanvas();
  origProf->Draw("BAR");

  std::stringstream funcString("");
  std::stringstream funcNameString("");

  for(int i=0; i<numRadiuses; ++i) {
    funcString.str("");
    funcString.clear();
    funcNameString.str("");
    funcNameString.clear();

    funcString << "(0.1*" << numCables << "*(TMath::E()^x+TMath::E()^(-x)))/(2*TMath::Pi()*" << radiuses[i] << "*0.1286*2)";
    funcNameString << "func" << i;

    TF1* func = new TF1(funcNameString.str().c_str(), funcString.str().c_str() , 0., 4.);
    func->SetLineColor(2+i);
    func->Draw("same");
  }
}
