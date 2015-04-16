void macro(std::string dir) {
  gROOT->ProcessLine(".L createTestCanvasAlg.cpp");
  TCanvas* canvas = createTestCanvas(dir);
}

