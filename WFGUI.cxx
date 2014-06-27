#include "WFGUI.h"
WFGUI::WFGUI(const TGWindow *p,UInt_t w,UInt_t h): TGMainFrame(p,w,h), dwpot(Potentials(300,1,200,30)) {
// this pointer points to mainframe
	radiobuttonstatus=MIPunif;			//radio button mip set as default
	
	BatchOn=false;
	fileName = "";
	//TThread::SetCancelOn();
	Connect("CloseWindow()", "WFGUI", this, "CloseWindow()");  // connect exit-button of window to CloseWindow()

	SetLayoutManager(new TGHorizontalLayout(this));
	
	// Tabs
	PotentialTab = new TGTab(this,500,200);

	// Tab "Drift Potential"
	DriftPTab = PotentialTab->AddTab("Drift Potential");
	DriftPTab->SetLayoutManager(new TGVerticalLayout(DriftPTab));

	// Tab "Weighting Potential" 
	WeightingPTab = PotentialTab->AddTab("Weighting Potential");
	WeightingPTab->SetLayoutManager(new TGVerticalLayout(WeightingPTab));

	// Tab "Currents"
	CurrentsTab = PotentialTab->AddTab("Currents and Oscilloscope");
	CurrentsTab->SetLayoutManager(new TGVerticalLayout(CurrentsTab));
	
	// Fill Tab Currents//////////////////////////////////////////////////////////////////////////////////////////////////
	// canvas for graphs of currents

	currentscanvas = new TRootEmbeddedCanvas("CurrentsCanvas",CurrentsTab,500,500);			
	CurrentsTab->AddFrame(currentscanvas,new TGLayoutHints(kLHintsExpandY | kLHintsExpandX,1,1,1,1));	
	// add current info frame to current tab					

	CurrentsInfoFrame = new TGVerticalFrame(CurrentsTab,100,200);				
	CurrentsTab->AddFrame(CurrentsInfoFrame,new TGLayoutHints(kLHintsExpandY | kLHintsExpandX,1,1,1,1));		
	// Add frame to current info frame for dispaying information on charge collection		
	
	
	//Charge Collection Frame
	ChargeCollectionFrame = new TGGroupFrame(CurrentsInfoFrame, "Charge Collection", kVerticalFrame);
	
	
	//Fill charge collection frame
	//frame1
	ChargeLabelFrame1 = new TGHorizontalFrame(ChargeCollectionFrame,100,200);		
	//Charges labels
	//electrons charge label
	QETextLabel =  new TGLabel(ChargeLabelFrame1, new TGString("e- charges (e): "));
	ChargeLabelFrame1->AddFrame(QETextLabel, new TGLayoutHints(kLHintsLeft,0,0,1,1));
	QELabel = new TGLabel(ChargeLabelFrame1, new TGString("0"));		
	QELabel->SetBackgroundColor(0xffffff);	
	ChargeLabelFrame1->AddFrame(QELabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));	
	
	//holes charge label										
	QHTextLabel =  new TGLabel(ChargeLabelFrame1, new TGString("h+ charges (e): "));
	ChargeLabelFrame1->AddFrame(QHTextLabel, new TGLayoutHints(kLHintsExpandX,0,0,1,1));
	QHLabel = new TGLabel(ChargeLabelFrame1, new TGString("0"));		//
	QHLabel->SetBackgroundColor(0xffffff);			//
	ChargeLabelFrame1->AddFrame(QHLabel, new TGLayoutHints(kLHintsLeft  | kLHintsTop,0,0,1,1));	

	QEHTextLabel =  new TGLabel(ChargeLabelFrame1, new TGString("e- + h+ charges (e): "));
	ChargeLabelFrame1->AddFrame(QEHTextLabel, new TGLayoutHints(kLHintsExpandX,0,0,1,1));
	QEHLabel = new TGLabel(ChargeLabelFrame1, new TGString("0"));		//
	QEHLabel->SetBackgroundColor(0xffffff);			//
	ChargeLabelFrame1->AddFrame(QEHLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));	

	ChargeCollectionFrame->AddFrame(ChargeLabelFrame1, new TGLayoutHints(kLHintsExpandX | kLHintsTop,1,1,1,1));	

	///////////////////////////////////////////////////////////////////////////////	
	//Charges labels
	//frame2
	ChargeLabelFrame2 = new TGHorizontalFrame(ChargeCollectionFrame,100,200);
	//gain electrons charge label
	QEGTextLabel =  new TGLabel(ChargeLabelFrame2, new TGString("Gain e- charges (e): "));
	ChargeLabelFrame2->AddFrame(QEGTextLabel, new TGLayoutHints(kLHintsLeft,0,0,1,1));
	QEGLabel = new TGLabel(ChargeLabelFrame2, new TGString("0"));		
	QEGLabel->SetBackgroundColor(0xffffff);	
	ChargeLabelFrame2->AddFrame(QEGLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));	
	
	//gain holes charge label										
	QHGTextLabel =  new TGLabel(ChargeLabelFrame2, new TGString("Gain h+ charges (e): "));
	ChargeLabelFrame2->AddFrame(QHGTextLabel, new TGLayoutHints(kLHintsExpandX,0,0,1,1));
	QHGLabel = new TGLabel(ChargeLabelFrame2, new TGString("0"));		//
	QHGLabel->SetBackgroundColor(0xffffff);			//
	ChargeLabelFrame2->AddFrame(QHGLabel, new TGLayoutHints(kLHintsLeft  | kLHintsTop,0,0,1,1));	
	
	//total charge label
	QEHGTextLabel =  new TGLabel(ChargeLabelFrame2, new TGString("Gain e- + h+ charges (e): "));
	ChargeLabelFrame2->AddFrame(QEHGTextLabel, new TGLayoutHints(kLHintsExpandX,0,0,1,1));
	QEHGLabel = new TGLabel(ChargeLabelFrame2, new TGString("0"));																		//
	QEHGLabel->SetBackgroundColor(0xffffff);																			//
	ChargeLabelFrame2->AddFrame(QEHGLabel, new TGLayoutHints(kLHintsLeft  | kLHintsTop,0,0,1,1));					//
	
	ChargeCollectionFrame->AddFrame(ChargeLabelFrame2, new TGLayoutHints(kLHintsExpandX | kLHintsTop,1,1,1,1));	
	
	////////////////////////////////////////////////////////////////////////////////////
	//frame3
	ChargeLabelFrame3 = new TGHorizontalFrame(ChargeCollectionFrame,100,200);
	//gain electrons charge label
	QETotTextLabel =  new TGLabel(ChargeLabelFrame3, new TGString(" Total e- charges (e): "));
	ChargeLabelFrame3->AddFrame(QETotTextLabel, new TGLayoutHints(kLHintsLeft,0,0,1,1));
	QETotLabel = new TGLabel(ChargeLabelFrame3, new TGString("0"));		
	QETotLabel->SetBackgroundColor(0xffffff);	
	ChargeLabelFrame3->AddFrame(QETotLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));	
	
	//gain holes charge label										
	QHTotTextLabel =  new TGLabel(ChargeLabelFrame3, new TGString("Total h+ charges (e): "));
	ChargeLabelFrame3->AddFrame(QHTotTextLabel, new TGLayoutHints(kLHintsExpandX,0,0,1,1));
	QHTotLabel = new TGLabel(ChargeLabelFrame3, new TGString("0"));		//
	QHTotLabel->SetBackgroundColor(0xffffff);			//
	ChargeLabelFrame3->AddFrame(QHTotLabel, new TGLayoutHints(kLHintsLeft  | kLHintsTop,0,0,1,1));	
	
	//total charge label
	QTotTextLabel =  new TGLabel(ChargeLabelFrame3, new TGString("Total Charges (e): "));
	ChargeLabelFrame3->AddFrame(QTotTextLabel, new TGLayoutHints(kLHintsExpandX,0,0,1,1));
	QTotLabel = new TGLabel(ChargeLabelFrame3, new TGString("0"));														//
	QTotLabel->SetBackgroundColor(0xffffff);																			//
	ChargeLabelFrame3->AddFrame(QTotLabel, new TGLayoutHints(kLHintsLeft  | kLHintsTop,0,0,1,1));                       //
	
	ChargeCollectionFrame->AddFrame(ChargeLabelFrame3, new TGLayoutHints(kLHintsExpandX | kLHintsTop,1,1,1,1));	

	
	///////////////////////////////////////////////////////////////////////
	

	CurrentsInfoFrame->AddFrame(ChargeCollectionFrame, new TGLayoutHints(kLHintsExpandX | kLHintsTop,1,1,1,1));			//
	
	// add lorentz frame to currents info frame																			//
	LorentzInfoFrame = new TGGroupFrame(CurrentsInfoFrame, "Lorentz Drift", kHorizontalFrame);							//
																														//
	LorentzeTextLabel =  new TGLabel(LorentzInfoFrame, new TGString("e- Lorentz Angle (degree): "));					//
	LorentzInfoFrame->AddFrame(LorentzeTextLabel, new TGLayoutHints(kLHintsExpandX,0,0,1,1));							//
	LorentzeLabel = new TGLabel(LorentzInfoFrame, new TGString("0"));													//
	//QELabel->Set3DStyle(kSunkenFrame);																				//
	LorentzeLabel->SetBackgroundColor(0xffffff);																		//
	LorentzInfoFrame->AddFrame(LorentzeLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));						//
																														//
	LorentzhTextLabel =  new TGLabel(LorentzInfoFrame, new TGString("h+ Lorentz Angle (degree): "));					//
	LorentzInfoFrame->AddFrame(LorentzhTextLabel, new TGLayoutHints(kLHintsExpandX,0,0,1,1));							//
	LorentzhLabel = new TGLabel(LorentzInfoFrame, new TGString("0"));													//
	//QELabel->Set3DStyle(kSunkenFrame);																				//
	LorentzhLabel->SetBackgroundColor(0xffffff);																		//
	LorentzInfoFrame->AddFrame(LorentzhLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));						//
																														//
	CurrentsLabelFrame = new TGHorizontalFrame(CurrentsInfoFrame,100,50);												//
																														//
	ElectronsLabel =  new TGLabel(CurrentsLabelFrame, new TGString("Electrons"));										//
	ElectronsLabel->SetBackgroundColor(0xff0000);																		//
	CurrentsLabelFrame->AddFrame(ElectronsLabel,new TGLayoutHints(kLHintsTop | kLHintsExpandX,1,1,1,1));				//

	GainElectronsLabel =  new TGLabel(CurrentsLabelFrame, new TGString("Gain El."));									      
	GainElectronsLabel->SetBackgroundColor(0xff00ff);													      
	CurrentsLabelFrame->AddFrame(GainElectronsLabel,new TGLayoutHints(kLHintsTop | kLHintsExpandX,1,1,1,1));

	HolesLabel =  new TGLabel(CurrentsLabelFrame, new TGString("Holes"));												//
	HolesLabel->SetBackgroundColor(0x0066ff);																			//
	CurrentsLabelFrame->AddFrame(HolesLabel,new TGLayoutHints(kLHintsTop | kLHintsExpandX,1,1,1,1));					//

	
	GainHolesLabel =  new TGLabel(CurrentsLabelFrame, new TGString("Gain Holes"));					
	GainHolesLabel->SetBackgroundColor(0x00ffff); 
	CurrentsLabelFrame->AddFrame(GainHolesLabel,new TGLayoutHints(kLHintsTop | kLHintsExpandX,1,1,1,1));

	TotalLabel =  new TGLabel(CurrentsLabelFrame, new TGString("Total"));												//
	TotalLabel->SetBackgroundColor(0x00ff00);																			//
	CurrentsLabelFrame->AddFrame(TotalLabel,new TGLayoutHints(kLHintsTop | kLHintsExpandX,1,1,1,1));					//

	CarriersInFrame = new TGHorizontalFrame(CurrentsInfoFrame,100,50);													//
	CarriersInLabel =  new TGLabel(CarriersInFrame, new TGString("Particle hits Detector at: "));						//
	CarriersInFrame->AddFrame(CarriersInLabel, new TGLayoutHints(kLHintsLeft,1,1,1,1));							     
	CarriersInNumberentry = new TGNumberEntry(CarriersInFrame, (Double_t) 0,8,-1,TGNumberFormat::kNESInteger,
						  TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0.,1000.);										//
	CarriersInFrame->AddFrame(CarriersInNumberentry, new TGLayoutHints(kLHintsLeft,2,2,2,2));							//
		
	CarriersAngleLabel =  new TGLabel(CarriersInFrame, new TGString("Angle (deg): "));
	CarriersInFrame->AddFrame(CarriersAngleLabel, new TGLayoutHints(kLHintsCenterX,1,1,1,1));
	CarriersAngleNumberentry = new TGNumberEntry(CarriersInFrame, (Double_t) 0,8,-1,TGNumberFormat::kNESInteger,
						     TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,45);					
	CarriersInFrame->AddFrame(CarriersAngleNumberentry, new TGLayoutHints(kLHintsLeft,2,2,2,2));							//
	
	//
	CurrentsProgressBar = new TGHProgressBar(CurrentsInfoFrame,TGProgressBar::kStandard,500);							//
	CurrentsInfoFrame->AddFrame(CurrentsProgressBar,new TGLayoutHints(kLHintsExpandX,1,1,1,1));							//
	CurrentsProgressBar->ShowPosition(kTRUE,kTRUE);	
	CurrentsProgressBar->Percent(0);

	CurrentsInfoFrame->AddFrame(CurrentsLabelFrame, new TGLayoutHints(kLHintsExpandX | kLHintsTop,1,1,1,1));			//
	CurrentsInfoFrame->AddFrame(CarriersInFrame, new TGLayoutHints(kLHintsExpandX,1,1,1,1));							//
	CurrentsInfoFrame->AddFrame(CarriersInFrame, new TGLayoutHints(kLHintsExpandX,1,1,1,1));							//
	CurrentsInfoFrame->AddFrame(ChargeCollectionFrame,new TGLayoutHints(kLHintsExpandX,1,1,1,1));						//
	CurrentsInfoFrame->AddFrame(LorentzInfoFrame,new TGLayoutHints(kLHintsExpandX,1,1,1,1));							//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	// Tab "Oscilloscope" 
	OscilloscopeTab = PotentialTab->AddTab("Electronics");
	OscilloscopeTab->SetLayoutManager(new TGVerticalLayout(OscilloscopeTab));
	
	oscilloscopecanvas = new TRootEmbeddedCanvas("OscilloscopeCanvas",OscilloscopeTab,500,500);			
	OscilloscopeTab->AddFrame(oscilloscopecanvas,new TGLayoutHints(kLHintsExpandY | kLHintsExpandX,1,1,1,1));	
	// add current info frame to current tab					

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	PotentialTab->SetTab(0);

	// Fill Drift and Weighting Potential Tab/////////////////////////////////////////////////////////////////////////////////////////

	DriftPTabFrame = new TGHorizontalFrame(DriftPTab,500,400);    
	WeightPTabFrame = new TGHorizontalFrame(WeightingPTab,500,400);																	//
	// create embedded canvases on Tab frames to draw histograms																	//
	driftcanvas = new TRootEmbeddedCanvas("DriftCanvas",DriftPTab,500,200); //canvas for drift potential
	driftpcanvas = new TRootEmbeddedCanvas("DriftPCanvas",DriftPTabFrame,500,300); // canvas for cut potential
	CalculatingLabel = new TGLabel(DriftPTab, new TGString("First press 'Set' then 'Calculate Potential' and finally 'Calculate Currents' "));
	DriftPTab->AddFrame(CalculatingLabel, new TGLayoutHints(kLHintsExpandX | kLHintsTop,0,0,1,1));								
	DriftPTab->AddFrame(driftcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,1,1,1,1)); // Add canvas to Tab frames		//
	DriftPTab->AddFrame(DriftPTabFrame,new TGLayoutHints( kLHintsBottom | kLHintsExpandX,1,1,1,1));	
	DriftPTabFrame->AddFrame(driftpcanvas,new TGLayoutHints(kLHintsExpandY | kLHintsExpandX,1,1,1,1));


	weightcanvas = new TRootEmbeddedCanvas("weightCanvas",WeightingPTab,500,200); // canvas for cut potential
	weightpcanvas = new TRootEmbeddedCanvas("weightPCanvas",WeightPTabFrame,500,300); // canvas for cut potential	


	// add Label to show calculation progress																				      																																	//
	CalculatingLabel2 = new TGLabel(WeightingPTab, new TGString(" Press Button 'Calculate' to calculate Potentials and Fields"));	//
	WeightingPTab->AddFrame(CalculatingLabel2, new TGLayoutHints(kLHintsExpandX | kLHintsTop,0,0,1,1));								//
	// add canvases to frames																										//


	//	DriftPTabFrame->AddFrame(driftfcanvas,new TGLayoutHints(kLHintsExpandY | kLHintsExpandX,1,1,1,1));								//
																																	//
	// labelstabframe = frame between drift potential histogram canvas and drift potential cuts and drift field cuts				//
	LabelsTabFrame = new TGHorizontalFrame(DriftPTab, 500,20);																		//
	DriftCutLabel = new TGLabel(LabelsTabFrame, new TGString("Plotting at:"));													//
	LabelsTabFrame->AddFrame(DriftCutLabel, new TGLayoutHints(kLHintsLeft,1,1,5,1));												//
	// create buttons for label frame																								//
	OnStripsButton = new TGTextButton(LabelsTabFrame,new TGHotString("On Strips"));	
	OnStripsButton->SetTextColor(0xff0000);			//
	LabelsTabFrame->AddFrame(OnStripsButton, new TGLayoutHints(kLHintsLeft,5,1,1,1));
	OnStripsButton->Connect("Clicked()", "WFGUI", this, "SetCutOnStrips()");														//
																																	//
	BetweenStripsButton = new TGTextButton(LabelsTabFrame,new TGHotString("Between Strips"));										//
	BetweenStripsButton->SetTextColor(0x0000ff);																					//
	LabelsTabFrame->AddFrame(BetweenStripsButton, new TGLayoutHints(kLHintsLeft,5,1,1,1));											//
	BetweenStripsButton->Connect("Clicked()", "WFGUI", this, "SetCutBetweenStrips()");
	WhereCut = new TGNumberEntry(LabelsTabFrame, (Double_t) 0,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax);
	LabelsTabFrame->AddFrame(WhereCut, new TGLayoutHints(kLHintsLeft,5,1,1,1));
	DrawCutsUserEntry = new TGTextButton(LabelsTabFrame, new TGHotString("Draw"));									       
	LabelsTabFrame->AddFrame(DrawCutsUserEntry, new TGLayoutHints(kLHintsLeft,1,1,1,1));
	DrawCutsUserEntry->Connect("Clicked()", "WFGUI", this, "DrawAll()");   	
	FieldLabel = new TGLabel(LabelsTabFrame, new TGString("Field:"));																								//
	//	EtotButton = new TGTextButton(LabelsTabFrame,new TGHotString("|Etot|"));
	//	LabelsTabFrame->AddFrame(EtotButton, new TGLayoutHints(kLHintsRight,5,1,1,1));	
	// EtotButton->Connect("Clicked()", "WFGUI", this, "DrawFieldsAbs(int)");																//
																																	//
	ExButton = new TGTextButton(LabelsTabFrame,new TGHotString("|Ex|"));															//
	LabelsTabFrame->AddFrame(ExButton, new TGLayoutHints(kLHintsRight,5,1,1,1));													//
	ExButton->Connect("Clicked()", "WFGUI", this, "DrawFieldsAbsX()");																//
																																	//
	EyButton = new TGTextButton(LabelsTabFrame,new TGHotString("|Ey|"));															//
	LabelsTabFrame->AddFrame(EyButton, new TGLayoutHints(kLHintsRight,5,1,1,1));													//
	EyButton->Connect("Clicked()", "WFGUI", this, "DrawFieldsAbsY()");																//
																																	//
	OnStripsButton->SetEnabled(kFALSE);				//
	BetweenStripsButton->SetEnabled(kFALSE);		//
	ExButton->SetEnabled(kFALSE);					//
	EyButton->SetEnabled(kFALSE);								//
	DrawCutsUserEntry->SetEnabled(kFALSE);

	LabelsTabFrame->AddFrame(FieldLabel, new TGLayoutHints(kLHintsRight,30,1,5,1));	
	DriftPTab->AddFrame(LabelsTabFrame, new TGLayoutHints(kLHintsExpandX,1,1,1,1));	
		//
	// labelstabframe2 = frame between weighting pot. histogram and weighting pot. cuts and weighting fields						//
	LabelsTabFrame2 = new TGHorizontalFrame(WeightingPTab, 500,20);	
	WeightingCutLabel = new TGLabel(LabelsTabFrame2, new TGString("Plotting at:"));
	LabelsTabFrame2->AddFrame(WeightingCutLabel, new TGLayoutHints(kLHintsLeft,1,1,5,1));	//
	OnStripsButton2 = new TGTextButton(LabelsTabFrame2,new TGHotString("On Strips"));
	OnStripsButton2->SetTextColor(0xff0000);	
	LabelsTabFrame2->AddFrame(OnStripsButton2, new TGLayoutHints(kLHintsLeft,5,1,1,1));
	OnStripsButton2->Connect("Clicked()", "WFGUI", this, "SetCutOnStrips()");																						
	BetweenStripsButton2 = new TGTextButton(LabelsTabFrame2,new TGHotString("Between Strips"));	
	BetweenStripsButton2->SetTextColor(0x0000ff);																					//
	LabelsTabFrame2->AddFrame(BetweenStripsButton2, new TGLayoutHints(kLHintsLeft,5,1,1,1));										//
	BetweenStripsButton2->Connect("Clicked()", "WFGUI", this, "SetCutBetweenStrips()");												//
																																	//
	WhereCut2 = new TGNumberEntry(LabelsTabFrame2, (Double_t) 0,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax);
	LabelsTabFrame2->AddFrame(WhereCut2, new TGLayoutHints(kLHintsLeft,5,1,1,1));

	DrawCutsUserEntry2 = new TGTextButton(LabelsTabFrame2, new TGHotString("Draw"));
	LabelsTabFrame2->AddFrame(DrawCutsUserEntry2, new TGLayoutHints(kLHintsLeft,1,1,1,1));	
	DrawCutsUserEntry2->Connect("Clicked()", "WFGUI", this, "DrawAllw()");					//
	FieldLabel2 = new TGLabel(LabelsTabFrame2, new TGString("Field:"));	
	ExButton2 = new TGTextButton(LabelsTabFrame2,new TGHotString("|Ex|"));	
	LabelsTabFrame2->AddFrame(ExButton2, new TGLayoutHints(kLHintsRight,5,1,1,1));	
	ExButton2->Connect("Clicked()", "WFGUI", this, "DrawFieldsAbsX()");			//
	EyButton2 = new TGTextButton(LabelsTabFrame2,new TGHotString("|Ey|"));
	LabelsTabFrame2->AddFrame(EyButton2, new TGLayoutHints(kLHintsRight,5,1,1,1));	
	EyButton2->Connect("Clicked()", "WFGUI", this, "DrawFieldsAbsY()");																				
	OnStripsButton2->SetEnabled(kFALSE);//
	BetweenStripsButton2->SetEnabled(kFALSE);		//
	ExButton2->SetEnabled(kFALSE);					//
	EyButton2->SetEnabled(kFALSE);				//
	DrawCutsUserEntry2->SetEnabled(kFALSE);
	//	EtotButton2->SetEnabled(kFALSE);																								//
	// add frames and canvases to correct frame																						//
	LabelsTabFrame2->AddFrame(FieldLabel2, new TGLayoutHints(kLHintsRight,30,1,5,1));												//
																																
	WeightingPTab->AddFrame(weightcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,1,1,1,1));
	WeightingPTab->AddFrame(LabelsTabFrame2, new TGLayoutHints(kLHintsExpandX,1,1,1,1));	
	WeightingPTab->AddFrame(WeightPTabFrame, new TGLayoutHints(kLHintsExpandX,1,1,1,1));	
	WeightPTabFrame->AddFrame(weightpcanvas,new TGLayoutHints(kLHintsExpandY | kLHintsExpandX,1,1,1,1));
	//	WeightPTabFrame->AddFrame(weightfcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,1,1,1,1));							//
																																	//
	AddFrame(PotentialTab,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,2,2,2,2)); // Add tabs to mainframe						//

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// General frame containing everything
	SettingsGlobalFrame = new TGVerticalFrame(this,800,90);  
	SettingsFrame = new TGHorizontalFrame(SettingsGlobalFrame,800,90);  
	//Pixel_t back = TColor::RGB2Pixel(255,0,0);
	//SettingsFrame->SetBackgroundColor(back);
	//da leggere
	//http://root.cern.ch/root/html/TGCompositeFrame.html
	//http://root.cern.ch/root/html/TGLayoutManager.html
	//www.eclipsezone.com/eclipse/forums/t99010.html
	//http://www.eclipse.org/eclipse/platform-core/documents/3.1/debug.html
	
	
	////////////////////////////////
	// File Name Frame /////////////////
	NameFrame = new TGGroupFrame(SettingsFrame," File Name",kHorizontalFrame);
	FileNameOnButton = new TGCheckButton(NameFrame,"ON ",0);
	NameFrame->AddFrame(FileNameOnButton, new TGLayoutHints(kLHintsLeft,5,5,1,5));//
	FileNameOnButton->Connect("Toggled(Bool_t)","WFGUI",this,"SetFileNameOn(Bool_t)");


	//NameFrame->SetLayoutBroken(kTRUE);
	FileNameEntry = new TGTextEntry(NameFrame, new TGTextBuffer(14),kSunkenFrame | kDoubleBorder | kOwnBackground);
	FileNameEntry->SetText("wf");
	FileNameEntry->SetMaxLength(4096);
	FileNameEntry->SetAlignment(kTextLeft);
	//FileNameEntry->SetText("");
	FileNameEntry->Resize(104,FileNameEntry->GetDefaultHeight());
	NameFrame->AddFrame(FileNameEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,0,0));
	FileNameEntry->MoveResize(56,80,200,22);
	FileNameEntry->SetState(kFALSE);

	
	


	
	SettingsFrame->AddFrame(NameFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft,2,2,2,2));
	//AddFrame(SettingsFrame,new TGLayoutHints(kLHintsExpandX,2,2,2,2));
		///////////////////////////////////////////////////////////////////
	// Batch Frame //////////


	BatchFrame = new TGGroupFrame(SettingsFrame,"Batch",kHorizontalFrame);
	BatchOnButton = new TGCheckButton(BatchFrame, "ON",0);
	BatchFrame->AddFrame(BatchOnButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,1,5));
	BatchOnButton->Connect("Toggled(Bool_t)","WFGUI",this,"SetBatchOn(Bool_t)");	
	EventsLabel = new TGLabel(BatchFrame,"# of events:");
	BatchFrame->AddFrame(EventsLabel, new TGLayoutHints(kLHintsLeft,1,1,1,1));
	EventsEntry = new TGNumberEntry(BatchFrame, (Int_t) 1,8,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative);
	BatchFrame->AddFrame(EventsEntry, new TGLayoutHints(kLHintsCenterX,20,1,1,1));
	EventsEntry->SetState(kFALSE);
	SettingsFrame->AddFrame(BatchFrame, new TGLayoutHints(kLHintsCenterY| kLHintsExpandX,2,2,0,0));
	
	///////// End Batch Frame //////////////////////
	
	SettingsGlobalFrame->AddFrame(SettingsFrame,new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	
	///////General Frame 2

	SettingsFrame2 = new TGHorizontalFrame(SettingsGlobalFrame,600,90);  
	RightFrame = new TGVerticalFrame(SettingsFrame2,600,90);  
	////////////////////////////////
	// Left Frame /////////////////
       	ControlFrame = new TGGroupFrame(SettingsFrame2," Control",kVerticalFrame);											//

	PrecisionFrame = new TGHorizontalFrame(ControlFrame,300,90);	
	PrecisionLabel = new TGLabel(PrecisionFrame,"Precision (1=best, 10=fastest):");
	PrecisionFrame->AddFrame(PrecisionLabel, new TGLayoutHints(kLHintsLeft,1,1,1,1));
	PrecisionEntry = new TGNumberEntry(PrecisionFrame, (Double_t) 1,8,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,1,10);
	PrecisionFrame->AddFrame(PrecisionEntry, new TGLayoutHints(kLHintsCenterX,5,1,1,1));
	PrecisionEntry->SetState(kTRUE);//
	ControlFrame->AddFrame(PrecisionFrame, new TGLayoutHints(kLHintsCenterY, 2,2,2,2));
	//////////////////////////////
	// Select Particles Frame ////

//	ParticlesPropertiesFrame = new TGButtonGroup(SettingsFrame,"Select Particles",kVerticalFrame);
	ParticlesPropertiesFrame = new TGButtonGroup(ControlFrame,"Select Particles",kVerticalFrame);

	ParticlesPropertiesFrame->SetLayoutBroken(kTRUE);
												//
	SelectPartButton[0] = new TGRadioButton(ParticlesPropertiesFrame,"MIP: uniform Q, Qtot = 75*Height");	//mip uniform 
	SelectPartButton[1] = new TGRadioButton(ParticlesPropertiesFrame,"MIP: non uniform Q, Qtot = 75*Height"); //mip non uniform
	SelectPartButton[2] = new TGRadioButton(ParticlesPropertiesFrame,"MIP: non uniform, Qtot = Landau");	//mip landau
	SelectPartButton[3] = new TGRadioButton(ParticlesPropertiesFrame,"MIP: uniform Q, Q/micron= ");		//mip uniform user defined charge
	SelectPartButton[4] = new TGRadioButton(ParticlesPropertiesFrame,"alpha from top");				//alpha particle
	SelectPartButton[5] = new TGRadioButton(ParticlesPropertiesFrame,"alpha from bottom");
	
	ParticlesPropertiesFrame->AddFrame(SelectPartButton[0], new TGLayoutHints(kLHintsCenterX,-90,10,0,0));	
	ParticlesPropertiesFrame->AddFrame(SelectPartButton[1], new TGLayoutHints(kLHintsCenterX,-90,10,0,0));	
	ParticlesPropertiesFrame->AddFrame(SelectPartButton[2], new TGLayoutHints(kLHintsCenterX,-90,10,0,0));	
	ParticlesPropertiesFrame->AddFrame(SelectPartButton[3], new TGLayoutHints(kLHintsCenterX,-90,10,0,0));	
	ParticlesPropertiesFrame->AddFrame(SelectPartButton[4], new TGLayoutHints(kLHintsCenterX,-90,10,0,0));	

	SelectPartButton[0]->Connect("Toggled(Bool_t)","WFGUI",this,"CallSetPart()");
	SelectPartButton[1]->Connect("Toggled(Bool_t)","WFGUI",this,"CallSetPart()");
	SelectPartButton[2]->Connect("Toggled(Bool_t)","WFGUI",this,"CallSetPart()");
	SelectPartButton[3]->Connect("Toggled(Bool_t)","WFGUI",this,"CallSetPart()");
	SelectPartButton[4]->Connect("Toggled(Bool_t)","WFGUI",this,"CallSetPart()");
	SelectPartButton[5]->Connect("Toggled(Bool_t)","WFGUI",this,"CallSetPart()");

	SelectPartButton[0]->SetTextJustify(36); //MIP uniform
	SelectPartButton[0]->SetMargins(0,0,0,0);
	SelectPartButton[0]->SetWrapLength(-1);
	ParticlesPropertiesFrame->AddFrame(SelectPartButton[0], new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	SelectPartButton[0]->MoveResize(8,16,220,17);
	SelectPartButton[0]->SetOn(kTRUE);
	///////////////////////////////////////////////////////////////////////////////////////////
	SelectPartButton[1]->SetTextJustify(36); //MIP non uniform
	SelectPartButton[1]->SetMargins(0,0,0,0);
	SelectPartButton[1]->SetWrapLength(-1);
	ParticlesPropertiesFrame->AddFrame(SelectPartButton[1], new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	SelectPartButton[1]->MoveResize(8,44,230,17);
	///////////////////////////////////////////////////////////////////////////////////////////
	SelectPartButton[2]->SetTextJustify(36); //MIP landau
	SelectPartButton[2]->SetMargins(0,0,0,0);
	SelectPartButton[2]->SetWrapLength(-1);
	ParticlesPropertiesFrame->AddFrame(SelectPartButton[2], new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	SelectPartButton[2]->MoveResize(8,72,220,17);
	///////////////////////////////////////////////////////////////////////////////////////////
	SelectPartButton[3]->SetTextJustify(36); //MIP user defined charge
	SelectPartButton[3]->SetMargins(0,0,0,0);
	SelectPartButton[3]->SetWrapLength(-1);
	ParticlesPropertiesFrame->AddFrame(SelectPartButton[3], new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	SelectPartButton[3]->MoveResize(8,102,180,17);	
	
	Chargeentry = new TGNumberEntry(ParticlesPropertiesFrame, (Double_t)75,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);				//
	ParticlesPropertiesFrame->AddFrame(Chargeentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,1,1,1,1));
	Chargeentry->MoveResize(180,102,60,22);
	Chargeentry->SetState(kFALSE);

	//////////////////////////////////////////////////////////////////////////////////////////
	SelectPartButton[4]->SetTextJustify(36); //alpha
	SelectPartButton[4]->SetMargins(0,0,0,0);
	SelectPartButton[4]->SetWrapLength(-1);
	SelectPartButton[4]->MoveResize(8,132,150,17);
	ParticlesPropertiesFrame->AddFrame(SelectPartButton[4], new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	SelectPartButton[5]->SetTextJustify(36); //alpha
	SelectPartButton[5]->SetMargins(0,0,0,0);
	SelectPartButton[5]->SetWrapLength(-1);
	SelectPartButton[5]->MoveResize(8,162,150,17);
	ParticlesPropertiesFrame->AddFrame(SelectPartButton[5], new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	// combo box
/*
	SetAlpha = new TGComboBox(ParticlesPropertiesFrame,-1,kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);
	ParticlesPropertiesFrame->AddFrame(SetAlpha, new TGLayoutHints(kLHintsNormal));
	//SetAlpha->AddEntry("select origin",-1);
	SetAlpha->AddEntry("top",1);
	SetAlpha->AddEntry("bottom",2);
	SetAlpha->Resize(102,22);
	SetAlpha->Select(-1);
	SetAlpha->MoveResize(64,132,102,22);
	SetAlpha->SetEnabled(kFALSE);*/
	
	AlphaRangeFrame = new TGHorizontalFrame(ParticlesPropertiesFrame,200,90);	
	RangeLabel = new TGLabel(AlphaRangeFrame,"Set range (um):");
	AlphaRangeFrame->AddFrame(RangeLabel, new TGLayoutHints(kLHintsLeft,10,1,1,1));
	RangeEntry = new TGNumberEntry(AlphaRangeFrame, (Double_t) 10,8,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative);
	AlphaRangeFrame->AddFrame(RangeEntry, new TGLayoutHints(kLHintsCenterX,25,1,1,1));
	AlphaRangeFrame->MoveResize(18,192,180,30);
	RangeEntry->SetState(kTRUE);//
	ParticlesPropertiesFrame->AddFrame(AlphaRangeFrame, new TGLayoutHints(kLHintsCenterY, 2,2,2,2));
	



	ParticlesPropertiesFrame->SetLayoutManager(new TGVerticalLayout(ParticlesPropertiesFrame));
	ParticlesPropertiesFrame->Resize(280,240);

	//	SettingsFrame->AddFrame(ParticlesPropertiesFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	ControlFrame->AddFrame(ParticlesPropertiesFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 1,1,1,1));


	//////////////////////////////////////
	// Plot setting Frame //

	PlotGroupFrame = new TGGroupFrame(ControlFrame,"Plot Settings",kVerticalFrame); // GroupFrame: plot settings	

	//	UpdateFrame = new TGHorizontalFrame(PlotGroupFrame,50,200);

	Eyes = new TGCheckButton(PlotGroupFrame, "Draw Electric Field",0);
	Eyes->Connect("Toggled(Bool_t)","WFGUI",this,"SetPlotFieldHist(Bool_t)");	
      
	//	UpdatePlotCheck = new TGCheckButton(PlotGroupFrame, "Update Plot while Calculating",0);	
	// UpdatePlotCheck->Connect("Toggled(Bool_t)","WFGUI",this,"SetPlotUpdate(Bool_t)");

	LessPlotCheck = new TGCheckButton(PlotGroupFrame, "No 1D Plots",0);	
	LessPlotCheck->Connect("Toggled(Bool_t)","WFGUI",this,"SetLessPlot(Bool_t)");

	Less2DPlotCheck = new TGCheckButton(PlotGroupFrame, "No 1D and 2D Plots",0);	
	Less2DPlotCheck->Connect("Toggled(Bool_t)","WFGUI",this,"SetLess2DPlot(Bool_t)");


	PlotGroupFrame->AddFrame(Eyes, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,1,5));
	//	PlotGroupFrame->AddFrame(UpdatePlotCheck, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,1,5));	
	PlotGroupFrame->AddFrame(LessPlotCheck, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,1,5));	
	PlotGroupFrame->AddFrame(Less2DPlotCheck, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,1,5));	

	//	PlotGroupFrame->AddFrame(UpdateFrame,new TGLayoutHints(kLHintsLeft,1,1,1,1));		
	//DetectorPropertiesFrame->AddFrame(PlotGroupFrame, new TGLayoutHints(kLHintsCenterY,1,1,1,1));

	ControlFrame->AddFrame(PlotGroupFrame, new TGLayoutHints(kLHintsCenterY | kLHintsExpandX,1,1,1,1));


	////////////////////////////////////////////
	// Fill currents setting frame 

	CurrentsFrame = new TGGroupFrame(ControlFrame,"Currents",kVerticalFrame);
	//Pixel_t back3 = TColor::RGB2Pixel(0,0,255);
	//CurrentsFrame->SetBackgroundColor(back3);
	CurrentsButton[0] = new TGCheckButton(CurrentsFrame,new TGHotString("Switch B-Field on and set to (T):"),0);
	CurrentsButton[1] = new TGCheckButton(CurrentsFrame,new TGHotString("Diffusion"),0);
	BfieldEntry = new TGNumberEntry(CurrentsFrame, (Double_t) 0,8,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber);
	BfieldEntry->SetState(kFALSE);	
					//
	
	
	
	
	/*
	Biasentry = new TGNumberEntry(VolSetFrame, (Double_t) 200,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);		
	VolSetFrame->AddFrame(Biasentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,1,1,1,1));	
	BiasLabel = new TGLabel(VolLabelFrame,new TGString("Bias Voltage (V):"));
	BiasLabel->SetMargins(0,0,4,4);	
	VolLabelFrame->AddFrame(BiasLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));
	Depletionentry = new TGNumberEntry(VolSetFrame, (Double_t) 50,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);		//
	VolSetFrame->AddFrame(Depletionentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,1,1,1,1));											//
	DepletionLabel = new TGLabel(VolLabelFrame,new TGString("Depletion Voltage (V):"));	
	DepletionLabel->SetMargins(0,0,4,4);
	VolLabelFrame->AddFrame(DepletionLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));	
	
	*/
	
	TempSetFrame = new TGHorizontalFrame(CurrentsFrame,50,90);	
		
	TempLabel = new TGLabel(TempSetFrame,"Temperature (K):");
	TempSetFrame->AddFrame(TempLabel, new TGLayoutHints(kLHintsLeft,1,1,1,1));
	TempEntry = new TGNumberEntry(TempSetFrame, (Double_t) 300,8,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative);
	TempSetFrame->AddFrame(TempEntry, new TGLayoutHints(kLHintsCenterX,20,1,1,1));
	//	TempEntry->SetState(kTRUE);//

	CurrentsButton[1]->Connect("Toggled(Bool_t)","WFGUI",this,"SetDiffusion(Bool_t)");
	CurrentsButton[0]->Connect("Toggled(Bool_t)","WFGUI",this,"SetBField(Bool_t)");		//
	CurrentsFrame->AddFrame(CurrentsButton[0], new TGLayoutHints(kLHintsLeft,0,40,1,5));
	CurrentsFrame->AddFrame(BfieldEntry, new TGLayoutHints(kLHintsLeft,25,40,1,5));	
	CurrentsFrame->AddFrame(CurrentsButton[1], new TGLayoutHints(kLHintsLeft,0,1,1,5));	
	CurrentsFrame->AddFrame(TempSetFrame,new TGLayoutHints(kLHintsLeft,0,40,1,5));
	
	
	/*
	CurrentsFrame->AddFrame(TempLabel, new TGLayoutHints(kLHintsLeft,25,40,1,5));	
	CurrentsFrame->AddFrame(TempEntry, new TGLayoutHints(kLHintsLeft,25,40,1,5));	
*/

	//SaveButton = new TGTextButton(ButtonFrame," Save ");	
	//ButtonCurFrame->AddFrame(SaveButton,  new TGLayoutHints(kLHintsCenterX | kLHintsBottom,1,1,1,1));
	//	SettingsFrame->AddFrame(CurrentsFrame,new TGLayoutHints(kLHintsCenterX,2,2,1,1));

	//CurrentsFrame->MoveResize(40,20,200,100);
	ControlFrame->AddFrame(CurrentsFrame,new TGLayoutHints(kLHintsCenterX | kLHintsExpandX,1,1,1,1));


	////////////////////////////////////////
	// Set-Calculate Potential Frame //////

	ButtonPotFrame = new TGHorizontalFrame(ControlFrame,50,100);	
	//Create Set Button						//
	SetButton = new TGTextButton(ButtonPotFrame," Set ");
	ButtonPotFrame->AddFrame(SetButton, new TGLayoutHints(kLHintsCenterX,1,1,1,1));//
	SetButton->Associate(this);	

	//Connect Set Button to CallBoundary Conditions()

	SetButton->Connect("Clicked()","WFGUI",this,"CallBoundaryConditions())");

	// Calculate potentials button	

	CalcPotButton = new TGTextButton(ButtonPotFrame, " Calculate Potentials ");

	ButtonPotFrame->AddFrame(CalcPotButton, new TGLayoutHints(kLHintsCenterX,1,1,1,1));

	//Connect button with threadstart method

	CalcPotButton->Connect("Clicked()","WFGUI",this,"ThreadstartPotential()");

	ControlFrame->AddFrame(ButtonPotFrame,new TGLayoutHints(kLHintsCenterY | kLHintsTop,2,2,2,2));					 


	/////////////////////////////////////////////
	// Calculate current Frame ///////////////////

	ButtonCurFrame = new TGHorizontalFrame(ControlFrame,50,10);			//
	CalculateButton = new TGTextButton(ButtonCurFrame," Calculate Currents ");
	ButtonCurFrame->AddFrame(CalculateButton,  new TGLayoutHints(kLHintsLeft | kLHintsBottom,1,1,1,1));									//
	//CalculateButton->Associate(this);									

	CalculateButton->Connect("Clicked()","WFGUI",this,"ThreadstartCurrents()"); // connect calculate button to threadstart	
	CalculateButton->SetEnabled(kFALSE);	
	//CurrentsFrame->AddFrame(ButtonCurFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,1,1));

	
	
	//bStopped=false;	
				
	StopButton = new TGTextButton(ButtonCurFrame," Stop ");																					//
	StopButton->Connect("Clicked()","WFGUI",this,"SetStopOn()");	
	StopButton->SetBackgroundColor(0xff0000);//
	ButtonCurFrame->AddFrame(StopButton,  new TGLayoutHints(kLHintsLeft| kLHintsBottom,2,2,1,1));										//
	StopButton->Associate(this);	

	ControlFrame->AddFrame(ButtonCurFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,1,1));


	///////////////////////////////
	//create and connect exit button

	//	ExitFrame = new TGHorizontalFrame(ControlFrame,100,100);			//
	ExitFrame = new TGHorizontalFrame(ControlFrame,100,100);			//
	ExitButton = new TGTextButton(ExitFrame," Exit ");	
	ExitButton->SetBackgroundColor(0x66cc66);	
	ExitFrame->AddFrame(ExitButton,  new TGLayoutHints( kLHintsCenterX | kLHintsBottom,1,1,1,1));									//

 	ExitButton->Associate(this);					
	ExitButton->SetCommand("gApplication->Terminate(0)");		
	ControlFrame->AddFrame(ExitFrame,new TGLayoutHints( kLHintsCenterX | kLHintsBottom,2,2,1,1));


	// Add Control Frame 

	// ControlFrame->SetLayoutManager(new TGVerticalLayout(ControlFrame));

	SettingsFrame2->AddFrame(ControlFrame, new TGLayoutHints(kLHintsLeft | kLHintsCenterY| kLHintsExpandX,1,1,1,1));


	///////////////////////////////////////////////////////////////////
	// Detector Properties Frame //////////

	//	DetectorPropertiesFrame = new TGGroupFrame(SettingsFrame2,"Detector Properties",kVerticalFrame);	
	DetectorPropertiesFrame = new TGGroupFrame(RightFrame,"Detector Properties",kVerticalFrame);	
			
	TypeFrameGroup  = new TGButtonGroup(DetectorPropertiesFrame,"Type",kHorizontalFrame); 
	//	TypeButtonGroup = new TGButtonGroup(Type, "Type", kHorizontalFrame);
	TypeButton[0] = new TGRadioButton(TypeFrameGroup, new TGHotString("Si"));
	TypeButton[1] = new TGRadioButton(TypeFrameGroup, new TGHotString("Diamond"));
	TypeButton[2] = new TGRadioButton(TypeFrameGroup, new TGHotString("Free"));
	TypeButton[0]->SetState(kButtonDown);

	TypeFrameGroup->AddFrame(TypeButton[0], new TGLayoutHints(kLHintsCenterX,-160,10,0,0));
	TypeFrameGroup->AddFrame(TypeButton[1], new TGLayoutHints(kLHintsCenterX,-40,10,0,0));
	TypeFrameGroup->AddFrame(TypeButton[2], new TGLayoutHints(kLHintsRight,5,10,0,0));
						
	TypeButton[0]->Connect("Toggled(Bool_t)", "WFGUI", this, "CallSetDetType()");	
	TypeButton[1]->Connect("Toggled(Bool_t)", "WFGUI", this, "CallSetDetType()");	
	TypeButton[2]->Connect("Toggled(Bool_t)", "WFGUI", this, "CallSetDetType()");	


	CallSetDetType();

				
	DimensionsFrame  = new TGGroupFrame(DetectorPropertiesFrame,"Dimensions",kHorizontalFrame); // GroupFrame: dimensionen, Pitch, Width	//
	DimLabelFrame = new TGVerticalFrame(DimensionsFrame ,200,60);
	DimSetFrame = new TGVerticalFrame(DimensionsFrame ,200,60);
	VoltageFrame  = new TGGroupFrame(DetectorPropertiesFrame,"Voltage",kHorizontalFrame); //GroupFrame: voltage								//
	VolLabelFrame = new TGVerticalFrame(VoltageFrame ,50,90);
	VolSetFrame = new TGVerticalFrame(VoltageFrame ,50,90);	
					//
	//	DopButtonGroup = new TGButtonGroup(DetectorPropertiesFrame,"Doping",kVerticalFrame); // GroupFrame: doping

	//	DopStripFrame = new TGButtonGroup(DopButtonGroup, "Strips", kHorizontalFrame);
	//DopBulkFrame = new TGButtonGroup(DopButtonGroup, "Bulk", kHorizontalFrame);
	DopStripFrame = new TGButtonGroup(DetectorPropertiesFrame, "Strips", kHorizontalFrame);
	DopBulkFrame = new TGButtonGroup(DetectorPropertiesFrame, "Bulk", kHorizontalFrame);
	DetectorPropertiesFrame->AddFrame(DopStripFrame,new TGLayoutHints(kLHintsLeft,0,0,0,0));	
DetectorPropertiesFrame->AddFrame(DopBulkFrame,new TGLayoutHints(kLHintsLeft,0,0,0,0));	
	
	StripButton[0] = new TGRadioButton(DopStripFrame, new TGHotString("n-type"));
	StripButton[1] = new TGRadioButton(DopStripFrame, new TGHotString("p-type"));
	StripButton[0]->SetState(kButtonDown);
	DopStripFrame->AddFrame(StripButton[0], new TGLayoutHints(kLHintsCenterX,-90,10,0,0));	
	DopStripFrame->AddFrame(StripButton[1], new TGLayoutHints(kLHintsCenterX,5,10,0,0));
	
	BulkButton[0] = new TGRadioButton(DopBulkFrame, new TGHotString("n-type"));
	BulkButton[1] = new TGRadioButton(DopBulkFrame, new TGHotString("p-type"));
	BulkButton[1]->SetState(kButtonDown);
	
	DopBulkFrame->AddFrame(BulkButton[0], new TGLayoutHints(kLHintsCenterX,-90,10,0,0));
	DopBulkFrame->AddFrame(BulkButton[1], new TGLayoutHints(kLHintsCenterX,5,10,0,0));

	// Connect buttons for bulk and strip doping		

	StripButton[0]->Connect("Toggled(Bool_t)", "WFGUI", this, "CallSetDoping1()");	
	StripButton[1]->Connect("Toggled(Bool_t)", "WFGUI", this, "CallSetDoping1()");
	BulkButton[0]->Connect("Toggled(Bool_t)", "WFGUI", this, "CallSetDoping0()");
	BulkButton[1]->Connect("Toggled(Bool_t)", "WFGUI", this, "CallSetDoping0()");	
	
	StripButton[0]->SetOn(kTRUE);
	BulkButton[1]->SetOn(kTRUE);
	CallSetDoping0();
	CallSetDoping1();

	//	PlotGroupFrame = new TGGroupFrame(DetectorPropertiesFrame,"Plot Settings",kVerticalFrame); // GroupFrame: plot settings	
	//////////// Plot Settings ///////////////////////

	// Fill Dimensions Frame: Numberentries for XMAX, YMAX, pitch, width   //
	XMAXentry = new TGNumberEntry(DimSetFrame, (Double_t)3,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);				//
	DimSetFrame->AddFrame(XMAXentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));		
	XMAXLabel = new TGLabel(DimLabelFrame,new TGString("# of strips (1,3,5..):"));
	XMAXLabel->SetMargins(0,0,4,4);	
	DimLabelFrame->AddFrame(XMAXLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));
	YMAXentry = new TGNumberEntry(DimSetFrame, (Double_t) 200,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);			//
	DimSetFrame->AddFrame(YMAXentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));
	YMAXLabel = new TGLabel(DimLabelFrame,new TGString("Detector Height (um):"));
	YMAXLabel->SetMargins(0,0,4,4);	
	DimLabelFrame->AddFrame(YMAXLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));
	Pitchentry = new TGNumberEntry(DimSetFrame, (Double_t) 300,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);			//
	DimSetFrame->AddFrame(Pitchentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));
	PitchLabel = new TGLabel(DimLabelFrame,new TGString("Strip Pitch (um):"));
	PitchLabel->SetMargins(0,0,4,4);	
	DimLabelFrame->AddFrame(PitchLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));
	Widthentry = new TGNumberEntry(DimSetFrame, (Double_t) 290,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);			//
	DimSetFrame->AddFrame(Widthentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));								

	WidthLabel = new TGLabel(DimLabelFrame,new TGString("Strip Width (um):"));
	WidthLabel->SetMargins(0,0,4,4);		//
	DimLabelFrame->AddFrame(WidthLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));							
	
	Gainentry = new TGNumberEntry(DimSetFrame, (Double_t)1,8,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,1.,20.);				//
	DimSetFrame->AddFrame(Gainentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));					

	GainLabel = new TGLabel(DimLabelFrame,new TGString("Gain Scale (1 = no G):"));	
	GainLabel->SetMargins(0,0,4,4);		
	DimLabelFrame->AddFrame(GainLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));
	//    e/h gain ratio

	GainRatioentry = new TGNumberEntry(DimSetFrame, (Double_t)0.0,8,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0.,1.);				//
	DimSetFrame->AddFrame(GainRatioentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));					

	GainRatioLabel = new TGLabel(DimLabelFrame,new TGString("h/e Gain ratio:"));	
	GainRatioLabel->SetMargins(0,0,4,4);		
	DimLabelFrame->AddFrame(GainRatioLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));


	GainIndententry = new TGNumberEntry(DimSetFrame, (Double_t)0.0,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative,TGNumberFormat::kNELLimitMinMax,0.,100.);				//
	DimSetFrame->AddFrame(GainIndententry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));					
	GainIndentLabel = new TGLabel(DimLabelFrame,new TGString("Gain layer recess (um):"));	
	GainIndentLabel->SetMargins(0,0,4,4);		
	DimLabelFrame->AddFrame(GainIndentLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));


	// Fill VoltageFrame: Numberentries and Labels for bias and depletion voltage	

	Biasentry = new TGNumberEntry(VolSetFrame, (Double_t) 790,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);	
	VolSetFrame->AddFrame(Biasentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,1,1,1,1));	
	BiasLabel = new TGLabel(VolLabelFrame,new TGString("Bias Voltage (V):"));
	BiasLabel->SetMargins(0,0,4,4);	
	VolLabelFrame->AddFrame(BiasLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));
	Depletionentry = new TGNumberEntry(VolSetFrame, (Double_t) 50,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);		//
	VolSetFrame->AddFrame(Depletionentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,1,1,1,1));											//
	DepletionLabel = new TGLabel(VolLabelFrame,new TGString("Depletion Voltage (V):"));	
	DepletionLabel->SetMargins(0,0,4,4);
	VolLabelFrame->AddFrame(DepletionLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));	

	// Add Frames						//

//	SettingsFrame->AddFrame(DetectorPropertiesFrame, new TGLayoutHints(kLHintsCenterX,0,1,1,1));	
	RightFrame->AddFrame(DetectorPropertiesFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 1,1,1,1));	
	SettingsFrame2->AddFrame(RightFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 1,1,1,1));	

	DetectorPropertiesFrame->AddFrame(TypeFrameGroup ,new TGLayoutHints(kLHintsCenterY| kLHintsExpandX,1,1,1,1));	
	DetectorPropertiesFrame->AddFrame(DimensionsFrame ,new TGLayoutHints(kLHintsCenterY| kLHintsExpandX,1,1,1,1));	
	DetectorPropertiesFrame->AddFrame(VoltageFrame ,new TGLayoutHints(kLHintsCenterY| kLHintsExpandX,1,1,1,1));
	//	DetectorPropertiesFrame->AddFrame(DopButtonGroup,new TGLayoutHints(kLHintsCenterY| kLHintsExpandX,1,1,1,1));

	DimensionsFrame->AddFrame(DimLabelFrame,new TGLayoutHints(kLHintsLeft,1,1,1,1));
	DimensionsFrame->AddFrame(DimSetFrame,new TGLayoutHints(kLHintsRight,1,1,1,1));	

	VoltageFrame->AddFrame(VolLabelFrame,new TGLayoutHints(kLHintsLeft,1,1,1,1));
	VoltageFrame->AddFrame(VolSetFrame,new TGLayoutHints(kLHintsRight,1,1,1,1));

	// frame for set and calculate potentials button
	//ButtonPotFrame = new TGHorizontalFrame(DetectorPropertiesFrame,50,100);	

	///////// End Detector Properties Frame //////////////////////
	SettingsGlobalFrame->AddFrame(SettingsFrame2,new TGLayoutHints(kLHintsExpandX | kLHintsCenterY,2,2,2,2));
	
	///////////////////////////////////////////////////////////////////
	// Oscilloscope Frame //////////

	SettingsFrame3 = new TGHorizontalFrame(SettingsGlobalFrame,600,90);
	
	//	ElectronicsFrame = new TGGroupFrame(SettingsFrame3," Electronics",kVerticalFrame);	
	ElectronicsFrame = new TGGroupFrame(RightFrame," Electronics",kVerticalFrame);	
	// ElectronicsFrame = new TGVerticalFrame(RightFrame,50, 100);	
	//	OscilloscopeFrame = new TGVerticalFrame(ElectronicsFrame,"Oscilloscope",kVerticalFrame);	
	OscilloscopeFrame = new TGVerticalFrame(ElectronicsFrame,50, 100);	

	// Fill Oscilloscope frame__________________________________________________________________________________________________			//
	OscOnButton = new TGCheckButton(OscilloscopeFrame, "ON",0);
	OscilloscopeFrame->AddFrame(OscOnButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,1,5));
	OscOnButton->Connect("Toggled(Bool_t)","WFGUI",this,"SetOscOn(Bool_t)");	
	// SetOscOn(false);

	OscBWSetFrame = new TGHorizontalFrame(OscilloscopeFrame,110,90);	
	OscBWLabel = new TGLabel(OscBWSetFrame,new TGString("Oscilloscope BW (GHz):"));
	OscBWLabel->SetMargins(0,0,4,4);	
	OscBWSetFrame->AddFrame(OscBWLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));
	OscBWEntry = new TGNumberEntry(OscBWSetFrame, (Double_t) 2,5,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative);
	OscBWSetFrame->AddFrame(OscBWEntry, new TGLayoutHints(kLHintsExpandX | kLHintsRight ,15,1,1,1));
	OscBWEntry->SetState(kFALSE);


	//	TFallSetFrame = new TGVerticalFrame(OscilloscopeFrame,90,50);	
	TRiseSetFrame = new TGHorizontalFrame(OscilloscopeFrame,200,90);	
	TRiseLabel = new TGLabel(TRiseSetFrame, new TGString("Shaper Int. Time (ns):"));
	TRiseLabel->SetMargins(0,0,4,4);	
	TRiseSetFrame->AddFrame(TRiseLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));
	TRiseEntry = new TGNumberEntry(TRiseSetFrame, (Double_t) 3.5,5,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative);
	TRiseSetFrame->AddFrame(TRiseEntry, new TGLayoutHints(kLHintsExpandX | kLHintsRight,15,1,1,1));
	TRiseEntry->MoveResize(60,22);
	TRiseEntry->SetState(kFALSE);
	
	TFallSetFrame = new TGHorizontalFrame(OscilloscopeFrame,110,90);	
	TFallLabel = new TGLabel(TFallSetFrame,new TGString("Shaper Decay Time (ns):"));
	TFallLabel->SetMargins(0,0,4,4);	
	TFallSetFrame->AddFrame(TFallLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));
	TFallEntry = new TGNumberEntry(TFallSetFrame, (Double_t) 5,5,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative);
	TFallSetFrame->AddFrame(TFallEntry, new TGLayoutHints(kLHintsExpandX | kLHintsRight,15,1,1,1));
	TFallEntry->SetState(kFALSE);
	

	VthSetFrame = new TGHorizontalFrame(OscilloscopeFrame,110,90);	
	VthLabel = new TGLabel(VthSetFrame,new TGString("Vth (in noise unit): "));
	VthLabel->SetMargins(0,0,4,4);	
	VthSetFrame->AddFrame(VthLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));
	VthEntry = new TGNumberEntry(VthSetFrame, (Double_t) 10,5,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative);
	VthSetFrame->AddFrame(VthEntry, new TGLayoutHints(kLHintsExpandX | kLHintsRight,15,1,1,1));
	VthEntry->SetState(kFALSE);
	




	OscilloscopeFrame->AddFrame(OscBWSetFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));
	OscilloscopeFrame->AddFrame(TRiseSetFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));   		
	OscilloscopeFrame->AddFrame(TFallSetFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));
	OscilloscopeFrame->AddFrame(VthSetFrame,new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));
	//kLHintsExpandX,5,40,1,5));

	//OscilloscopeFrame->Resize();

	ElectronicsFrame->AddFrame(OscilloscopeFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 1,1,1,1));
	RightFrame->AddFrame(ElectronicsFrame, new TGLayoutHints(kLHintsExpandX| kLHintsTop , 0,0,0,0));	

	///////// End Oscilloscope Frame //////////////////////

	
	SettingsGlobalFrame->AddFrame(SettingsFrame3,new TGLayoutHints(kLHintsExpandX | kLHintsCenterY,2,2,2,2));
	AddFrame(SettingsGlobalFrame,new TGLayoutHints(kLHintsExpandX,2,2,2,2));
	
	
	canvasp = driftcanvas->GetCanvas();
	canvaspc = driftpcanvas->GetCanvas();
	// canvaspc->Divide(1,2);
	canvaspc->Divide(2,1,0.002,0.002);

	//	canvaspf = driftfcanvas->GetCanvas();

	canvasw = weightcanvas->GetCanvas();
	canvaswc = weightpcanvas->GetCanvas();
	canvaswc->Divide(2,1,0.002,0.002);

	// canvaswf = weightfcanvas->GetCanvas();

	curcanvas=currentscanvas->GetCanvas();
	osccanvas=oscilloscopecanvas->GetCanvas();

	//set canvas margins:

	canvasp->SetBottomMargin(0.15);
	canvasp->SetTopMargin(0.1);
	canvasp->SetLeftMargin(0.15);
	canvasp->SetRightMargin(0.15);

	canvaspc->SetBottomMargin(0.15);
	canvaspc->SetTopMargin(0.1);
	canvaspc->SetLeftMargin(0.15);
	canvaspc->SetRightMargin(0.15);

	//	canvaspf->SetBottomMargin(0.15);
	//canvaspf->SetTopMargin(0.1);
	//canvaspf->SetLeftMargin(0.15);
	//canvaspf->SetRightMargin(0.15);

	canvaswc->SetBottomMargin(0.15);
	canvaswc->SetTopMargin(0.1);
	canvaswc->SetLeftMargin(0.15);
	canvaswc->SetRightMargin(0.15);

	canvasw->SetBottomMargin(0.15);
	canvasw->SetTopMargin(0.1);
	canvasw->SetLeftMargin(0.15);
	canvasw->SetRightMargin(0.15);	

	curcanvas->SetBottomMargin(0.15);
	curcanvas->SetTopMargin(0.1);
	curcanvas->SetLeftMargin(0.15);
	curcanvas->SetRightMargin(0.15);
	
	osccanvas->SetBottomMargin(0.15);
	osccanvas->SetTopMargin(0.1);
	osccanvas->SetLeftMargin(0.2);
	osccanvas->SetRightMargin(0.2);

	


	// Create histograms for potentials and fields
	ellipse = new TEllipse();
	dhist = new TH2F("dhist"," ; x [um]; y [um]",dwpot.GetXMAX(),0,dwpot.GetXMAX(),dwpot.GetYMAX(), 0,dwpot.GetYMAX() );  
	whist = new TH2F("whist"," ; x [um]; y [um]",dwpot.GetXMAX(),0,dwpot.GetXMAX(),dwpot.GetYMAX(), 0,dwpot.GetYMAX() );  
	wfhist = new TH2F("wfhist"," ",dwpot.GetXMAX(),0,dwpot.GetXMAX(),dwpot.GetYMAX(), 0,dwpot.GetYMAX() );  
	dfhist = new TH2F("dfhist"," ",dwpot.GetXMAX(),0,dwpot.GetXMAX(),dwpot.GetYMAX(), 0,dwpot.GetYMAX() );  
	// set offset of axistitles
	dhist->SetTitleOffset(1, "Y");
	dhist->SetTitleOffset(1, "X");
	whist->SetTitleOffset(1, "Y");
	whist->SetTitleOffset(1, "X");

	jhist = new TH1F("jhist","Jitter; Time [ns]; ",300,0,3.);  
	tvthhist = new TH1F("tvthhist","Time Delay; Time [ns];  ",300,0,6.);  
	twtothist = new TH2F("twtothist","Time Walk vs ToT; Time over Threshold [ns];  ",100,0,15., 250,0,5.);  
	twtothist->SetMarkerColor(2);
	twtothist->SetMarkerSize(0.4);
	twtothist->SetMarkerStyle(2);

	// Initialize variables
	weightcut = new TGraph();
	driftcut = new TGraph();
	wherecut = dwpot.GetXMAX()/2+1;
	wherecut2 = dwpot.GetXMAX()/2+1;	
	WhereCut->SetNumber(wherecut);
	WhereCut2->SetNumber(wherecut2);
	stopped=false;
	PotentialThread=0;
	CurrentsThread=0;
	plotupdate=false;
	LessPlot=false;
	Less2DPlot=false;
	bfieldon=false;
	diffusionon=false;
	gainon = false;
	OscOn = false;
	FileNameOn = false;
	BatchOn = false;
	currentson=false;
	fieldyes=false;
	stripdoping=1;
	bulkdoping=0;

	MapSubwindows();
	SetWindowName("Weightfield 2.43");
	Resize(GetDefaultSize());
	MapWindow();
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::CallCalculatePotentials() {	


	if(!GetLess2DPlot())
	  {
	    canvasw->Clear(); // when new calculation is started, clear all canvas
	    canvasp->Clear();
	    canvaswc->Clear();
	    canvaspc->Clear();
	    

	    canvasw->Update();
	    canvasp->Update();
	    canvaspc->Update();
	    canvaswc->Update();
	  }

	curcanvas->Clear();
	curcanvas->Update();
	osccanvas->Clear();
	osccanvas->Update();

	dhist->Reset();
	whist->Reset();
	twtothist->Reset();
	tvthhist->Reset();
	jhist->Reset();

	dwpot.SetDoping(stripdoping,bulkdoping);
	CalculatingLabel->SetBackgroundColor(0xff0000); // set progress label color to red
	CalculatingLabel->SetTitle("Calculating Potentials ..."); // update progess label title
	CalculatingLabel2->SetBackgroundColor(0xff0000); // set progress label color to red
	CalculatingLabel2->SetTitle("Calculating Potentials ..."); // update progess label title


	//disable buttons

	
	//cout<<"calc potentials in: dimy= "<<YMAXentry->GetNumber()<<", dimx="<<XMAXentry->GetNumber()<<endl;
	// set pitch, width, XMAX and YMAX

	dwpot.SetPitchWidthXY(GetYMax(),GetXMax(),Pitchentry->GetNumber(),Widthentry->GetNumber());	

	//dwpot.SetGain(Gainentry->GetNumber());

	
	
	WhereCut->SetLimitValues(2, dwpot.GetXMAX()-2.);	
	WhereCut2->SetLimitValues(2,dwpot.GetXMAX()-2);	
	CarriersInNumberentry->SetLimitValues(0,dwpot.GetXMAX());	
	
	dwpot.SetV(Biasentry->GetNumber(),Depletionentry->GetNumber());  				// set depeletion and bias voltage

	for(int i=0; i<dwpot.GetXMAX(); i++) {			// reset potentials to zero as a precaution
			for(int j=0; j<dwpot.GetYMAX(); j++) {
				dwpot.SetwPotential(j,i,0.0);
				dwpot.SetdPotential(j,i,0.0);
			}
		}		
		
	dwpot.SetBoundaryConditions();		// set boundary conditions
	dhist->GetXaxis()->SetLabelColor(0);	// set label color to white. because of multigrid approach, the tick labels of the axis are going to vary during calculation
	dhist->GetYaxis()->SetLabelColor(0);	

	dwpot.Multigrid(this); // calculate potential


	///////////////// Add GAIN Layer by hand ////////////////////////


	double Scaledpot =0;
	SetGainon(false);
	if (Gainentry->GetNumber() > 1.)
	  {
	    
	    SetGainon(true);
	    SetYGain( (double) Gainentry->GetNumber()) ;	    
	    float Gdist=0;

	    if(( StripButton[0]->IsOn() && BulkButton[1]->IsOn())  )
	      {
		for(int j= Getygainlow(); j<dwpot.GetYMAX(); j++) 
		  {
		    if (j <Getygainhigh() )    Gdist+=2.;

		    int Under = 0;
		    for(int i=0; i<dwpot.GetXMAX(); i++)
		      {		
			if (dwpot.Getdpot(dwpot.GetYMAX()-1,i) ==  Biasentry->GetNumber())
			  {
			    Under++;
			    //			    cout << "i = " <<  i << " Under =  " << Under <<  endl;
			    if (Under > GainIndententry->GetNumber() && Under <=Widthentry->GetNumber()-GainIndententry->GetNumber())
			      {
				// cout << Under  << endl;
				dwpot.SetdPotential(j,i,dwpot.Getdpot(j,i)+pow(Gdist,2.1));

			      }
			//	else   dwpot.SetdPotential(j,i,dwpot.Getdpot(j,i)+pow(Gdist,2.1));
			  }
			else Under = 0;

		      }
		  }		
		Scaledpot = Biasentry->GetNumber()/dwpot.Getdpot(dwpot.GetYMAX()-1,dwpot.GetXMAX()/2);
		for(int i=0; i<dwpot.GetXMAX(); i++) 		
		  for(int j=0; j<dwpot.GetYMAX(); j++) 
			  dwpot.SetdPotential(j,i,dwpot.Getdpot(j,i)*Scaledpot);
	      }

	    else if(StripButton[0]->IsOn() && BulkButton[0]->IsOn())
	      {
		for(int j= Getygainlow(); j<dwpot.GetYMAX(); j++) 
		  {
		    if (j <Getygainhigh() )    Gdist+=2.;
		    
		    for(int i=0; i<dwpot.GetXMAX(); i++)  dwpot.SetdPotential(j,i,dwpot.Getdpot(j,i)+pow(Gdist,2.1));
		    
		  }		
		Scaledpot = Biasentry->GetNumber()/dwpot.Getdpot(dwpot.GetYMAX()-1,dwpot.GetXMAX()/2);
		for(int i=0; i<dwpot.GetXMAX(); i++) 		
		  for(int j=0; j<dwpot.GetYMAX(); j++) 
		    dwpot.SetdPotential(j,i,dwpot.Getdpot(j,i)*Scaledpot);
	      }
	    

	    else if(StripButton[1]->IsOn() && BulkButton[1]->IsOn())
	      {
		for(int j= Getygainhigh(); j>-1; j--) 
		  {
		    if (j >Getygainlow()-1 )    Gdist+=2.;

		    for(int i=0; i<dwpot.GetXMAX(); i++)
		      {		
			dwpot.SetdPotential(j,i,dwpot.Getdpot(j,i)+pow(Gdist,2.1));
		      }
		  
		  }
		Scaledpot = Biasentry->GetNumber()/dwpot.Getdpot(0,dwpot.GetXMAX()/2);
		for(int i=0; i<dwpot.GetXMAX(); i++) 		
		  for(int j=0; j<dwpot.GetYMAX(); j++) 
		      dwpot.SetdPotential(j,i,dwpot.Getdpot(j,i)*Scaledpot);
	      }
	  
	    else if(StripButton[1]->IsOn() && BulkButton[0]->IsOn())
	      {

		for(int j= Getygainhigh(); j>-1; j--) 
		  {
		    if (j >Getygainlow()-1 )    Gdist+=2.;

		    for(int i=0; i<dwpot.GetXMAX(); i++)
		      {		
			//			if (dwpot.Getdpot(dwpot.GetYMAX()-1,i) ==  0) // only under the electrode
			  dwpot.SetdPotential(j,i,dwpot.Getdpot(j,i)+pow(Gdist,2.1));

		      }
		  }
		Scaledpot = Biasentry->GetNumber()/dwpot.Getdpot(0,dwpot.GetXMAX()/2);

		for(int i=0; i<dwpot.GetXMAX(); i++) 		
		  for(int j=0; j<dwpot.GetYMAX(); j++) 
		      dwpot.SetdPotential(j,i,dwpot.Getdpot(j,i)*Scaledpot);
	      }
	  	    	    	     
	   


	  } // end of Gain potential addition

	/////////////////////////////////////////

	CallCalculateFields(); 			// calculate electric field

	//

	if(stopped==true) {
		CalculatingLabel->SetTitle("Calculation has been stopped.");// set progress label in case of "stop"
		CalculatingLabel2->SetTitle("Calculation has been stopped.");// set progress label in case of "stop"
	}
	else {
		CalculatingLabel->SetBackgroundColor(0x00ff00);	// when calculation completed, set progress label color to green
		CalculatingLabel->SetTitle("Done.");
		CalculatingLabel2->SetBackgroundColor(0x00ff00);	// when calculation completed, set progress label color to green
		CalculatingLabel2->SetTitle("Done.");
	}
	// enable buttons

//	return NULL;

	//Threadstop();

}	
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::CallCalculateCurrents() {	

	CalculateButton->SetEnabled(kFALSE);
	CalcPotButton->SetEnabled(kFALSE);
	SetButton->SetEnabled(kFALSE);

	CurrentsProgressBar->Reset();
	CurrentsProgressBar->Percent(0);
	CurrentsProgressBar->ShowPosition(kTRUE,kTRUE);
	CalculatingLabel->SetBackgroundColor(0xff0000);				
	CalculatingLabel->SetTitle("Calculating Currents ...");	// update progress label
	CalculatingLabel2->SetBackgroundColor(0xff0000);				
	CalculatingLabel2->SetTitle("Calculating Currents ...");	// update progress label
	B=0.0;
	Temp=0.0;

	jhist->Reset();
	tvthhist->Reset();
	//	SetYGain( (double) Gainentry->GetNumber()) ;	    
	
	SetAlphaRange(RangeEntry->GetNumber());
	SetPrecision(PrecisionEntry->GetNumber());
	SetYGain(Gainentry->GetNumber());
	SetGainRatio(GainRatioentry->GetNumber());


	if(bfieldon==true) 
	  {
	    B = BfieldEntry->GetNumber();
	  }
	
	Temp = TempEntry->GetNumber();
	cout << "Temperature set to = "<< Temp << endl;		
	//cout<<"Flag gain ON = "<<GetGainon()<<endl;
	cout << "Upper detector dimension = "<<dwpot.GetYMAX()<<endl;
	int NBatch = 1.;

	dimMaxCarriers= 2*dwpot.Getmipcharge()+2000000;
	

	// dimMaxCarriers= 2*dwpot.Getmipcharge()+100000;
	carriers =new Carriers[dimMaxCarriers];


	if (GetBatchOn() == true)
	{ 
	      CurrentsProgressBar->SetMax(EventsEntry->GetNumber());
	      CurrentsProgressBar->Percent(0);
	      NBatch = EventsEntry->GetNumber();
	      osccanvas->Divide(1,2,0.002,0.002);
	}
	for (int e=1; e<= NBatch; e++)
	  {	
	    cout << "Processing event " << e << endl;

	    for (int i=0;i<dimMaxCarriers;i++) carriers[i].Setinside(-1);	//initialize carriers array
	
	    carriersin = CarriersInNumberentry->GetNumber();
	    switch (radiobuttonstatus){
	    case MIPunif:
	      SetConstQFlag(1);
	      SetUniformQFlag(1);
	      dwpot.Setmipcharge(dwpot.GetYMAX()*75/cos(TMath::Pi()/180*GetAngle()));
	      CreateCharges(dwpot,carriers,carriersin,this);
	      break;
	      
	    case MIPnonunif:
	      SetConstQFlag(1);
	      SetUniformQFlag(0);
	      CreateCharges(dwpot,carriers,carriersin,this);
	      break;
	      
	    case MIPlandau:
	      SetConstQFlag(0);
	      SetUniformQFlag(0);
	      CreateCharges(dwpot,carriers,carriersin,this);
	      break;
	      
	    case ALPHA_TOP:
	      dwpot.Setmipcharge(ALPHACHARGE);
	      CreateChargesAlphaTop(dwpot,carriers,carriersin,this);

	      break;
	      
	    case ALPHA_BOTTOM:
	      dwpot.Setmipcharge(ALPHACHARGE);
	      CreateChargesAlphaBottom(dwpot,carriers,carriersin,this);
	      break;
	      
	    case USR_CHARGE:
	      SetConstQFlag(1);
	      SetUniformQFlag(1);
	      SetUserUniformQFlag(1);
	      dwpot.Setmipcharge(Chargeentry->GetNumber()*dwpot.GetYMAX()/cos(TMath::Pi()/180*GetAngle()));
	      CreateCharges(dwpot,carriers,carriersin,this);

	      break;
	      
	    default: break;
	    }

	    
	    SetLorentz(dwpot,carriers,B, Temp, CallGetDetType());
	    /*	
		
		if (GetBatchOn() == true)
		{ 
		CurrentsProgressBar->SetMax(EventsEntry->GetNumber());
		CurrentsProgressBar->Percent(0);
		for (int e=0; e< (EventsEntry->GetNumber()); e++)
		{
	    */

	    if (e%100 == 0)
	      {
		cout<<"Event Number: "<<e <<endl;
	      }
	    
	    CalculateCurrents(dwpot,df,wf,carriers,this,e);

	    if(Getstopped()) 
	      {
		Setstopped(false);
		break;
	      }
	    if (GetBatchOn() == true) 
	      {
		CurrentsProgressBar->Increment(1);
		jhist->Fill(GetJitter());
		tvthhist->Fill(GetTVth());
		twtothist->Fill(GetFTVth()-GetTVth(),GetTVth());
		osccanvas->Clear();
		osccanvas->Divide(2,2,0.002,0.002);
		osccanvas->cd(1);
		tvthhist->Draw();
		osccanvas->cd(2);
		jhist->Draw();
		osccanvas->cd(3);
		twtothist->Draw();
		osccanvas->Update();
		// cout << " jitter plots " << endl;
	      }
	    if (GetBatchOn() == false) break; // exit from the batch loop if you de-select the check box
	  }
	if (GetBatchOn() == true) cout<<"Run finished: "<< EventsEntry->GetNumber() <<" events"<<endl;
	

	CalculatingLabel->SetBackgroundColor(0x00ff00);
	CalculatingLabel->SetTitle("Done.");
	CalculatingLabel2->SetBackgroundColor(0x00ff00);
	CalculatingLabel2->SetTitle("Done.");
	curcanvas->Update();
	osccanvas->Update();
	CurrentsInfoFrame->Layout();
	QELabel->Layout();
	CalculateButton->SetEnabled(kTRUE);
	CalcPotButton->SetEnabled(kTRUE);
	SetButton->SetEnabled(kTRUE);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::CallCalculateFields() {
	df = new Field*[(dwpot.GetYMAX())];
	for (int i = 0; i < (dwpot.GetYMAX()); i++) df[i] = new Field[(dwpot.GetXMAX())];
	
	wf = new Field*[(dwpot.GetYMAX())];
	for (int i = 0; i < (dwpot.GetYMAX()); i++) wf[i] = new Field[(dwpot.GetXMAX())];	
	CalculateFields(dwpot,wf,df);
	CalculateAbsFields(dwpot,wf,df);	
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetPlotFieldHist(Bool_t onoff) {
	fieldyes=onoff;
}
/////////////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetB() {
	return B;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetQLabel(const char* qetext,const char* qhtext,const char* qehtext,const char* qegtext,const char* qhgtext,const char* qehgtext,const char* qetottext,const char* qhtottext,const char* qtottext) {
	QELabel->SetTitle(qetext);
	QHLabel->SetTitle(qhtext);
	QEHLabel->SetTitle(qehtext);
	QEGLabel->SetTitle(qegtext);
	QHGLabel->SetTitle(qhgtext);
	QEHGLabel->SetTitle(qehgtext);
	QETotLabel->SetTitle(qetottext);
	QHTotLabel->SetTitle(qhtottext);
	QTotLabel->SetTitle(qtottext);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetAngleLabel(const char* ethetatext, const char* hthetatext) {
	LorentzeLabel->SetTitle(ethetatext);
	LorentzhLabel->SetTitle(hthetatext);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawFieldsAbs(int LCol = 1) {
  // return;

	wherecut = WhereCut->GetNumber();
	//	cout << "Drawing FieldsAbs" << endl;

	double* q1;
	q1 = new double[dwpot.GetYMAX()];
	for(int k=0;k<dwpot.GetYMAX();k++) q1[k]=k; //(int)(pow(2,pot.Getref()));

	double *dabs; // Absolute value of drift field
	double *wabs; // absolute value of weighting field
	dabs = new double[dwpot.GetYMAX()];		
	wabs = new double[dwpot.GetYMAX()];		 				
	
	for(int i=0; i<dwpot.GetYMAX(); i++) {
		wabs[i]=0.0;
		dabs[i]=0.0;
	}		

	double Maxdabs = 0;
	double Maxwabs = 0;

	for(int k=0;k<dwpot.GetYMAX();k++) 
	  {
	    wabs[k] = (wf[k][wherecut].Getabs() > 0.1)? wf[k][wherecut].Getabs() : 0. ;
	    dabs[k] = (df[k][wherecut].Getabs()/1e5> 0.1)? df[k][wherecut].Getabs()/1e5 : 0;

	    if (Maxwabs < wabs[k]) Maxwabs = wabs[k];
	    if (Maxdabs < dabs[k]) Maxdabs = dabs[k];
	  }


	// cout << Maxdabs << endl;

	dfieldcut = new TGraph(dwpot.GetYMAX(),q1,dabs);
   	dfieldcut->GetXaxis()->SetTitle("y [um]");
   	dfieldcut->GetXaxis()->SetNdivisions(5);
	dfieldcut->SetTitle("Drift Field E (kV/cm) ");
   	dfieldcut->GetYaxis()->SetLabelSize(0.05);
   	dfieldcut->GetYaxis()->SetLabelOffset(.01);
   	dfieldcut->GetXaxis()->SetLabelSize(0.05);
   	dfieldcut->GetXaxis()->SetLabelOffset(.01);
   	dfieldcut->GetXaxis()->SetTitleSize(0.05);
   	dfieldcut->GetYaxis()->SetRangeUser(0.0,Maxdabs*1.3);
   	dfieldcut->SetLineWidth(3);


	canvaspc->cd(2);
	dfieldcut->SetLineColor(LCol); // set line color to black
	if (GetGainon()) 
	  {
	    gPad->SetLogy(1);
	    dfieldcut->GetYaxis()->SetRangeUser(1.0,Maxdabs*2);
	  }
	dfieldcut->Draw();
	canvaspc->Update();

	wfieldcut = new TGraph(dwpot.GetYMAX(),q1,wabs);
   	wfieldcut->GetXaxis()->SetTitle("y (um)");
   	wfieldcut->GetXaxis()->SetNdivisions(5);
	wfieldcut->SetTitle("Weighting Field Ew (1/m) ");
   	wfieldcut->GetYaxis()->SetLabelSize(0.05);
   	wfieldcut->GetYaxis()->SetLabelOffset(.01);
   	wfieldcut->GetXaxis()->SetLabelSize(0.05);
   	wfieldcut->GetXaxis()->SetLabelOffset(.01);
   	wfieldcut->GetXaxis()->SetTitleSize(0.05);
   	wfieldcut->GetYaxis()->SetRangeUser(0.0,Maxwabs*1.3);
   	wfieldcut->SetLineWidth(3);
	//	canvaswf->Divide(1,1);
	//	canvaswf->cd();
	//	canvaswf->Clear(); //Nicolo1
	//	canvaswf->Clear(); //Nicolo1
	//	wfieldcut->Draw("ACP");
	//	gPad->SetLeftMargin(0.35);
	canvaswc->cd(2);
	wfieldcut->SetLineColor(LCol); // set line color to black
	wfieldcut->Draw("AL");
	wfieldcut->GetYaxis()->SetLimits(0., Maxwabs*1.2);
	canvaswc->Update();
}

/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawFieldsAbsX() {
  // return;
	wherecut = WhereCut->GetNumber();
	wherecut2 = WhereCut2->GetNumber();

	double* q2;
	q2 = new double[dwpot.GetYMAX()];
	for(int k=0;k<dwpot.GetYMAX();k++) q2[k]=k;

	double *dxabs; // Absolute value of drift field
	dxabs = new double[dwpot.GetYMAX()];	
	double *wxabs; // Absolute value of weighting field
	wxabs = new double[dwpot.GetYMAX()];	

	for(int i=0; i<dwpot.GetYMAX(); i++) {
		wxabs[i]=0.0;
		dxabs[i]=0.0;
	}			


	for(int k=0;k<dwpot.GetYMAX();k++)  dxabs[k] = (fabs(df[k][wherecut].GetFieldx())/1e5 > 0.1)? fabs(df[k][wherecut].GetFieldx()/1e5)  : 0. ;
	for(int k=0;k<dwpot.GetYMAX();k++)  wxabs[k] = ( fabs(wf[k][wherecut].GetFieldx())  > 0.1) ?  fabs(wf[k][wherecut].GetFieldx()) : 0;

	dfieldxcut = new TGraph(dwpot.GetYMAX(),q2,dxabs);
   	dfieldxcut->GetXaxis()->SetTitle("y [um]");
   	dfieldxcut->GetXaxis()->SetNdivisions(5);
	dfieldxcut->SetTitle("Drift Field Ex (kV/cm) ");

   	dfieldxcut->GetYaxis()->SetLabelSize(0.05);
   	dfieldxcut->GetYaxis()->SetLabelOffset(.01);
   	dfieldxcut->GetXaxis()->SetLabelSize(0.05);
   	dfieldxcut->GetXaxis()->SetLabelOffset(.01);
   	dfieldxcut->GetXaxis()->SetTitleSize(0.05);
   	dfieldxcut->SetLineWidth(3);

	//	canvaspf->Divide(1,1,0.002,0.002);
	//	canvaspf->cd();
	//canvaspf->Clear()
	// canvaspf->Update();

	canvaspc->cd(2);
	dfieldxcut->Draw("AL");
	canvaspc->Update();

	wfieldxcut = new TGraph(dwpot.GetYMAX(),q2,wxabs);
   	wfieldxcut->GetXaxis()->SetTitle("y (um)");
   	wfieldxcut->GetXaxis()->SetNdivisions(5);
	wfieldxcut->SetTitle("Weighting Field Ex (1/m) ");
   	wfieldxcut->GetYaxis()->SetLabelSize(0.05);
   	wfieldxcut->GetYaxis()->SetLabelOffset(.01);
   	wfieldxcut->GetXaxis()->SetLabelSize(0.05);
   	wfieldxcut->GetXaxis()->SetLabelOffset(.01);
   	wfieldxcut->GetXaxis()->SetTitleSize(0.05);
   	wfieldxcut->SetLineWidth(3);

	canvaswc->cd(2);
	wfieldxcut->Draw("AL");
	canvaswc->Update();
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawFieldsAbsY() {
  // return;
	wherecut = WhereCut->GetNumber();
	//	wherecut = wherecut+dwpot.GetXMAX()/2+1;
	wherecut2 = WhereCut2->GetNumber();
	// wherecut2 = wherecut2+dwpot.GetXMAX()/2+1;
	
	double* q3;
	q3 = new double[dwpot.GetYMAX()];
	for(int k=0;k<dwpot.GetYMAX();k++) q3[k]=k;

	double *dyabs; // Absolute value of drift field in y direction
	dyabs = new double[dwpot.GetYMAX()];					
	double *wyabs; // Absolute value of weighting field in x direction
	wyabs = new double[dwpot.GetYMAX()];	

	for(int i=0; i<dwpot.GetYMAX(); i++) { // initialize
		wyabs[i]=0.0;
		dyabs[i]=0.0;
	}		
	for(int k=0;k<dwpot.GetYMAX();k++) dyabs[k] = (fabs(df[k][wherecut].GetFieldy())/1e5 > 0.1)? fabs(df[k][wherecut].GetFieldy()/1e5)  : 0. ;
	  // dyabs[k] = fabs(df[k][wherecut].GetFieldy()/1e5);		
	for(int k=0;k<dwpot.GetYMAX();k++)  wyabs[k] = ( fabs(wf[k][wherecut].GetFieldy())  > 0.1) ?  fabs(wf[k][wherecut].GetFieldy()) : 0;
	// wyabs[k] = fabs(wf[k][wherecut].GetFieldy()); 



	dfieldycut = new TGraph(dwpot.GetYMAX(),q3,dyabs); 
   	dfieldycut->GetXaxis()->SetTitle("y (um)");
   	dfieldycut->GetXaxis()->SetNdivisions(5);
	dfieldycut->SetTitle("Drift Field Ey (kV/cm) ");

   	dfieldycut->GetYaxis()->SetLabelSize(0.05);
   	dfieldycut->GetYaxis()->SetLabelOffset(.01);
   	dfieldycut->GetXaxis()->SetLabelSize(0.05);
   	dfieldycut->GetXaxis()->SetLabelOffset(.01);
   	dfieldycut->GetXaxis()->SetTitleSize(0.05);
   	dfieldycut->SetLineWidth(3);

	canvaspc->cd(2);
	dfieldycut->Draw("AL");
	canvaspc->Update();

	wfieldycut = new TGraph(dwpot.GetYMAX(),q3,wyabs);
   	wfieldycut->GetXaxis()->SetTitle("y (um)");
   	wfieldycut->GetXaxis()->SetNdivisions(5);
	wfieldycut->SetTitle("Weighting Field Ey (1/m) ");
   	wfieldycut->GetYaxis()->SetLabelSize(0.05);
   	wfieldycut->GetYaxis()->SetLabelOffset(.01);
   	wfieldycut->GetXaxis()->SetLabelSize(0.05);
   	wfieldycut->GetXaxis()->SetLabelOffset(.01);
   	wfieldycut->GetXaxis()->SetTitleSize(0.05);
   	wfieldycut->SetLineWidth(3);
	//	canvaswf->Divide(1,1);
	//	canvaswf->cd();
	// canvaswf->Clear();  //Nicolo1
	//	wfieldycut->Draw("ACP");
	canvaswc->cd(2);
	wfieldycut->Draw("AL");
	canvaswc->Update();

	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawFieldHist() {
  if (GetLess2DPlot()) return;
  
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
	
	dfhist->SetStats(0);	
	canvasp->cd();
	//	gStyle->SetPaintTextFormat(const char* format="g")
	dwpot.DriftPal();
	//	dfhist->Draw("SAMES" "ARR"); //draw on same canvas as dhist and use arrows	
	dfhist->Draw("SAMES" "CONT2"); //draw on same canvas as dhist and use arrows	
	canvasp->Update();
	return;

	//whist->Draw("COLZ");
	wfhist->Reset();	//wfhist = histogram of weighting field
	wfhist->TH2F::SetBins((binsx/p+1), 0,(binsx/p+1) ,(binsy/p+1),0.,(binsy/p+1) );
	//	canvasw->Divide(1,1,0.002,0.002);
	canvasw->cd();
					
	for(int i=0;i<binsx/p+1;i++) {
		for(int j=0;j<binsy/p+1;j++) {
			h=i*p;
			a=j*p;
			wfhist->SetBinContent(i+1,j+1,dwpot.Getwpot(a,h));
		}
	}
	wfhist->SetStats(0);
	dwpot.WeightPal();
	wfhist->Draw("SAMES" "ARR");	
	canvasw->Update();				
	// return;


}
/////////////////////////////////////////////////////////////////////////////////////////////
bool WFGUI::Getdiffusionon() {
	return diffusionon;
}

/////////////////////////////////////////////////////////////////////////////////////////////
int WFGUI::SetStopOn(){
      if (stopped==false) {
      stopped=true;
      return 0;
      }
      
     else return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool WFGUI::Getstopped() {
	return stopped;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::Setstopped(Bool_t x) {
	stopped=x;
}
/////////////////////////////////////////////////////////////////////////////////////////////
TGLabel * WFGUI::GetCalculatingLabel() {
	return CalculatingLabel;
}
/////////////////////////////////////////////////////////////////////////////////////////////
TGLabel * WFGUI::GetCalculatingLabel2() {
	return CalculatingLabel2;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetPlotUpdate(Bool_t onoff) {
	plotupdate=onoff;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool WFGUI::Getplotupdate() {
	return plotupdate;
}	
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetLess2DPlot(Bool_t onoff) {
	Less2DPlot=onoff;
	SetLessPlot(onoff);
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool WFGUI::GetLess2DPlot() {
	return Less2DPlot;
}	

/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetLessPlot(Bool_t onoff) {
	LessPlot=onoff;
	if (onoff)
	  {
	    OnStripsButton2->SetEnabled(kFALSE);
	    BetweenStripsButton2->SetEnabled(kFALSE);		
	    ExButton2->SetEnabled(kFALSE);				
	    EyButton2->SetEnabled(kFALSE);				
	    DrawCutsUserEntry2->SetEnabled(kFALSE);
	    OnStripsButton->SetEnabled(kFALSE);
	    BetweenStripsButton->SetEnabled(kFALSE);		
	    ExButton->SetEnabled(kFALSE);				
	    EyButton->SetEnabled(kFALSE);		
	    DrawCutsUserEntry->SetEnabled(kFALSE);		
	  }
	else if(!GetLess2DPlot())
	  {
	    OnStripsButton2->SetEnabled(kTRUE);
	    BetweenStripsButton2->SetEnabled(kTRUE);		
	    ExButton2->SetEnabled(kTRUE);				
	    EyButton2->SetEnabled(kTRUE);				
	    DrawCutsUserEntry2->SetEnabled(kTRUE);
	    OnStripsButton->SetEnabled(kTRUE);
	    BetweenStripsButton->SetEnabled(kTRUE);		
	    ExButton->SetEnabled(kTRUE);				
	    EyButton->SetEnabled(kTRUE);	
	    DrawCutsUserEntry->SetEnabled(kTRUE);			
	  }

}
/////////////////////////////////////////////////////////////////////////////////////////////
bool WFGUI::GetLessPlot() {
	return LessPlot;
}	
/////////////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetYMax(){
  return  YMAXentry->GetNumber()*0.95;
}
/////////////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetXMax(){
  int TempXMAX = XMAXentry->GetNumber();
  if (TempXMAX%2==0) 
    {
       TempXMAX++;
       //cout << "Number of strips increased to be an  odd number " << endl;
    }
  //  cout << TempXMAX*Pitchentry->GetNumber() << " " << TempXMAX << endl;
  return TempXMAX;
    //  Pitchentry->GetNumber()*3-8;
}

////////////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetChargeEntry(){
	return Chargeentry->GetNumber();
}
////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::CallBoundaryConditions() {

  CallSetDoping0();
  CallSetDoping1();

  XMAXentry->GetNumber();
  YMAXentry->GetNumber();
  Pitchentry->GetNumber();
  Widthentry->GetNumber();
  TempEntry->GetNumber();
  SetSWidth(Widthentry->GetNumber());
	if(PotentialThread==0)
	{	
		
    	  dwpot.SetPitchWidthXY(GetYMax(),GetXMax(),Pitchentry->GetNumber(),Widthentry->GetNumber());      // set pitch, width, XMAX and YMAX
		dwpot.SetV(Biasentry->GetNumber(),Depletionentry->GetNumber());  		// set depletion and bias voltage
		dwpot.SetDoping(stripdoping,bulkdoping);

		for(int i=0; i<dwpot.GetXMAX(); i++) {			// reset potentials to zero as a precaution
			for(int j=0; j<dwpot.GetYMAX(); j++) {
				dwpot.SetwPotential(j,i,0.0);
				dwpot.SetdPotential(j,i,0.0);
			}
		}
		// set histogram bins:

		dhist->Reset(); // Reset histograms before filling them again
		whist->Reset();

		dhist->TH2F::SetBins( dwpot.GetXMAX(),0, dwpot.GetXMAX(), dwpot.GetYMAX(),0, dwpot.GetYMAX() );
		whist->TH2F::SetBins(dwpot.GetXMAX(),0, dwpot.GetXMAX(), dwpot.GetYMAX(),0, dwpot.GetYMAX() );				     
		dfhist->TH2F::SetBins(dwpot.GetXMAX(),0, dwpot.GetXMAX(), dwpot.GetYMAX(),0, dwpot.GetYMAX() );				      
		wfhist->TH2F::SetBins(dwpot.GetXMAX(),0, dwpot.GetXMAX(), dwpot.GetYMAX(),0, dwpot.GetYMAX() );
				      
		WhereCut->SetNumber(dwpot.GetXMAX()/2+1);
		WhereCut2->SetNumber(dwpot.GetXMAX()/2+1);

		dwpot.SetBoundaryConditions(); // set boundary conditions	
		for(int i=0;i<dwpot.GetXMAX();i++) { // fill histogram
			for(int j=0;j<dwpot.GetYMAX();j++) {
				dhist->SetBinContent(i+1,j+1,dwpot.Getdpot(j,i));
				whist->SetBinContent(i+1,j+1,dwpot.Getwpot(j,i));
			}
		}
		CarriersInNumberentry->SetNumber(dwpot.GetXMAX()/2);

		OnStripsButton2->SetEnabled(kFALSE);
		BetweenStripsButton2->SetEnabled(kFALSE);				
		ExButton2->SetEnabled(kFALSE);				
		EyButton2->SetEnabled(kFALSE);				
		DrawCutsUserEntry2->SetEnabled(kFALSE);
		OnStripsButton->SetEnabled(kFALSE);
		BetweenStripsButton->SetEnabled(kFALSE);		
		ExButton->SetEnabled(kFALSE);				
		EyButton->SetEnabled(kFALSE);		
		DrawCutsUserEntry->SetEnabled(kFALSE);		
		DrawAllGraph(-1);
		
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
int WFGUI::CallSetDetType() {
  if (TypeButton[0]->IsOn())
    {
	DetType = 0;
    }
  else if (TypeButton[1]->IsOn()) 
    {
	DetType= 1;
    }
  else if (TypeButton[2]->IsOn()) 
    {
      	DetType =  2;
    }
  return -1;
}
/////////////////////////////////////////////////////////////////////////////////////////////
int WFGUI::CallGetDetType() {
  return DetType;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::CallSetDoping0() {	
  //set bulk doping
	if(BulkButton[0]->IsOn())  bulkdoping = NTYPE;	
	else bulkdoping = PTYPE;
	//	cout << "CallSetDoping0 bulkdoping " << bulkdoping << endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////

void WFGUI::CallSetDoping1() {			//set strip doping
	if(StripButton[0]->IsOn()) 
	  {
	    stripdoping = NTYPE;
	  }
	else 
	  {
	    stripdoping = PTYPE;
	  }
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::CallSetPart() {
	if(SelectPartButton[0]->IsOn())  
	{
	      radiobuttonstatus=MIPunif;
	     // SetAlpha->SetEnabled(kFALSE);
	      Chargeentry->SetState(kFALSE);
	      //RangeEntry->SetState(kFALSE);
	      
	  cout << "Selecting MIP uniform distributed" << endl;
	}
	else if(SelectPartButton[1]->IsOn())  
	{
	      radiobuttonstatus=MIPnonunif;
	      //SetAlpha->SetEnabled(kFALSE);
	      Chargeentry->SetState(kFALSE);
	      //RangeEntry->SetState(kFALSE);

	      
	      
	  cout << "Selecting MIP non uniform distributed" << endl;
	}
	else if(SelectPartButton[2]->IsOn())  
	{
	      radiobuttonstatus=MIPlandau;
	      //SetAlpha->SetEnabled(kFALSE);
	      Chargeentry->SetState(kFALSE);
	      //RangeEntry->SetState(kFALSE);
	      
	      
	  cout << "Selecting MIP Landau distributed" << endl;
	}	
	else if (SelectPartButton[3]->IsOn())
	{
	      radiobuttonstatus=USR_CHARGE;
	      //SetAlpha->SetEnabled(kFALSE);
	      Chargeentry->SetState(kTRUE);
	     // RangeEntry->SetState(kFALSE);
	      //	      cout << "Selecting MIP uniform distributed. Charge= "<<Chargeentry->GetNumber()<< endl;
	}
	else if (SelectPartButton[4]->IsOn())
	{

	      Chargeentry->SetState(kFALSE);
	      radiobuttonstatus=ALPHA_TOP;
	      //RangeEntry->SetState(kTRUE);
	      SetAlphaRange(RangeEntry->GetNumber());

	      //Nicolo
	      //radiobuttonstatus=ALPHA_TOP;

	      cout << "Selecting Alpha from top" << endl;

	      //Nicolo
	      
     
	}
	else if (SelectPartButton[5]->IsOn())
	{

	      Chargeentry->SetState(kFALSE);
	      radiobuttonstatus=ALPHA_BOTTOM;
	      //RangeEntry->SetState(kTRUE);
	      SetAlphaRange(RangeEntry->GetNumber());

	      //Nicolo
	      //radiobuttonstatus=ALPHA_TOP;

	      cout << "Selecting Alpha from bottom" << endl;

	      //Nicolo
	      
     
	}
	return;
}


/////////////////////////////////////////////////////////////////////////////////////////////

bool  WFGUI::CallSetAlpha() {
 		 TGLBEntry* pEntry = NULL;
 		 pEntry = SetAlpha->GetSelectedEntry();
		 // 		 cout << "pEntry " << pEntry << endl;
 		 if (pEntry == 0)
 		 {
 			 	cout << "Alpha Type not Selected, choosing Alpha from Top" << endl;
				return true;
 		 }
 		 else
 		 {
 			 	 if (strcmp(pEntry->GetTitle(),"top")==0) return true;
 			 	 else return false;
 		 }



}
/////////////////////////////////////////////////////////////////////////////////////////////
/*void WFGUI::SetCharge(Bool_t on) {
	chargeusr=on;
	if(chargeusr) BfieldEntry->SetState(kTRUE);
	else BfieldEntry->SetState(kFALSE);
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::FillHist()
{
	canvasp->Clear();
	canvasw->Clear();
	dhist->Reset();	//Reset histogram
	dhist->TH2F::SetBins( dwpot.GetXMAX() ,0., dwpot.GetXMAX(),dwpot.GetYMAX(), 0, dwpot.GetYMAX() );
	dhist->GetXaxis()->SetLabelColor(1);	
	dhist->GetYaxis()->SetLabelColor(1);					
	dhist->SetStats(0);
	
	whist->Reset();	//Reset histogram
	whist->TH2F::SetBins( dwpot.GetXMAX() ,0., dwpot.GetXMAX(),dwpot.GetYMAX(), 0, dwpot.GetYMAX() );
	whist->GetXaxis()->SetLabelColor(1);	
	whist->GetYaxis()->SetLabelColor(1);					
	whist->SetStats(0);
		
	for(int i=0;i<dwpot.GetXMAX();i++) {		// fill histogram
		for(int j=0;j<dwpot.GetYMAX();j++) {
			dhist->SetBinContent(i+1,j+1,dwpot.Getdpot(j,i));
		}
	}
	for(int i=0;i<dwpot.GetXMAX();i++) {		// fill histogram
		for(int j=0;j<dwpot.GetYMAX();j++) {
			whist->SetBinContent(i+1,j+1,dwpot.Getwpot(j,i));
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawAllGraph(int LCol = 1)
{
  //  cout << " DrawAllGraph 2DPlot " << GetLess2DPlot() << " LCol= " <<LCol << " GetLessPlot " << GetLessPlot()<< endl;

  wherecut = WhereCut->GetNumber(); 
  wherecut2 = WhereCut2->GetNumber(); 

  //  cout << wherecut << " wherecut" <<  " " << "LCol = " << LCol << endl;

  if (!GetLess2DPlot())
    {  

      canvasp->Clear();
      canvasw->Clear();
      canvasp->Update(); 
      canvasw->Update();     
      



      NLine1 = new TLine();
      NLine2 = new TLine();
      NLine1->SetLineStyle(2);
      NLine2->SetLineStyle(2);
      

      canvasp->cd();
      dwpot.DriftPal();
      dhist->SetStats(0); // hide statistics box
      whist->SetStats(0);
      dhist->Draw("COLZ");
      canvasp->Update();       
      if (LCol == -1) return;

      //  cout << "Line1" << endl;
      
      NLine1->DrawLine(wherecut,0,wherecut,dwpot.GetYMAX()-2);
      if(fieldyes==true) DrawFieldHist(); // Nicolo1
      canvasp->Update(); 
      
      

      canvasw->cd();
      dwpot.DriftPal();
      whist->Draw("COLZ");
      
      // cout << "Line2" << endl;
      NLine2->DrawLine(wherecut2,0,wherecut2,dwpot.GetYMAX()-2);
      canvasw->Update(); 
    }
  if (GetLessPlot() or LCol == 0) return;
  
  canvaswc->Clear();
  canvaswc->Divide(2,1,0.002,0.002);
  canvaswc->Update();
  canvaspc->Clear();
  canvaspc->Divide(2,1,0.002,0.002);
  canvaspc->Update();
  
  
  DrawCutGraph(LCol);
  DrawFieldsAbs(LCol);
  
  return;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawCutGraph(int LCol)
{
  // return;
	wherecut=WhereCut->GetNumber();
	wherecut2=WhereCut2->GetNumber();
	cout << " " << endl;
	cout << "Plotting the field cutting in x  at " << wherecut << endl;
	double* q=0;
	q = new double[dwpot.GetYMAX()];
	for(int k=0;k<dwpot.GetYMAX();k++) q[k]=k;
	double *dpoty=0;
	float dMax = 0.;
	dpoty = new double[dwpot.GetYMAX()];					
	for(int k=0;k<dwpot.GetYMAX();k++) 
	  {
	    dpoty[k] = dwpot.Getdpot(k,wherecut);
	    if (dpoty[k]> dMax) dMax = dpoty[k];
	  }
	driftcut= new TGraph(dwpot.GetYMAX(),q,dpoty);
	driftcut->GetXaxis()->SetTitle("y [um]");
   	driftcut->GetXaxis()->SetNdivisions(5);
	driftcut->SetTitle(" Drift Potential V [V]");
	driftcut->GetXaxis()->SetLabelColor(1);
	driftcut->GetYaxis()->SetLabelColor(1);
  	driftcut->GetYaxis()->SetLabelSize(0.05);
   	driftcut->GetYaxis()->SetLabelOffset(.01);
   	driftcut->GetXaxis()->SetLabelSize(0.05);
   	driftcut->GetXaxis()->SetLabelOffset(.01);
   	driftcut->GetXaxis()->SetTitleSize(0.05);
   	driftcut->GetYaxis()->SetRangeUser(0.0,dMax*1.3);
	
   	driftcut->SetLineWidth(3);
	driftcut->SetLineColor(LCol); // set line color to black

	canvaspc->cd(1);
	driftcut->Draw("AL");
	canvaspc->Update();

	//	cout << " Done Drift" << endl;

	double *wpoty=0;
	float wMax = 0.;
	wpoty = new double[dwpot.GetYMAX()];					
	for(int k=0;k<dwpot.GetYMAX();k++) 
	  {
	  wpoty[k] = dwpot.Getwpot(k,wherecut);
	  if (wpoty[k] > wMax) wMax = wpoty[k];
	  }

	weightcut= new TGraph(dwpot.GetYMAX(),q,wpoty);
	weightcut->GetXaxis()->SetTitle("y (um)");
   	weightcut->GetXaxis()->SetNdivisions(5);
	weightcut->SetTitle("Weighting Potential  ");	
	weightcut->GetXaxis()->SetLabelColor(1);
	weightcut->GetYaxis()->SetLabelColor(1);
   	weightcut->GetYaxis()->SetLabelSize(0.05);
   	weightcut->GetYaxis()->SetLabelOffset(.01);
   	weightcut->GetXaxis()->SetLabelSize(0.05);
   	weightcut->GetXaxis()->SetLabelOffset(.01);
   	weightcut->GetXaxis()->SetTitleSize(0.05);
   	weightcut->GetYaxis()->SetRangeUser(0.0,wMax*1.3);
   	weightcut->SetLineWidth(3);
	weightcut->SetLineColor(LCol); // set line color to black
	//	canvaswc->Divide(1,1,0.002,0.002);
	canvaswc->cd(1);
	weightcut->Draw("AL");
	canvaswc->Update();

	//	cout << " Done Weighting" << endl;



	
}
/////////////////////////////////////////////////////////////////////////////////////////////

void WFGUI::SetCutOnStrips() {

  wherecut = dwpot.GetXMAX()/2+1;
  wherecut2 = dwpot.GetXMAX()/2+1;
  WhereCut->SetNumber(wherecut);
  WhereCut2->SetNumber(wherecut2);	
  
  DrawAllGraph(2);
  return;

}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawAll() {

  wherecut = WhereCut->GetNumber(); 
  WhereCut->SetNumber(wherecut);
  WhereCut2->SetNumber(wherecut);	
  DrawAllGraph(1);
  return;

}

/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawAllw() {

  wherecut2 = WhereCut2->GetNumber(); 
  WhereCut2->SetNumber(wherecut2);
  WhereCut->SetNumber(wherecut2);	
  DrawAllGraph(1);
  return;

}

/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetCutBetweenStrips() {
  // return;
	wherecut = dwpot.GetXMAX()/2+1+(int)dwpot.Getpitch()/2;	
	wherecut2 = dwpot.GetXMAX()/2+1+(int)dwpot.Getpitch()/2;	
	WhereCut->SetNumber(wherecut);
	WhereCut2->SetNumber(wherecut2);

	DrawAllGraph(4);
	return;

}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetBField(Bool_t on) {
	bfieldon=on;
	if(bfieldon) BfieldEntry->SetState(kTRUE);
	else BfieldEntry->SetState(kFALSE);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetDiffusion(Bool_t on) {
	diffusionon=on;
	//	if(diffusionon) TempEntry->SetState(kTRUE);
	// else TempEntry->SetState(kFALSE);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetOscOn(Bool_t State) {
	OscOn=State;
	if(OscOn==true) 
	{	
	    OscBWEntry->SetState(kTRUE);
	    TRiseEntry->SetState(kTRUE);
	    TFallEntry->SetState(kTRUE);
	    VthEntry->SetState(kTRUE);
	    
	}
	else 	
	{
	    OscBWEntry->SetState(kFALSE);
	    TRiseEntry->SetState(kFALSE);
	    TFallEntry->SetState(kFALSE);
	    VthEntry->SetState(kFALSE);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetFileNameOn(Bool_t State) {
	FileNameOn=State;
	if(FileNameOn==true) 
	{	
	    FileNameEntry->SetState(kTRUE);
	}
	else 	
	{
	    FileNameEntry->SetState(kFALSE);

	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetOscBW(double bw) {
	OscBW=bw;
}
//////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetOscBW() {
	return OscBWEntry->GetNumber();
//	else OscBWEntry->SetState(kFALSE);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetTRise(double tr) {
	TRise=tr;
}
//////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetTRise() {
	return TRiseEntry->GetNumber();

}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetTFall(double tf) {
	TFall=tf;
}
//////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetTFall() {
	return TFallEntry->GetNumber();

}

//////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetVth() {
  return VthEntry->GetNumber();

}

/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetTVth(double vt) {
	TVth=vt;
}
//////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetTVth() {
  return TVth;

}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetJitter(double vt) {
	Jitter=vt;
}
//////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetJitter() {
  return Jitter;

}

/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetFTVth(double vt) {
	FTVth=vt;
}
//////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetFTVth() {
  return FTVth;

}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetFJitter(double vt) {
	FJitter=vt;
}
//////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetFJitter() {
  return FJitter;

}

/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetSWidth(double tf) {
	SWidth= tf;
}
//////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetSWidth() {
  return SWidth;

}

/////////////////////////////////////////////////////////////////////////////////////////////
bool WFGUI::GetOscOn() {
	return OscOn;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool WFGUI::GetFileNameOn() {
	return FileNameOn;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetBatchOn(Bool_t on) {
	BatchOn=on;
	if(BatchOn) EventsEntry->SetState(kTRUE);
	else EventsEntry->SetState(kFALSE);
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool WFGUI::GetBatchOn() {
	return BatchOn;
}
/////////////////////////////////////////////////////////////////////////////////////////////
TH2F* WFGUI::Getwhist() {
	return whist;
}
/////////////////////////////////////////////////////////////////////////////////////////////
TH2F* WFGUI::Getdhist() {
	return dhist;
}
/////////////////////////////////////////////////////////////////////////////////////////////
TLine* WFGUI::GetNLine1() {
	return NLine1;
}
/////////////////////////////////////////////////////////////////////////////////////////////
TEllipse* WFGUI::GetEllipse() {
	return ellipse;
}

/////////////////////////////////////////////////////////////////////////////////////////////
TLine* WFGUI::GetNLine2() {
	return NLine2;
}

/////////////////////////////////////////////////////////////////////////////////////////////
WFGUI::~WFGUI() {
	Cleanup();
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::CloseWindow() // Got close message for this MainFrame. Terminates the application.
{
    gApplication->Terminate();
}
/////////////////////////////////////////////////////////////////////////////////////////////
TThread* WFGUI::GetPotentialThread() {
	return PotentialThread;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void* WFGUI::StartPotentialCalcTh(void* arg)
{	

	WFGUI* gui = (WFGUI*) arg;
	gui->SetAllButton(0);
	gui->CallCalculatePotentials();
	gui->ThreadstopPotential();
	gui->DrawAllGraph(1);
	sleep(1);
	gui->SetAllButton(1);
	//gui->DrawFieldsAbs();

	return NULL;	
}
/////////////////////////////////////////////////////////////////////////////////////////////
Int_t WFGUI::ThreadstartPotential(){

	CallBoundaryConditions();
	if(!PotentialThread){
		//stopped=false;
		WFGUI* arg = this;
		PotentialThread= new TThread("memberfunction",
			            (void(*) (void *))&StartPotentialCalcTh,
			            (void*) arg);
		PotentialThread->Run();	
		return 0;
	}
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////
Int_t WFGUI::ThreadstopPotential(){
  	FillHist();


	//	if(fieldyes==true) DrawFieldHist();
	if(PotentialThread){
	  //Nicolo
	  // stopped=true;
		TThread::Delete(PotentialThread);
		//delete PotentialThread;
		PotentialThread=0;
		return 0;
	}      
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void* WFGUI::StartCurrentsCalcTh(void* arg)
{	
	WFGUI* gui = (WFGUI*) arg;
	gui->CallCalculateCurrents();
	gui->ThreadstopCurrents();
	gui->stopped=false;
	return NULL;	
}
/////////////////////////////////////////////////////////////////////////////////////////////
Int_t WFGUI::ThreadstartCurrents(){
	if(!CurrentsThread){
		//stopped=false;
		WFGUI* arg = this;
		CurrentsThread= new TThread("memberfunction",
			            (void(*) (void *))&StartCurrentsCalcTh,
			            (void*) arg);
		CurrentsThread->Run();	
		return 0;
	}
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////
Int_t WFGUI::ThreadstopCurrents(){
	if(CurrentsThread){
		//stopped=true;
		TThread::Delete(CurrentsThread);
		//delete CurrentsThread;
		CurrentsThread=0;
		return 0;
	}      
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::KillPotentialThread() {
		TThread::Delete(PotentialThread);
		//delete PotentialThread;
		PotentialThread=0;
}
/////////////////////////////////////////////////////////////////////////////////////////////
TCanvas* WFGUI::Getcanvasp() {
	return canvasp;
}
/////////////////////////////////////////////////////////////////////////////////////////////
TCanvas* WFGUI::Getcanvasw() {
	return canvasw;
}
/////////////////////////////////////////////////////////////////////////////////////////////
TCanvas* WFGUI::Getcurcanvas() {
	return curcanvas;
}
/////////////////////////////////////////////////////////////////////////////////////////////
TCanvas* WFGUI::Getosccanvas() {
	return osccanvas;
}
/////////////////////////////////////////////////////////////////////////////////////////////
TGHProgressBar* WFGUI::GetProgressBar() {
	return CurrentsProgressBar;
}
/////////////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetT() {
	return Temp;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetGainon(bool gain) {
	gainon=gain;
}
//////////////////////////////////////////////////////////////////////////////////////
bool WFGUI::GetGainon() {
	return gainon;
}
//////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetGainRatio(double gain) {
	GainRatio=gain;
}
//////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetGainRatio() {
	return GainRatio;

}
//////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetYGain(double y_gain) {
	ygain=y_gain;
}
//////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetYGain() {
	return ygain;
}

//////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetAngle() {
	//angle = (TMath::Pi()/180)*CarriersAngleNumberentry->GetNumber();
	angle = CarriersAngleNumberentry->GetNumber(); //angle in degrees
	return angle;
}
/////////////////////////////////////////////////////////////////////////
int WFGUI::GetDimMaxCarriers(){
	return dimMaxCarriers;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetAllButton(int NN) {
  if (NN == 1)
    {
      if(!GetLessPlot())
	{
	  OnStripsButton->SetEnabled(kTRUE);
	  BetweenStripsButton->SetEnabled(kTRUE);
	  ExButton->SetEnabled(kTRUE);
	  EyButton->SetEnabled(kTRUE);
	  OnStripsButton2->SetEnabled(kTRUE);
	  BetweenStripsButton2->SetEnabled(kTRUE);
	  ExButton2->SetEnabled(kTRUE);
	  EyButton2->SetEnabled(kTRUE);
	  DrawCutsUserEntry->SetEnabled(kTRUE);
	  DrawCutsUserEntry2->SetEnabled(kTRUE);
	}
	CalculateButton->SetEnabled(kTRUE);
	CalcPotButton->SetEnabled(kTRUE);

    }
  else
    {
	OnStripsButton->SetEnabled(kFALSE);
	BetweenStripsButton->SetEnabled(kFALSE);
	ExButton->SetEnabled(kFALSE);
	EyButton->SetEnabled(kFALSE);
	//	EtotButton->SetEnabled(kFALSE);
	CalculateButton->SetEnabled(kFALSE);
	CalcPotButton->SetEnabled(kFALSE);
	OnStripsButton2->SetEnabled(kFALSE);
	BetweenStripsButton2->SetEnabled(kFALSE);
	ExButton2->SetEnabled(kFALSE);
	EyButton2->SetEnabled(kFALSE);
	DrawCutsUserEntry->SetEnabled(kFALSE);
	DrawCutsUserEntry2->SetEnabled(kFALSE);
	//	EtotButton2->SetEnabled(kFALSE);
    }

	return;

}

/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetFileName(string* fname) {
	fileName = *fname;
}


/////////////////////////////////////////////////////////////////////////////////////////////
string* WFGUI::GetFileName() {
  
  if (fileName=="" || strncmp(fileName.c_str(),"wf_",3)==0)
    {
      
      //      CallSetFileName("wf");
      CallSetFileName();  
    }
  
  //
  return &fileName;
}
/////////////////////////////////////////////////////////////////////////////////////////////

string* WFGUI::CallSetFileName() {

  
  //  TDatime *NameDate = new TDatime();
  Int_t Dif = 0;
  Float_t BW=0.;
  if(Getdiffusionon() ) Dif = 1;  
  if(OscOn==true)   BW = GetOscBW();
  string Part;
  Int_t Qc = 0;

  switch (radiobuttonstatus){
  case MIPunif:
    Part = "_MIPU";
    Qc = 75;

    break;
    
  case MIPnonunif:
    Part = "_MIPNONU";
    Qc = 0;
    break;
    
  case MIPlandau:
    Part = "_MIPL";
    Qc = 0;
    break;
    
  case ALPHA_TOP:
    Part = "_AT";
    Qc = RangeEntry->GetNumber();
    break;
    
  case ALPHA_BOTTOM:

    Part  = "_AB";
    Qc = RangeEntry->GetNumber();

    break;
    
  case USR_CHARGE:
    Part = "_USRQ";
    Qc = Chargeentry->GetNumber();
    break;
    
  default: break;
  }
  
  string defaultName = FileNameEntry->GetText();
  if (defaultName=="") defaultName = "wf";


  //  cout << Part << endl;
  //  ss << FileNameEntry->GetText()
  std::stringstream ss;

  ss << defaultName 
    //  ss << FileNameEntry->GetText()
    << "_BV" <<  Biasentry->GetNumber()
     << "_DV" << Depletionentry->GetNumber()
     << "_W" << dwpot.GetXMAX()
     << "_H" << dwpot.GetYMAX()
    << "_SP" << Pitchentry->GetNumber()
    << "_SW" << Widthentry->GetNumber()
    << "_G" << Gainentry->GetNumber()
    << "_GR" << GainRatioentry->GetNumber()
    << "_T" << TempEntry->GetNumber()
     << "_A" << CarriersAngleNumberentry->GetNumber()
     << "_D" << Dif  
     << "_BW" << BW  
     << Part
     << Qc

    //    << "_" << NameDate->GetDate()
    // << "_" << NameDate->GetTime()
    << ".txt";
	fileName = ss.str();
	if (GetFileNameOn() ) 
	  cout <<"Creating output file: "<< fileName << endl;
	return &fileName;

}
/////////////////////////////////////////////////////////////////////////////////////////////
/*
string* WFGUI::CallSetFileName(string defaultName) {
  
  std::stringstream ss;
  ss << FileNameEntry->GetText()
    << "_BV" <<  Biasentry->GetNumber()
    << "_W" << XMAXentry->GetNumber()
    << "_H" << YMAXentry->GetNumber()
    << "_SP" << Pitchentry->GetNumber()
    << "_SW" << Widthentry->GetNumber()
    << "_G" << Gainentry->GetNumber()
    << "_T" << TempEntry->GetNumber()
    //    << "_" << NameDate->GetDate()
    // << "_" << NameDate->GetTime()
    << ".txt";
	fileName = ss.str();
	cout <<"Creating output file: "<< fileName << endl;
	return &fileName;

}
 
*/
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetAlphaRange(double valrange) {
	AlphaRange=valrange;
}
//////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetAlphaRange() {
	return AlphaRange;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetPrecision(int valprecision) {
	Precision=valprecision;
}
//////////////////////////////////////////////////////////////////////////////////////
int WFGUI::GetPrecision() {
	return Precision;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetConstQFlag(bool cflag) {
	ConstQFlag=cflag;
}
//////////////////////////////////////////////////////////////////////////////////////
bool WFGUI::GetConstQFlag() {
	return ConstQFlag;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetUniformQFlag(bool uflag) {
	UniformQFlag=uflag;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetUserUniformQFlag(bool uflag) {
	UserUniformQFlag=uflag;
}
//////////////////////////////////////////////////////////////////////////////////////
bool WFGUI::GetUniformQFlag() {
	return UniformQFlag;
}
//////////////////////////////////////////////////////////////////////////////////////
bool WFGUI::GetUserUniformQFlag() {
	return UserUniformQFlag;
}
//////////////////////////////////////////////////////////////////////////////////////

void WFGUI::SetEvNumber(int ev) {
	EvNumber=ev;
}
//////////////////////////////////////////////////////////////////////////////////////
int WFGUI::GetEvNumber() {
	return EvNumber;
}
//////////////////////////////////////////////////////////////////////////////////////
int WFGUI::GetEvent() {
	return ThisEvent;
}
//////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetEvent(int thisev) {
	ThisEvent=thisev;
	
}
//////////////////////////////////////////////////////////////////////////////////////
float WFGUI::Getygainlow(){
  if(StripButton[1]->IsOn()*BulkButton[0]->IsOn() || StripButton[0]->IsOn()*BulkButton[1]->IsOn() ) return (dwpot.GetYMAX()- GAINDEPTH - GAINLENGHT);
  else return GAINDEPTH;
}
//////////////////////////////////////////////////////////////////////////////////////
float WFGUI::Getygainhigh(){
   if(StripButton[1]->IsOn()*BulkButton[0]->IsOn() || StripButton[0]->IsOn()*BulkButton[1]->IsOn() ) return (dwpot.GetYMAX()- GAINDEPTH) ;
   else return GAINDEPTH+GAINLENGHT;
}

