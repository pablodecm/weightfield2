#include "WeightfieldClass.h"

WeightfieldClass::WeightfieldClass()
{
  XMAX = 3;
  YMAX = 300;
  pitch = 80.;
  width = 30.;
  stripDop = 1;
  bulkDop = 0;
  biasVolt = 300.;
  depVolt = 85.;
  dwpot = new Potentials(YMAX,XMAX,pitch,width) ;
  dField = new Field*[(dwpot->GetYMAX())];
  for (int i = 0; i < (dwpot->GetYMAX()); i++) {
    dField[i] = new Field[(dwpot->GetXMAX())];
  }

  wField = new Field*[(dwpot->GetYMAX())];
  for (int i = 0; i < (dwpot->GetYMAX()); i++) {
    wField[i] = new Field[(dwpot->GetXMAX())];
  }
}

void WeightfieldClass::calculatePotentials()
{

  // set doping
  dwpot->SetDoping(stripDop, bulkDop) ;

  // bias and depletion voltage
  dwpot->SetV(biasVolt,depVolt);

  // reset potentials to zero as a precaution
  for(int i=0; i<dwpot->GetXMAX(); i++) {
     for(int j=0; j<dwpot->GetYMAX(); j++) {
       dwpot->SetwPotential(j,i,0.0);
       dwpot->SetdPotential(j,i,0.0);
     }
  }

  // set BCs
  dwpot->SetBoundaryConditions() ;
  // Numerically solve PDEs
  dwpot->Multigrid();
}

void WeightfieldClass::calculateFields()
{
  CalculateFields(dwpot,wField,dField);
  CalculateAbsFields(dwpot,wField,dField);
}

TH2D* WeightfieldClass::getHistDPot()
{
  // init histogram
   TH2D *dhist   = new TH2D("HistDPot"," ; x [um]; y [um]",dwpot->GetXMAX(),0,dwpot->GetXMAX(),dwpot->GetYMAX(), 0, dwpot->GetYMAX());

  for(int i=0;i<dwpot->GetXMAX();i++) { // fill histogram
    for(int j=0;j<dwpot->GetYMAX();j++) {
      dhist->SetBinContent(i+1,j+1,dwpot->Getdpot(j,i));
    }
  }
  return dhist;
}

TH2D* WeightfieldClass::getHistWPot()
{
  // init histogram
   TH2D *whist   = new TH2D("HistWPot"," ; x [um]; y [um]",dwpot->GetXMAX(),0,dwpot->GetXMAX(),dwpot->GetYMAX(), 0, dwpot->GetYMAX());

  for(int i=0;i<dwpot->GetXMAX();i++) { // fill histogram
    for(int j=0;j<dwpot->GetYMAX();j++) {
      whist->SetBinContent(i+1,j+1,dwpot->Getwpot(j,i));
    }
  }
  return whist;
}

TH2D* WeightfieldClass::getHistDFieldX()
{
  int binsx=dwpot->GetXMAX(); // binsx = number of bins on x-axis
  int binsy=dwpot->GetYMAX(); // binsy = number of bins on y-axis

  TH2D *hFieldX=new TH2D("HistDFieldX"," ; x [um]; y [um]",binsx,0,binsx,binsy, 0,binsy);

  for(int i=0;i<(int) binsx;i++) {
    for(int j=0;j< (int) binsy;j++) {
      hFieldX->SetBinContent(i,j,dField[j][i].GetFieldx()); // SI
    }
  }

  return hFieldX;
}

TH2D* WeightfieldClass::getHistDFieldY()
{
  int binsx=dwpot->GetXMAX(); // binsx = number of bins on x-axis
  int binsy=dwpot->GetYMAX(); // binsy = number of bins on y-axis

  TH2D *hFieldY=new TH2D("HistDFieldY"," ; x [um]; y [um]",binsx,0,binsx,binsy, 0,binsy);

  for(int i=0;i<(int) binsx;i++) {
    for(int j=0;j< (int) binsy;j++) {
      hFieldY->SetBinContent(i,j,dField[j][i].GetFieldy()); // SI
    }
  }
  return hFieldY;
}

TH2D* WeightfieldClass::getHistWFieldX()
{
  int binsx=dwpot->GetXMAX(); // binsx = number of bins on x-axis
  int binsy=dwpot->GetYMAX(); // binsy = number of bins on y-axis

  TH2D *hFieldX=new TH2D("HistWFieldX"," ; x [um]; y [um]",binsx,0,binsx,binsy, 0,binsy);

  for(int i=0;i<(int) binsx;i++) {
    for(int j=0;j< (int) binsy;j++) {
      hFieldX->SetBinContent(i,j,wField[j][i].GetFieldx()); // SI
    }
  }

  return hFieldX;
}

TH2D* WeightfieldClass::getHistWFieldY()
{
  int binsx=dwpot->GetXMAX(); // binsx = number of bins on x-axis
  int binsy=dwpot->GetYMAX(); // binsy = number of bins on y-axis

  TH2D *hFieldY=new TH2D("HistWFieldY"," ; x [um]; y [um]",binsx,0,binsx,binsy, 0,binsy);

  for(int i=0;i<(int) binsx;i++) {
    for(int j=0;j< (int) binsy;j++) {
      hFieldY->SetBinContent(i,j,wField[j][i].GetFieldy()); // SI
    }
  }
  return hFieldY;
}

WeightfieldClass::~WeightfieldClass()
{
  for (int i = 0; i < (dwpot->GetYMAX()); i++) delete  dField[i];
  delete dField;
  delete dwpot;
}



