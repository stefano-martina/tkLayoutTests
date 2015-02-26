

class MapSegFunction {
public:
  MapSegFunction(double_t newZMin, double_t newRMin, double_t newZMax, double_t newRMax, char newDirection, double_t newQuantity, std::string newUnit, double_t newX0, double_t newDensity) {
    zMin = newZMin;
    rMin = newRMin;
    zMax = newZMax;
    rMax = newRMax;
    direction = newDirection;
    quantity = newQuantity;
    unit = newUnit;
    X0 = newX0;
    density = newDensity;

    errorOrientation = "Error in orientation";
    errorUnit = "Error in unit";
  };

  double_t operator() (double_t *x, double_t *p) {
    Double_t z = x[0];
    Double_t r = x[1];
    Double_t result = 0.;

    if((z >= zMin) && (z <= zMax) && (r >= rMin) && (r <= rMax)) {
      Double_t eta = - log(sqrt((sqrt(z*z + r*r)-z)/(sqrt(z*z + r*r)+z)));

      if (unit.compare("g/m") == 0) {
        if (direction == 'H') {
          //return (quantity/1000)/(TMath::Pi()*(rMin+rMax)*(X0/100));
          result = (quantity/1000*(TMath::Exp(eta)+TMath::Exp(-eta)))/(2*TMath::Pi()*(rMin+rMax)*(X0/100));
        } else if (direction == 'V') {
          //return (quantity/1000)/(TMath::Pi()*(rMin+rMax)*(X0/100));
          result = (quantity/1000*(TMath::Exp(2*eta)+1))/(TMath::Pi()*(rMin+rMax)*(X0/100)*(TMath::Exp(2*eta)-1));
        } else {
          std::cout << errorOrientation << std::endl;
        }
      } else if (unit.compare("mm") == 0) {
        if (direction == 'H') {
          //return (quantity*(density/1000))/(X0/100);
          result = (quantity*(density/1000)*(TMath::Exp(eta)+TMath::Exp(-eta)))/((X0/100)*2);
        } else if (direction == 'V') {
          //return (quantity*(density/1000))/(X0/100);
          result = (quantity*(density/1000)*(TMath::Exp(2*eta)+1))/((X0/100)*(TMath::Exp(2*eta)-1));
        } else {
          std::cout << errorOrientation << std::endl;
        }
      } else if (unit.compare("g") == 0) {
        if (direction == 'H') {
          result = (quantity*(TMath::Exp(eta)+TMath::Exp(-eta)))/(2*TMath::Pi()*(rMin+rMax)*(zMax-zMin)*(X0/100));
        } else if (direction == 'V') {
          result = (quantity*(TMath::Exp(2*eta)+1))/(TMath::Pi()*(rMax*rMax-rMin*rMin)*(X0/100)*(TMath::Exp(2*eta)-1));
        } else {
          std::cout << errorOrientation << std::endl;
        }
      }  else {
        std::cout << errorUnit << std::endl;
      }
    }
    return result;
  };

  
private:
  std::string errorOrientation;
  std::string errorUnit;
  
  double_t zMin;
  double_t rMin;
  double_t zMax;
  double_t rMax;
  char direction;
  double_t quantity;
  std::string unit;
  double_t X0;
  double_t density;
};


class MapFunction {
public:
  MapFunction() { }

  double_t operator() (double_t* x, double_t* p) {
    double_t result = 0.;
    for(std::set<MapSegFunction*>::iterator funcIter = funcs.begin(); funcIter != funcs.end(); ++funcIter) {
      result += (**funcIter)(x, p);
    }
    return result;
  }

  void addFunction(MapSegFunction* newFunction) {
    funcs.insert(newFunction);
  }
private:
  std::set<MapSegFunction*> funcs;
};



class SegFunction {
public:
  SegFunction(double_t newZMin, double_t newRMin, double_t newZMax, double_t newRMax, char newDirection, double_t newQuantity, std::string newUnit, double_t newX0, double_t newDensity) {
    zMin = newZMin;
    rMin = newRMin;
    zMax = newZMax;
    rMax = newRMax;
    direction = newDirection;
    quantity = newQuantity;
    unit = newUnit;
    X0 = newX0;
    density = newDensity;

    errorOrientation = "Error in orientation";
    errorUnit = "Error in unit";
  };

  double_t operator() (double_t *x, double_t *p) {
    Double_t eta = x[0];

    if (unit.compare("g/m") == 0) {
      if (direction == 'H') {
        return (quantity/1000*(TMath::Exp(eta)+TMath::Exp(-eta)))/(2*TMath::Pi()*(rMin+rMax)*(X0/100));
      } else if (direction == 'V') {
        return (quantity/1000*(TMath::Exp(2*eta)+1))/(TMath::Pi()*(rMin+rMax)*(X0/100)*(TMath::Exp(2*eta)-1));
      } else {
        std::cout << errorOrientation << std::endl;
        return 0.;
      }
    } else if (unit.compare("mm") == 0) {
      if (direction == 'H') {
        return (quantity*(density/1000)*(TMath::Exp(eta)+TMath::Exp(-eta)))/((X0/100)*2);
      } else if (direction == 'V') {
        return (quantity*(density/1000)*(TMath::Exp(2*eta)+1))/((X0/100)*(TMath::Exp(2*eta)-1));
      } else {
        std::cout << errorOrientation << std::endl;
        return 0.;
      }
    } else if (unit.compare("g") == 0) {
      if (direction == 'H') {
        return (quantity*(TMath::Exp(eta)+TMath::Exp(-eta)))/(2*TMath::Pi()*(rMin+rMax)*(zMax-zMin)*(X0/100));
      } else if (direction == 'V') {
        return (quantity*(TMath::Exp(2*eta)+1))/(TMath::Pi()*(rMax*rMax-rMin*rMin)*(X0/100)*(TMath::Exp(2*eta)-1));
      } else {
        std::cout << errorOrientation << std::endl;
        return 0.;
      }
    }  else {
      std::cout << errorUnit << std::endl;
      return 0.;
    }
  };

  
private:
  std::string errorOrientation;
  std::string errorUnit;
  
  double_t zMin;
  double_t rMin;
  double_t zMax;
  double_t rMax;
  char direction;
  double_t quantity;
  std::string unit;
  double_t X0;
  double_t density;
};

class AreaFunction {
public:
  AreaFunction(double_t newEtaStart, double_t newEtaEnd, std::set<SegFunction*> newActiveFuncs) {
    etaStart = newEtaStart;
    etaEnd = newEtaEnd;
    activeFuncs = newActiveFuncs;
  }

  double_t operator() (double_t* x, double_t* p) {
    double_t result = 0.;
    double_t eta = x[0];
    if ((eta > etaStart) && (eta < etaEnd)) {
      for(std::set<SegFunction*>::iterator funcIter = activeFuncs.begin(); funcIter != activeFuncs.end(); ++funcIter) {
        result += (**funcIter)(x, p);
      }
    }
    return result;
  }

private:
  double_t etaStart;
  double_t etaEnd;
  std::set<SegFunction*> activeFuncs;
};


class FinalFunction {
public:
  FinalFunction() { }

  double_t operator() (double_t* x, double_t* p) {
    double_t result = 0.;
    for(std::set<AreaFunction*>::iterator funcIter = funcs.begin(); funcIter != funcs.end(); ++funcIter) {
      result += (**funcIter)(x, p);
    }
    return result;
  }

  void addFunction(AreaFunction* newFunction) {
    funcs.insert(newFunction);
  }
private:
  std::set<AreaFunction*> funcs;
};

enum Operation {START, END};

struct FuncTableRow {
  double_t eta;
  Operation operation;
  SegFunction* function;

  FuncTableRow(double_t newEta, Operation newOperation, SegFunction* newFunction) {
    eta = newEta;
    operation = newOperation;
    function = newFunction;
  }

  bool operator<(const FuncTableRow& other) const {
    if(eta != other.eta) {
      return eta < other.eta;
    }
    if((operation == END) && (other.operation == START)) {
      return true;
    }
    if((operation == START) && (other.operation == END)) {
      return false;
    }
    return function < other.function;
  }
};






TCanvas* createTestCanvas(std::string dir) {
  //read csv
  std::stringstream stringStreamCsv;
  std::string stringCsv;
  stringStreamCsv << dir << "/in.csv";
  stringStreamCsv >> stringCsv;

  std::ifstream fileCsv(stringCsv.c_str(), std::ios::in);
  std::string line;
  std::string fieldString;
  double_t zMin, rMin, zMax, rMax, quantity, X0, density, eta1, eta2;
  char direction;
  Operation op1, op2;
  std::string funcName;
  std::string unit;
  std::set<FuncTableRow> funcTable;
  FinalFunction* finalFunction = new FinalFunction();
  MapFunction* mapFunction = new MapFunction();
  
  std::getline(fileCsv, line);
  while(std::getline(fileCsv, line)) {
    std::stringstream lineStream(line);
    if((line.size() >= 2) && ((line[0] != '/') || (line[1] != '/'))) {
      std::getline(lineStream, fieldString, ',');
      std::stringstream fieldStream1(fieldString);
      fieldStream1 >> zMin;

      std::getline(lineStream, fieldString, ',');
      std::stringstream fieldStream2(fieldString);
      fieldStream2 >> rMin;

      std::getline(lineStream, fieldString, ',');
      std::stringstream fieldStream3(fieldString);
      fieldStream3 >> zMax;

      std::getline(lineStream, fieldString, ',');
      std::stringstream fieldStream4(fieldString);
      fieldStream4 >> rMax;

      std::getline(lineStream, fieldString, ',');
      std::stringstream fieldStream5(fieldString);
      fieldStream5 >> direction;

      std::getline(lineStream, fieldString, ',');
      std::stringstream fieldStream6(fieldString);
      fieldStream6 >> quantity;

      std::getline(lineStream, fieldString, ',');
      std::stringstream fieldStream7(fieldString);
      fieldStream7 >> unit;

      std::getline(lineStream, fieldString, ',');
      std::stringstream fieldStream8(fieldString);
      fieldStream8 >> X0;

      std::getline(lineStream, fieldString, ',');
      std::stringstream fieldStream9(fieldString);
      fieldStream9 >> density;

      //create function
      SegFunction* func = new SegFunction(zMin, rMin, zMax, rMax, direction, quantity, unit, X0, density);
      MapSegFunction* mapSegFunc = new MapSegFunction(zMin, rMin, zMax, rMax, direction, quantity, unit, X0, density);
  
      //create table rows
      if(direction == 'H') {
        double_t rMid = (rMin + rMax)/2;
        eta1 = - log(sqrt((sqrt(zMin*zMin + rMid*rMid)-zMin)/(sqrt(zMin*zMin + rMid*rMid)+zMin)));
        eta2 = - log(sqrt((sqrt(zMax*zMax + rMid*rMid)-zMax)/(sqrt(zMax*zMax + rMid*rMid)+zMax)));
      } else if (direction == 'V') {
        double_t zMid = (zMin + zMax)/2;
        eta1 = - log(sqrt((sqrt(zMid*zMid + rMin*rMin)-zMid)/(sqrt(zMid*zMid + rMin*rMin)+zMid)));
        eta2 = - log(sqrt((sqrt(zMid*zMid + rMax*rMax)-zMid)/(sqrt(zMid*zMid + rMax*rMax)+zMid)));
      } else {
        std::cout << "Error in orientation" << std::endl;
        return nullptr;
      }

      if(eta1 < eta2) {
        op1 = START;
        op2 = END;
      } else {
        op1 = END;
        op2 = START;
      }

      funcTable.insert(FuncTableRow(eta1, op1, func));
      funcTable.insert(FuncTableRow(eta2, op2, func));

      mapFunction->addFunction(mapSegFunc);
    }
  }

  //create functions with effects sum
  if (funcTable.size() >= 2) {
    std::set<SegFunction*> activeFuncs;
    std::set<SegFunction*>::iterator activeFuncsIter;
    
    std::set<FuncTableRow>::iterator funcTableIterStart = funcTable.begin();
    std::set<FuncTableRow>::iterator funcTableIterEnd = funcTable.begin();
    std::set<FuncTableRow>::iterator funcTableIterFind;
    ++funcTableIterEnd;
    while(funcTableIterEnd != funcTable.end()) {
      if(funcTableIterStart->operation == START) {
        activeFuncs.insert(funcTableIterStart->function);
      } else {
        activeFuncsIter = activeFuncs.find(funcTableIterStart->function);
        if (activeFuncsIter != activeFuncs.end()) {
          activeFuncs.erase(activeFuncsIter);
        }
      }

      if ((! activeFuncs.empty()) && (funcTableIterStart->eta < funcTableIterEnd->eta)) {
        AreaFunction* func = new AreaFunction(funcTableIterStart->eta, funcTableIterEnd->eta, activeFuncs);
        finalFunction->addFunction(func);
      }
      ++funcTableIterStart;
      ++funcTableIterEnd;
    }
  }

  TF1* funcRoot = new TF1("funcRoot", finalFunction, 0., 4., 0, "FinalFunction");
  //TF2* mapFuncRoot = new TF2("mapFuncRoot", mapFunction, 0., 3000., 0., 1300., 0, "MapFunction");

  std::stringstream stringStreamOrig;
  std::string stringOrig;
  stringStreamOrig << dir << "/orig.root";
  stringStreamOrig >> stringOrig;


  TFile* file = TFile::Open(stringOrig.c_str());
  TCanvas* origCanv = (TCanvas*) file->Get("matFull001");
  TPad* origPad = (TPad*) origCanv->GetPrimitive("overviewMaterialpixel_1");
  TProfile* origProf = (TProfile*) origPad->GetPrimitive("rfullvolume");
  
  double_t w = 1400;
  double_t h = 1000;
  TCanvas* outCanv = new TCanvas("out", "Test", w, h);
  outCanv->SetWindowSize(w + (w - outCanv->GetWw()), h + (h - outCanv->GetWh()));
  outCanv->Divide(2,2,0.01,0.01);

  outCanv->cd(3);
  funcRoot->SetTitle("Computed function");
  funcRoot->GetXaxis()->SetTitle("#eta");
  funcRoot->SetLineColor(2);
  funcRoot->Draw();

  outCanv->cd(4);
  // mapFuncRoot->SetTitle("Computed map");
  // mapFuncRoot->GetXaxis()->SetTitle("z");
  // mapFuncRoot->GetYaxis()->SetTitle("r");
  // mapFuncRoot->Draw("Colz");

  TProfile2D* mapFuncRootHist = new TProfile2D("mapFuncRootHist", "Computed map", 3000, 0., 3000., 1300, 0., 1300.);

  int binX, binY;
  double_t binXcenter, binYcenter, binValue;
  double_t args[2];
  double_t parms[0];

  for(binX = 1; binX <= mapFuncRootHist->GetNbinsX(); ++binX) {
    for(binY = 1; binY <= mapFuncRootHist->GetNbinsY(); ++binY) {
      binXcenter = mapFuncRootHist->GetXaxis()->GetBinCenter(binX);
      binYcenter = mapFuncRootHist->GetYaxis()->GetBinCenter(binY);
      args[0] = binXcenter;
      args[1] = binYcenter;
      binValue = (*mapFunction)(args, parms);
      if(binValue > 0.) {
        mapFuncRootHist->Fill(binXcenter, binYcenter, binValue);
      }
    }
  }
  mapFuncRootHist->GetXaxis()->SetTitle("z");
  mapFuncRootHist->GetYaxis()->SetTitle("r");
  mapFuncRootHist->SetStats(0);
  mapFuncRootHist->Draw("Colz");



  outCanv->cd(1);
  origProf->SetTitle("TkLayout estimate vs computed");
  origProf->Draw("BAR");

  TGraph* corr = new TGraph();
  TGraph* fit = new TGraph();
  double_t eta, expect, measure;
  double_t max=0.;

  for(int bin=1; bin<=origProf->GetNbinsX(); ++bin) {
    eta = origProf->GetXaxis()->GetBinCenter(bin);
    measure = origProf->GetBinContent(bin);
    //expect = funcRoot->Eval(eta,0,0);
    double_t etas[1] = {eta};
    double_t parms[0] = {};
    expect = (*finalFunction)(etas,parms);

    if(measure > max)
      max = measure;
    if(expect > max)
      max = expect;
    
    corr->SetPoint(corr->GetN(), measure, expect);
    fit->SetPoint(fit->GetN(), eta, expect);
  }

  fit->SetMarkerStyle(7);
  fit->SetMarkerColor(2);
  fit->Draw("SAME P");

  //right plot
  outCanv->cd(2);

  corr->SetTitle("Correlation");
  corr->GetXaxis()->SetTitle("TkLayout estimate");
  corr->GetYaxis()->SetTitle("Computed");
  corr->GetYaxis()->SetTitleOffset(1.5);
  corr->SetMarkerStyle(7);
  corr->SetMarkerColor(2);
  corr->Draw("AP");

  TGraph* line45 = new TGraph();
  line45->SetPoint(corr->GetN(), 0., 0.);
  line45->SetPoint(corr->GetN(), max, max);
  line45->SetLineColor(4);
  line45->Draw("same");

  return outCanv;
}
