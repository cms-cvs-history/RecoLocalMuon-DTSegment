/** \file
 *
 * $Date: 2006/05/04 12:19:17 $
 * $Revision: 1.1 $
 * \author Riccardo Bellan - INFN TO <riccardo.bellan@cern.ch>
 */

/* This Class Header */
// ----------------
#include "RecoLocalMuon/DTSegment/test/DTRecSegment4DReader.h"

/* Collaborating Class Header */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// this is to retrieve also 1D rechits
#include "DataFormats/DTRecHit/interface/DTRecHit1DPair.h"
#include "DataFormats/DTRecHit/interface/DTRecHit1D.h"
#include "DataFormats/DTRecHit/interface/DTRecHitCollection.h"

#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/DTGeometry/interface/DTChamber.h"
#include "DataFormats/LTCDigi/interface/LTCDigi.h"


// for CSC
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"

// for StandAlone ----
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "RecoMuon/TrackingTools/interface/MuonPatternRecoDumper.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

/* C++ Headers */
#include <string>
#include <sstream>

using namespace std;
/* ====================================================================== */

/// Constructor
DTRecSegment4DReader::DTRecSegment4DReader(const edm::ParameterSet& pset) {
  // Get the debug parameter for verbose output
  debug = pset.getUntrackedParameter<bool>("debug");
  theRootFileName = pset.getUntrackedParameter<string>("rootFileName");
 


 
 
 // the name of the 4D rec hits collection
  theRecHits4DLabel = pset.getParameter<string>("recHits4DLabel");


  if(debug)
//aaaa      cout << "[DTRecSegment4DReader] Constructor called" << endl;

  // Create the root file
  theFile = new TFile(theRootFileName.c_str(), "RECREATE");
  theFile->cd();
  
// book histos...
  for ( int iwheel = 1; iwheel !=3; iwheel++)     {   // wheels
   stringstream string_wheel; string_wheel << iwheel;
   for (int isect = 10; isect != 12; isect++ ) { // sectors
    stringstream string_sect; string_sect << isect;	
    for (int ist = 1; ist != 5; ist++){   // loop on stations
      stringstream string_st; string_st << ist;	
      string histoname = "hnsMB" + string_st.str() + "_s" + string_sect.str() + "_w" + string_wheel.str();  //  track multiplicity
      new TH1F(histoname.c_str(),"Ntk ", 20, 0., 20. );    
    } // next station
   } // next station 
  } // next wheel 
  
// trigger bits
   hLTCbits = new TH1F("hLTCbits","Trigger bits",10,0.,10.);
 
// DT and CSC segments 
  hNsegDT  = new TH1F("hNsegDT"," DT segments", 20, 0, 20); 
  hNsegCSC = new TH1F("hNsegCSC"," CSC segments", 20, 0, 20); 
  hNsegRPC = new TH1F("hNsegRPC"," RPC segments", 20, 0, 20); 


  T0Zedvst_PosX_1 = new TH2F("T0Zedvst_PosX_1","T0Zedvst_PosX in MB1",60, -30.,  30., 60,-30.,30.);
  T0Zedvst_PosX_2 = new TH2F("T0Zedvst_PosX_2","Zedvst_PosX in MB2",60, -30.,  30., 60,-30.,30.);
  T0Zedvst_PosX_3 = new TH2F("T0Zedvst_PosX_3","T0Zedvst_PosX in MB3",60, -30.,  30., 60,-30.,30.);
  T0Phivst_PosZ_1 = new TH2F("T0Phivst_PosZ_1"," T0Phivst_Pos in MB1",60, -30.,  30., 60,-30.,30.);
  T0Phivst_PosZ_2 = new TH2F("T0Phivst_PosZ_2"," T0Phivst_Pos in MB2",60, -30.,  30., 60,-30.,30.);
  T0Phivst_PosZ_3 = new TH2F("T0Phivst_PosZ_3"," T0Phivst_Pos in MB3",60, -30.,  30., 60,-30.,30.);
  hPosXvsZ_1 = new TH2F("hPosXvsZ_1","X vs Z Position in MB1",100, 150., 700., 100,-150.,400.);
  hPosXvsZ_2 = new TH2F("hPosXvsZ_2","X vs Z Position in MB2",100, 150., 700., 100,-150.,400.);
  hPosXvsZ_3 = new TH2F("hPosXvsZ_3","X vs Z Position in MB3",100, 150., 700., 100,-150.,400.);
  hPosXvsZ_4 = new TH2F("hPosXvsZ_4","X vs Z Position in MB4",100, 150., 700., 100,-150.,400.);    
  hPosXMB1 = new TH1F("hPosXMB1","X Position in MB1",100, -150,150);
  hPosZMB1 = new TH1F("hPosZMB1","Z Position in MB1",100, 400,700);
  hPhiMB1  = new TH1F("hPhiMB1","Phi Direction in MB1",200,-1.5,1.5);
  hThMB1   = new TH1F("hThMB1","Theta Direction in MB1",200,-1.5,1.5);

  htPhiMB1 = new TH1F("hT0PhiMB1","T0Phi  in MB1",100,-50.,50.);
  hto_vdriftMB1 = new TH1F("to_DvdriftMB1","T0_Dvdrift % in MB1",100,-50.,50.);
  htThMB1  = new TH1F("hT0ThMB1","T0Theta  in MB1",100,-50.,50.);
  hctPhiMB1 = new TH1F("hcT0PhiMB1","T0Phi  in MB1",100,-50.,50.);
  hctThMB1  = new TH1F("hcT0ThMB1","T0cTheta  in MB1",100,-50.,50.);
   hDelT012_110= new TH2F("hDelT012_110","diff T0 MB2 vs T0 MB1",100,-50.,50.,100,-50.,50.);
   hDelT0cut12_110= new TH2F("hDelT0cut12_110"," Sel diff T0 MB2 vs T0 MB1",100,-50.,50.,100,-50.,50.);
  hPosXMB2 = new TH1F("hPosXMB2","X Position in MB2",100, -150,150);
  hPosZMB2 = new TH1F("hPosZMB2","Z Position in MB2",100, 400,700);
  hPredXMB2 = new TH1F("hPredXMB2","X Position in MB2",100, -150,150);
  hPredZMB2 = new TH1F("hPredZMB2","Z Position in MB2",100, 400,700);
  hPhiMB2  = new TH1F("hPhiMB2","Phi Direction in MB2",200,-1.5,1.5);
  hThMB2   = new TH1F("hThMB2","Theta Direction in MB2",200,-1.5,1.5);
  htPhiMB2 = new TH1F("htPhiMB2","T0Phi  in MB2",100,-50.,50.);
  hto_vdriftMB2 = new TH1F("to_DvdriftMB2","T0_Dvdrift % in MB2",100,-50.,50.);
  htThMB2  = new TH1F("htThMB2","T0Theta  in MB2",100,-50.,50.);
  hctPhiMB2 = new TH1F("hctPhiMB2","T0Phi  in MB2",100,-50.,50.);
  hctThMB2  = new TH1F("hctThMB2","T0cTheta  in MB2",100,-50.,50.);

  hPosXMB3 = new TH1F("hPosXMB3","X Position in MB3",100, -150,150);
  hPosZMB3 = new TH1F("hPosZMB3","Z Position in MB3",100, 400,700);
  hPredXMB3 = new TH1F("hPredXMB3","X Position in MB3",100, -150,150);
  hPredZMB3 = new TH1F("hPredZMB3","Z Position in MB3",100, 400,700);
  hPhiMB3  = new TH1F("hPhiMB3","Phi Direction in MB3",200,-1.5,1.5);
  hThMB3   = new TH1F("hThMB3","Theta Direction in MB3",200,-1.5,1.5);
  htPhiMB3 = new TH1F("htPhiMB3","T0Phi  in MB3",100,-50.,50.);
  hto_vdriftMB3 = new TH1F("to_DvdriftMB3","T0_Dvdrift % in MB3",100,-50.,50.);
  htThMB3  = new TH1F("htThMB3","T0Theta  in MB3",100,-50.,50.);
  hctPhiMB3 = new TH1F("hctPhiMB3","T0Phi  in MB3",100,-50.,50.);
  hctThMB3  = new TH1F("hctThMB3","T0cTheta  in MB3",100,-50.,50.);

  hPosXMB4 = new TH1F("hPosXMB4","X Position in MB4",100,-150,150);
  hPhiMB4 = new TH1F("hPhiMB4","Phi Direction in MB4",100,-1.5,1.5);
  htPhiMB4 = new TH1F("htPhiMB4","T0Phi  in MB4",100,-50.,50.);
  hto_vdriftMB4 = new TH1F("to_DvdriftMB4","T0_Dvdrift % in MB4",100,-50.,50.);
  hctPhiMB4 = new TH1F("hctPhiMB4","T0cPhi  in MB4",100,-50.,50.);
 
  hResZedMB1_l2  = new TH1F("hResZedMB1_l2","hResZedMB1_l2",100,-0.25,0.25);
  hResZedMB1_vs_z = new TH2F("hResZedMB1_vs_z","hResZedMB1_vs_z",60,400.,700.,100,-0.25,0.25);
  hnptZedMB1= new TH1F("nptZedMB1","nptZedMB1",10,0.5,10.5);
  hResZedMB2_vs_z = new TH2F("hResZedMB2_vs_z","hResZedMB2_vs_z",60,400.,700.,100,-0.25,0.25);
  hnptZedMB2= new TH1F("nptZedMB2","nptZedMB2",10,0.5,10.5);
  hResZedMB3_vs_z = new TH2F("hResZedMB3_vs_z","hResZedMB3_vs_z",60,400.,700.,100,-0.25,0.25);
  hnptZedMB3= new TH1F("nptZedMB3","nptZedMB3",10,0.5,10.5);
  hnptZedMB4= new TH1F("nptZedMB4","nptZedMB4",10,0.5,10.5);
  hResZedMB2_l2  = new TH1F("hResZedMB2_l2","hResZedMB2_l2",100,-0.25,0.25);
  hResZedMB3_l2  = new TH1F("hResZedMB3_l2","hResZedMB3_l2",100,-0.25,0.25);
  hResZedMB4_l2  = new TH1F("hResZedMB4_l2","hResZedMB4_l2",100,-0.25,0.25);
  hResPhiMB1_l5  = new TH1F("hResPhiMB1_l5","hResPhiMB1_l5",100,-0.25,0.25);
  hResPhiMB1_vs_z = new TH2F("hResPhiMB1_vs_z","hResPhiMB1_vs_z",60,400.,700.,100,-0.25,0.25);
  hnptPhiMB1= new TH1F("nptPhiMB1","nptPhiMB1",10,0.5,10.5);
  hResPhiMB2_vs_z = new TH2F("hResPhiMB2_vs_z","hResPhiMB2_vs_z",60,400.,700.,100,-0.25,0.25);
  hnptPhiMB2= new TH1F("nptPhiMB2","nptPhiMB2",10,0.5,10.5);
  hResPhiMB3_vs_z = new TH2F("hResPhiMB3_vs_z","hResPhiMB3_vs_z",60,400.,700.,100,-0.25,0.25);
  hnptPhiMB3= new TH1F("nptPhiMB3","nptPhiMB3",10,0.5,10.5);
  hnptPhiMB4= new TH1F("nptPhiMB4","nptPhiMB4",10,0.5,10.5);
/*  hResPhiMB1_l2  = new TH1F("hResPhiMB1_l2","hResPhi_l2",100,-0.25,0.25);
  hResPhiMB1_l3  = new TH1F("hResPhiMB1_l3","hResPhi_l3",100,-0.25,0.25);
  hResPhiMB1_l4  = new TH1F("hResPhiMB1_l4","hResPhi_l4",100,-0.25,0.25);
  hResPhiMB1_l5  = new TH1F("hResPhiMB1_l5","hResPhi_l5",100,-0.25,0.25);
  hResPhiMB1_l6  = new TH1F("hResPhiMB1_l6","hResPhi_l6",100,-0.25,0.25);
  hResPhiMB1_l7  = new TH1F("hResPhiMB1_l7","hResPhi_l7",100,-0.25,0.25);
  hResPhiMB1_l8  = new TH1F("hResPhiMB1_l8","hResPhi_l8",100,-0.25,0.25);*/
  hResPhiMB2_l5  = new TH1F("hResPhiMB2_l5","hResPhiMB2_l5",100,-0.25,0.25);
  hResPhiMB3_l5  = new TH1F("hResPhiMB3_l5","hResPhiMB3_l5",100,-0.25,0.25);
  hResPhiMB4_l5  = new TH1F("hResPhiMB4_l5","hResPhiMB4_l5",100,-0.25,0.25);
  
 
  hDelx12_110 = new TH1F("hDelx12_110","Dx 1-2",100, -10.,10.);
  hDelz12_110 = new TH1F("hDelz12_110","Dz 1-2",100, -10.,10.);
  hDelx12_210 = new TH1F("hDelx12_210","Dx 1-2",100, -10.,10.);
  hDelz12_210 = new TH1F("hDelz12_210","Dz 1-2",100, -10.,10.);
  hDelx12_211 = new TH1F("hDelx12_211","Dx 1-2",100, -10.,10.);
  hDelz12_211 = new TH1F("hDelz12_211","Dz 1-2",100, -10.,10.);
  hDelx12_110_211 = new TH1F("hDelx12_110_211","Dx 1-2",100, -10.,10.);
  hDelz12_110_211 = new TH1F("hDelz12_110_211","Dz 1-2",100, -10.,10.);
  
  hDelx23_110 = new TH1F("hDelx23_110","Dx 1-2",100, -10.,10.);
  hDelz23_110 = new TH1F("hDelz23_110","Dz 1-2",100, -10.,10.);
  hDelx23_210 = new TH1F("hDelx23_210","Dx 1-2",100, -10.,10.);
  hDelz23_210 = new TH1F("hDelz23_210","Dz 1-2",100, -10.,10.);
  hDelx23_211 = new TH1F("hDelx23_211","Dx 1-2",100, -10.,10.);
  hDelz23_211 = new TH1F("hDelz23_211","Dz 1-2",100, -10.,10.);
  hDelx23_110_211 = new TH1F("hDelx23_110_211","Dx 1-2",100, -10.,10.);
  hDelz23_110_211 = new TH1F("hDelz23_110_211","Dz 1-2",100, -10.,10.);
  
  hDelx34_110 = new TH1F("hDelx34_110","Dx 1-2",100, -10.,10.);
  hDelx34_210 = new TH1F("hDelx34_210","Dx 1-2",100, -10.,10.);
  hDelx34_211 = new TH1F("hDelx34_211","Dx 1-2",100, -10.,10.);
  hDelx34_110_211 = new TH1F("hDelx34_110_211","Dx 1-2",100, -10.,10.);


  hDphi12_110 = new TH1F("hDphi12_110","Dphi  1-2",100, -0.25,0.25);
  hDth12_110  = new TH1F("hDth12_110","Dtheta 1-2",100, -0.25,0.25);
  hDphi12_210 = new TH1F("hDphi12_210","Dphi  1-2",100, -0.25,0.25);
  hDth12_210  = new TH1F("hDth12_210","Dtheta 1-2",100, -0.25,0.25);
  hDphi12_211 = new TH1F("hDphi12_211","Dphi  1-2",100, -0.25,0.25);
  hDth12_211  = new TH1F("hDth12_211","Dtheta 1-2",100, -0.25,0.25);
  hDphi12_110_211 = new TH1F("hDphi12_110_211","Dphi  1-2",100, -0.25,0.25);
  hDth12_110_211  = new TH1F("hDth12_110_211","Dtheta 1-2",100, -0.25,0.25);

  hDphi23_110 = new TH1F("hDphi23_110","Dphi  1-2",100, -0.25,0.25);
  hDth23_110  = new TH1F("hDth23_110","Dtheta 1-2",100, -0.25,0.25);
  hDphi23_210 = new TH1F("hDphi23_210","Dphi  1-2",100, -0.25,0.25);
  hDth23_210  = new TH1F("hDth23_210","Dtheta 1-2",100, -0.25,0.25);
  hDphi23_211 = new TH1F("hDphi23_211","Dphi  1-2",100, -0.25,0.25);
  hDth23_211  = new TH1F("hDth23_211","Dtheta 1-2",100, -0.25,0.25);
  hDphi23_110_211 = new TH1F("hDphi23_110_211","Dphi  1-2",100, -0.25,0.25);
  hDth23_110_211  = new TH1F("hDth23_110_211","Dtheta 1-2",100, -0.25,0.25);

  hDphi34_110 = new TH1F("hDphi34_110","Dphi  1-2",100, -0.25,0.25);
  hDphi34_210 = new TH1F("hDphi34_210","Dphi  1-2",100, -0.25,0.25);
  hDphi34_211 = new TH1F("hDphi34_211","Dphi  1-2",100, -0.25,0.25);
  hDphi34_110_211 = new TH1F("hDphi34_110_211","Dphi  1-2",100, -0.25,0.25);

  hdphi_23vs12 = new TH2F("hdphi_23vs12","Dphi23 vs Dphi12",100, -0.05, 0.05,100,-0.05,0.05);
  hdphi_34vs23 = new TH2F("hdphi_34vs23","Dphi34 vs Dphi23",100, -0.05, 0.05,100,-0.05,0.05);
  hdelx_23vs12 = new TH2F("hdelx_23vs12","Delx23 vs Delx12",100, -10., 10.0,100,-10.0,10.0);
  hdelx_34vs23 = new TH2F("hdelx_34vs23","Delx34 vs Delx23",100, -10., 10.0,100,-10.0,10.0);
  
  hpt12 = new TH1F("hpt12","pt  1-2",100, -5.,5.);
  hpt23 = new TH1F("hpt23","pt  2-3",100, -5.,5.);
  hpt34 = new TH1F("hpt34","pt  3-4",100, -5.,5.);
  hdpt1 = new TH1F("hdpt1","dpt  1",100, -5., 5.);
  hdpt2 = new TH1F("hdpt2","dpt  2",100, -5., 5.);      
}

/// Destructor
DTRecSegment4DReader::~DTRecSegment4DReader() {
  if(debug) 
//aaaa      cout << "[DTRecSegment4DReader] Destructor called" << endl;

  // Write the histos to file
  theFile->cd();

// write histos...
  for ( int iwheel = 1; iwheel !=3; iwheel++)     {   // wheels
   stringstream string_wheel; string_wheel << iwheel;
   for (int isect = 10; isect != 12; isect++ ) { // sectors
    stringstream string_sect; string_sect << isect;	
    for (int ist = 1; ist != 5; ist++){   // loop on stations
      stringstream string_st; string_st << ist;	
      string histoname = "hnsMB" + string_st.str() + "_s" + string_sect.str() + "_w" + string_wheel.str();  //  track multiplicity
      histo(histoname.c_str())->Write();
    } // next station
   } // next station 
  } // next wheel 
   hLTCbits ->Write();
   hNsegDT ->Write();
   hNsegCSC ->Write();
//   hNsegRPC ->Write();

  T0Zedvst_PosX_1 ->Write(); 
  T0Zedvst_PosX_2 ->Write(); 
  T0Zedvst_PosX_3 ->Write(); 
  T0Phivst_PosZ_1 ->Write(); 
  T0Phivst_PosZ_2 ->Write(); 
  T0Phivst_PosZ_3 ->Write(); 

   hPosXvsZ_1 ->Write();
   hPosXvsZ_2 ->Write();
   hPosXvsZ_3 ->Write();   
   hPosXvsZ_4 ->Write();

    hPosXMB1->Write();
    hPosZMB1->Write();
    hPhiMB1->Write();
    hThMB1->Write();
    htPhiMB1->Write();
    hto_vdriftMB1->Write();
    htThMB1->Write();
    hctPhiMB1->Write();
    hctThMB1->Write();
    hDelT012_110->Write();
    hDelT0cut12_110->Write();

    hPosXMB2->Write();
    hPosZMB2->Write();
    hPredXMB2->Write();
    hPredZMB2->Write();
    hPhiMB2->Write();
    hThMB2->Write();
    htPhiMB2->Write();
    hto_vdriftMB2->Write();
    htThMB2->Write();
    hctPhiMB2->Write();
    hctThMB2->Write();

    hPosXMB3->Write();
    hPosZMB3->Write();
    hPredXMB3->Write();
    hPredZMB3->Write();
    hPhiMB3->Write();
    hThMB3->Write();
    htPhiMB3->Write();
    hto_vdriftMB3->Write();
    htThMB3->Write();
    hctPhiMB3->Write();
    hctThMB3->Write();

    hPosXMB4->Write();
    hPhiMB4->Write();
    htPhiMB4->Write();
    hto_vdriftMB4->Write();
    hctPhiMB4->Write();
    
    hnptZedMB2->Write();
    hnptPhiMB2->Write();
    hResPhiMB2_vs_z->Write();
    hnptZedMB3->Write();
    hnptPhiMB3->Write();
    hResPhiMB3_vs_z->Write();
    hnptPhiMB1->Write();
    hnptZedMB1->Write();
    hResPhiMB1_vs_z->Write();
    hnptPhiMB4->Write();
//    hResPhiMB1_vs_z->Write();
    hResPhiMB1_l5->Write();
    hResPhiMB2_l5->Write();
    hResPhiMB3_l5->Write();
    hResPhiMB4_l5->Write();
    hResZedMB1_l2->Write();
    hResZedMB2_l2->Write();
    hResZedMB3_l2->Write();
    hResZedMB4_l2->Write();


  hDelx12_110->Write();
  hDelz12_110->Write();
  hDelx12_210->Write();
  hDelz12_210->Write();
  hDelx12_211->Write();
  hDelz12_211->Write();
  hDelx12_110_211->Write();
  hDelz12_110_211->Write(); 

  hDphi12_110->Write();
  hDphi12_210->Write();  
  hDphi12_211->Write();
  hDth12_110->Write();
  hDth12_210->Write();
  hDth12_211->Write();
  hDphi12_110_211->Write();
  hDth12_110_211->Write(); 

  hDelx23_110->Write();
  hDelz23_110->Write();
  hDelx23_210->Write();
  hDelz23_210->Write();
  hDelx23_211->Write();
  hDelz23_211->Write();
  hDelx23_110_211->Write();
  hDelz23_110_211->Write(); 

  hDphi23_110->Write();
  hDphi23_210->Write();  
  hDphi23_211->Write();
  hDth23_110->Write();
  hDth23_210->Write();
  hDth23_211->Write();
  hDphi23_110_211->Write();
  hDth23_110_211->Write();  

  hDelx34_110->Write();
  hDelx34_210->Write();
  hDelx34_211->Write();
  hDelx34_110_211->Write(); 

  hDphi34_110->Write();
  hDphi34_210->Write();  
  hDphi34_211->Write();
  hDphi34_110_211->Write();

  hdphi_23vs12 ->Write();
  hdphi_34vs23 ->Write();
  hdelx_23vs12 ->Write();
  hdelx_34vs23 ->Write();
  
  hpt12->Write();
  hpt23->Write();
  hpt34->Write();
  hdpt1->Write();
  hdpt2->Write();  
  theFile->Close();
}



/* utility */
TH1F*  DTRecSegment4DReader::histo(const char* name) const{
   return dynamic_cast<TH1F*>(theFile->Get(name));
 }
   



/* Operations */ 
void DTRecSegment4DReader::analyze(const edm::Event & event, const
                                 edm::EventSetup& eventSetup) {

//aaaa    cout << endl<<"--- [DTRecSegment4DReader] Event analysed #Run: " << event.id().run()
//aaaa         << " #Event: " << event.id().event() << endl;


// get LTDC info ----------------------------------------------

edm::Handle<LTCDigiCollection> ltcdigis;
 event.getByType(ltcdigis);

 string triggerType;
 int  iwheeltest =2;
 int  isectest   =1;
 int iDTtrigg  = 0;
 int iCSCtrigg = 0;  
 int  iRPC1trigg= 0;  
 int  iRPC2trigg= 0;  
// cout<<"[LTCDigiCollection]: size "<<ltcdigis->size()<<endl;
 for (std::vector<LTCDigi>::const_iterator ltc_it =
 ltcdigis->begin(); ltc_it != ltcdigis->end(); ltc_it++){
 
   for (int i = 0; i < 6; i++) {
   //  cout<<"[LTCDigi]: bit "<<i <<" has Trigger? "<<(*ltc_it).HasTriggered(i)<<endl;
     if ((*ltc_it).HasTriggered(i)) hLTCbits->Fill(i);
   }  
   if ((*ltc_it).HasTriggered(0)) {
     triggerType = "DT";
     iDTtrigg = 1;
     }
   if ((*ltc_it).HasTriggered(1)) {
     iCSCtrigg = 1;
     }
   if ((*ltc_it).HasTriggered(2)) {
     iRPC1trigg = 1;
     }
   if ((*ltc_it).HasTriggered(3)) {
     iRPC2trigg = 1;
     }
   if (!(*ltc_it).HasTriggered(0))
     triggerType = "NoDT";

   int otherTriggerSum=0;
   for (int i = 1; i < 6; i++)
     otherTriggerSum += int((*ltc_it).HasTriggered(i));

   if ((*ltc_it).HasTriggered(0) && otherTriggerSum == 0)
     triggerType = "DTOnly";

 } // next LTC digi

// **** events with ONLY DT trigger **** 
//  if ( triggerType != "DTOnly" ) return;

 if ( (iwheeltest ==2) && (iRPC2trigg == 0) ) return;
 if ( (iwheeltest ==1) && (iRPC1trigg == 0) ) return;


//  if ( iCSCtrigg == 1 ) return;


 // Get the DT Geometry
  edm::ESHandle<DTGeometry> dtGeom;
  eventSetup.get<MuonGeometryRecord>().get(dtGeom);


  // Get the rechit collection from the event
  edm::Handle<DTRecSegment4DCollection> all4DSegments;
  event.getByLabel(theRecHits4DLabel, all4DSegments);

  DTRecSegment4DCollection::const_iterator segment;

//  cout<<"Reconstructed segments: "<<endl;
  
// local vectors to store info ---------------------------------------  
  int nsMB1 = 0,  nsMB2 = 0,  nsMB3 = 0,  nsMB4 = 0; 
  int nsMB[5][3][3];  // (station , sect, wheel)  matrix  
  for ( int iwheel = 1; iwheel !=3; iwheel++) {
   for ( int isect = 1; isect !=3; isect++) {
    for ( int ist = 1; ist !=5; ist++) {
      nsMB[ist][isect][iwheel]=0;
     }
   }
  } 
  int flag1[200], flag2[200], flag3[200], flag4[200]; 
  float xMB1[200], yMB1[200],zMB1[200];  // segm position in MB1
  float xMB2[200], yMB2[200],zMB2[200]; // segments pos. in MB2
  float xMB3[200], yMB3[200],zMB3[200];
  float xMB4[200], yMB4[200],zMB4[200];
  float xslMB1[200], yslMB1[200], zslMB1[200]; // segm slopes
  float xslMB2[200], yslMB2[200], zslMB2[200]; 
  float xslMB3[200], yslMB3[200], zslMB3[200];  
  float xslMB4[200], yslMB4[200], zslMB4[200];  
  float tcPhiMB1[200],tcZedMB1[200];
  float tcPhiMB2[200],tcZedMB2[200];
  float tcPhiMB3[200],tcZedMB3[200];
  float tcPhiMB4[200],tcZedMB4[200];
  float tPhiMB1[200],tZedMB1[200];
  float tPhiMB2[200],tZedMB2[200];
  float tPhiMB3[200],tZedMB3[200];
  float tPhiMB4[200],tZedMB4[200];
  float invvproptdc =0.05246 ;
  float invvpropns  =0.041   ;

// loop on reco segments ---------------------------------------  
 for (segment = all4DSegments->begin(); segment != all4DSegments->end(); ++segment){
   float vdrift_cor=-1000.;
   float t0_vdriftPhi=-1000.;
   float t0corPhi = -1000.;
   float t0corZed = -1000.;
   float t0_corPhi = 0.;
   float t0_corZed = 0.;
   float t0_wPhi = 0.;
   float t0_wZed = 0.;
    const DTChamber* ch = dtGeom->chamber(segment->chamberId());
    int ist    = segment->chamberId().station();
    int iwheel = segment->chamberId().wheel();
//    if ( iwheel = 2 ) continue;
    int isect  = segment->chamberId().sector()- 9; // sect 10-> isect=1, sect 11-> isect = 2;
    if( segment->chamberId().sector() == 14 ) isect = 1;  // handle MB4 chambers in sect.10 ...
//  trasforma da locale a globale

    GlobalPoint   glbPoint = ch->toGlobal((*segment).localPosition());
    GlobalVector  glbDir   = ch->toGlobal((*segment).localDirection());

//    cout << "xlocal " << (*segment).localPosition().x() << " ylocal " << (*segment).localPosition().y() << " zlocal " << (*segment).localPosition().z() << endl;

    bool hasPhi=segment->hasPhi();
    bool hasZed=segment->hasZed();


    if (hasPhi) { 
      float t0segn = (*segment->phiSegment()).t0();
	if (t0segn!=0) {
 	int   t0segn_10time_ns = static_cast<int>(t0segn);
  	float dvDrift0 = t0segn -  t0segn_10time_ns;
  	float dvDrift=abs(dvDrift0);
  	int signvdrift =static_cast<int>(dvDrift*10);
  	if (signvdrift==1)  dvDrift = -(dvDrift - 0.1);
   	t0_vdriftPhi =-dvDrift;
        t0corPhi = 0.1 * t0segn_10time_ns ;}
  	// cout << "*** Drift time_t0_seg  ="<< t0segn <<" dvDrift== "<< t0_vdriftPhi << "t0_corr ns= "<<t0corPhi << endl;
    }
    if (hasZed) t0corZed = 0.1* (*segment->zSegment()).t0();
    if (t0corZed ==0) t0corZed =-1000.;
//aaaa    cout <<"t0cor = "<<t0corPhi << " " << t0corZed <<endl;
//    float aaa=(*segment->phiSegment().theT0())
    int nptfitPhi=0;
//    if (hasPhi && ( iwheel == 2 ))   {
//     if (hasPhi)   {
    if (hasPhi )   {
     const DTChamberRecSegment2D& segPhi = *segment->phiSegment();
     LocalPoint  posPhi= segPhi.localPosition();
     LocalVector dirPhi= segPhi.localDirection();
     float slope = dirPhi.x()/dirPhi.z();
       vector<float> x;
       vector<float> y;
       vector<float> sigy;
       vector<float> resPhi;
       vector<float> layPhi;
       vector<DTRecHit1D> hits=(*segment->phiSegment()).specificRecHits();
       for (vector<DTRecHit1D>::const_iterator hit=hits.begin();
              hit!= hits.end(); ++hit) {
              nptfitPhi++;
              // I have to get the hits position (the hit is in the layer rf) in SL frame...
              GlobalPoint glbPos = ( dtGeom->layer( hit->wireId().layerId() ) )->toGlobal(hit->localPosition());
              LocalPoint  pos = ( dtGeom->idToDet((*segment->phiSegment()).geographicalId()) )->toLocal(glbPos);
          //    int layPhin=(  hit->wireId().layerId());
          //    layPhi.push_back(  hit->wireId().layerId()); 
              x.push_back(pos.z()); 
              y.push_back(pos.x());
              float zphi=pos.z();
              float xphi=pos.x();
              float rP=pos.x()-(posPhi.x()+((pos.z()-posPhi.z())*slope));
              resPhi.push_back(rP);
          //     cout << " res = " << rP << " in zphi=pos.z()= "<< pos.z() << "in xphi=pos.x()= " << xphi<< endl;
//           if ( iwheel == (iwheeltest) ){
           if ( (iwheel == iwheeltest ) && (isect == isectest)){
             if ( ( ist == 1) && ( nptfitPhi==5 ) )   hResPhiMB1_l5->Fill(rP);
             if ( ( ist == 2) && ( nptfitPhi==5 ) )  hResPhiMB2_l5->Fill(rP);
             if ( ( ist == 3) && ( nptfitPhi==5 ) )  hResPhiMB3_l5->Fill(rP);
             if ( ( ist == 4) && ( nptfitPhi==5 ) )  hResPhiMB4_l5->Fill(rP);
             if (hasZed) {
          //   cout << "glbPoint.z()= "<<glbPoint.z() << endl;
             	if (  (ist == 1 ) &&  (nptfitPhi==5  ) )  hResPhiMB1_vs_z->Fill(glbPoint.z(),rP);
            	if ( ( ist == 2 ) &&  (nptfitPhi==5  ) )  hResPhiMB2_vs_z->Fill(glbPoint.z(),rP);
             	if ( ( ist == 3 ) &&  (nptfitPhi==5  ) )  hResPhiMB3_vs_z->Fill(glbPoint.z(),rP);
               } //endif hasZed
	     } //endif wheel */

	  } //end loop RecHit
	  
//aaaa     cout << "glbPoint.z()= "<<glbPoint.z() << " nptfitPhi= "<< nptfitPhi<< endl;
        if ( (iwheel == iwheeltest ) && (isect == isectest)){
	  if ( ist == 1 && nptfitPhi>0  )  hnptPhiMB1->Fill(nptfitPhi);
   	  if ( ist == 2 && nptfitPhi>0  )  hnptPhiMB2->Fill(nptfitPhi);
   	  if ( ist == 3 && nptfitPhi>0  )  hnptPhiMB3->Fill(nptfitPhi);
   	  if ( ist == 4 && nptfitPhi>0  )  hnptPhiMB4->Fill(nptfitPhi);
	}
  
/*   cout << " # Point in the fit = " << nptfitPhi  << endl;
   cout << " x = " << x << endl;
   cout << " y = " << x << endl;
   cout << "resPhi = " << resPhi << endl;/
*/
   
  }

 float nptfitZed=0;
     if (hasZed)   {
     const DTSLRecSegment2D   segZed = *segment->zSegment();
     LocalPoint  posTh= segZed.localPosition();
     LocalVector dirTh= segZed.localDirection();
     float slope = dirTh.x()/dirTh.z();
       vector<float> x;
       vector<float> y;
       vector<float> sigy;
       vector<float> resPhi;
       vector<float> layPhi;
       vector<DTRecHit1D> hits=(*segment->zSegment()).specificRecHits();
       for (vector<DTRecHit1D>::const_iterator hit=hits.begin();
              hit!=hits.end(); ++hit) {
              nptfitZed++;
              // I have to get the hits position (the hit is in the layer rf) in SL frame...
              GlobalPoint glbPos = ( dtGeom->layer( hit->wireId().layerId() ) )->toGlobal(hit->localPosition());
              LocalPoint  pos = ( dtGeom->idToDet((*segment->zSegment()).geographicalId()) )->toLocal(glbPos);
          //    int layPhin=(  hit->wireId().layerId());
          //    layPhi.push_back(  hit->wireId().layerId()); 
              x.push_back(pos.z()); 
              y.push_back(pos.x());
              float zphi=pos.z();
              float xphi=pos.x();
              float rP=pos.x()-(posTh.x()+((pos.z()-posTh.z())*slope));
              resPhi.push_back(rP);
          //     cout << " res = " << rP << " in zphi=pos.z()= "<< pos.z() << "in xphi=pos.x()= " << xphi<< endl;
           if ( (iwheel == iwheeltest ) && (isect ==isectest)){
             if ( ( ist == 1 ) && ( nptfitZed==2 ) )  hResZedMB1_l2->Fill(rP);
             if ( ( ist == 2 ) && ( nptfitZed==2 ) )  hResZedMB2_l2->Fill(rP);
             if ( ( ist == 3 ) && ( nptfitZed==2 ) )  hResZedMB3_l2->Fill(rP);
             if (hasPhi) {
          //   cout << "glbPoint.z()= "<<glbPoint.z() << endl;
             	if ( ( ist == 1 ) && ( nptfitZed==2 )  )  hResZedMB1_vs_z->Fill(glbPoint.z(),rP);
            	if ( ( ist == 2 ) && ( nptfitZed==2 )  )  hResZedMB2_vs_z->Fill(glbPoint.z(),rP);
             	if ( ( ist == 3 ) && ( nptfitZed==2 )  )  hResZedMB3_vs_z->Fill(glbPoint.z(),rP);
               } //endif hasZed
	     } //endif wheel

	  } //end loop RecHit
	  
        if ( (iwheel == iwheeltest ) && (isect == isectest)){
   	  if ( ist == 1 && nptfitZed>0  )  hnptZedMB1->Fill(nptfitZed);
   	  if ( ist == 2 && nptfitZed>0  )  hnptZedMB2->Fill(nptfitZed);
   	  if ( ist == 3 && nptfitZed>0  )  hnptZedMB3->Fill(nptfitZed);
	}
	  
//a     cout << "glbPoint.z()= "<<glbPoint.z() << " nptfitPhi= "<< nptfitPhi<< endl;
  
/*   cout << " # Point in the fit = " << nptfitPhi  << endl;
   cout << " x = " << x << endl;
   cout << " y = " << x << endl;
   cout << "resPhi = " << resPhi << endl;/
*/
   
  }
 
//aaaa    if (hasZed)    cout <<*segment->zSegment()  <<endl;

//    hPositionX->Fill( (*segment).localPosition().x());
//    cout << segment->chamberId().wheel() << endl;  
//    cout << segment->chamberId().sector() << endl;
//    cout << segment->chamberId().station() << endl;      


//  trasforma da locale a globale

//    GlobalPoint   glbPoint = ch->toGlobal((*segment).localPosition());
//    GlobalVector  glbDir   = ch->toGlobal((*segment).localDirection());

//     cout << "xlocal " << (*segment).localPosition().x() << " ylocal " << (*segment).localPosition().y() << " zlocal " << (*segment).localPosition().z() << endl;
  if(hasZed && hasPhi) { 
//       t0_corPhi = invvproptdc*((*segment).localPosition().y());     
//       t0_corZed = invvproptdc*((*segment).localPosition().x());
      t0_wPhi = invvpropns*((*segment).localPosition().y());     
      t0_wZed = invvpropns*((*segment).localPosition().x());
//      double CHI2Phi = ((*segment->phiSegment()).chi2);     
//      double CHI2Zed = ((*segment->phiSegment()).chi2);
//      cout << "invvproptdc*((*segment).localPosition().x()) = "<<(*segment).localPosition().x()<< " t0_cor Wire prop = "<<t0_wZed <<endl;
//      cout << "invvproptdc*((*segment).localPosition().y()) = "<<(*segment).localPosition().y()<< " t0_cor Wire prop = "<<t0_wPhi <<endl;
  }
      t0_corPhi = t0corPhi-t0_wPhi;
      t0_corZed = t0corZed-t0_wZed;
//      cout << "OK roptdc*((*segment).localPosition().x()) = "<<(*segment).localPosition().x()<< " t0_corZ = "<<t0_corZed << " t0corZ = "<<t0corZed<<endl;
//      cout << "OK roptdc*((*segment).localPosition().y()) = "<<(*segment).localPosition().y()<< " t0_corP = "<<t0_corPhi <<" t0corP = "<<t0corPhi<<endl;
//    cout << "xglob " << glbPoint.x() << " yglob " << glbPoint.y() << " zglob " << glbPoint.z() << endl;
//    cout << "xdir  " << glbDir.x()   << " ydir  " << glbDir.y()   << " zdir  " << glbDir.z()   << endl;  



  float xdir = glbDir.x();
  float ydir = glbDir.y();
  float phixy = atan(xdir/ydir);
//aaaa    cout << "phixy " << 57.296*phixy << endl ;
  nsMB[ist][isect][iwheel]++;
  if ((  ist == 1 )&& (nptfitPhi > 5))  hPosXvsZ_1->Fill( glbPoint.z(), glbPoint.x());
  if ((  ist == 2 )&& (nptfitPhi > 5))  hPosXvsZ_2->Fill( glbPoint.z(), glbPoint.x());
  if ((  ist == 3 )&& (nptfitPhi > 5))  hPosXvsZ_3->Fill( glbPoint.z(), glbPoint.x());
  if ((  ist == 4 )&& (nptfitPhi > 5))  hPosXvsZ_4->Fill( glbPoint.z(), glbPoint.x());

  if ( ist == 1 )  T0Zedvst_PosX_1->Fill( t0corZed, t0_wPhi); 
  if ( ist == 2 )  T0Zedvst_PosX_2->Fill( t0corZed, t0_wPhi); 
  if ( ist == 3 )  T0Zedvst_PosX_3->Fill( t0corZed, t0_wPhi); 

  if ( ist == 1 )    T0Phivst_PosZ_1 ->Fill(t0corPhi ,t0_wZed); 
  if ( ist == 2 )    T0Phivst_PosZ_2 ->Fill(t0corPhi ,t0_wZed ); 
  if ( ist == 3 )    T0Phivst_PosZ_3 ->Fill(t0corPhi ,t0_wZed ); 
  
    
//  if ( (*segment).localPosition().y() != 0 &&
//       (*segment).localPosition().x() != 0   ) {      // true 4D segments in MB1,2,3...
  if ( (*segment).localPosition().x() != 0 ) {   // r-phi segments in MB1,2,3...
   if ( ist == 1 && nsMB1 < 199 ) {                 // MB1 --------
     nsMB1++;
     flag1[nsMB1] = 100*iwheel+ segment->chamberId().sector();
     xMB1[nsMB1] = glbPoint.x(); // store segments positions 
     yMB1[nsMB1] = glbPoint.y();
     zMB1[nsMB1] = glbPoint.z();
     xslMB1[nsMB1] = glbDir.x(); // store segments slopes
     yslMB1[nsMB1] = glbDir.y();
     zslMB1[nsMB1] = glbDir.z();
     tPhiMB1[nsMB1] = t0corPhi;
     tZedMB1[nsMB1] = t0corZed;
     tcPhiMB1[nsMB1] = t0_corPhi;
     tcZedMB1[nsMB1] = t0_corZed;
      hPosXMB1->Fill( xMB1[nsMB1]);
      hPosZMB1->Fill( zMB1[nsMB1]);
      float phiMB1 = xslMB1[nsMB1] / yslMB1[nsMB1]; 
      float thMB1  = zslMB1[nsMB1] / yslMB1[nsMB1]; 
      hPhiMB1->Fill( phiMB1);
      hThMB1->Fill( thMB1);

      if(hasPhi&&hasZed) {
        htPhiMB1->Fill(t0corPhi );
        hto_vdriftMB1->Fill(t0_vdriftPhi*100);
        htThMB1->Fill(t0corZed);
        hctPhiMB1->Fill(t0_corPhi );
        hctThMB1->Fill(t0_corZed);

      }
    } // endif MB1 ---------- 
    if ( ist == 2 && nsMB2 < 199)                   { // MB2 -----------
     nsMB2++;
     flag2[nsMB2] = 100*iwheel+ segment->chamberId().sector();
     xMB2[nsMB2] = glbPoint.x(); // store segments positions 
     yMB2[nsMB2] = glbPoint.y();
     zMB2[nsMB2] = glbPoint.z();
     xslMB2[nsMB2] = glbDir.x(); // store segments slopes
     yslMB2[nsMB2] = glbDir.y();
     zslMB2[nsMB2] = glbDir.z();
     tPhiMB2[nsMB2] = t0corPhi;
     tZedMB2[nsMB2] = t0corZed;
     tcPhiMB2[nsMB2] = t0_corPhi;
     tcZedMB2[nsMB2] = t0_corZed;
//      hPosXMB2->Fill( xMB2[nsMB2]);
//      hPosZMB2->Fill( zMB2[nsMB2]);
      float phiMB2 = xslMB2[nsMB2] / yslMB2[nsMB2]; 
      float thMB2  = zslMB2[nsMB2] / yslMB2[nsMB2]; 
      hPhiMB2->Fill( phiMB2);
      hThMB2->Fill( thMB2);

      if(hasPhi&&hasZed) {
        htPhiMB2->Fill(t0corPhi );
        hto_vdriftMB2->Fill(t0_vdriftPhi*100);
        htThMB2->Fill(t0corZed);
        hctPhiMB2->Fill(t0_corPhi );
        hctThMB2->Fill(t0_corZed);
      }
    } // endif MB2 ---------
    if ( ist == 3 && nsMB3 < 199)                 { // MB3 ---------
     nsMB3++;
     flag3[nsMB3] = 100*iwheel+ segment->chamberId().sector();
     xMB3[nsMB3] = glbPoint.x(); // store segments positions 
     yMB3[nsMB3] = glbPoint.y();
     zMB3[nsMB3] = glbPoint.z();
     xslMB3[nsMB3] = glbDir.x(); // store segments slopes
     yslMB3[nsMB3] = glbDir.y();
     zslMB3[nsMB3] = glbDir.z();
     tPhiMB3[nsMB3] = t0corPhi;
     tZedMB3[nsMB3] = t0corZed;
     tcPhiMB3[nsMB3] = t0_corPhi;
     tcZedMB3[nsMB3] = t0_corZed;
//      hPosXMB3->Fill( xMB3[nsMB3]);
//      hPosZMB3->Fill( zMB3[nsMB3]);
      float phiMB3 = xslMB3[nsMB3] / yslMB3[nsMB3]; 
      float thMB3  = zslMB3[nsMB3] / yslMB3[nsMB3]; 
      hPhiMB3->Fill( phiMB3);
      hThMB3->Fill( thMB3);

      if(hasPhi&&hasZed) {
        htPhiMB3->Fill(t0corPhi );
        hto_vdriftMB3->Fill(t0_vdriftPhi*100);
        htThMB3->Fill(t0corZed);
        hctPhiMB3->Fill(t0_corPhi );
        hctThMB3->Fill(t0_vdriftPhi*100);
      }
    } // endif MB3 ----- 
   } // endif 4D segment
 if ( ist == 4 && nsMB4 < 199)                 { // MB4 ----------
     nsMB4++;
     flag4[nsMB4] = 100*iwheel+ segment->chamberId().sector();
     xMB4[nsMB4] = glbPoint.x(); // store segments positions 
     yMB4[nsMB4] = glbPoint.y();
     zMB4[nsMB4] = glbPoint.z();
     xslMB4[nsMB4] = glbDir.x(); // store segments slopes
     yslMB4[nsMB4] = glbDir.y();
     zslMB4[nsMB4] = glbDir.z();
     tPhiMB4[nsMB4] = t0corPhi;
     tcPhiMB4[nsMB4] = t0_corPhi;
      hPosXMB4->Fill( xMB4[nsMB4]);
      float phiMB4 = xslMB4[nsMB4] / yslMB4[nsMB4]; 
      hPhiMB4->Fill( phiMB4);

      if(hasPhi)htPhiMB4->Fill(t0corPhi );
      if(hasPhi) hto_vdriftMB4->Fill(t0_vdriftPhi*100);

   }  // endif MB4
 
  } // next segment ----------------------------------------------
  
  
// fill histos with final track statistics...
  for ( int iwheel = 1; iwheel !=3; iwheel++)     {   // wheels
   stringstream string_wheel; string_wheel << iwheel;
   for (int isect = 1; isect != 3; isect++ ) { // sectors
    stringstream string_sect; string_sect << isect+9;	
    for (int ist = 1; ist != 5; ist++){   // loop on stations
      stringstream string_st; string_st << ist;	
      string histoname = "hnsMB" + string_st.str() + "_s" + string_sect.str() + "_w" + string_wheel.str();  //  track multiplicity
      histo(histoname.c_str())->Fill( nsMB[ist][isect][iwheel]);
    } // next station
   } // next station 
  } // next wheel   
 
    
 // analysis in sector 10, wheel 2 ================================
// geometry: global coordinate of chamber plane in sect. 10 in which segments are given:: ===========
//  MB1: yglob -431.175 
//  MB2: yglob -512.475 
//  MB3: yglob -617.875 
//  MB4: yglob -720.2
// =================================================================================================
    
   float dphi12=0., dphi23=0., dphi34=0.;
   float delx12=0., delx23=0., delx34=0.;
   float delx2=0. , delx3=0. , delx4=0.;
   float phiMB1=0., phiMB2=0., phiMB3=0., phiMB4=0.;
    
// MB1->MB2 extrapolations & MB2 efficiency--------------------------
  float dist = 81.30;                // radial distance between MB1 and MB2
  float d12 = dist;
  float ux = 0.,  uy = -1.    , uz = 0.;  // versor normal to chamber plane in sector 10:
//  float ux = 0.5, uy = -0.8660, uz = 0.;  // versor normal to chamber plane in sector 11:
// find the two closest candidates in angle...
    float psmax = -1.;
    int jbest1=0, jbest2=0;
    for  (int j1 = 1; j1 < nsMB1+1; ++j1){
          for  (int j2 = 1; j2 < nsMB2+1; ++j2){
	    float ps = xslMB2[j2]*xslMB1[j1] + yslMB2[j2]*yslMB1[j1] + zslMB2[j2]*zslMB1[j1];  
	    if (ps > psmax) {
	     psmax = ps;
	     jbest2 = j2;
	     jbest1 = j1;
	    }
          } // next track in MB2
     } // next track in MB1
  if( jbest1 > 0 && jbest2 > 0 ) { // tracks found in MB1 & MB2
    if( flag1[jbest1] == 211 ) ux = 0.5, uy = -0.8660, uz = 0.;   
 // angle between track direction and normal versor in X-Y plane
    float coth = ux*xslMB2[jbest1] + uy*yslMB2[jbest1] + uz*zslMB2[jbest1];
    float ds = dist/coth ; //  path length between the 2 chambers
    float xextr = xMB1[jbest1] + ds*xslMB1[jbest1];  
    float yextr = yMB1[jbest1] + ds*yslMB1[jbest1];
    float zextr = zMB1[jbest1] + ds*zslMB1[jbest1];
    delx12 = (xMB2[jbest2]-xextr)/ uy ;
    delx2  =  xMB2[jbest2]-xMB1[jbest1] ;
    float delz12 =  zMB2[jbest2]-zextr ;
    phiMB1 = xslMB1[jbest1] / yslMB1[jbest1]; 
    phiMB2 = xslMB2[jbest2] / yslMB2[jbest2];
    float thMB1 = zslMB1[jbest1] / yslMB1[jbest1]; 
    float thMB2 = zslMB2[jbest2] / yslMB2[jbest2];
    float t0phMB1 = tPhiMB1[jbest1];
    float t0phMB2 = tPhiMB2[jbest2];
    dphi12 = phiMB2-phiMB1 ;
    
    if( flag1[jbest1] == 110 && flag2[jbest2] == 110 ) { // in wheel/sector 1/10
      hDphi12_110->Fill( phiMB2-phiMB1);
      hDth12_110->Fill( thMB2-thMB1); 
      hDelx12_110->Fill(delx12);   
      hDelz12_110->Fill(delz12);
      hDelT012_110->Fill(t0phMB1 , t0phMB2);
      if((abs(phiMB1)<=.02)&& (abs(thMB1)<=.02)) hDelT0cut12_110->Fill(t0phMB1 , t0phMB2);
    } 
    if( flag1[jbest1] == 210 && flag2[jbest2] == 210 ) { // in wheel/sector 2/10
      hDphi12_210->Fill( phiMB2-phiMB1);
      hDth12_210->Fill( thMB2-thMB1);    
      hDelx12_210->Fill(delx12);   
      hDelz12_210->Fill(delz12);
    } 
    if( flag1[jbest1] == 211 && flag2[jbest2] == 211 ) { // in wheel/sector 2/11
      hDphi12_211->Fill( phiMB2-phiMB1);
      hDth12_211->Fill( thMB2-thMB1);    
      hDelx12_211->Fill(delx12);   
      hDelz12_211->Fill(delz12);
    } 
    if( flag1[jbest1] == 110 && flag2[jbest2] == 211 ) { // in wheel/sector 1/10 -> 2/11
      hDphi12_110_211->Fill( phiMB2-phiMB1);
      hDth12_110_211->Fill( thMB2-thMB1);    
      hDelx12_110_211->Fill(delx12);   
      hDelz12_110_211->Fill(delz12);
    } 
 }  // endif MB1-> MB2 extrapolation ---------------------   
	int jbest3=0;
  if ( (jbest1!=0) && (jbest2!=0) && (jbest3!=0) ) {
    float dFlight12 = sqrt( pow(xMB2[jbest2] - xMB1[jbest1],2)+
                            pow(yMB2[jbest2] - yMB1[jbest1],2)+
   			    pow(zMB2[jbest2] - zMB1[jbest1],2));
    float dFlight23 = sqrt( pow(xMB3[1] - xMB2[jbest2],2)+
                            pow(yMB3[1] - yMB2[jbest2],2)+
 			    pow(zMB3[1] - zMB2[jbest2],2));
 			   
//aaaa      cout << "all T0Phi: " << tPhiMB1[jbest1] << " " << tPhiMB2[jbest2] << " " << tPhiMB3[1] << endl;
//aaaa      cout << "all T0Zed: " << tZedMB1[jbest1] << " " << tZedMB2[jbest2] << " " << tZedMB3[1] << endl;
    float t0PhiDiff12 = tPhiMB2[jbest2] - tPhiMB1[jbest1];
    float t0PhiDiff23 = tPhiMB3[1] - tPhiMB2[jbest2];
    float t0ZedDiff12 = tZedMB2[jbest2] - tZedMB1[jbest1];
    float t0ZedDiff23 = tZedMB3[1] - tZedMB2[jbest2];
     t0PhiDiff12 = tcPhiMB2[jbest2] - tcPhiMB1[jbest1];
     t0PhiDiff23 = tcPhiMB3[1] - tcPhiMB2[jbest2];
     t0ZedDiff12 = tcZedMB2[jbest2] - tcZedMB1[jbest1];
     t0ZedDiff23 = tcZedMB3[1] - tcZedMB2[jbest2];
      cout << "dist12: " << dFlight12 << " t0PhiDiff12= " << t0PhiDiff12 << "t0ZedDiff12=  " << t0ZedDiff12 << endl;
      cout << "dist23: " << dFlight23 << " " << t0PhiDiff23 << " " << t0ZedDiff23 << endl;
      cout << "dist23/dist12: " << dFlight23/dFlight12 << " " << t0PhiDiff23/t0PhiDiff12 << " " << t0ZedDiff23/t0ZedDiff12 << endl;
  }

// MB2->MB3 extrapolations & MB3 efficiency--------------------------
  dist = 105.40;                // radial distance between MB1 and MB2
  float d23 = dist;
  ux = 0.,  uy = -1.    , uz = 0.;  // versor normal to chamber plane in sector 10:
//  float ux = 0.5, uy = -0.8660, uz = 0.;  // versor normal to chamber plane in sector 11:
// find the two closest candidates in angle...
    psmax = -1.;
    jbest1=0, jbest2=0;
    for  (int j1 = 1; j1 < nsMB2+1; ++j1){
          for  (int j2 = 1; j2 < nsMB3+1; ++j2){
	    float ps = xslMB3[j2]*xslMB2[j1] + yslMB3[j2]*yslMB2[j1] + zslMB3[j2]*zslMB2[j1];  
	    if (ps > psmax) {
	     psmax = ps;
	     jbest2 = j2;
	     jbest1 = j1;
	    }
          } // next track in MB2
     } // next track in MB1
  if( jbest1 > 0 && jbest2 > 0 ) { // tracks found in MB1 & MB2
    if( flag2[jbest1] == 211 ) ux = 0.5, uy = -0.8660, uz = 0.;   
 // angle between track direction and normal versor in X-Y plane
    float coth = ux*xslMB2[jbest1] + uy*yslMB2[jbest1] + uz*zslMB2[jbest1];
    float ds = dist/coth ; //  path length between the 2 chambers
    float xextr = xMB2[jbest1] + ds*xslMB2[jbest1];  
    float yextr = yMB2[jbest1] + ds*yslMB2[jbest1];
    float zextr = zMB2[jbest1] + ds*zslMB2[jbest1];
    delx23 = (xMB3[jbest2]-xextr)/ uy ;
    delx3  =  xMB3[jbest2]-xMB2[jbest1] ;
    float delz12 =  zMB3[jbest2]-zextr ;
    phiMB2 = xslMB2[jbest1] / yslMB2[jbest1]; 
    phiMB3 = xslMB3[jbest2] / yslMB3[jbest2];
    float thMB1 = zslMB2[jbest1] / yslMB2[jbest1]; 
    float thMB2 = zslMB3[jbest2] / yslMB3[jbest2];
    dphi23 = phiMB2-phiMB1 ;
    if( flag2[jbest1] == 110 && flag3[jbest2] == 110 ) { // in wheel/sector 1/10
      hDphi23_110->Fill( phiMB3-phiMB2);
      hDth23_110->Fill( thMB2-thMB1); 
      hDelx23_110->Fill(delx12);   
      hDelz23_110->Fill(delz12);
    } 
    if( flag2[jbest1] == 210 && flag3[jbest2] == 210 ) { // in wheel/sector 2/10
      hDphi23_210->Fill( phiMB3-phiMB2);
      hDth23_210->Fill( thMB2-thMB1);    
      hDelx23_210->Fill(delx12);   
      hDelz23_210->Fill(delz12);
    } 
    if( flag2[jbest1] == 211 && flag3[jbest2] == 211 ) { // in wheel/sector 2/11
      hDphi23_211->Fill( phiMB3-phiMB2);
      hDth23_211->Fill( thMB2-thMB1);    
      hDelx23_211->Fill(delx12);   
      hDelz23_211->Fill(delz12);
    } 
    if( flag2[jbest1] == 110 && flag3[jbest2] == 211 ) { // in wheel/sector 1/10 -> 2/11
      hDphi23_110_211->Fill( phiMB3-phiMB2);
      hDth23_110_211->Fill( thMB2-thMB1);    
      hDelx23_110_211->Fill(delx12);   
      hDelz23_110_211->Fill(delz12);
    } 
 }  // endif MB2-> MB3 extrapolation ---------------------   


// MB3->MB4 extrapolations & MB3 efficiency--------------------------
  dist = 105.40;                // radial distance between MB1 and MB2
  float d34 = dist;
  ux = 0.,  uy = -1.    , uz = 0.;  // versor normal to chamber plane in sector 10:
//  float ux = 0.5, uy = -0.8660, uz = 0.;  // versor normal to chamber plane in sector 11:
// find the two closest candidates in angle...
    psmax = -1.;
    jbest1=0, jbest2=0;
    for  (int j1 = 1; j1 < nsMB3+1; ++j1){
          for  (int j2 = 1; j2 < nsMB4+1; ++j2){
	    float ps = xslMB4[j2]*xslMB3[j1] + yslMB4[j2]*yslMB3[j1] + zslMB4[j2]*zslMB3[j1];  
	    if (ps > psmax) {
	     psmax = ps;
	     jbest2 = j2;
	     jbest1 = j1;
	    }
          } // next track in MB4
     } // next track in MB3
  if( jbest1 > 0 && jbest2 > 0 ) { // tracks found in MB1 & MB2
    if( flag3[jbest1] == 211 ) ux = 0.5, uy = -0.8660, uz = 0.;   
 // angle between track direction and normal versor in X-Y plane
    float coth = ux*xslMB3[jbest1] + uy*yslMB3[jbest1] + uz*zslMB3[jbest1];
    float ds = dist/coth ; //  path length between the 2 chambers
    float xextr = xMB3[jbest1] + ds*xslMB3[jbest1];  
    float yextr = yMB3[jbest1] + ds*yslMB3[jbest1];
    float zextr = zMB3[jbest1] + ds*zslMB3[jbest1];
    delx34 = (xMB4[jbest2]-xextr)/ uy ;
    delx4  =  xMB4[jbest2]-xMB3[jbest1] ;
    phiMB3 = xslMB3[jbest1] / yslMB3[jbest1]; 
    phiMB4 = xslMB4[jbest2] / yslMB4[jbest2];
    dphi34 = phiMB4-phiMB3 ;
     if( flag3[jbest1] == 110 && flag4[jbest2] == 110 ) { // in wheel/sector 1/10
      hDphi34_110->Fill( phiMB4-phiMB3); 
      hDelx34_110->Fill(delx12);   
    } 
    if( flag3[jbest1] == 210 && flag4[jbest2] == 210 ) { // in wheel/sector 2/10
      hDphi34_210->Fill( phiMB4-phiMB3); 
      hDelx34_210->Fill(delx12);   
    } 
    if( flag3[jbest1] == 211 && flag4[jbest2] == 211 ) { // in wheel/sector 2/11
      hDphi34_211->Fill( phiMB4-phiMB3);   
      hDelx34_211->Fill(delx12);   
    } 
    if( flag3[jbest1] == 110 && flag4[jbest2] == 211 ) { // in wheel/sector 1/10 -> 2/11
      hDphi34_110_211->Fill( phiMB4-phiMB3);  
      hDelx34_110_211->Fill(delx12);   
    } 
 }  // endif MB3-> MB4 extrapolation ---------------------   


   float B = 1.2; // mag. field (T)
   float pt12 =0., pt23 = 0., pt34 = 0.;
   if(dphi12 !=0. && dphi23 != 0.) {
       hdphi_23vs12->Fill(dphi23,dphi12);
       hdelx_23vs12->Fill(delx23,delx12);
       float phi1 = atan(phiMB1);
       float r12 = 0.5*(delx2*delx2 + d12*d12) / (delx2*cos(phi1)-d12*sin(phi1)) ;
       pt12 = 0.003*B*r12;  // B (T), r (cm), pt (GeV)
       float phi2 = atan(phiMB2);
       float r23 = 0.5*(delx3*delx3 + d23*d23) / (delx3*cos(phi2)-d23*sin(phi2)) ;
       pt23 = 0.003*B*r23;
       hpt12->Fill(pt12);
       hpt23->Fill(pt23);
       hdpt1->Fill(pt12-pt23);
   }    
   if(dphi34 !=0. && dphi23 != 0.) {
       hdphi_34vs23->Fill(dphi34,dphi23);
       hdelx_34vs23->Fill(delx34,delx23);
       float phi2 = atan(phiMB2);
       float r23 = 0.5*(delx3*delx3 + d23*d23) / (delx3*cos(phi2)-d23*sin(phi2)) ;
       pt23 = 0.003*B*r23;
       float phi3 = atan(phiMB3);
       float r34 = 0.5*(delx4*delx4 + d34*d34) / (delx4*cos(phi3)-d34*sin(phi3)) ;
       pt34 = 0.003*B*r34;
       hpt34->Fill(pt34);
       hdpt2->Fill(pt23-pt34);
   }
//aaaa      cout <<  " pt12 " << pt12 << " pt23 " << pt23 << " pt34 " << pt34 << endl;
    
/*
// MB2 efficiency analysis------------------------------
  if ( nsMB[1] >  0 && nsMB[3] >  0 ) {    // find the two closest candidates in angle in MB1 & MB3...
    float dphimin = 9999.;
    int jbest1=0, jbest2=0;
    for  (int j1 = 1; j1 < nsMB[1]+1; ++j1){
         for  (int j2 = 1; j2 < nsMB[3]+1; ++j2){ 
             float phiMB1 = xslMB1[j1] / yslMB1[j1]; 
             float phiMB3 = xslMB3[j2] / yslMB3[j2];
	     float dphi = abs(phiMB1-phiMB3);
	    if (dphi < dphimin) {
	     dphimin = dphi;
	     jbest2 = j2;
	     jbest1 = j1;
	    }
         }
    }
    hDalpha13->Fill(dphimin);
    if ( dphimin < 0.1) {                                   // good muon in MB1 & MB3
 // angle between track direction and normal versor in X-Y plane
    float coth = ux*xslMB1[jbest1] + uy*yslMB1[jbest1] + uz*zslMB1[jbest1];
    float ds = dist/coth ; //  path length between the 2 chambers
    float xextr = xMB1[jbest1] + ds*xslMB1[jbest1];  
    float yextr = yMB1[jbest1] + ds*yslMB1[jbest1];
    float zextr = zMB1[jbest1] + ds*zslMB1[jbest1];
    if( zextr > 430. && zextr < 640.) hPredXMB2->Fill( xextr);
    hPredZMB2->Fill( zextr);
    float adelxmin = 9999.;
    int jmin = 0;
    for  (int j2 = 1; j2 < nsMB[2]+1; ++j2){       // loop on MB2 candidates
      float delx12 = (xMB2[j2]-xextr)/ uy ;
      float dely12 =  yMB2[j2]-yextr ;
      float delz12 =  zMB2[j2]-zextr ;
      float adelx = abs(delx12);
      if (adelx < adelxmin ) {
       adelxmin = adelx;
       jmin = j2;
      }
     } // next MB2 candidate
     if  (jmin > 0 ) {  
      hDelX12->Fill( (xMB2[jmin]-xextr)/ uy );
      hDelY12->Fill( yMB2[jmin]-yextr);
      hDelZ12->Fill( zMB2[jmin]-zextr);
       if ( adelxmin < 5. ) {
         if( zextr > 430. && zextr < 640.) hPosXMB2->Fill( xextr);
         hPosZMB2->Fill( zextr);
       }	 
     }
    } // endif good muon in MB1 & MB3
  } // endif MB2 efficiency ------------------------------------------------------------


// MB2->MB3 extrapolations & MB3 efficiency--------------------------
  dist = 105.40;                // radial distance between MB2 and MB3
  ux = 0., uy = -1., uz = 0.;  // versor normal to chamber plane in sector 10:
  if ( nsMB[2] >  0 && nsMB[3] >  0 ) {    
// find the two closest candidates in angle...
    float psmax = -1.;
    int jbest1=0, jbest2=0;
    for  (int j1 = 1; j1 < nsMB[2]+1; ++j1){
          for  (int j2 = 1; j2 < nsMB[3]+1; ++j2){
	    float ps = xslMB3[j2]*xslMB2[j1] + yslMB3[j2]*yslMB2[j1] + zslMB3[j2]*zslMB2[j1];  
	    if (ps > psmax) {
	     psmax = ps;
	     jbest2 = j2;
	     jbest1 = j1;
	    }
          }
    } 
 // angle between track direction and normal versor in X-Y plane
  float coth = ux*xslMB2[jbest1] + uy*yslMB2[jbest1] + uz*zslMB2[jbest1];
  float ds = dist/coth ; //  path length between the 2 chambers
  float xextr = xMB2[jbest1] + ds*xslMB2[jbest1];  
  float yextr = yMB2[jbest1] + ds*yslMB2[jbest1];
  float zextr = zMB2[jbest1] + ds*zslMB2[jbest1];
  float delx23 = (xMB3[jbest2]-xextr)/ uy ;
  float dely23 =  yMB3[jbest2]-yextr ;
  float delz23 =  zMB3[jbest2]-zextr ;
   float phiMB2 = xslMB2[jbest1] / yslMB2[jbest1]; 
   float phiMB3 = xslMB3[jbest2] / yslMB3[jbest2];
   float thMB2 = zslMB2[jbest1] / yslMB2[jbest1]; 
   float thMB3 = zslMB3[jbest2] / yslMB3[jbest2];
   hDphi23->Fill( phiMB3-phiMB2);
   hDth23->Fill( thMB3-thMB2);    
 } // endif MB2-> MB3 extrapolation ---------------------   
// MB3 efficiency analysis------------------------------
  if ( nsMB[2] >  0 && nsMB[4] >  0 ) {    // find the two closest candidates in angle in MB2 & MB4...
    float dphimin = 99999.;
    int jbest1=0, jbest2=0;
    for  (int j1 = 1; j1 < nsMB[2]+1; ++j1){
         for  (int j2 = 1; j2 < nsMB[4]+1; ++j2){
            float phiMB2 = xslMB2[j1] / yslMB2[j1]; 
            float phiMB4 = xslMB4[j2] / yslMB4[j2];
	    float dphi = abs(phiMB2-phiMB4);
	    if (dphi < dphimin) {
	     dphimin = dphi;
	     jbest2 = j2;
	     jbest1 = j1;
	    }
         }
    }
    hDalpha24->Fill(dphimin);
    if ( dphimin < 0.1) {                                   // good muon in MB2 & MB4
 // angle between track direction and normal versor in X-Y plane
    float coth = ux*xslMB2[jbest1] + uy*yslMB2[jbest1] + uz*zslMB2[jbest1];
    float ds = dist/coth ; //  path length between the 2 chambers
    float xextr = xMB2[jbest1] + ds*xslMB2[jbest1];  
    float yextr = yMB2[jbest1] + ds*yslMB2[jbest1];
    float zextr = zMB2[jbest1] + ds*zslMB2[jbest1];
    if( zextr > 430. && zextr < 640.) hPredXMB3->Fill( xextr);
    hPredZMB3->Fill( zextr);
    float adelxmin = 9999.;
    int jmin = 0;
    for  (int j2 = 1; j2 < nsMB[3]+1; ++j2){       // loop on MB3 candidates
      float delx23 = (xMB3[j2]-xextr)/ uy ;
      float dely23 =  yMB3[j2]-yextr ;
      float delz23 =  zMB3[j2]-zextr ;
      float adelx = abs(delx23);
      if (adelx < adelxmin ) {
       adelxmin = adelx;
       jmin = j2;
      }
     } // next MB3 candidate
     if  (jmin > 0 ) {  
      hDelX23->Fill( (xMB3[jmin]-xextr)/ uy );
      hDelY23->Fill( yMB3[jmin]-yextr);
      hDelZ23->Fill( zMB3[jmin]-zextr);
       if ( adelxmin < 5. ) {
         if( zextr > 430. && zextr < 640.) hPosXMB3->Fill( xextr);
         hPosZMB3->Fill( zextr);
       }	 
     }
    } // endif good muon in MB2 & MB4
  } // endif MB3 efficiency ------------------------------------------------------------

    
  
// MB3->MB4 extrapolations ----------------------------------------
  if ( nsMB[4] < 10 && nsMB[3] < 10 ) {
  if ( nsMB[4] >  0 && nsMB[3] >  0 ) {
    
// find the two closest candidates in phi angle...
    float dphimin = 99999.;
    int jbest4=0, jbest3=0;
    for  (int j3 = 1; j3 < nsMB[3]+1; ++j3){
          float phiMB3 = xslMB3[j3] / yslMB3[j3];
          for  (int j4 = 1; j4 < nsMB[4]+1; ++j4){  
            float phiMB4 = xslMB4[j4] / yslMB4[j4]; 
            float dphi = abs(phiMB4 - phiMB3);
//	    cout << " phi3 " << phiMB3 << " phi4 " << phiMB4 << " dphi " << dphi << endl;
	    if (dphi < dphimin) {
	     dphimin = dphi;
	     jbest3 = j3;
	     jbest4 = j4;
	    }
          }
    }
    float phiMB3 = xslMB3[jbest3] / yslMB3[jbest3]; 
    float phiMB4 = xslMB4[jbest4] / yslMB4[jbest4]; 
    hDphi34->Fill(phiMB4 - phiMB3);
    float yMB4_10 = -720.2 ;
    float xextr = xMB3[jbest3] + phiMB3*(yMB4_10 - yMB3[jbest3]);
    float delx34 = xMB4[jbest4]-xextr;
    hDelX34->Fill( delx34);
//    cout << " phi3 " << phiMB3 << " phi4 " << phiMB4 << " dphi " << dphimin << endl;
//    cout << " x3 " << xMB3[jbest3] << " y3 " << yMB3[jbest3]  << " z3 " << zMB3[jbest3]  << endl;
//    cout << " x4 " << xMB4[jbest4] << " y4 " << yMB4[jbest4]  << " z4 " << zMB4[jbest4]  << endl;
//    cout << " xextr " << xextr << endl;

     } // endif nsMB4, nsMB3 > 0
  } // end  nsMB3-> nsMB4 ectrapolation---------------------------

*/
    
   
 
//  cout<<"0 " <<event.id().event()<<" #Event: ====================== end event ======================= "<<endl;


}

DEFINE_FWK_MODULE(DTRecSegment4DReader)
