#include "Riostream.h"
#include "TLatex.h"
#include "TApplication.h"
#include "TH1.h"
#include "Carriers.h"
#include "TThread.h"
#include "TRandom3.h"
#include "TMutex.h"
#include "TDatime.h"
#include "TFile.h"
#include "TEllipse.h"
#include "TColor.h"
#include <string.h>
#include <sstream>

#include "Potentials.h"
#include "Field.h"

class WeightfieldClass
{
private:
  Int_t XMAX, YMAX;
  Double_t pitch, width;
  Int_t stripDop, bulkDop;
  Double_t biasVolt, depVolt;
  Potentials *dwpot;
  Field **wField;   // weighting field
  Field **dField;   // drift field


public:
  WeightfieldClass(); // Default constructor
  ~WeightfieldClass();
  WeightfieldClass(int _XMAX, int _YMAX, double _pitch, double _width, int _stripDop, int _bulkDop, double _biasVolt, double _depVolt );
  // important methods
  void calculatePotentials();
  void calculateFields();
  // set and get methods for class members
  int getXMAX();
  int setYMAX();
  void setXMAX(int _XMAX);
  void setYMAX(int _YMAX);
  double getPitch();
  double getWidth();
  void setPitch(double _pitch);
  void setWidth(double _width);
  int getStripDop();
  int getBulkDop();
  void setStripDop(int _stripDop);
  void setBulkDop(int _bulkDop);
  double getBiasVolt();
  double getDepVolt();
  void setBiasVolt(double _biasVolt);
  void setDepVolt(double _depVolt);
  // get histograms methods
  TH2D *getHistDPot();
  TH2D *getHistWPot();
  TH2D *getHistDFieldX();
  TH2D *getHistDFieldY();
  TH2D *getHistWFieldX();
  TH2D *getHistWFieldY();

};
