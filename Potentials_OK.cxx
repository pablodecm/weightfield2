//#include "Potentials.h"
#include "WFGUI.h"
//////////////////////////////////////////////////////////////////////
Potentials::Potentials()  //constructor
{	
  multig=0;
  ref=0;
  XMAX=0;
  YMAX=0;
  pitch=0;
  width=0;
  count=0;
  vbias=0;
  vdepl=0;
  poissonf=0;
  **dpot=0;
  **wpot=0;
  strips=0;	
  bulk=1;		
  mipcharge=0;
}
//////////////////////////////////////////////////////////////////////
Potentials::Potentials(int dimy, int dimx)	// constructor 
{

  multig=(int)((log10(dimy*dimx/3)/log(4)))+1;
  ref=0;
  XMAX=dimx;
  YMAX=dimy;
  finest=dimx;
  pitch=0;
  width=0;
  count=0;
  poissonf=0;
  strips=0;
  bulk=1;
  mipcharge=75*YMAX; 

  wpot = new double*[YMAX];
  for (int i = 0; i < YMAX; ++i) wpot[i] = new double[XMAX];
  
  dpot = new double*[YMAX];
  for (int i = 0; i < YMAX; ++i) dpot[i] = new double[XMAX];
  
  fix = new int*[YMAX];
  for (int i = 0; i < YMAX; ++i) fix[i] = new int[XMAX];
  
  for(int i=0; i<XMAX; i++) {			// reset potentials to zero as a precaution
    for(int j=0; j<YMAX; j++) {
      dpot[j][i]=0.0;
      wpot[j][i]=0.0;
      fix[j][i]=0;
    }
  }



}
//////////////////////////////////////////////////////////////////////
Potentials::Potentials(int dimy, int dimx, double p, double w) // constructor 
{
  SetPitchWidthXY(dimy, dimx,p,w);
  return;
  multig=(int)((log10(dimy*dimx/3)/log(4)))+1;   // determine the number of multigrids used
  ref=0;		
  pitch=p;
  width=w;
  XMAX=dimx;
  YMAX=dimy;
  //cout<<"constructor: dimy= "<<dimy<<", multig="<<multig<<endl;
  if((int)pitch%2==0) pitch++;
  if((int)width%2==0) width++;      
  
  count = (double)XMAX/pitch;    //number of strips
  
  
  if(count-(int)count) {
    count=(int)count+1; 
  }
  
  if (((int)count)%2==0) count++;		
  
  XMAX=pitch*count;
  
  if(YMAX%2==0) YMAX++;
  
  int tempxmax=XMAX;
  int tempymax=YMAX;
  // cout<<"1: tempxmax= "<<tempxmax<<", tempymax= "<<tempymax<<endl;
  for(int i=0; i<multig; i++) { 		//calculates finer grid
    tempxmax=tempxmax/2+1;	
    if((tempxmax)%2==0)
      {
	pitch=pitch+pow(2,i+1);
	tempxmax=pitch*count;
	tempxmax=tempxmax/(pow(2,i+1))+1;
      }
    tempymax=tempymax/2+1;
    if((tempymax)%2==0) YMAX=YMAX+pow(2,i+1);
  }
  //cout<<"2: tempxmax= "<<tempxmax<<", tempymax= "<<tempymax<<endl;
  XMAX=pitch*count;	
  finest=XMAX;
  mipcharge=75*YMAX;
  
  wpot = new double*[YMAX];				// allocate memory of potentials and fix matrix
  for (int i = 0; i < YMAX; ++i) wpot[i] = new double[XMAX];
  
  dpot = new double*[YMAX];
  for (int i = 0; i < YMAX; ++i) dpot[i] = new double[XMAX];
  
  fix = new int*[YMAX];
  for (int i = 0; i < YMAX; ++i) fix[i] = new int[XMAX];
  
  for(int i=0; i<XMAX; i++) {			// reset potentials to zero as a precaution
    for(int j=0; j<YMAX; j++) {
      dpot[j][i]=0.0;
      wpot[j][i]=0.0;
      fix[j][i]=0;
    }
  }
       cout << " FXMAX= " << XMAX << " FYMAX= " << YMAX << endl; 
}
//////////////////////////////////////////////////////////////////////
void Potentials::SetwPotential(int posy, int posx, double val) {
  wpot[posy][posx]=val;
}
//////////////////////////////////////////////////////////////////////
void Potentials::SetdPotential(int posy, int posx, double val) {
  dpot[posy][posx]=val;
}
//////////////////////////////////////////////////////////////////////
void Potentials::SetV(double bias, double depl) {
  vbias=bias;
  vdepl=depl;
}

/////////////////////////////////////////////////////////////////////
int Potentials::GetXMAX() {
  return XMAX;
}
//////////////////////////////////////////////////////////////////////
int Potentials::GetYMAX() {
  return YMAX;
}
///////////////////////////////////////////////////////////////////////
double Potentials::Getdpot(int posy, int posx) {
  return dpot[posy][posx];
}
///////////////////////////////////////////////////////////////////////
double Potentials::Getwpot(int posy, int posx) {
  return wpot[posy][posx];
}
//////////////////////////////////////////////////////////////////////
void Potentials::Setmipcharge(int ch) {
  mipcharge=ch;
  return;
}
//////////////////////////////////////////////////////////////////////
int Potentials::Getmipcharge() {
  return mipcharge;
}
/////////////////////////////////////////////////////////////////////
int Potentials::Getfix(int posy, int posx) {
  return fix[posy][posx];
}
//////////////////////////////////////////////////////////////////////
int Potentials::Getref() {
  return ref;
}
//////////////////////////////////////////////////////////////////////
double Potentials::Getvbias() {
  return vbias;
}
//////////////////////////////////////////////////////////////////////
double Potentials::Getpitch() {
  return pitch;
}
//////////////////////////////////////////////////////////////////////
void Potentials::SetDoping(unsigned char s, unsigned char b) {
  strips=s;
  bulk=b;	
}
//////////////////////////////////////////////////////////////////////
void Potentials::SetPitchWidthXY(int dimy, int dimx,double p, double w)
{
  multig=(int)((log10(dimy*dimx/3)/log(4)))+1;   // determine the number of multigrids used
  ref=0;
  pitch=p;
  width=w;
  XMAX=dimx;
  YMAX=dimy;
  //cout<<"setpitch dimy= "<<dimy<<", multig="<<multig<<endl;
  
  if((int)pitch%2==0) pitch++;
  if((int)width%2==0) width++;      


  //new
  /*  
  XMAX = dimx;
  YMAX = dimy;


  if (dimx <pitch) XMAX = pitch;     //sanity check
  if ( (int)XMAX/pitch<2) XMAX = pitch; //needed for boundary condition

  //  XMAX=pitch*count;	//la dimensione orizzontale multiplo intero del pitch
  
  if(YMAX%2==0) YMAX++;
  if(XMAX%2==0) XMAX++;
  
  int tempxmax=XMAX;
  int tempymax=YMAX;
    
  for(int i=0; i<multig; i++) {
    tempxmax=tempxmax/2+1;	
    if((tempxmax)%2==0) XMAX=XMAX+pow(2,i+1);
    tempymax=tempymax/2+1;
    if((tempymax)%2==0) YMAX=YMAX+pow(2,i+1);
  }

 
  count = (int)XMAX/pitch;    //count è il numero di strip
  
  if((int) count%2==0)
    {
      XMAX += pitch; 
      count = (int)XMAX/pitch;    //count è il numero di strip

      if(YMAX%2==0) YMAX++;
      if(XMAX%2==0) XMAX++;
      
      int tempxmax=XMAX;
      int tempymax=YMAX;
      
      for(int i=0; i<multig; i++) {
	tempxmax=tempxmax/2+1;	
	if((tempxmax)%2==0) XMAX=XMAX+pow(2,i+1);
	tempymax=tempymax/2+1;
	if((tempymax)%2==0) YMAX=YMAX+pow(2,i+1);
      }

    }
  finest=XMAX;


*/

   // old
  
  count = (double)XMAX/pitch;    //number of strips
  
  
  if(count-(int)count) {
    count=(int)count+1; 
  }
  
  if (((int)count)%2==0) count++;		
  
  XMAX=pitch*count;
  
  if(YMAX%2==0) YMAX++;
  if(XMAX%2==0) XMAX++;
 
  int tempxmax=XMAX;
  int tempymax=YMAX;
  // cout<<"1: tempxmax= "<<tempxmax<<", tempymax= "<<tempymax<<endl;
  for(int i=0; i<multig; i++) { 		//calculates finer grid
    tempxmax=tempxmax/2+1;	
    if((tempxmax)%2==0)
      {
	pitch=pitch+pow(2,i+1);
	tempxmax=pitch*count;
	tempxmax=tempxmax/(pow(2,i+1))+1;
      }
    tempymax=tempymax/2+1;
    if((tempymax)%2==0) YMAX=YMAX+pow(2,i+1);
  }
  //cout<<"2: tempxmax= "<<tempxmax<<", tempymax= "<<tempymax<<endl;

  XMAX=pitch*count;	


  finest=XMAX;


  cout << "count  = " << count << " XY-Xmax = " << XMAX<< " XY-YMAX = " << YMAX << endl;


  //mipcharge=75*dimy;    // prima era mipcharge=75*YMAX;
  mipcharge=75*YMAX;

  wpot = new double*[YMAX];
  for (int i = 0; i < YMAX; ++i) wpot[i] = new double[XMAX];
  
  dpot = new double*[YMAX];
  for (int i = 0; i < YMAX; ++i) dpot[i] = new double[XMAX];
  
  fix = new int*[YMAX];
  for (int i = 0; i < YMAX; ++i) fix[i] = new int[XMAX];
  
  for(int i=0; i<XMAX; i++) {
    for(int j=0; j<YMAX; j++) {
      wpot[j][i]=0.0;
      dpot[j][i]=0.0;
      fix[j][i]=0;
    }
  }



}
//////////////////////////////////////////////////////////////////////
void Potentials::SetBoundaryConditions()   // reset electrodes and potentials according to user input
{			  
  for (int i=0; i<XMAX; i++)
    {	// bottom electrode (backplane): set to Vbias for p-type strips, otherwise 0	
      dpot[0][i]=(strips==PTYPE) ? vbias : 0.0;
      wpot[0][i]=0.0;						
      fix[0][i]=1;             
    }
  // 
  for(int j=0;j<(int)count;j++)	// set strips
    {
      for(int k=0;k<width;k++)
	{
	  dpot[YMAX-1][(int)(XMAX-(int)pitch*(int) count)/2 + j*(int)pitch+(int)pitch/2-(int)width/2+k]=(strips==NTYPE) ? vbias : 0.0;
	  wpot[YMAX-1][(int)(XMAX-(int)pitch*(int) count)/2 + j*(int)pitch+(int)pitch/2-(int)width/2+k]=0.0;			
	  
	  if(j==((int)count/2))
	    {
	      fix[YMAX-1][(int)(XMAX-(int)pitch*(int) count)/2+j*(int)pitch+(int)pitch/2-(int)width/2+k]=2;
	      wpot[YMAX-1][(int)(XMAX-(int)pitch*(int) count)/2+j*(int)pitch+(int)pitch/2-(int)width/2+k]=1.0;
	    }
	  else
	    {
	      fix[YMAX-1][(int)(XMAX-(int)pitch*(int) count)/2+j*(int)pitch+(int)pitch/2-(int)width/2+k]=1;			
	    }
	}
    }	
}  
//////////////////////////////////////////////////////////////////////
void Potentials::Iteration(void *wfgui)	// 
{				// method for iterative calculation, see: http://en.wikipedia.org/wiki/Relaxation_%28iterative_method%29
  //int r=1;
  double sum=0;		// [(old potential) - (new potential)]^2/(XMAX*YMAX)
  double err=0.001;
  long it=0;	// iteration index 
  int N=1;		// every N iterations do plot update
  
  int **tempfix;			// allocate memory for fix matrix for current grid
  tempfix = new int*[YMAX];
  for (int i = 0; i < YMAX; ++i) tempfix[i] = new int[XMAX];
  
  tempfix=FixRestriktor();	// determine fix matrix for current grid
  
  WFGUI* gui= (WFGUI*) wfgui; 
  
  if(XMAX==finest) N=1000;		// at the finest grid, do plot updates every 600th interation
  else {
    if(XMAX*2-1==finest) N=700;	// at the first coarser grid, do plot updates every 400th interation
    else N=100;				// else, every 100th iteration
  }
  
  poissonf=((bulk==PTYPE) ? +1.0 : -1.0)*(2.0*vdepl)/(YMAX*YMAX);	 // right side of Poisson equation
  
  while(1)		// calculate until accuracy is reached
    {
      if(gui->Getstopped()==1) {
	break;
      }
      
      if(XMAX==finest || XMAX==finest/2+1) err=0.005; // if calculating on finest grid, set accuracy to 0.001


     cout << " XMAX= " << XMAX << " YMAX= " << YMAX << endl;
     //     Potentials newpot(YMAX,XMAX,pitch,width);	// create temporary new object newpot, used for iterative calculation	   
      Potentials newpot(YMAX,XMAX);	// create temporary new object newpot, used for iterative calculation	   
      // inside the grid - use the given equations to average around neighbors
 

      cout << " XMAX2= " << XMAX << " YMAX2= " << YMAX << endl;
  
      for (int x=0; x<XMAX; x++)
	{		

	  for (int y=1; y<(YMAX-1); y++)
	    {		
	      if (!tempfix[y][x])
		{
		  // weighting potential
		  newpot.wpot[y][x]=0.25*(wpot[y+1][x]+wpot[y-1][x]+wpot[y][(x+1+XMAX)%(XMAX)]+wpot[y][(x-1+XMAX)%(XMAX)]);
		  // drift potential
		  //  cout << "crash37 " << endl;
		  if (gui->CallGetDetType() == 0 || gui->CallGetDetType() == 2)
		    {
		      // cout << "crash38 " << endl;
		    newpot.dpot[y][x]=0.25*(dpot[y+1][x]+dpot[y-1][x]+dpot[y][(x+1+XMAX)%(XMAX)]+dpot[y][(x-1+XMAX)%(XMAX)]-poissonf);

		    }
		  else
		    {
		      //		      cout << "crash39 " << endl;
		      newpot.dpot[y][x]=0.25*(dpot[y+1][x]+dpot[y-1][x]+dpot[y][(x+1+XMAX)%(XMAX)]+dpot[y][(x-1+XMAX)%(XMAX)]);

		    }
		}
	      else
		{
		  cout << "crash391 " << endl;
		  newpot.wpot[y][x]=wpot[y][x];		// weighting potential
		  newpot.dpot[y][x]=dpot[y][x];		// drift potential 
		}
	    }	  
	  // special treatment for top and bottom rows (potential above/below is replaced by cell itself)
	  //	  cout << "crash392 " << endl;
	  if(tempfix[0][x])
	    {
	      //	      cout << "crash393 " << x << " " << XMAX << endl;
	      newpot.wpot[0][x]=wpot[0][x];			// weighting potential
	      //cout << "crash3931 " << x << " x= " << XMAX << " y= "<< YMAX<< " "  << dpot[0][x] << endl;
	      newpot.dpot[0][x]=dpot[0][x];
	      // cout << "crash3932 " << x << " " << XMAX << " " << dpot[0][x] << endl;
	    }
	  
	  //	  cout << "crash4 " << endl;
	  if (tempfix[YMAX-1][x]==0)
	    {
	      newpot.wpot[YMAX-1][x]=0.25*(wpot[YMAX-1][(x-1+XMAX)%(XMAX)]+wpot[YMAX-1][(x+1+XMAX)%(XMAX)]+wpot[YMAX-2][x]+wpot[YMAX-1][x]);			 // weighting potential
	      newpot.dpot[YMAX-1][x]=0.25*(dpot[YMAX-1][(x-1+XMAX)%(XMAX)]+dpot[YMAX-1][(x+1+XMAX)%(XMAX)]+dpot[YMAX-2][x]+dpot[YMAX-1][x]-poissonf);   // drift potential 
	    }
	  
	  if(tempfix[YMAX-1][x])
	    {
	      newpot.wpot[YMAX-1][x]=wpot[YMAX-1][x];	// weighting potential	
	      newpot.dpot[YMAX-1][x]=dpot[YMAX-1][x];	// drift potential 
	    } 
	}
      cout << "crash5 " << endl;
      if(it%100==0) // check every 100 iterations wether break condition fulfilled
	{
	  sum=0.0;
	  for(int i=0; i<XMAX; i++) {		// calculate sum
	    for(int j=0; j<YMAX; j++) {
	      sum+=fabs(dpot[j][i]-newpot.dpot[j][i]);
	    }
	  }
	  cout << "crash6 " << endl;
	  if(it%N==0 && gui->Getplotupdate()==1 && gui->Getstopped()==0)	// do plot update every N iterations
	    {
	      //gui->DrawHist();
	      gui->Getdhist()->Reset();	//Reset histogram. dhist1 = histogram of drift potential
	      //      gui->Getdhist()->TH2F::SetBins((XMAX),-(XMAX)*0.5,(XMAX)*0.5,(YMAX),-(YMAX)*0.5,(YMAX)*0.5); // set bins of histogram
	      gui->Getdhist()->TH2F::SetBins((XMAX),0.,XMAX,YMAX,0,YMAX); // set bins of histogram
	      gui->Getdhist()->GetXaxis()->SetTitle("x [um]");
	      gui->Getdhist()->GetYaxis()->SetTitle("y [um]");

	      gui->Getwhist()->Reset();	//whist1 = histogram of weighting potential					
	      gui->Getwhist()->TH2F::SetBins(XMAX,0,XMAX,YMAX, 0,YMAX);
	      
	      for(int i=0;i<XMAX;i++) {		// fill histogram
		for(int j=0;j<YMAX;j++) {
		  gui->Getdhist()->SetBinContent(i+1,j+1,newpot.dpot[j][i]);
		  gui->Getwhist()->SetBinContent(i+1,j+1,newpot.wpot[j][i]);
		}
	      }	

	      cout << " Potentials 2DPlot " << gui->GetLess2DPlot() << endl;
	      gui->DrawAllGraph(0);

	      /*	      if (!gui->GetLess2DPlot()) 
		{
		  gui->Getcanvasp()->cd();
		  gui->Getdhist()->SetStats(0);	// hide statistics box
		  gui->Getdhist()->Draw("COLZ"); // draw histogram				
		

		  gui->Getcanvasw()->cd();
		  gui->Getwhist()->SetStats(0);
		  gui->Getwhist()->Draw("COLZ");
		  
		  DriftPal();
		  gui->Getcanvasp()->Update();
		  WeightPal();
		  gui->Getcanvasw()->Update();
		}
*/

	    }//End of if(it%N==0)
	  cout << "crash7 " << endl;	  
	  
	  if(sum/((double)(XMAX*YMAX))<err)	//check wether accuracy reached
	    {
	      for(int i=0;i<YMAX;i++) {	// overwrite potential with newpotential	
		for(int j=0; j<XMAX;j++) {
		  dpot[i][j]=newpot.dpot[i][j];
		  wpot[i][j]=newpot.wpot[i][j];
		}
	      }
	      break;	//break out of while loop if accuracy reached 
	    }
	}//end of if(it%100)
      
      for(int i=0;i<YMAX;i++) {	// overwrite potential with newpotential	
	for(int j=0; j<XMAX;j++) {
	  dpot[i][j]=newpot.dpot[i][j];
	  wpot[i][j]=newpot.wpot[i][j];
	}
      }
      it++;
      cout << "crash8 " << endl;
      ///} // end of if(gui-<Getstopped()==0)
    }//end of while loop
  cout << "crash9 " << endl;
  
  //	for (int j = 0; j < YMAX; j++) delete [] tempfix[j] ;	    	
  //	delete [] tempfix;
}
//////////////////////////////////////////////////////////////////////
Potentials::~Potentials() {	// destructor
  for (int j = 0; j < YMAX; j++) delete [] dpot[j] ;	    	
  delete [] dpot;
  
  for (int j = 0; j < YMAX; j++) delete [] wpot[j] ;
  delete [] wpot;
  
  for (int j = 0; j < YMAX; j++) delete [] fix[j] ;
  delete [] fix;
}
//////////////////////////////////////////////////////////////////////
void Potentials::Restriktor()    //method to restrict potentials to a coarser grid, with XMAX/2+1
{
  int z=0,s=0;	// z: lines, s: columns
  ref=ref+1;	// increase ref 
  Potentials newpot(YMAX/2+1,XMAX/2+1); // temporary new object newpot on coarser grid
  
  for(int i=0;i<YMAX/2+1;i++)	// copy mutual points from 'old' potential to new potential
    {
      s=0;			
      for(int j=0; j<XMAX/2+1;j++)
	{
	  newpot.dpot[i][j]=dpot[z][s];
	  newpot.wpot[i][j]=wpot[z][s];
	  s=s+2;
	}
      z=z+2;
    }
  
  XMAX=XMAX/2+1;		// set XMAX and YMAX to new values
  YMAX=YMAX/2+1;		
  
  wpot = new double*[YMAX];
  for (int i = 0; i < YMAX; ++i) wpot[i] = new double[XMAX];
  
  dpot = new double*[YMAX];
  for (int i = 0; i < YMAX; ++i) dpot[i] = new double[XMAX];
  
  for(int i=0;i<YMAX;i++)	 // overwriting dpot and wpot with dpot and wpot of new potential
    {		
      for(int j=0; j<XMAX;j++)
	{
	  dpot[i][j]=newpot.dpot[i][j];
	  wpot[i][j]=newpot.wpot[i][j];
	}
    }	
}
//////////////////////////////////////////////////////////////////////
int** Potentials::FixRestriktor()  
{
  int z=0,s=0;   // lines and columns counter 
  int **nfix=0;
  
  nfix = new int*[YMAX];	// allocate memory for restricted fix matrix
  for (int i = 0; i<YMAX; i++) nfix[i] = new int[XMAX];
  
  for(int i=0;i<YMAX;i++)	// copy mutual points from old fix matrix to nfix
    {
      s=0;			
      for(int j=0; j<XMAX;j++)
	{
	  nfix[i][j]=fix[z][s];	
	  s=s+(int)pow(2,ref);
	}
      z=z+(int)pow(2,ref);
    }		
  return nfix;
}
//////////////////////////////////////////////////////////////////////
void Potentials::Prolongation() // method to prolongate potentials to finer grid with XMAX*2-1
{			
  ref=ref-1;	//decrease ref (going from coarser to finer grid)											
  int z=0,s=0;	// columns and lines counter for loop
  YMAX=YMAX*2-1;	// setting YMAX and XMAX
  XMAX=XMAX*2-1;
  Potentials newpot(YMAX,XMAX);	// temporary Potentials object
  
  int **tempfix=0;	// allocate memory for temporary fix matrix 
  tempfix = new int*[YMAX];
  for (int i = 0; i < YMAX; ++i) tempfix[i] = new int[XMAX];
  
  tempfix=FixRestriktor();	// set fix matrix
  
  for(int i=0;i<YMAX;i=i+2)    // copy mutual points from coarser grid to finer grid
    {		
      s=0;	
      for(int j=0; j<XMAX;j=j+2)
	{
	  newpot.dpot[i][j]=dpot[z][s];
	  newpot.wpot[i][j]=wpot[z][s];	
	  s++;
	}
      z++;
    }
  
  for(int i=1;i<YMAX;i=i+2)	// calculate mean value for grid points without 4 neighbours
    {							
      for(int j=1;j<XMAX;j=j+2)
	{
	  newpot.dpot[i][j]=0.25*(newpot.dpot[i-1][j-1]+newpot.dpot[i-1][j+1]+newpot.dpot[i+1][j+1]+newpot.dpot[i+1][j-1]);
	  newpot.wpot[i][j]=0.25*(newpot.wpot[i-1][j-1]+newpot.wpot[i-1][j+1]+newpot.wpot[i+1][j+1]+newpot.wpot[i+1][j-1]);
	}
    }
  // set backplane to potential
  for (int j=0; j<XMAX; j++)
    {
      newpot.dpot[0][j]=(strips==PTYPE) ? vbias : 0.0; // bottom electrode (backplane): set to Vbias for p-type strips, otherwise 0
      newpot.wpot[0][j]=0.0;										             
    } 
  int l=0,f=0;  // auxiliary variable to calculate boundary points	
  for(int i=1;i<YMAX;i++)	// average over neighbours of grid points which are not set yet
    {						
      for(int j=(i+1)%2;j<XMAX;j=j+2)  //// depending on line (i), counter j starts at 0 or 1				
	{							
	  if(tempfix[i][j]==1) // fix jenes gitters auf das erweitert wird
	    {
	      newpot.dpot[i][j]=(strips==NTYPE) ? vbias : 0.0;	
	      newpot.wpot[i][j]=0.0;
	    }
	  
	  if(tempfix[i][j]==2)
	    {
	      newpot.dpot[i][j]=(strips==NTYPE) ? vbias : 0.0;
			
		newpot.wpot[i][j]=1.0;
	    }
	  
	  if(tempfix[i][j]==0)
	    {
	      f=0;	//special treatment for bottom and top (potentials above/below is replaced by cell itself)	
	      l=0;							
	      
	      if(i==YMAX-1) f=1;
	      if(i==0) l=1;
	      if (XMAX != 1)
          {
              newpot.dpot[i][j]=0.25*(newpot.dpot[i][(j-2+XMAX) % (XMAX-1)]+newpot.dpot[i+1-f][j]+newpot.dpot[i-1+l][j]+newpot.dpot[i][(j+XMAX)%(XMAX-1)]);
	      newpot.wpot[i][j]=0.25*(newpot.wpot[i][(j-2+XMAX)% (XMAX-1)]+newpot.wpot[i+1-f][j]+newpot.wpot[i-1+l][j]+newpot.wpot[i][(j+XMAX)%(XMAX-1)]);
          }
          }
	}
    }
  wpot = new double*[YMAX];
  for (int i = 0; i < YMAX; ++i) wpot[i] = new double[XMAX];
  
  dpot = new double*[YMAX];
  for (int i = 0; i < YMAX; ++i) dpot[i] = new double[XMAX];
  
  for(int i=0;i<YMAX;i++) {			// overwriting old dpot and wpot with new ones
    for(int j=0;j<XMAX;j++) {
      wpot[i][j]=newpot.wpot[i][j];
      dpot[i][j]=newpot.dpot[i][j];
    }
  }
}
//////////////////////////////////////////////////////////////////////////
void Potentials::Multigrid(void *wfgui)
{
  WFGUI* gui= (WFGUI*) wfgui;
  
  for(int i=0; i<multig; i++) { // starting with calculation on coarsest grid, we need to restrict potentials
    Restriktor();
  }
  
  char * gridlabel;
  gridlabel = new char[50];
  sprintf(gridlabel, "Calculating Potentials: grid number: %d/%d",1,multig+1);
  
  char * gridlabel2;
  gridlabel2 = new char[50];
  sprintf(gridlabel2, "Calculating Potentials: grid number: %d/%d",1,multig+1);
  
  cout << "crash31" << endl;
  Iteration(gui);
  cout << "crash32" << endl;
  gui->GetCalculatingLabel2()->SetTitle(gridlabel2);
  gui->GetCalculatingLabel()->SetTitle(gridlabel);
  
  for(int i=0; i<multig; i++) { 
    Prolongation();
    cout << "crash33" << endl;
    sprintf(gridlabel, "Calculating Potentials: grid number: %d/%d",i+2,multig+1);
    sprintf(gridlabel2, "Calculating Potentials: grid number: %d/%d",i+2,multig+1);	
    gui->GetCalculatingLabel2()->SetTitle(gridlabel2);
    gui->GetCalculatingLabel()->SetTitle(gridlabel);
    cout << "crash34" << endl;
    Iteration(gui);
    cout << "crash35 " << i <<  endl;
  }
  gui->Getdhist()->GetXaxis()->SetLabelColor(1);	
  gui->Getwhist()->GetYaxis()->SetLabelColor(1);
}
//////////////////////////////////////////////////////////////////////////
unsigned char Potentials::GetDoping() {
  return strips;
}
//////////////////////////////////////////////////////////////////////////
void Potentials::DriftPal()
{
  static Int_t  colors[999];
  static Bool_t initialized = kFALSE;
  gStyle->SetNumberContours(999);
  
  Double_t r[]    = {0.0, 0.0, 0.0, 1.0, 1.0}; //0.54 1 1 0 0 0
  Double_t g[]    = {0.0, 1.0, 1.0, 1.0, 0.0}; //0.17 0 1 1 1 0
  Double_t b[]    = {1.0, 1.0, 0.0, 0.0, 0.0}; //0.89 0 0 0 1 1
  Double_t stop[] = {0.0, 0.25, 0.50,.75,1.0};
  
  if(!initialized)
    {
      Int_t FI = TColor::CreateGradientColorTable(5, stop, r, g, b, 999);
      for(int i=0;i<999;i++) colors[i] = FI+i;
      initialized = kTRUE;
      return;
    }
  gStyle->SetPalette(999,colors);
}
//////////////////////////////////////////////////////////////////////////
void Potentials::WeightPal()
{
  static Int_t  colors[999];
  static Bool_t initialized = kFALSE;
  gStyle->SetNumberContours(999);
  
  Double_t r[]    = {0.0, 0.0, 0.0, 1.0, 1.0}; //0.54 1 1 0 0 0
  Double_t g[]    = {0.0, 1.0, 1.0, 1.0, 0.0}; //0.17 0 1 1 1 0
  Double_t b[]    = {1.0, 1.0, 0.0, 0.0, 0.0}; //0.89 0 0 0 1 1
  Double_t stop[] = {0.0, 0.02, 0.20, .30, 1.0};
  
  if(!initialized)
    {
      Int_t FI = TColor::CreateGradientColorTable(5, stop, r, g, b, 999);
      for(int i=0;i<999;i++) colors[i] = FI+i;
      initialized = kTRUE;
      return;
    }
  gStyle->SetPalette(999,colors);
}
//////////////////////////////////////////////////////////////////////
void Potentials::SetAbove(bool above) {
  alpha_above=above;
}
//////////////////////////////////////////////////////////////////////////////////////

bool Potentials::GetAbove() {
  return alpha_above;
}
