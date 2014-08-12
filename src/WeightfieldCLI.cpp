// C++ includes
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
// ROOT includes
#include "TH1.h"
#include "TFile.h"
#include "TString.h"
// Weightfield includes
#include <WeightfieldClass.h>


using namespace std;
void saveHistsToFile(TString fname, WeightfieldClass* weightF );

int main(int argc, char **argv)
{
    int opt,a,b,c;
    while ((opt = getopt(argc,argv,"abc:d")) != EOF)
        switch(opt)
        {
            case 'a': a = 1; cout <<" a is enabled"<<a <<endl; break;
            case 'b': b = 1; cout <<" b is enabled"<<b <<endl; break;
            case 'c': c = 1; cout << "value of c is"<< optarg <<endl ; break;
            case '?': fprintf(stderr, "usuage is \n -a : for enabling a \n -b : for enabling b \n -c: <value> ");
            default: cout<<endl; abort();
        }


    WeightfieldClass* weightF = new WeightfieldClass();
    weightF->calculatePotentials();

    weightF->calculateFields();


    saveHistsToFile("wdhistos.root", weightF);


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
