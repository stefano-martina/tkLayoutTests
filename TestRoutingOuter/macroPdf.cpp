void macroPdf(std::string dir) {
  gROOT->ProcessLine(".L createTestCanvasAlg.cpp");
  TCanvas* canvas = createTestCanvas(dir);

  // std::stringstream stringStreamPdf;
  // stringStreamPdf << "pdf/" << dir << ".pdf";
  // outCanv->Print(stringStreamPdf.str().c_str(),"pdf");

  canvas->Print("pdf/out.pdf","pdf");
  exit(0);
}
