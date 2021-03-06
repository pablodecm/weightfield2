// C++ includes
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <getopt.h>
// ROOT includes
#include "TH1.h"
#include "TFile.h"
#include "TString.h"
// Weightfield includes
#include <WeightfieldClass.h>


using namespace std;
void saveHistsToFile(TString fname, WeightfieldClass* weightF );
void display_usage();

int main(int argc, char **argv)
{

    // Default values
    int XMAX = 3;
    int YMAX = 300;
    double pitch = 80.;
    double width = 30.;
    int stripDop = 1;
    int bulkDop = 0;
    double biasVolt = 300.;
    double depVolt = 85.;
    TString fname = "wdhistos.root";

    int c;

    while (1)
    {
      static struct option long_options[] =
      {
        {"XMAX", required_argument, 0, 'x'},
        {"YMAX", required_argument, 0, 'y'},
        {"pitch", required_argument, 0, 'p'},
        {"width", required_argument, 0, 'w'},
        {"stripDop", required_argument, 0, 's'},
        {"bulkDop", required_argument, 0, 'b'},
        {"biasVolt", required_argument, 0, 'v'},
        {"depVolt", required_argument, 0, 'd'},
        {"fname", required_argument, 0, 'o'},
        {"help", no_argument, 0, 'h'}
      };
      int option_index = 0;

      c = getopt_long (argc, argv, "x:y:p:w:s:b:v:d:o:h",long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
      {
        case 'x':
          XMAX = atoi(optarg);
          break;
        case 'y':
          YMAX = atoi(optarg);
          break;
        case 'p':
          pitch = atof(optarg);
          break;
        case 'w':
          width = atof(optarg);
          break;
        case 's':
          stripDop = atoi(optarg);
          break;
        case 'b':
          bulkDop = atoi(optarg);
          break;
        case 'v':
          biasVolt = atof(optarg);
          break;
        case 'd':
          depVolt = atof(optarg);
          break;
        case 'o':
          fname = TString(optarg);
          break;
        case 'h': /*Intentional fall-through */
        case '?':
          /* getopt_long already printed an error message. */
          display_usage();
          return 0;
          break;
        default:
           abort ();
      }
    }

    // Set class and calculate fields and potentials
    WeightfieldClass* weightF = new WeightfieldClass(XMAX, YMAX, pitch, width, stripDop, bulkDop, biasVolt, depVolt);
    weightF->calculatePotentials();
    weightF->calculateFields();

    // Save histograms to file
    saveHistsToFile(fname, weightF);

    return 0;
}

void saveHistsToFile(TString fname, WeightfieldClass* weightF ){
  // Open file and get hists
  TFile *fout  = new TFile( fname ,"recreate" );
  TH2D* histDPot = weightF->getHistDPot();
  TH2D* histWPot = weightF->getHistWPot();
  TH2D* histDFieldX = weightF->getHistDFieldX();
  TH2D* histDFieldY = weightF->getHistDFieldY();
  TH2D* histWFieldX = weightF->getHistWFieldX();
  TH2D* histWFieldY = weightF->getHistWFieldY();
  // Write everything to file and close
  histDPot->Write();
  histWPot->Write();
  histDFieldX->Write();
  histDFieldY->Write();
  histWFieldX->Write();
  histWFieldY->Write();
  fout->Close();
}

void display_usage()
{
  printf("weighfield3-cli usage: \n");
  printf("  --XMAX or -x : number of strips to simulate \n");
  printf("  --YMAX or -y : height of the detector (mum)  \n");
  printf("  --pitch or -p : strip pitch (mum) \n");
  printf("  --width or -w : strip width (mum) \n");
  printf("  --stripDop or -s : strip doping type 1=p and 0=n \n");
  printf("  --bulkDop or -b : bulk doping type 1=p and 0=n  \n");
  printf("  --biasVolt or -v : bias voltage (V) \n");
  printf("  --depVolt or -d : depletion voltage(V) \n");
  printf("  --fname or -o : root output file name \n");
  printf("  --help or -h : display usage \n");
}
