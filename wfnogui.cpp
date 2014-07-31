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
#include "Carriers.h"

void DrawFieldHist( Potentials &dwpot , Field **df , TH2D *wdhist, TH2D *wfhist ) ;

typedef enum ParticleID_enum {
	MIPunif,
	MIPnonunif,
	MIPlandau,
	ALPHA_UNDEF,
	ALPHA_TOP,
	ALPHA_BOTTOM,
	USR_CHARGE
} ParticleID ;

int main( int argc, char *argv[] ) {

  Int_t XMAX = 3 , YMAX=300 ;
  Double_t pitch=80 , width = 30 ;
  Potentials dwpot = Potentials(YMAX,XMAX,pitch,width) ;
 
  /* CALCULATE POTENTIALS() --------------------------------------*/

  // Strips and bulk    1=p,0=n (grep NTYPE Potentials.h)
  dwpot.SetDoping(1,0) ; 


	//CarriersInNumberentry->SetLimitValues(0,dwpot.GetXMAX());	???
 
  //Bias,depletion voltage
  dwpot.SetV(300.,85.);

  //Reset potentials to zero as a precaution	
  for(int i=0; i<dwpot.GetXMAX(); i++) {			

     for(int j=0; j<dwpot.GetYMAX(); j++) {
	     dwpot.SetwPotential(j,i,0.0);
	     dwpot.SetdPotential(j,i,0.0);
     }

  }		

  //This is what you run when you click bottom " Set "
  dwpot.SetBoundaryConditions() ;  
  
  dwpot.Multigrid(  ) ;

  /* End of CallCalculatePotentials() --------------------------------------*/





  
  /* CALCULATE ELECTRIC FIELD -------- CALLCALCULATEFIELDS -----------------*/
  
  Field **wf;	  // weighting field
  Field **df;	  // drift field

  df = new Field*[(dwpot.GetYMAX())];
  for (int i = 0; i < (dwpot.GetYMAX()); i++) { //Program goes very slow if inlined
    df[i] = new Field[(dwpot.GetXMAX())];
  }

  wf = new Field*[(dwpot.GetYMAX())];
  for (int i = 0; i < (dwpot.GetYMAX()); i++) {
    wf[i] = new Field[(dwpot.GetXMAX())];	
  }
  	
  CalculateFields(dwpot,wf,df);
  CalculateAbsFields(dwpot,wf,df);	
	
  /* End of CallcalculateFields() --------------------------------------*/




   
  /* Saving histos for potentials,field */ 
  
  TH2D *dhist  = new TH2D("dhist"," ; x [um]; y [um]",dwpot.GetXMAX(),0,dwpot.GetXMAX(),dwpot.GetYMAX(), 0,dwpot.GetYMAX() );  
  TH2D *whist  = new TH2D("whist"," ; x [um]; y [um]",dwpot.GetXMAX(),0,dwpot.GetXMAX(),dwpot.GetYMAX(), 0,dwpot.GetYMAX() );  
  TH2D *wfhist = new TH2D("wfhist"," ",dwpot.GetXMAX(),0,dwpot.GetXMAX(),dwpot.GetYMAX(), 0,dwpot.GetYMAX() );  
  TH2D *dfhist = new TH2D("dfhist"," ",dwpot.GetXMAX(),0,dwpot.GetXMAX(),dwpot.GetYMAX(), 0,dwpot.GetYMAX() );  

  dhist->Reset();
  whist->Reset();

  for(int i=0;i<dwpot.GetXMAX();i++) { // fill histogram
	  for(int j=0;j<dwpot.GetYMAX();j++) {
		  dhist->SetBinContent(i+1,j+1,dwpot.Getdpot(j,i));
		  whist->SetBinContent(i+1,j+1,dwpot.Getwpot(j,i));
	  }
  }

  DrawFieldHist( dwpot, df , wfhist, dfhist );

  TFile *fout  = new TFile( "wdhistos.root" ,"recreate" ) ;
  dhist->Write(); 
  whist->Write(); 
  dfhist->Write(); 
  fout->Close();



  
  
 //  /* CALCULATE CURRENTS -------------------------------------------------- */
  
 //  ParticleID pID = MIPunif;
 //  Int_t XEntryPoint = (int) (pitch +0.5*width) ;
 //  Int_t Angle = 0 ;
  
 //  Double_t B=0.0;
 //  Double_t Temp=0.0;

 //  SetAlphaRange(RangeEntry->GetNumber());
 //  SetPrecision(PrecisionEntry->GetNumber());
 //  SetYGain(Gainentry->GetNumber());
 //  SetGainRatio(GainRatioentry->GetNumber());


 //  if(bfieldon==true) 
 //    {
 //      B = BfieldEntry->GetNumber();
 //    }

 //  Temp = TempEntry->GetNumber();
 //  cout << "Temperature set to = "<< Temp << endl;		

 //  //mfg:Oversizing an array, I dont think this should be needed
 //  dimMaxCarriers= 2*dwpot.Getmipcharge()+2000000;
 //  carriers =new Carriers[dimMaxCarriers];
  
 //  /* 
 //    I believe "batch" simulates several events of the same kind, for instance,
 //    several shots of MIPs 
 //  */
 //  int NBatch = 1 , radiobuttonstatus=0;

 //  for (int e=1; e<= NBatch; e++)
 //    {	
 //      cout << "Processing event " << e << endl;

 //      for (int i=0;i<dimMaxCarriers;i++) carriers[i].Setinside(-1);	//initialize carriers array

 //      carriersin = XEntryPoint;
 //      switch (pID){
 //      case MIPunif:
	// SetConstQFlag(1);
	// SetUniformQFlag(1);
	// dwpot.Setmipcharge(dwpot.GetYMAX()*75/cos( TMath::Pi()/180. * Angle));
	// CreateCharges(dwpot,carriers,carriersin,this);
	// break;

 //      case MIPnonunif:
	// SetConstQFlag(1);
	// SetUniformQFlag(0);
	// CreateCharges(dwpot,carriers,carriersin,this);
	// break;

 //      case MIPlandau:
	// SetConstQFlag(0);
	// SetUniformQFlag(0);
	// CreateCharges(dwpot,carriers,carriersin,this);
	// break;

 //      case ALPHA_TOP:
	// dwpot.Setmipcharge(ALPHACHARGE);
	// CreateChargesAlphaTop(dwpot,carriers,carriersin,this);

	// break;

 //      case ALPHA_BOTTOM:
	// dwpot.Setmipcharge(ALPHACHARGE);
	// CreateChargesAlphaBottom(dwpot,carriers,carriersin,this);
	// break;

 //      case USR_CHARGE:
	// SetConstQFlag(1);
	// SetUniformQFlag(1);
	// SetUserUniformQFlag(1);
	// dwpot.Setmipcharge(Chargeentry->GetNumber()*dwpot.GetYMAX()/cos(TMath::Pi()/180*GetAngle()));
	// CreateCharges(dwpot,carriers,carriersin,this);

	// break;

 //      default: break;
 //      }


 //      SetLorentz(dwpot,carriers,B, Temp, CallGetDetType());

 //      CalculateCurrents(dwpot,df,wf,carriers,this,e);
  
  
}

//-------------------------------------------------------

void DrawFieldHist( Potentials &dwpot , Field **df, TH2D *wfhist, TH2D *dfhist ) {

      int p=1;
      int a=0;
      int h=0;
      int binsx=dwpot.GetXMAX(); // binsx = number of bins on x-axis
      int binsy=dwpot.GetYMAX(); // binsy = number of bins on y-axis
      dfhist->Reset();	 //dfhist = histogram of drift field 
      //dfhist->TH2F::SetBins((int) (binsx/p),0,(binsx),(int) (binsy/p), 0.,(binsy) );

      for(int i=0;i<(int) binsx;i++) {
	h=  i;
	// cout << h << " : " ;
	for(int j=0;j< (int) binsy;j++) 
	  {
	    a= j;

	    //	      dfhist->SetBinContent(i,j,dwpot.Getdpot(a,h));
	    dfhist->SetBinContent(i,j,df[a][h].Getabs()/1e5);	    
	    //  cout << a << ","  << df[a][h].Getabs()/1e5 << "   " ;

	  }
	//	      cout << endl;

      }

      return;


}
/////////////////////////////////////////////////////////////////////////////////////////////
