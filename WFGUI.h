#include "TGClient.h"
#include "TGNumberEntry.h"
#include "TGFrame.h"
#include "TGButtonGroup.h"
#include "TGCanvas.h"
#include "TCanvas.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGTab.h"
#include "TGObject.h"
#include "TRootEmbeddedCanvas.h"
#include "Riostream.h"
#include "TLatex.h"
#include "TApplication.h"
#include "TH1.h"
#include "TGaxis.h"
#include "Carriers.h"
#include "TThread.h"
#include "TRandom3.h"
#include "TMutex.h"
#include "TDatime.h"
#include "TGProgressBar.h"
#include "TGComboBox.h"
#include "TGScrollBar.h"
#include "TFile.h"
#include "TEllipse.h"
// #include "TGuiBldDragManager.h"
// #include "TRootGuiBuilder.h"
#include "TGListTree.h"
#include "TColor.h"
#include <string.h>
#include <sstream>

typedef enum ParticleRadioButtonStatus_enum {
	MIPunif,
	MIPnonunif,
	MIPlandau,
	ALPHA_UNDEF,
	ALPHA_TOP,
	ALPHA_BOTTOM,
	USR_CHARGE
}ParticleRadioButtonStatus;

class WFGUI : public TGMainFrame {

	private:

		bool plotupdate;
		bool LessPlot;
		bool Less2DPlot;
		bool stopped;
		bool bfieldon;
		bool diffusionon;
		bool currentson;
		bool fieldyes;
		bool gainon;
		bool OscOn;
		bool FileNameOn;
		bool BatchOn;
		bool ConstQFlag;
		bool UniformQFlag;
		bool UserUniformQFlag;
		unsigned char stripdoping;
		unsigned char  bulkdoping;

		int DetType;
		int carriersin;
		float ygain;
		float GainRatio;
		float GainIndent;
		double Temp;					// temperature
		int dimMaxCarriers;
		int Precision;
		double AlphaRange;
		double OscBW; 					//oscilloscope bandwidth
		int EvNumber;					//total number of events
		int ThisEvent;					//
		double angle;
		double TFall;
		double SWidth;
		double TRise;
		double Vth;
		double Jitter;
		double TVth;
		double FJitter;
		double FTVth;
		
		string fileName;

		ParticleRadioButtonStatus radiobuttonstatus;

		//TMutex* mut;

		TRootEmbeddedCanvas *driftcanvas;
		TRootEmbeddedCanvas *driftpcanvas;		
		TRootEmbeddedCanvas *weightcanvas;
		TRootEmbeddedCanvas *weightpcanvas;
		TRootEmbeddedCanvas *driftfcanvas;
		TRootEmbeddedCanvas *weightfcanvas;
		TRootEmbeddedCanvas *currentscanvas;
		TRootEmbeddedCanvas *oscilloscopecanvas;
		TGTab *PotentialTab;
		TGCompositeFrame *DriftPTab;
		TGCompositeFrame *WeightingPTab;
		TGCompositeFrame *CurrentsTab;
		TGCompositeFrame *OscilloscopeTab;
		
		TGVerticalFrame *SettingsGlobalFrame;	
		// Contains:
		TGHorizontalFrame *SettingsFrame; 
		TGHorizontalFrame *SettingsFrame2; 		
		TGHorizontalFrame *SettingsFrame3; 

		// SettingsFrame contains:
		TGGroupFrame *NameFrame;
		TGGroupFrame *BatchFrame;

		// SettingsFrame2 contains:
		TGGroupFrame *ControlFrame;
		TGVerticalFrame *RightFrame;	

		//ControlFrame contains 
		TGGroupFrame *ParticlesPropertiesFrame;

		//RightFrame contains 
		TGGroupFrame *DetectorPropertiesFrame;
		//	TGVerticalFrame *ElectronicsFrame;
		TGGroupFrame *ElectronicsFrame;



		TGHorizontalFrame *ExitFrame;			
		TGGroupFrame *ParticlesPropertiesFrame1;
		TGGroupFrame *ChargeCollectionFrame;
		TGGroupFrame *LorentzInfoFrame;
		TGGroupFrame *DimensionsFrame;
		TGGroupFrame *VoltageFrame;
		TGGroupFrame *CurrentsFrame;
		TGGroupFrame *PlotGroupFrame;
		TGGroupFrame *SelectPartFrame; //!!!!!!!!!!!!!!
		//		TGGroupFrame *OscilloscopeFrame;
		TGVerticalFrame *OscilloscopeFrame;


		TGVerticalFrame *DimSetFrame;
		TGVerticalFrame *DimLabelFrame; 
		TGVerticalFrame *VolSetFrame;
		TGVerticalFrame *VolLabelFrame; 

		TGButtonGroup *TypeFrameGroup;
		TGButtonGroup *TypeButtonGroup;
		TGButtonGroup *DopButtonGroup;
		TGButtonGroup *PartButtonGroup;
		TGHorizontalFrame *ButtonPotFrame;
		TGHorizontalFrame *ButtonCurFrame;
		TGHorizontalFrame *UpdateFrame;
		TGHorizontalFrame *DriftPTabFrame;
		TGHorizontalFrame *WeightPTabFrame;
		TGHorizontalFrame *LabelsTabFrame;
		TGHorizontalFrame *LabelsTabFrame2;
		TGHorizontalFrame *CurrentsLabelFrame;
		TGHorizontalFrame *CarriersInFrame;
		TGHorizontalFrame *ChargeLabelFrame1;
		TGHorizontalFrame *ChargeLabelFrame2;
		TGHorizontalFrame *ChargeLabelFrame3;
		TGVerticalFrame *CurrentsInfoFrame;
		TGHorizontalFrame *TempSetFrame;
		TGHorizontalFrame *AlphaRangeFrame;
		TGHorizontalFrame *PrecisionFrame;
		TGHorizontalFrame *OscBWSetFrame;
		TGHorizontalFrame *TFallSetFrame;
		TGHorizontalFrame *VthSetFrame;
		TGHorizontalFrame *TRiseSetFrame;
		TGHorizontalFrame *BatchEventSetFrame;

		TGButtonGroup *DopBulkFrame;
		TGButtonGroup *DopStripFrame;
		TGNumberEntry *XMAXentry;
		TGNumberEntry *YMAXentry;
		TGNumberEntry *Pitchentry;
		TGNumberEntry *Widthentry;
		TGNumberEntry *Gainentry; //!!!
		TGNumberEntry *GainRatioentry;
		TGNumberEntry *GainIndententry;
		TGNumberEntry *Biasentry;
		TGNumberEntry *Depletionentry;
		TGNumberEntry *PlotUpdateEntry;
		TGNumberEntry *LessPlotEntry;
		TGNumberEntry *Less2DPlotEntry;
		TGNumberEntry *BfieldEntry;
		TGNumberEntry *TempEntry;
		TGNumberEntry *WhereCut;
		TGNumberEntry *WhereCut2;		
		TGNumberEntry *CarriersInNumberentry;
		TGNumberEntry *CarriersAngleNumberentry;
		TGNumberEntry *Chargeentry;
		TGNumberEntry *RangeEntry;
		TGNumberEntry *PrecisionEntry;
		TGNumberEntry *OscBWEntry;
		TGNumberEntry *TFallEntry;
		TGNumberEntry *VthEntry;
		TGNumberEntry *TRiseEntry;
		TGNumberEntry *EventsEntry;
		TGRadioButton *TypeButton[3];
		TGRadioButton *BulkButton[2];
		TGRadioButton *StripButton[2];
		TGRadioButton *SelectPartButton[6];

		TGCheckButton *CurrentsButton[2];
		TGCheckButton *Eyes;
		TGCheckButton *UpdatePlotCheck;
		TGCheckButton *LessPlotCheck;
		TGCheckButton *Less2DPlotCheck;
		TGCheckButton *OscOnButton;
		TGCheckButton *FileNameOnButton;
		TGCheckButton *BatchOnButton;
		TGLabel *XMAXLabel;
		TGLabel *YMAXLabel;
		TGLabel *PitchLabel;
		TGLabel *WidthLabel;
		TGLabel *GainLabel;	//!!!
		TGLabel *GainRatioLabel;    
		TGLabel *GainIndentLabel;    
		TGLabel *BiasLabel;
		TGLabel *DepletionLabel;
		TGLabel *QELabel;
		TGLabel *QHLabel;
		TGLabel *QEHLabel;
		TGLabel *QEGLabel;
		TGLabel *QHGLabel;
		TGLabel *QEHGLabel;
		TGLabel *QETotLabel;
		TGLabel *QHTotLabel;
		TGLabel *QTotLabel;
		TGLabel *PlotUpdateLabel;
		TGLabel *LessPlotLabel;
		TGLabel *Less2DPlotLabel;
		TGLabel *BulkLabel;
		TGLabel *StripLabel;
		TGLabel *QHTextLabel;
		TGLabel *QETextLabel;
		TGLabel *QEHTextLabel;
		TGLabel *QHGTextLabel;
		TGLabel *QEGTextLabel;
		TGLabel *QEHGTextLabel;
		TGLabel *QETotTextLabel;
		TGLabel *QHTotTextLabel;
		TGLabel *QTotTextLabel;
		TGLabel *LorentzhTextLabel;
		TGLabel *LorentzeTextLabel;
		TGLabel *LorentzeLabel;
		TGLabel *LorentzhLabel;
		TGLabel *CalculatingLabel;
		TGLabel *CalculatingLabel2;
		TGLabel *DriftCutLabel;
		TGLabel *WeightingCutLabel;
		TGLabel *FieldLabel;
		TGLabel *FieldLabel2;
		TGLabel *ElectronsLabel;
		TGLabel *HolesLabel;
		TGLabel *GainElectronsLabel;
		TGLabel *GainHolesLabel;
		TGLabel *TotalLabel;
		TGLabel *CarriersInLabel;
		TGLabel *CarriersAngleLabel;
		TGLabel *TempLabel;
		TGLabel *RangeLabel;
		TGLabel *PrecisionLabel;
		TGLabel *OscBWLabel;
		TGLabel *TRiseLabel;
		TGLabel *TFallLabel;
		TGLabel *VthLabel;
		TGLabel *EventsLabel;
		TGTextButton *CalculateButton;
		TGTextButton *SaveButton;
		TGTextButton *SetButton;
		TGTextButton *StopButton;
		TGTextButton *ExitButton;
		TGTextButton *BetweenStripsButton;
		TGTextButton *BetweenStripsButton2;
		TGTextButton *OnStripsButton;
		TGTextButton *OnStripsButton2;
		//		TGTextButton *EtotButton;
		TGTextButton *ExButton;
		TGTextButton *EyButton;
		//	TGTextButton *EtotButton2;
		TGTextButton *ExButton2;
		TGTextButton *EyButton2;
		TGTextButton *CalcPotButton;
		TGTextButton *DrawCutsUserEntry;
		TGTextButton *DrawCutsUserEntry2;
		TGTextButton *FileNameButton;
		TGTextEntry *FileNameEntry;
		
		TGComboBox *SetAlpha;		//!!!!
		TGHorizontalFrame *fSliceExtentL;
		TGComboBox *fSliceLengthUnit;

		TGLabel *fSliceExtentLLabel;


		Potentials dwpot;			// potentials
		Field **wf;					// weighting field
		Field **df;					// drift field
		Carriers *carriers;	// carriers

		TCanvas *canvasp;		// canvas for drift potential histogram
		TCanvas *canvaspc;		// canvas for drift potential cuts
		//		TCanvas *canvaspf;		// canvas for drift potential fields
		TCanvas *canvasw;		// canvas for weighting potential histogram
		TCanvas *canvaswc;		// canvas for weighting potential cuts
		// TCanvas *canvaswf;		// canvas for weighting potential fields
		TCanvas *curcanvas;		// canvas for currents
		TCanvas *osccanvas;		// canvas for oscilloscope output

		TH2F* wfhist;	// wfhist = weighting field histogram, dfhit = drift potentials histogram
		TH2F* dfhist;	

		TH1F* jhist;
		TH1F* tvthhist;
		TH2F* twtothist;
	
		TThread *PotentialThread;	// thread for calculation of potentials
		TThread *CurrentsThread;

		TH2F* dhist;	// histograms:
		TH2F* whist;	// dhist = drift potentials histogram, whist = weighting potentials histogram

		TLine *NLine1;
		TLine *NLine2;

		TEllipse *ellipse;

		TGHProgressBar *CurrentsProgressBar;

	public:
		double B;		// magnetic field
		int wherecut;	// draw drift potential cut where? (x coordinate)
		int wherecut2;	// draw weighting potential cut where?

		TGraph *driftcut;	// graph for drift potential cut
		TGraph *weightcut;	// graph for weighting potential cut
		TGraph *dfieldcut;	// graph for drift field cut
		TGraph *wfieldcut;	// graph for weighting potential cut
		TGraph *dfieldxcut;	// graph for drift field cut
		TGraph *wfieldxcut;	// graph for weighting potential cut
		TGraph *dfieldycut;	// graph for drift field cut
		TGraph *wfieldycut;	// graph for weighting potential cut

		WFGUI(const TGWindow *p,UInt_t w,UInt_t h);	// constructor of GUI
		int CallSetDetType();				
		int CallGetDetType();				
		void CallCalculatePotentials();			// Method to Calculate Potentials
		void CallSetDoping0();					// Method to set bulk n-type, strips p-type
		void CallSetDoping1();					// Method to set bulk p-type, strips n-type
		void CallSetPart(); //!!!!!!
		bool CallSetAlpha();//!!!!!!
		void CallBoundaryConditions();			// Method to set boundary conditions
		void CallCalculateFields();				// Method to calculate fields
		void CallCalculateCurrents();				// Method to calculate currents
		void CloseWindow();						// Method to close window
		TH2F* Getwhist();						// Method which returns whist
		TH2F* Getdhist();						// Method which returns dhist 
		TCanvas* Getcanvasp();					// Method which returns canvasp
		TCanvas* Getcanvasw();					// Method which returns canvasw
		TCanvas* Getcurcanvas();					// Method which returns curcanvas
		TCanvas* Getosccanvas();					// Method which returns osccanvas
		void FillHist();						// Method to draw histograms of drift and weighting potential
		void DrawFieldHist();					// Method to draw field histogram
		TLine* GetNLine1();
		TLine* GetNLine2();
		TEllipse* GetEllipse();
		virtual ~WFGUI();						// destructor of GUI
		static void* StartPotentialCalcTh(void*);
		virtual Int_t ThreadstartPotential();
		virtual Int_t ThreadstopPotential();
		static void* StartCurrentsCalcTh(void*);
		virtual Int_t ThreadstartCurrents();
		virtual Int_t ThreadstopCurrents();
		bool Getstopped();
		void Setstopped(Bool_t);
		//bool GetStopOn();
		int SetStopOn();
		TThread* GetPotentialThread();
		void KillPotentialThread();
		bool Getplotupdate();
		bool GetLessPlot();
		bool GetLess2DPlot();
		void SetPlotUpdate(Bool_t);
		void SetBField(Bool_t);
		void SetLessPlot(Bool_t);
		void SetLess2DPlot(Bool_t);
		
		void SetDiffusion(Bool_t);
		void SetPlotFieldHist(Bool_t);
		TGLabel* GetCalculatingLabel();
		TGLabel* GetCalculatingLabel2();
		void DrawCutGraph(int);
		void DrawAllGraph(int);
		void DrawFieldsAbs(int);
		//static void* StartPotentialCalcTh(void*);
		void SetCutOnStrips();
		void DrawAll();
		void DrawAllw();
		// static void* SetCutOnStrips(void*);
		void SetCutBetweenStrips();
		void DrawFieldsAbsX();
		void DrawFieldsAbsY();
		bool Getdiffusionon();
		//void DrawCurrents();
		TGHProgressBar* GetProgressBar();
		double GetB();
		double GetT();
		void SetQLabel(const char*,const char*,const char*,const char*,const char*,const char*,const char*,const char*,const char*);
		void SetAngleLabel(const char*, const char*);
		void SetGainon(bool);		//sets gain layer active
		bool GetGainon();			//returns if gain layer is active
		void SetYGain(double);	
		void SetGainRatio(double); 
		void SetAllButton(int);
		double GetYGain();	
		double GetGainRatio();
		int GetDimMaxCarriers();
		double GetYMax();
		double GetXMax();
		double GetChargeEntry();
		void SetFileName(string*);
		string* GetFileName();
		//		string* CallSetFileName(string);
		string* CallSetFileName();
		void SetAlphaRange(double);
		void SetPrecision(int);
		double GetAlphaRange();
		int GetPrecision();
		void SetConstQFlag(bool);
		bool GetConstQFlag();
		void SetUniformQFlag(bool);
		void SetUserUniformQFlag(bool);
		bool GetUniformQFlag();
		bool GetUserUniformQFlag();
		void SetOscBW(double);
		double GetOscBW();
		void SetTRise(double);
		double GetTRise();
		void SetTFall(double);
		double GetTFall();
		double GetVth();
		void SetTVth(double);
		double GetTVth();
		void SetJitter(double);
		double GetJitter();
		void SetFTVth(double);
		double GetFTVth();
		void SetFJitter(double);
		double GetFJitter();


		void SetSWidth(double);
		double GetSWidth();
		void SetOscOn(bool);
		bool GetOscOn();
		void SetFileNameOn(bool);
		bool GetFileNameOn();

		void SetBatchOn(bool);
		bool GetBatchOn();
		void SetEvNumber(int);
		int GetEvNumber();
		void SetEvent(int);
		int GetEvent();
		float Getygainlow();
		float Getygainhigh();
		double GetAngle();
		//void SetAngle(double);

		ClassDef(WFGUI,0);  // muss auskommentiert sein wenn .x ... .cxx+
};
