void testRoutingVert(string fileName, int numCables, int numRadiuses, double* radiusMin, double* radiusMax) {
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

    funcString << "(0.1*"<<numCables<<"*(TMath::E()^(2*x)+1))/(TMath::Pi()*("<<radiusMax[i]<<"+"<<radiusMin[i]<<")*0.1286*(TMath::E()^(2*x)-1))";
    funcNameString << "func" << i;
    std::cout << funcString.str();

    TF1* func = new TF1(funcNameString.str().c_str(), funcString.str().c_str(), 0., 4.);
    func->SetLineColor(2+i);
    func->Draw("same");
  }
}
  
