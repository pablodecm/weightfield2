#include "WFGUI.h"
//////////////////////////////////////////////////////////////////////////////////////
Carriers::Carriers() {
	x=0;
	y=0;
	inside=-1;
	vx=0;
	vy=0;
	charge=0;
	leftovergain=0;
}
//////////////////////////////////////////////////////////////////////////////////////
void Carriers::Setx(double valx) {
	x=valx;
}
//////////////////////////////////////////////////////////////////////////////////////
void Carriers::Sety(double valy) {
	y=valy;
}
//////////////////////////////////////////////////////////////////////////////////////
void Carriers::Setinside(int val) {
	inside=val;
}
//////////////////////////////////////////////////////////////////////////////////////
int Carriers::Getinside() {
	return inside;
}
//////////////////////////////////////////////////////////////////////////////////////
double Carriers::Getx() {
	return x;
}
//////////////////////////////////////////////////////////////////////////////////////
double Carriers::Gety() {
	return y;
}
//////////////////////////////////////////////////////////////////////////////////////
void Carriers::Setetheta(double angle) {
	etheta=angle;
}
//////////////////////////////////////////////////////////////////////////////////////
void Carriers::Seththeta(double angle) {
	htheta=angle;
}
//////////////////////////////////////////////////////////////////////////////////////
double Carriers::Getetheta() {
	return etheta;
}
//////////////////////////////////////////////////////////////////////////////////////
double Carriers::Geththeta() {
	return htheta;
}
//////////////////////////////////////////////////////////////////////////////////////
void Carriers::SetGainPart(bool gain) {
	egain=gain;
}
//////////////////////////////////////////////////////////////////////////////////////
bool Carriers::GetGainPart() {
	return egain;
}
//////////////////////////////////////////////////////////////////////////////////////
void Carriers::SetVx(double velx){
	vx=velx;
}
//////////////////////////////////////////////////////////////////////////////////////
void Carriers::SetVy(double vely){
	vy=vely;
}
//////////////////////////////////////////////////////////////////////////////////////
double Carriers::GetVx(){
	return vx;
}
//////////////////////////////////////////////////////////////////////////////////////
double Carriers::GetVy(){
	return vy;
}
//////////////////////////////////////////////////////////////////////////////////////
void Carriers::SetCharge(int valcharge){
	charge=valcharge;
}
//////////////////////////////////////////////////////////////////////////////////////
int Carriers::GetCharge(){
	return charge;
}
//////////////////////////////////////////////////////////////////////////////////////
void Carriers::SetLeftovergain(double vallog=0){
	leftovergain=vallog;
}
//////////////////////////////////////////////////////////////////////////////////////
double Carriers::GetLeftovergain(){
  return leftovergain;
}
//////////////////////////////////////////////////////////////////////////////////////
double Vsat_e(double Temp, int DType){

  float vs = 0;
  if( DType == 0 ||  DType == 2)
    {

      double vso = 2.4e5;
      double C=0.8;
      double I=600;
      vs=vso/(1+C*exp(Temp/I));
    }
  else
    {
      
      vs=VHSAT_D_300;
    }
return vs;
}
//////////////////////////////////////////////////////////////////////////////////////
double Vsat_h(double Temp, int DType){

  float vs = 0 ;
  if( DType == 0 ||  DType == 2)
    {
      double vso = 2.4e5;
      double C=0.8;
      double I=600;
      vs= vso/(1+C*exp(Temp/I))*VHSAT_300/VESAT_300;      
    }
  else
    {
      vs=VESAT_D_300;
    }


return vs ;
}
//////////////////////////////////////////////////////////////////////////////////////
double Mu_e(double Temp, int DType){

  double mu = 0;
  if( DType == 0 ||  DType == 2)
    {
      mu = MU_E_300/pow(300,-2.4)*pow(Temp, -2.4);
    }
  else
    {
      mu = MU_E_D_300;
    }


  return mu ;
}
//////////////////////////////////////////////////////////////////////////////////////
double Mu_h(double Temp, int DType){

  double mu = 0;
  if( DType == 0 ||  DType == 2)
    {
      mu = MU_H_300/pow(300,-2.2)*pow(Temp, -2.2);
    }
  else
    {
      mu = MU_H_D_300;
    }

  return mu ;
}

//////////////////////////////////////////////////////////////////////////////////////
void CreateCharges(Potentials &pot, Carriers *carriers, int hit, void* wfgui)   // crea cariche con MIP
{     // create electrons and holes, evenly distributed	
  

  //  cout << "Choosing Incidente particle " << endl;
  
  WFGUI* gui= (WFGUI*) wfgui; // pointer to gui
  //  cout << "Detector Type = " << gui->CallGetDetType()<< endl;
  //  TFile Myf("dEdz_200_5mu.root");
  TFile Myf("Geant_Vin.root");
  int TotPairs = 0;

  int PairsPer5Micron[100];
  int ll = 0;
  int SumPairs=0;
  int SumSteps=0;
  int mm = 0;
  float LastBinRescale = 1;

  //  bool ConstQFlag = 0; // flag to fix the total charge at (75 pairs per micron)*d
  // bool UniformQFlag = 0; //flag to have a uniform charge distribution of 75 pairs per micron


  bool ConstQFlag = gui->GetConstQFlag(); // flag to fix the total charge at (75 pairs per micron)*d
  bool UniformQFlag = gui->GetUniformQFlag(); //flag to have a uniform charge distribution of 75 pairs per micron
  bool UserUniformQFlag = gui->GetUserUniformQFlag(); //flag to have a uniform charge distribution user defined
  bool CoutFlag = (gui->GetBatchOn() == false) ? 1:0; //flag to have a uniform charge distribution of 75 pairs per micron
  int chargescale = gui->GetPrecision();
  cout << "Tracking one e/h pair for every " << chargescale << " pairs" << endl;

  float NormConstFlag = 1.;

  
  gui->GetEllipse()->SetFillStyle(0);

  if (!gui->GetLess2DPlot())
    {
      pot.DriftPal();
      gui->Getcanvasp()->cd();
      gui->Getdhist()->Draw("COLZ");
    }
      double angle=0;
      //if (gui->GetAngle()==0) angle =0;
      if ( gui->GetAngle()<45) angle =gui->GetAngle()*(TMath::Pi()/180);
      else if (gui->GetAngle()>=45) 
	{
	  cout<<"Angle set at = 45 deg"<<endl;
	  angle=45*(TMath::Pi()/180);
	}
      cout<<"Incidente Angle = "<< gui->GetAngle() << " Degrees " << endl;
      int DistHisto = 5; //The histogram has the energy deposition every 5 microns  
      if (angle == TMath::Pi()/2) angle = 1.5;
      float Path = pot.GetYMAX()/cos(angle);
      
      if (Myf.IsOpen())  
    {
      //      Myf.ls();
      float EPair = 0;
      float AvPair = 0;
      TH1F * EDist;
	//	TH1F * EDist =  (TH1F*)Myf.Get("AllNL");

      if( gui->CallGetDetType() == 0 ||  gui->CallGetDetType() == 2)
	{
	  EDist =  (TH1F*)Myf.Get("Silicon_Vin");
	  EPair = 3.6;
	  AvPair = 75;
	}
      else
	{
	  EDist =  (TH1F*)Myf.Get("Diamond_Vin");
      	  EPair = 13.6;
	  AvPair = 36;
	}

      
      if (!ConstQFlag && !UniformQFlag && CoutFlag)  cout << "MIP non-uniform charge deposition and Landau amplitude " << endl;
      if (ConstQFlag && !UniformQFlag && CoutFlag)  cout << "MIP non-uniform charge deposition and Fixed amplitude " << endl;
      if ( !UserUniformQFlag && ConstQFlag &&CoutFlag)   cout << "MIP uniform charge deposition " << endl;
      if ( UserUniformQFlag && CoutFlag)   cout << "MIP uniform charge deposition with User Charge = " << gui->GetChargeEntry() << " pairs/micron" << endl;

      TotPairs = 0;

      for (int il =0; il<=Path; il+=DistHisto)
	{
	  if (!UniformQFlag)  
	    {
	      //		  if (!ConstQFlag && CoutFlag)  cout << "Pairs in each 5-um layer: " << endl;	  
	      float ran = EDist->GetRandom();
	      //	      PairsPer5Micron[ll] = ran*1000/(3.6);
	      PairsPer5Micron[ll] = ran*1e6/EPair;
	    }	      
	  else 
	    {
	      if (ConstQFlag && ! UserUniformQFlag)  
		{
		  PairsPer5Micron[ll] = AvPair*5.;		  
		}
	      else if (UserUniformQFlag)  
		{
		  PairsPer5Micron[ll] = gui->GetChargeEntry()*5.;
		}
	    }
	  if (il+DistHisto>Path) 
	    {
	      LastBinRescale = (1.-(double((il+DistHisto*cos(angle)-Path))/double(DistHisto*cos(angle))));
	      PairsPer5Micron[ll] *= LastBinRescale;
	      // cout << "Last bin rescaled by = " <<  LastBinRescale << endl;
	    }
	  //	      if (!ConstQFlag ) cout <<  " ll = " << ll << " " << PairsPer5Micron[ll]<< ", " ;
	  TotPairs += PairsPer5Micron[ll] ;
	  ll++;	    
	}
      

      if (ConstQFlag && !UniformQFlag) //
	{
	  if ( CoutFlag) cout << "MIP Non uniform Q with Qtot = " << AvPair << " pairs*um = " 
			      << AvPair*pot.GetYMAX()/cos(TMath::Pi()/180*gui->GetAngle()) << endl;
	  
	  NormConstFlag = float (AvPair*pot.GetYMAX()/cos(TMath::Pi()/180*gui->GetAngle()) )/float(TotPairs); //renormalization constant
	  // cout << "Scaling factor = " << NormConstFlag << endl;	      
	  // if ( CoutFlag) cout << "Pairs in each 5-um layer: " << endl;	  

	  cout << endl;
	  
	}
      
      //	  cout << "Total number of pairs for particle loop= " << TotPairs << endl;
      //      cout << "ll = " << ll << endl; 
      
      //Need to rescale the charges if the track is not perpendicular
      
      //	  for (mm = 0; mm<ll; mm++) PairsPer5Micron[mm] *= 1/cos(angle);
      
      SumPairs = 0;

      TotPairs  *=1./chargescale;

      for (mm = 0; mm<ll; mm++)
	{
	  float ypos = 0;
	  float Elypos = 10;
	  float xangle = 10;
	  PairsPer5Micron[mm] *=1./chargescale;
	  for (int i=SumSteps; i<SumSteps+PairsPer5Micron[mm]; i++)
	    {


	      ypos = mm*DistHisto*cos(angle)+((i-SumSteps)/double(PairsPer5Micron[mm]))*double(DistHisto*cos(angle));		  
	      xangle= hit - ypos*tan(angle);
	      //		  cout << "electron " << i << " at y = " << ypos << endl;
	      //rescale the last bin, as it's not always 5 micron long
	      if (mm == ll-1) ypos = 0.001+ mm*DistHisto*cos(angle)+((i-SumSteps)/double(PairsPer5Micron[mm]))*double(DistHisto*cos(angle)*LastBinRescale);
	      if (ypos>=pot.GetYMAX()-0.5 || ypos <0.5) continue; //remove particles at the very edges
	      Elypos =  ypos;

	      // super-electrons 
	      
	      carriers[i].Setx(xangle);
	      carriers[i].Sety(ypos);
	      //  
	      carriers[i].SetCharge(-1);
	      carriers[i].Setinside(1);    // yes
	      carriers[i].SetGainPart(false);  //is not a gain electron!
	      // cout << "super-holes" << endl;
	      
	      carriers[TotPairs+i].Setx(xangle);
	      carriers[TotPairs+i].Sety(ypos);
	      carriers[TotPairs+i].SetCharge(1);
	      carriers[TotPairs+i].Setinside(1);    // yes
	      carriers[TotPairs+i].SetGainPart(false); 	//is not a gain hole
	      //	      cout << "hole " << TotPairs +i << " at y = " << carriers[TotPairs+i].Gety() << endl;
	      //cout<<"pot.getymax= "<<pot.GetYMAX()<<endl;
	      SumPairs++;
	      
	      
	      
	    }

	  SumSteps +=PairsPer5Micron[mm];
	  //draw ellipses
	  
	  if (!gui->GetLess2DPlot())  gui->Getcanvasp()->cd();
	  //	  cout << "Calculating Ellipses " << mm << endl;
	  
	  double eradius= 2*PairsPer5Micron[mm]*0.003*chargescale;
	  //	  cout << Elypos << endl;
	  
	  if (!gui->GetLess2DPlot()) 
	    gui->GetEllipse()->DrawEllipse(xangle,Elypos,eradius,eradius,0.,360.,0.);

	}

      //      cout << "Updating Canvas " << endl;
      if( !gui->GetLess2DPlot()) gui->Getcanvasp()->Update();

      Myf.Close();
    }
  

  if ( CoutFlag)      cout << "Total number of pairs = " << SumPairs*chargescale << endl;
  pot.Setmipcharge(SumPairs);

  
}

//////////////////////////////////////////////////////////////////////////////////////
void CreateChargesAlphaTop(Potentials &pot, Carriers *carriers, int hit, void* wfgui)   // crea coppie eh da particelle alfa
{     // create electrons and holes, evenly distributed	

  WFGUI* gui= (WFGUI*) wfgui; // pointer to gui
  bool CoutFlag = (gui->GetBatchOn() == false) ? 1:0; 

  TEllipse *ellipse = new TEllipse();
  ellipse->SetFillStyle(0);

 
  double angle=0;
  if ( gui->GetAngle()<85) angle =gui->GetAngle()*(TMath::Pi()/180);
  else if (gui->GetAngle()>=85) 
  {
      cout<<"Maximum angle = 45 deg"<<endl;
      angle=45*(TMath::Pi()/180);
  }
    
  double xangle=0;
  if (!gui->GetLess2DPlot()) 
    {
      pot.DriftPal();
      gui->Getcanvasp()->cd();
      gui->Getdhist()->Draw("COLZ");
    }
      xangle=hit;

  for (int i=0; i<pot.Getmipcharge(); i++)
    {	
 
      carriers[i].Sety((double)pot.GetYMAX()- (i+10)/((double) pot.Getmipcharge())*gui->GetAlphaRange());
      carriers[i].Setx(xangle+(pot.GetYMAX()-carriers[i].Gety())*sin(angle)); 
      carriers[i].SetCharge(-1);
      carriers[i].Setinside(1);    // yes
      carriers[i].SetGainPart(false);  //is not a gain electron!
      // super-holes		
      carriers[pot.Getmipcharge()+i].Sety((double)pot.GetYMAX()-(i+10)/(pot.Getmipcharge())*gui->GetAlphaRange()-1);
      carriers[pot.Getmipcharge()+i].Setx(xangle+(pot.GetYMAX()-carriers[i].Gety())*sin(angle));	//posiziona la lacuna
      carriers[pot.Getmipcharge()+i].SetCharge(1);
      carriers[pot.Getmipcharge()+i].Setinside(1);    // yes
      carriers[pot.Getmipcharge()+i].SetGainPart(false); 	//is not a gain hole
      //      cout << pot.Getmipcharge()+i << " From Tot" << endl;
      
    }
  if ( CoutFlag)	cout << "Alpha range from " << carriers[1].Gety() << " to "<<carriers[pot.Getmipcharge()-1].Gety()<<endl;

  double eradius= 2*75*5*0.003;  

  for (int il = 0; il< gui->GetAlphaRange(); il++)
    {
      if ((int) il % 5 == 0 && 	 !gui->GetLess2DPlot() )
	{
	  gui->Getcanvasp()->cd();
	  ellipse->DrawEllipse(xangle+(il-2.5)*sin(angle), pot.GetYMAX()-il-2.5,eradius,eradius,0.,360.,0.);
	}
    }
  if (!gui->GetLess2DPlot())   cout << "Updating Canvas, Drawing Ellipses " << endl;
  if (!gui->GetLess2DPlot())   gui->Getcanvasp()->Update(); 
}
//////////////////////////////////////////////////////////////////////////////////////
void CreateChargesAlphaBottom(Potentials &pot, Carriers *carriers, int hit, void* wfgui)   // crea coppie eh da particelle alfa
{     
  WFGUI* gui= (WFGUI*) wfgui; // pointer to gui
  bool CoutFlag = (gui->GetBatchOn() == false) ? 1:0;
  
  TEllipse *ellipse = new TEllipse();
  ellipse->SetFillStyle(0);

  
  double angle=0;
    if ( gui->GetAngle()<45) angle =gui->GetAngle()*(TMath::Pi()/180);
  else if (gui->GetAngle()>=45) 
  {
      cout<<"Maximum angle = 45 deg"<<endl;
      angle=45*(TMath::Pi()/180);
  }
  cout<<"Angle = "<<angle<<endl;
  
  double xangle=0;
  if (!gui->GetLess2DPlot()) 
    {
      gui->Getcanvasp()->cd();
      pot.DriftPal();
      gui->Getdhist()->Draw("COLZ");
    }
  xangle=hit;
  
  for (int i=0; i<pot.Getmipcharge(); i++)
    {
  
      // super-electrons 
      
      carriers[i].Sety((i+10)/((double) pot.Getmipcharge() )*gui->GetAlphaRange()+1); 
      carriers[i].Setx(xangle + carriers[i].Gety()*sin(angle)); 	//posiziona l'elettrone nella x in cui la mip colpisce il rivelatore
      carriers[i].Setinside(1);    // yes
      carriers[i].SetCharge(-1);
      carriers[i].SetGainPart(false);  //is not a gain electron!
      

      carriers[pot.Getmipcharge()+i].Sety((i+10)/((double) pot.Getmipcharge() )*gui->GetAlphaRange());
      carriers[pot.Getmipcharge()+i].Setx(xangle+carriers[i].Gety()*sin(angle));	//posiziona la lacuna
      carriers[pot.Getmipcharge()+i].Setinside(1);    // yes
      carriers[pot.Getmipcharge()+i].SetGainPart(false); 	//is not a gain hole
      carriers[pot.Getmipcharge()+i].SetCharge(1);
 
      //      cout << carriers[pot.Getmipcharge()+i].Gety() << " " << carriers[pot.Getmipcharge()+i].Getx() << endl;

           
    }
      
  if ( CoutFlag)	cout << "Alpha range from " << carriers[1].Gety() << " to "<<carriers[pot.Getmipcharge()-1].Gety()<<endl;
  
  double eradius= 2*75*5*0.003;  
  
  for (int il = 0; il< gui->GetAlphaRange(); il++)
    {
      if ((int) il % 5 == 0 && !gui->GetLess2DPlot())
	{
	  gui->Getcanvasp()->cd();	  
	  ellipse->DrawEllipse(xangle+(il+2.5)*sin(angle), il+2.5,eradius,eradius,0.,360.,0.);
	}
    }
  if (!gui->GetLess2DPlot())   cout << "Updating canvas, Drawing Ellipses " << endl;
  if (!gui->GetLess2DPlot()) gui->Getcanvasp()->Update();

 
}
//////////////////////////////////////////////////////////////////////////////////////

void SetLorentz(Potentials &pot, Carriers *carriers, double b, double Temp, int DType) {
  double mu_e = Mu_e(Temp, DType);
  double mu_h = Mu_h(Temp, DType);
	for(int i=0; i<pot.Getmipcharge(); i++) {
		carriers[i].Setetheta(atan(mu_e*b)); //1.15
		carriers[i].Seththeta(0.0);
		carriers[i].SetCharge(-1);
	}
	for(int j=pot.Getmipcharge(); j<2*pot.Getmipcharge(); j++) {
		carriers[j].Seththeta(atan(mu_h*b)); //0.72
		carriers[j].Setetheta(0.0);		
		carriers[j].SetCharge(1);
	}	
}
//////////////////////////////////////////////////////////////////////////////////////
void CalculateCurrents(Potentials &pot, Field **dfield, Field **wfield, Carriers *carriers, void* wfgui, int Counter) //pot = un puntatore a dwpot!
{
	WFGUI* gui= (WFGUI*) wfgui; // pointer to gui

	//	const char* fileName = (gui->GetFileName())->c_str();
	const char* fileName = (gui->CallSetFileName())->c_str();

	stringstream ss;
	ss << fileName << "_TW";
	string  fileNameT = ss.str();
	const char* fileNameTW = fileNameT.c_str();

	stringstream ss1;
	ss1 << fileName << "_" <<  Counter;
	string  fileNameT1 = ss1.str();
	fileName = fileNameT1.c_str();


	FILE* pfile =  NULL;
	FILE* pfileTW =  NULL;
	if (gui->GetFileNameOn() ) 
	  {
	    if (gui->GetBatchOn()==false)
	      {
		pfile =  fopen(fileName, "w");	
		pfileTW =  fopen(fileNameTW, "w");	
	      }
	    else
	      {
		//		pfile=  fopen(fileName, "a+");	
		// pfileTW=  fopen(fileNameTW, "a+");	
		pfile=  fopen(fileName, "w");	
		pfileTW=  fopen(fileNameTW, "w");	
	      }
	    if (pfile==NULL) cout<<"Unable to open "<<fileName<< " file"<<endl;
	  }
	//	int tpath = (int)(0.000368984*pot.GetYMAX()*pot.GetYMAX()+7);	// estimate time in ns the particles need to leave detector
	
//number of time steps, simulation parameter: Total time = timeunit*tmax= 1.0e-12*3.0e4 = 30 ns

	double TIMEUNIT=1.0e-12;	//time step in s
	int TMAX=30000; //Nicolo //=10000;  


	if(gui->Getdiffusionon() && gui->GetT()!=0.0) {
		TIMEUNIT=1.0e-13;	// time unit for current calculation
		TMAX=300000;		// number of time steps
	}


	if (gui->GetBatchOn()==false)
	  {
	    gui->GetProgressBar()->SetMax(TMAX*TIMEUNIT*1e9);
	    gui->GetProgressBar()->Percent(0);
	    
	  }


	bool bStop=false;		//bool to join currentsthread
	double edpar=0;			// absolute value of E field
	double mu=0;				// carrier mobility
	double ynew=0;				// new y position of carrier after calculation
	double xnew=0;				// new x position of carrier after calculation
	double ex=0,ey=0;			// x and y component of drift field
	double qe=0,qh=0,qeh=0,qeg=0,qhg=0,qehg=0,qetot=0,qhtot=0,qtot=0;		// collected charge of electrons, holes and total
	double *itot=new double[TMAX];// total induced current
	double *ih=new double[TMAX];	// induced current of non gain holes
	double *ie=new double[TMAX];	// induced current of non gain electrons
	double *ieg=new double[TMAX];	// induced current of gain electrons
	double *ihg=new double[TMAX];	// induced current of gain holes
	double *ietot=new double[TMAX];//induced current of electrons
	double *ihtot=new double[TMAX];//induced current of holes
	double *ioscGraph=new double[TMAX];// total induced current for oscilloscope graph
	double *DioscGraph=new double[TMAX];// total induced current for oscilloscope graph
	double *qtot_sh=new double[TMAX];// total induced current for oscilloscope graph



	// double *ShaperGraph=new double[TMAX];// Shaper graph
	double d=0;
	double gain=0;

	int firstfreeslot=2*pot.Getmipcharge();

	//	cout << "FirstfreeSlot = " << firstfreeslot << endl;

	int carrierslimit=0;

	TRandom3 *xi;		// create random number
	TDatime *time;		// current time
	time = new TDatime();
	xi = new TRandom3(time->TDatime::GetTime());  //seed = current time
	double k = xi->Uniform(); // uniformly distributed random number
	double rand = xi->Uniform();

	double vabs=0,rx=0,ry=0,vdiffy=0,vdiffx=0; // absolute value of diffusion velocity
	double me = 0.26*m0; // mass of electron see: DOI: 10.1109/TNS.2009.2021426
	double mh = 0.36*m0; // mass of holes
	double Temp = gui->GetT();
	double m=0;  //temporary mass value

	//Nicolo'
	int IMax = 0; //Max index of currents
	int IMaxSh = 0; //Max index of currents
	double TimeMax = 0; //Max time of window
	double tau=0;
	double Jitter = 0;

	int chargescale = gui->GetPrecision();
	
	//	int dop=1;
	// if(pot.GetDoping()==true) dop=1;	// if strips are PTYPE, set dop = 1
	// if(pot.GetDoping()==false) dop=-1;	// if strips are NTYPE

	double theta1=carriers[1].Getetheta(); // lorentz angle of electrons
	double theta2=carriers[pot.Getmipcharge()+1].Geththeta(); // lorentz angle of holes

	//	cout << " Theta1 "<< theta1 << " theta2 " << theta2 << endl;
	for(int i=0; i<TMAX; i++) { // initialize currents to zero
	      itot[i]=0.0;	  
	      ie[i]=0.0;
	      ih[i]=0.0;
	      ieg[i]=0.0;
	      ihg[i]=0.0;
	      ihtot[i]=0.0;
	      ietot[i]=0.0;
	      ioscGraph[i]=0.0;	  
	      DioscGraph[i]=0.0;	  
	      qtot_sh[i]=0.0;	  
	}
	
	Field** tempdf1;	 // temporary field for electrons
	tempdf1 = new Field*[(pot.GetYMAX())]; // allocate memory
	for (int i = 0; i < (pot.GetYMAX()); i++) tempdf1[i] = new Field[(pot.GetXMAX())];
	tempdf1=RotateField(pot,dfield,theta1); // rotate field vectors about theta1 for calculation of drift velocity

	Field** tempdf2; // temporary field for holes
	tempdf2 = new Field*[(pot.GetYMAX())]; // allocate memory
	for (int i = 0; i < (pot.GetYMAX()); i++) tempdf2[i] = new Field[(pot.GetXMAX())];
	tempdf2=RotateField(pot,dfield,theta2); // rotate field vectors about theta2 for calculation of drift velocity

	//	double y_max;
	// int y_lgad;

	//	y_max = gui->GetYMax();
	// y_lgad = y_max- GAINDEPTH - gui->GetYGain();
	
	
	//	double prop=pot.GetYMAX()/y_max; 	//multiplication constant for steps
	
// 	if(gui->GetBatchOn()==false)
// 	    cout<<"coefficient of expansion = "<<prop<<endl;
	
//	y_lgad=(int)(y_lgad*prop);		//gain layer extends from 
	
	int totaltime = TMAX;	//total simulation time, IT CAN BE CHANGED
	float ygainlow= gui->Getygainlow();
	float ygainhigh= gui->Getygainhigh();
	float GConst = 0;
	GConst = gui->GetGainRatio();

	//	cout << gui->Getygainlow() << " " << gui->Getygainhigh() << endl;


	//	if(dop == 1) 	// if strips are PTYPE
	// {
	//   ygainlow= pot.GetYMAX()- ygainhigh;
	//   ygainhigh=ygainlow+GAINLENGHT;
	// }

	// cout <<"ygainlow = "<< " " <<" ygainhigh = "<<<<endl;
	if(gui->GetBatchOn()==false)
	{
	  
	    	if (gui->GetGainon()==true )
		{
		    cout<<"Detector with internal gain. Gain layer extends from "<<ygainlow<<" to "<< ygainhigh <<endl;
		    cout << "Ratio e/h gain = " <<  gui->GetGainRatio() << endl;

		}
		else
		{
		    cout<<"Detector without internal gain. "<<endl;
		}
	    
	}
	double mu_e = Mu_e(Temp, gui->CallGetDetType());
	double mu_h = Mu_h(Temp, gui->CallGetDetType());
	double sq_mu_e_over_v_e = pow(mu_e/Vsat_e(Temp, gui->CallGetDetType()),2);
	double mu_h_over_v_h = mu_h/Vsat_h(Temp, gui->CallGetDetType());

	if(gui->GetBatchOn()==false)
	{
	    cout << "Mobility e = " << mu_e << " [m2/(V*s)]" << " Mobility h = " << mu_h << " [m2/(V*s)]"<< endl;
	    cout << "V Saturation:  e = " << Vsat_e(Temp, gui->CallGetDetType()) << " [m/s]"<<  "  h = " << Vsat_h(Temp,gui->CallGetDetType() ) << " [m/s]"<<endl;
	    cout << "Maximum time limit simulated " << TMAX*TIMEUNIT*1e9 << " nanoseconds "<<endl;
	}
	
	double dist = 0;
	//double InitX = 0;
	//double InitY = 0;
	double itot_max= 0;
	double vout_max= 0;
	double vout04= 0;
	double csa_max= 0;
	double sh_max= 0;

	float DielConst;
	if( gui->CallGetDetType() == 0 ||  gui->CallGetDetType() == 2)
	  {
	    DielConst = 11.9;
	  }
	else
	  {
	    DielConst = 5.7;
	  }
	
	double DCap = (gui->GetSWidth()*gui->GetSWidth())/pot.GetYMAX()*DielConst*8.854e-3; //fF Backplane
	DCap += 0.017*DielConst*4*gui->GetSWidth(); //fF side
	DCap += 50; //fF fixed


	double TRise = gui->GetTRise()*1e-9;
	double TFall = gui->GetTFall()*1e-9;

	double TauRise = TRise/2.2;
	double TauFall = TFall/2.2;
	// double TauShaper = (500+DCap)*1e-15; // second (500 fF input impedence of the CSA)
	//	cout << " Tau Shaper " << TauShaper << " tau rise " << TauRise << endl; 


	// Note time rise = 0.35/BW, tau = time rise/2.2
	tau = 2.2*(double (0.35/(1.0e9*gui->GetOscBW())));

	double Noise =  2.*DCap/sqrt(TRise*1e9); // ~ 300 electron of Noise at 5.5 ns shaping time (NA62)
	//	double Coeff = 2.8*1e-09; //Shaper to electron constant
	double VTh = gui->GetVth()*Noise*1.6*1e-4; // in fQ with Noise in electrons
	// if (gui->GetGainon()) VTh *= gui->GetYGain();
	bool FVTh = true;

	if (gui->GetOscOn()==true  && gui->GetBatchOn()==false)
	  {
	    cout << "Oscilloscope  BW = " << gui->GetOscBW() << " GHz" << endl;
	    cout << "Shaper Int. Time (10-90)= " <<  TRise*1e9 << " ns" << endl;
	    cout << "Shaper Fall Time = " << TFall*1e9 << " ns" << endl;
	    cout << "Noise = " << Noise << " ENC" << endl;
	    cout << "Shaper Threshold set at VTh = " << VTh << " fQ" << endl;

	  }
	double STime = 0;	
	int IintTime= 0;
	//	int IintTimeSh= 0;
	// int IintTimeSCA= 0;

	if (gui->GetOscOn()==true)
	  {
	    IintTime =  2*(TRise+TFall)/double(TIMEUNIT);	//Extra bins for integration time

	    //	    IintTime =  4*TRise/double(TIMEUNIT);	//Extra bins for integration time
	    //	    IintTimeSh =  4*TFall/double(TIMEUNIT);	//Extra bins for integration time
	 //   IintTimeSCA =  40*TauShaper/double(TIMEUNIT);	//Extra bins for integration time
	  }

	for (int i=0; i<totaltime; i++)	
	  {
	    if (gui->Getstopped()==true) {
	    bStop=true;
	    break;
	    }
	    //	    cout << i << " totaltime" << endl;
	    //if(i==2) return;
	    
	    // sweep over all supercharges
	    
	    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	    carrierslimit=firstfreeslot-1;

	    //	    cout<< "t= "<<i<<" firstfreeslot="<<firstfreeslot<<endl;

	    for (int j=0; j<carrierslimit ; j++)

	      {
		
		// only if the charge is still inside the detector and if it is initialized

		if (carriers[j].Getinside()!=0 && carriers[j].Getinside()!=-1)	//if charges are inside the detector
		  {
		    // calculate drift velocity
		    
		    if (carriers[j].GetCharge()==-1)	//electrons
		      {

			ex=tempdf1[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldx();  // x component of rotated E-field
			ey=tempdf1[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldy(); // y component of rotated E-field
			edpar=sqrt(ex*ex+ey*ey);	// calculate abs. value of field
			mu=(0.9*mu_e/sqrt(1+sq_mu_e_over_v_e*edpar*edpar)); // electron mobility
			m=me;
			// if (j == 200 ) cout << "field in the y direction " << dfield[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldy() << endl;

			//Electrons drift toward higher Potential, so against E = -dV/dy
		

			//			carriers[j].SetVy(mu*ey); 
			carriers[j].SetVy(-mu*ey); 
			carriers[j].SetVx(-mu*ex); // carrier velocity			
		      }
		    else					//holes
		      {
			ex=tempdf2[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldx(); // x component of rotated E-field
			ey=tempdf2[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldy(); // y component of rotated E-field
			edpar=sqrt(ex*ex+ey*ey);	// calculate abs. value of field						
			mu=(1.1*mu_h/(1+mu_h_over_v_h*edpar)); // hole 
			m=mh;
			//Holes drift toward lower Potential, so with E = -dV/dy
			//		carriers[j].SetVy(-mu*ey);
			carriers[j].SetVy(mu*ey);
			carriers[j].SetVx(mu*ex); // carrier velocity			
	
		      }
		    //std::cout << "electrons " << mu << std::endl;

		    //		    cout<<"vx=" <<carriers[j].GetVx()<<", vy="<<carriers[j].GetVy()<<endl;
		    
		    // Diffusion only outside the gain layer

		    if(gui->Getdiffusionon() && (carriers[j].Gety() <ygainlow))		     
		      // || (carriers[j].Gety() > ygainhigh )))
		    {
			//calculate diffusion velocity
			rand = xi->Uniform();		
			// create uniformly distributed random number [0,1]	
			vabs = sqrt(2*kB*Temp/m*log(fabs(1/(1-rand))));  // [m/s]
			// create boltzmann distributed random number (= abs. value of diffusion velocity)
			k = xi->Uniform()*2*M_PI;	
			// create uniformly distributed random number [0,2pi]
			rx = cos(k);				
			// create random directional vector (rx,ry) of diffusion velocity, abs.value=1
			ry = sin(k);
			vdiffx = vabs*rx;			
			// determine x and y component of diffusion velocity
			vdiffy = vabs*ry;
			
			
			carriers[j].SetVx(carriers[j].GetVx()+vdiffx) ;		// determine total (drift+diffusion) velocity
			carriers[j].SetVy(carriers[j].GetVy()+vdiffy) ;
		     }

		    // calculate induced currents using the dot product of velocity and weighting field
		    if (carriers[j].GetGainPart()==false)
		    {	
			if (carriers[j].GetCharge()==-1)
			  ie[i]+=(-1)*chargescale*ECHARGE*(wfield[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldx()*carriers[j].GetVx() 
					       + wfield[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldy()*carriers[j].GetVy());   
			else
			  ih[i]+=chargescale*ECHARGE*(wfield[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldx()*carriers[j].GetVx() 
					  + wfield[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldy()*carriers[j].GetVy());
		    }
		    else
		    {
			if (carriers[j].GetCharge()==-1)
			  ieg[i]+=(-1)*chargescale*ECHARGE*(wfield[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldx()*carriers[j].GetVx() 
						+ wfield[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldy()*carriers[j].GetVy());   
			else
			  ihg[i]+=chargescale*ECHARGE*(wfield[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldx()*carriers[j].GetVx() 
					   + wfield[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldy()*carriers[j].GetVy());
		    }
		    

		    //	    if (i == 10)
		    //  cout << j << " ie " << ie[i] << " ih " << ih[i] << " x,y= " << (int)carriers[j].Gety() << ","<< (int) carriers[j].Getx() 
		    //	   <<  " Ey " << wfield[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldy() << " Vy= " << carriers[j].GetVy() << endl;
		    // calculate the new position due to motion in the drift field = drift for electrons and holes
		    ynew=carriers[j].Gety()+carriers[j].GetVy()*TIMEUNIT/GRIDDIST;
		    xnew=carriers[j].Getx()+carriers[j].GetVx()*TIMEUNIT/GRIDDIST;
		    

		    if(xnew<=0) xnew=pot.GetXMAX()+xnew;     // periodic boundary conditions
		    if(xnew>=pot.GetXMAX()) xnew=0.0+(xnew-pot.GetXMAX());
		    if (j==2000)
		    {
			//if (i == 0)
			//{
			 //   InitX = xnew;
			  //  InitY = ynew;
			//}
			if (i>0) dist += sqrt( pow(carriers[j].GetVy()*TIMEUNIT/GRIDDIST,2) + pow(carriers[j].GetVx()*TIMEUNIT/GRIDDIST,2));
			//   cout << " x = " << xnew << " y  = " << ynew << " distance = "<< dist 
			// <<" Vx = " << carriers[j].GetVx() << " Vy = " << carriers[j].GetVy() << endl;

		    }
		    // check if charge is already outside of detector
		    //*** fine ynew xnew
		    
		    double yold =  carriers[j].Gety();
		    int ngain = 0;
		    d = 0;
		    bool ynewInside = false;
		    bool yoldInside = false;

		    if	(ynew> ygainlow && ynew< ygainhigh) ynewInside = true;		   
		    if	(yold> ygainlow && yold< ygainhigh) yoldInside = true;		   


		    // GAIN Cycle
		    
		    //Enters this cycle only if gain !=0 


		    if (gui->GetGainon() &&   (ynewInside || yoldInside) )
		      //			( (ynew> ygainlow && ynew< ygainhigh)  || (yold> ygainlow && yold< ygainhigh) ))
		      {
			// ynew AND yold inside
			if ( ynewInside && yoldInside )
			  {
			    //gain distance= new-old					    
			    d= fabs(ynew-carriers[j].Gety()) ;						    
			  }
			// ynew inside, yold outside
			    //			else if ( ynewInside && (yold< ygainlow || yold > ygainhigh) )
			else if ( ynewInside && !yoldInside )
			  {
			    if (yold< ygainlow) 
			      d = fabs(ygainlow-ynew);
			    else if (yold > ygainhigh) 
			      d = fabs(ygainhigh-ynew);
			  }
			// new outside, old inside
			    //			else if  ( ( ynew < ygainlow || ynew > ygainhigh)  && (yold > ygainlow && yold < ygainhigh) ) 
			else if  ( !ynewInside  && yoldInside ) 
			  {
			    if (ynew< ygainlow) 
			      d = fabs(ygainlow-yold);
			    else if (ynew > ygainhigh) 
			      d = fabs(ygainhigh-yold);
			  }
			
	
			//theory
			/*double Alpha_Const = 1.8*pow(10,-35);
			  float Alpha = pow(LocalE,7)*Alpha_Const*1e-4; //Alpha in micron
			  gain= gui->GetYGain()*(TMath::Exp(Alpha*d) -1.);*/
			//Alpha_Const = 1.8*pow(10,-5);
			//Alpha = 2*500*pow(LocalE,1)*Alpha_Const*1e-4; //Alpha in micron
			
			
			
			double LocalE = fabs(dfield[(int)carriers[j].Gety()][(int)carriers[j].Getx()].GetFieldy())/1e2;

			//			if (j == 200) cout << " Local E" << LocalE << endl;
			//if (j % 1000 == 0) cout << j << endl;
			
			//torino lab
			double Alpha_Const=0;
			float Alpha=0;
			Alpha_Const = .2*pow(10,-5); //Alpha in micron

			Alpha = LocalE*Alpha_Const*d;
			if (carriers[j].GetCharge()==1) Alpha *= GConst;

			gain=sqrt(gui->GetYGain()-1.)*Alpha;
			
			
			gain += carriers[j].GetLeftovergain();			
			ngain=gain;
			carriers[j].SetLeftovergain(gain-ngain);
			
			//  cout << " j = " << j << " Gain = " << gain << " ngain = " << ngain << " Leftover = " << gain-ngain << endl;
			// Deal with the left over gain
			
			if (ynew > ygainhigh || ynew < ygainlow ) 
			  {
			    if (xi->Uniform() < (gain-ngain))
			      {
				ngain++;					  
			      }
			    carriers[j].SetLeftovergain(0);
			  }
			

			if (firstfreeslot > gui->GetDimMaxCarriers()) 
			  {
			    cout << " Warning: " << endl;
			    cout << " Carriers array at the limit, the program stops" << endl;
			    cout << " Reduce your Gain Scale " << endl;
			    cout << " You created more than "  << firstfreeslot/2 << " particles" << endl;
			    return;
			  }
			

			for (int k=0;k<ngain; k++)
			  {
			    //cout<<"firstfreeslot= "<<firstfreeslot<<endl;
			    carriers[firstfreeslot].SetCharge(-1);	//creates gain electrons
			    carriers[firstfreeslot].SetGainPart(true);
			    carriers[firstfreeslot].Setinside(1);
			    carriers[firstfreeslot].Sety(ynew);
			    carriers[firstfreeslot].Setx(xnew);
			    
			    carriers[firstfreeslot+1].SetCharge(1);	//creates gain holes
			    carriers[firstfreeslot+1].SetGainPart(true);
			    carriers[firstfreeslot+1].Setinside(1);
			    carriers[firstfreeslot+1].Sety(ynew);
			    carriers[firstfreeslot+1].Setx(xnew);
			    firstfreeslot=firstfreeslot+2;
			  }//endfor
			//			cout << "Outside gain layer loop" << endl;
		//	ngain = 0;
			
		      } //end if gain is set active in gui
		    

		    //if ((ynew<0.0)||(ynew>y_max))		
		    if ((ynew<1.0)||(ynew>pot.GetYMAX()-1))
		      carriers[j].Setinside(0);   // if yes, remove it from further calculations
		    else
		      {
			carriers[j].Setx(xnew);	  // otherwise update position
			carriers[j].Sety(ynew);
		      }
		    //cout<<"x= "<<carriers[j].Getx()<<", y= "<<carriers[j].Gety()<<endl;
		    //		    cout<<" time = " << i << " particle j = "<<j<<endl;
		    
		  }//end if charges inside detector

	      }//end for carrierslimit drift

	    //	    cout<<"t= "<<i<<" ie= "<<ie[i]<<" ih= "<<ih[i]<<" ieg= "<<ieg[i]<<" ihg= "<<ihg[i]<<endl;
	    
	    itot[i]=ie[i]+ih[i]+ieg[i]+ihg[i];		// calculate total sum
	    ietot[i]=ie[i]+ieg[i];
	    ihtot[i]=ih[i]+ihg[i];
	    qe+=ie[i]*TIMEUNIT;			// sum up collected charge due to electrons motion
	    qh+=ih[i]*TIMEUNIT;			// sum up collected charge due to holes motion
	    qeh+=(ie[i]+ih[i])*TIMEUNIT;
	    qeg+=ieg[i]*TIMEUNIT;			// sum up collected charge due to electrons motion
	    qhg+=ihg[i]*TIMEUNIT;			// sum up collected charge due to gain holes motion
	    qehg+=(ieg[i]+ihg[i])*TIMEUNIT;
	    qetot+=ietot[i]*TIMEUNIT;
	    qhtot+=ihtot[i]*TIMEUNIT;
	    qtot+=itot[i]*TIMEUNIT;		// sum up total collected charge
	    qtot_sh[i] = qtot; 
	    ioscGraph[i]=0;
	    DioscGraph[i]=0;
	    //CSAOut[i]=qtot;
	    
		//nicolo

	    IMax = i; 
	//    TimeMax =i*TIMEUNIT*1.2;

	    if (itot[i] == 0 && i >300) 
	    {
		if (itot[i-100] == 0 && itot[i-5] == 0)
		{
		    if (gui->GetBatchOn()==false)
			cout << "Current at 0 in  = " << IMax*TIMEUNIT*1e9 << " nanoseconds"  << endl;
		    
		    break; // terminate loop as the current goes to 0
		}
	    }
	    	    	    	    
	    if (fabs(itot[i]) > itot_max) itot_max = fabs(itot[i]); 
   
	      if (gui->GetBatchOn()==false)
		{	
		  if (fabs(itot[i])>1e-9)  gui->GetProgressBar()->Increment(TIMEUNIT*1e9);
		}
	      
	}//end for loop t


	IMax =TMath::Min((int) (IMax+3.*tau/TIMEUNIT), TMAX); // It's an integer	
	TimeMax = IMax*TIMEUNIT; //It's a time 

	double* q; 				// x coordinate for graph Current(time)	
	q = new double[IMax]; 		// allocate memory	
	for(int k=0;k<IMax;k++)  q[k]=TIMEUNIT*(double)k; // set t coordinate
	
	double* qSh = 0; 	      	// x coordinate for graph Current(time)	
	double *ShaperOut= 0;
	double *ShaperOut_temp = 0;
	double *ShaperOut_der = 0;
	double *vout = 0 ;
	double *vout_temp = 0;
	double *CSAOut = 0;
	float MaxOsc = 0;
	float MinOsc = 0;
	int NMax_sh = 0;
	int NMax_sc = 0;
	int NMax04 = 0;

	
	

	//oscilloscope loop
	int TransImp = 9; //Transimpedance amplifier		  
	if (gui->GetOscOn()==true)  //if scope=on,
	  {
	    
	    // nicolo


	    
	    IMaxSh = IMax+IintTime; // Maximum number of bins for electronics
	    
	    
	    qSh = new double[IMaxSh]; 		// allocate memory
	    ShaperOut=new double[IMaxSh];//output voltage
	    ShaperOut_temp=new double[IMaxSh];//output voltage
	    ShaperOut_der=new double[IMaxSh];//output voltage
	    vout=new double[IMaxSh];//output voltage
	    vout_temp=new double[IMaxSh];//output voltage
	    CSAOut=new double[IMaxSh];//output voltage

	    
	    
	    for(int k=0;k<IMaxSh;k++)
	      {
		qSh[k]=TIMEUNIT*(double)k; // set t coordinate
		ShaperOut[k]=0.0;
		ShaperOut_temp[k]=0.0;
		ShaperOut_der[k]=0.0;		    
		vout[k]=0.0;
		vout_temp[k]=0.0;
		if (k<IMax && fabs(qtot_sh[k])>0)  CSAOut[k]=fabs(qtot_sh[k])*1e15;
		else   CSAOut[k] = fabs(qtot)*1e15;
		//	cout << CSAOut[k] << " " << k << endl;

	      }
	  	    
	    
	    int Step = 10;

	    for (int i=0;i<IMaxSh;i+=Step)
	      {
		for (int ll = 0; ll<i;ll+=Step)
		  {
		    if (i-ll>0) // shaper  integration
		      {
			ShaperOut_temp[i] += TransImp*Step*CSAOut[i-ll]*TMath::Exp(-ll*TIMEUNIT/TauRise)/(TIMEUNIT/TauRise)*
			  1./(1./((TIMEUNIT/TauRise)*(TIMEUNIT/TauRise))*(1-TMath::Exp(-IintTime*TIMEUNIT/TauRise)));   			
		      }		    
		  }
		//		cout << ShaperOut_temp[i] << " " << i << endl;

		float Qdif = 0;
		if (i== 0 ) Qdif  = itot[0] ; 
		else if (i>0 && i< IMax)    Qdif  = itot[i] - itot[i-Step];
		else Qdif = 0;
		
		for (int ll = 0; ll<IMaxSh-i;ll+=Step)  // valid only up to IMaxSh 
		  {		    
		    vout[i+ll] += Qdif*(1.-(TMath::Exp(-ll*TIMEUNIT/tau)));
		    if (i>Step) ShaperOut[i+ll] += (ShaperOut_temp[i]-ShaperOut_temp[i-Step])*TMath::Exp(-ll*TIMEUNIT/TauFall);
		    
		  }
		ShaperOut_der[i] = 1e-12/TIMEUNIT*((ShaperOut[i]-ShaperOut[i-Step]) )/(1e9*TIMEUNIT*Step); //[mV/ns]				    		    

		if (i*TIMEUNIT*1e9>0.37 && i*TIMEUNIT*1e9<0.4)    
		  {
		    vout04 = fabs(vout[i]); 
		    NMax04 = i;
		    //    cout << vout04 << " " << NMax04 << endl;
		  }
		
	      }




	    
	    // Fill in interpolation
	    
	    vout_max = 0;
	    for (int i=0;i<IMaxSh;i+=Step)
	      {
		for (int ml = 0;ml<Step;ml++) 
		  if(i>ml) 
		    {
		      vout[i-ml] = vout[i]-ml*(vout[i]-vout[i-Step])/Step;
		      ShaperOut[i-ml] = ShaperOut[i]-ml*(ShaperOut[i]-ShaperOut[i-Step])/Step;
		      CSAOut[i-ml] = CSAOut[i]-ml*(CSAOut[i]-CSAOut[i-Step])/Step;
		      ShaperOut_der[i-ml] = ShaperOut_der[i]-ml*(ShaperOut_der[i]-ShaperOut_der[i-Step])/Step;
				    }
		if (fabs(CSAOut[i])>fabs(csa_max))  csa_max = CSAOut[i];		    

		if (fabs(ShaperOut[i])>fabs(sh_max)) 
		  {
		    sh_max = ShaperOut[i];		    
		    NMax_sh = i;
		  }

		if (fabs(vout[i]) > vout_max)    
		  {
		    vout_max = fabs(vout[i]); 
		    NMax_sc = i;
		  }

	      } // end interpolation

	    float OscArea = 0;
	    for (int io = 0; io<IMax ; io++) OscArea +=vout[io];   
	    OscArea *=TIMEUNIT*50*1e12;   
	    cout << "Signal Area = " << OscArea <<  " [pVs] " << endl;
	    cout << "Collected charge = " << OscArea/50*1e3 <<  " [fF] "  << endl;
	    cout << "Average Current = " << OscArea/50/(IMax*TIMEUNIT)*1e-6 <<  " [uA] "  << endl;
	    cout << "dV/dt (CSA + Shaper) = " << (sh_max*1e-12/TIMEUNIT)/(NMax_sh*TIMEUNIT*1e9) <<  " [mV/ns] " << endl;
	    cout << "dV/dt (Cividec) = " << (vout_max*1e3*50*100)/(NMax_sc*TIMEUNIT*1e9) <<  " [mV/ns] " << endl;
	    cout << "dV/dt (Cividec < 0.4ns) = " << (vout04*1e3*50*100)/(NMax04*TIMEUNIT*1e9) <<  " [mV/ns] " << endl;

	    for (int io = 0; io<IMax ; io++)  
	      {
		ioscGraph[io] = vout[io]*(itot_max/vout_max);
		//if (io> 5 ) DioscGraph[io] = (ioscGraph[io]-ioscGraph[io-5])/(5*TIMEUNIT);
		DioscGraph[io] = vout[io]*100*50*1e3;
		// else if(io==5) DioscGraph[4] = (ioscGraph[io]-0)/(5*TIMEUNIT);
		//		cout << itot[io] << " " << io << endl;
		//ioscGraph[io] = vout[io]*(qtot/vout_max);

	      }	    

	    if (bStop==true) 
	      {
		cout<<"Exiting currents calculation"<<endl;
		return;
	      }
	  
	    
	    for (int i=0; i<totaltime;i++)
	      {
		if (fabs(ShaperOut[i])>fabs(VTh) && FVTh )
		  {
		    FVTh = false;
		    STime = (double) i*TIMEUNIT*1e9;
		    float dVdt = 1e-15*(ShaperOut[i+50]-ShaperOut[i-50])/(TIMEUNIT*100); //Coulomb/Sec
		    Jitter = (Noise*ECHARGE)/dVdt*1e9; // in ns
		    gui->SetTVth(STime);
		    gui->SetJitter(Jitter);
		    // break;
		  }
		if (fabs(ShaperOut[i])<fabs(VTh) && !FVTh )
		  {
		    FVTh = true;
		    float dVdt = 1e-15*(ShaperOut[i-50]-ShaperOut[i+50])/(TIMEUNIT*100);
		    gui->SetFTVth((double) i*TIMEUNIT*1e9);
		    gui->SetFJitter(Noise*ECHARGE/dVdt*1e9);
		    break;
		  }
		
		
	      }
	    
	    
	    cout << "Rising Jitter [ns] =  " <<  Jitter << " Falling Jitter = " << gui->GetFJitter() << endl;
	    cout << "VTh Time Crossing [ns] =  " << STime <<  " and " << gui->GetFTVth() << endl;
	  } // end of scope 

	for (int io = 0; io<IMax ; io++)  
	  {
	    if (gui->GetOscOn()==true)  //if scope=on,
	      if ( ioscGraph[io]> MaxOsc) MaxOsc =  ioscGraph[io];
	    if ( itot[io]> MaxOsc) MaxOsc =  itot[io];
	    if ( ih[io]> MaxOsc) MaxOsc =  ih[io];
	    if ( ie[io]> MaxOsc) MaxOsc =  ie[io];
	    if ( ihg[io]> MaxOsc) MaxOsc =  ihg[io];
	    if ( ieg[io]> MaxOsc) MaxOsc =  ieg[io];

	    if (gui->GetOscOn()==true)  //if scope=on,
	      if ( ioscGraph[io]< MinOsc) MinOsc =  ioscGraph[io];		
	    if ( itot[io]< MinOsc) MinOsc =  itot[io];
	    if ( ih[io]< MinOsc) MinOsc =  ih[io];
	    if ( ie[io]< MinOsc) MinOsc =  ie[io];
	    if ( ihg[io]< MinOsc) MinOsc =  ihg[io];
	    if ( ieg[io]< MinOsc) MinOsc =  ieg[io];
	  }	    

	//	for (int i=0;i<IMaxSh;i++)
	//  {
	//   ShaperOut[i-ml] = ShaperOut[i]*100*1e-12/TIMEUNIT;
	//  }
	if ( gui->GetFileNameOn())
	  {
	    fprintf(pfileTW,"%E %E\n",STime,Jitter);
	    	    
	    Int_t IPrint = 0 ;
	    IPrint = (gui->GetOscOn()) ? IMaxSh : IMax;

	    IPrint = (IPrint >TMAX) ? TMAX : IPrint; // At most, print TMAX point

	    fprintf(pfile,"%E %E %E %E %E %E %E %E %E\n",0.,0.,0.,0.,0.,0.,0.,0.,0.);
	    
	    for (Int_t i=0;i<IPrint;i++)
	      {
		if (i % 20 == 0 )
		  {
		    if (gui->GetOscOn()==true) fprintf(pfile,"%E %E %E %E %E %E %E %E %E\n",(i)*TIMEUNIT+ 2.000000E-11,
		    				       itot[i],ie[i],ih[i],ieg[i],ihg[i],
		    			       vout[i],ShaperOut[i],CSAOut[i]);
		     else  fprintf(pfile,"%E %E %E %E %E %E \n",(i)*TIMEUNIT+ 2.000000E-11,itot[i],ie[i],ih[i],ieg[i],ihg[i]);
		    //fprintf(pfile,"%E %E\n",(i)*TIMEUNIT+ 2.000000E-11,itot[i]);
		  }
	      }
	  }

	if(gui->GetBatchOn()==false)
	  {
	    cout << "Total number of e-h pair simulated = " << firstfreeslot/2 << endl;	  
	    cout << "Total number of e-h pair  = " << firstfreeslot/2*chargescale << endl;	  
	    cout << "Gain  = " << (float) (firstfreeslot/2)/pot.Getmipcharge() << endl;	  
  	    
	    cout<<"================================================================="<<endl;
	    

	    TGraph *gr;							
	    gr = new TGraph(IMax,q,itot);				// Graph for total current
	    gr->GetXaxis()->SetTitle("time (s)");		// set title of x-axis
	    gr->GetYaxis()->SetTitle("Current (A)");	// set title of y-axis
	    gr->SetLineColor(3);					// set line color to green
	    gr->SetTitle(" ");						// set title
	    gui->Getcurcanvas()->cd();				// change to canvas
	    gr->SetLineWidth(3);
	    gr->GetXaxis()->SetRangeUser(0,TimeMax*1.1); 
	    //	    if (gui->GetOscOn()==true)
	      gr->GetYaxis()->SetRangeUser( MinOsc*1.1, MaxOsc*1.1); 
	    gui->Getcurcanvas()->cd();				// change to canvas
	    gr->Draw("AL");						// draw graph (axis and line)
	    
	    //	    ioscGraph[io] = vout[io]*(itot_max/vout_max);

	    TGraph *gr1;				// Graph for current of holes
	    gr1 = new TGraph(IMax,q,ih);
	    gr1->SetLineColor(4);		// set line color to blue
	    gr1->SetLineWidth(2);
	    gui->Getcurcanvas()->cd();
	    gr1->Draw("L");
	    
	    
	    TGraph *gr2;				// Graph for current of electrons
	    gr2 = new TGraph(IMax,q,ie);
	    gr2->SetLineColor(2);		// set line color to red
	    gr2->SetLineWidth(2);
	    gui->Getcurcanvas()->cd();
	    gr2->Draw("L");
	    
	    
	    if (gui->GetGainon()==true)
	      {
		TGraph *gr3;				// Graph for current of gain electrons
		gr3 = new TGraph(IMax,q,ieg);
		gr3->SetLineColor(6);		// set line color to lightblue
		gr3->SetLineWidth(2);
		gui->Getcurcanvas()->cd();
		gr3->Draw("L");
		
		TGraph *gr4;				// Graph for current of gain holes
		gr4 = new TGraph(IMax,q,ihg);
		gr4->SetLineColor(7);		// set line color to purple
		gr4->SetLineWidth(2);
		gui->Getcurcanvas()->cd();
		gr4->Draw("L");
	      }
	    
	    if (gui->GetOscOn()==true)
	      {
		
		TGraph *gr5;				// Graph for output voltage
		gr5 = new TGraph(IMax,q,ioscGraph);
		gui->Getcurcanvas()->cd();
		gr5->SetLineColor(1);		// set line color to 
		gr5->SetLineWidth(2);
		gr5->SetLineStyle(2);
		gr5->Draw("L");


		//		ioscGraph[io] = vout[io]*(itot_max/vout_max);
		//		cout <<	"  vout_max "  << vout_max << " , "<< itot_max << endl;
		TGaxis *axis;
		float OscScale = vout_max*50;
		if (MinOsc>=0 && MaxOsc>0 ) axis = new TGaxis(TimeMax*1.1,0.1*MaxOsc*1.1,TimeMax*1.1,MaxOsc*1.1,
					 0.1*1e3*OscScale*1.1,1e3*OscScale*1.1,510,"+");

		if (MinOsc<0 && MaxOsc<=0)
		  axis = new TGaxis(TimeMax*1.1,0.9*MinOsc*1.1 ,TimeMax*1.1,0, 
				    -0.9*1e3*OscScale*1.1,0,510,"+");

		if (MinOsc<0 && MaxOsc> 0)
		  {
		    if (fabs(MaxOsc)>fabs(MinOsc))
		      axis = new TGaxis(TimeMax*1.1, 0.9*MinOsc*1.1,TimeMax*1.1,MaxOsc*1.1,
					-0.9*1e3*vout_max*fabs(MinOsc/MaxOsc)*1.1*50,1e3*vout_max*1.1*50,510,"+");
		    else
		      axis = new TGaxis(TimeMax*1.1, 0.9*MinOsc*1.1,TimeMax*1.1,MaxOsc*1.1,
					-0.9*1e3*vout_max*1.1*50,1e3*vout_max*fabs(MaxOsc/MinOsc)*1.1*50,510,"+");
		  }
		axis->SetTitle("Voltage [mV]");
		axis->SetLabelOffset(0.07);
		axis->SetTitleOffset(1.35);
		axis->Draw();

		
	     

		
		// Shaper  canvas
		
		
		gui->Getosccanvas()->Clear();
		gui->Getosccanvas()->Divide(2,2,0.002,0.002);
		gui->Getosccanvas()->Update();
		TGraph *gr6;							
		gr6 = new TGraph(IMaxSh,qSh,ShaperOut);			
		gr6->SetLineColor(3);					// set line color to green
		gr6->SetTitle("Cividec CSA");						// set title
		gr6->SetLineWidth(3);
		gui->Getosccanvas()->cd(1);
		gr6->GetYaxis()->SetRangeUser(0, sh_max*1.1); 
		gr6->Draw("AL");	
		gr6->GetXaxis()->SetTitle("Time (s)");		// set title of x-axis
		gr6->GetYaxis()->SetTitle("Amplitude (mV)");	// set title of y-axis
		//		TGaxis*axis1;
		//axis1 = new TGaxis(IMaxSh*TIMEUNIT*0.8,0.1*sh_max*1.1,IMaxSh*TIMEUNIT*0.8,sh_max*1.1,
		//			 sh_max*0.1*1e-12*TransImp/TIMEUNIT*1.1,sh_max*1e-12*TransImp/TIMEUNIT*1.1,510,"+");
		
		// axis1->SetTitle("Voltage [mV]");
		// axis1->SetLabelOffset(0.07);
		// axis1->SetTitleOffset(1.4);
		// axis1->Draw();


		gui->Getosccanvas()->cd(2);
		TGraph *gr7;							
		gr7 = new TGraph(NMax_sh,qSh,ShaperOut_der);			
		gr7->SetLineColor(4);					// set line color to green
		gr7->SetTitle("Shaper Rising edge derivative");						// set title
		gr7->SetLineWidth(2);
		gr7->GetXaxis()->SetTitle("Time (s)");		// set title of x-axis
		gr7->GetYaxis()->SetTitle("dV/dt (mV/ns)");	// set title of y-axis
		gr7->GetYaxis()->SetTitleOffset(1.5);
		gr7->Draw("AL");	
		gr7->GetXaxis()->SetRangeUser(0.,sh_max);		// set title of x-axis



		gui->Getosccanvas()->cd(3);
		TGraph *grSh;			
		grSh = new TGraph(IMax,qSh,CSAOut);
		grSh->SetTitle("Charge Sensitive Ampl.");						// set title
		grSh->SetLineColor(2);		// set line color to 
		grSh->SetLineWidth(2);
		grSh->GetXaxis()->SetTitle("Time (s)");		// set title of x-axis
		grSh->GetYaxis()->SetTitle("Charge (fQ)");	// set title of y-axis
		grSh->Draw("AL");
		gui->Getosccanvas()->Update();

		gui->Getosccanvas()->cd(4);
		TGraph *grD;			
		grD = new TGraph(IMax,q,DioscGraph);
		grD->SetTitle("Cividec braodBand (50 Ohm and 40db)");						// set title
		grD->SetLineColor(2);		// set line color to 
		grD->SetLineWidth(2);
		grD->GetXaxis()->SetTitle("Time (s)");		// set title of x-axis
		grD->GetYaxis()->SetTitle(" Amplitude [mV] ");	// set title of y-axis
		grD->Draw("AL");
		gui->Getosccanvas()->Update();

	      }
	    
	    
	    char * qestring;		// char array for output of number of collected electrons in gui
	    qestring = new char[20];
	    sprintf(qestring, "%.0lf", qe/ECHARGE);
	    
	    char * qhstring;		// char array for output of number of collected holes in gui
	    qhstring = new char[20];
	    sprintf(qhstring, "%.0lf", qh/ECHARGE);
	    
	    char * qehstring;		
	    qehstring = new char[20];
	    sprintf(qehstring, "%.0lf", qeh/ECHARGE);
	    
	    char * qegstring;		// char array for output of number of collected electrons in gui
	    qegstring = new char[20];
	    sprintf(qegstring, "%.0lf", qeg/ECHARGE);
	    
	    char * qhgstring;		// char array for output of number of collected holes in gui
	    qhgstring = new char[20];
	    sprintf(qhgstring, "%.0lf", qhg/ECHARGE);
	    
	    char * qehgstring;		
	    qehgstring = new char[20];
	    sprintf(qehgstring, "%.0lf", qehg/ECHARGE);
	    
	    char * qetotstring;		// char array for output of number of collected holes in gui
	    qetotstring = new char[20];
	    sprintf(qetotstring, "%.0lf", qetot/ECHARGE);
	    
	    char * qhtotstring;		
	    qhtotstring = new char[20];
	    sprintf(qhtotstring, "%.0lf", qhtot/ECHARGE);
	    
	    char * qtotstring;		// char array for output of total number of collected charges in gui
	    qtotstring = new char[20];
	    sprintf(qtotstring, "%.0lf", qtot/ECHARGE);
	    char * ethetastring;	// char array for output of lorentz angle of electrons in degree in gui
	    ethetastring = new char[20];
	    sprintf(ethetastring, "%.2lf", theta1*180/M_PI);
	    
	    char * hthetastring;	// char array for output of lorentz angle of holes in degree in gui
	    hthetastring = new char[20];
	    sprintf(hthetastring, "%.2lf", theta2*180/M_PI);
	    
	    //gui->SetQLabel(qestring,qhstring,qehstring,qegstring,qhgstring,qehgstring);	// Update Labels in gui
	    gui->SetQLabel(qestring,qhstring,qehstring,qegstring,qhgstring,qehgstring,qetotstring,qhtotstring,qtotstring);
	    gui->SetAngleLabel(ethetastring,hthetastring);


	    
	  }//end if batchon=false
	
	if(gui->GetFileNameOn())
	  {
	    fclose(pfile);
	    fclose(pfileTW);
	  }
	
}

Carriers::~Carriers() {

}

