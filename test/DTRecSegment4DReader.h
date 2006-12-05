#ifndef DTSegment_DTRecSegment4DReader_h
#define DTSegment_DTRecSegment4DReader_h

/** \class DTRecSegment4DReader
 *
 * Description:
 *  
 * detailed description
 *
 * $Date: 2006/05/04 12:19:17 $
 * $Revision: 1.1 $
 * \author Riccardo Bellan - INFN TO <riccardo.bellan@cern.ch>
 *
 */

/* Base Class Headers */
#include "FWCore/Framework/interface/EDAnalyzer.h"
namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

/* Collaborating Class Declarations */
#include "FWCore/Framework/interface/Handle.h"
class TFile;
class TH1F;
class TH2F;

/* C++ Headers */
#include <iostream>

/* ====================================================================== */

/* Class DTRecSegment4DReader Interface */

class DTRecSegment4DReader : public edm::EDAnalyzer {

  public:

/// Constructor
    DTRecSegment4DReader(const edm::ParameterSet& pset) ;

/// Destructor
    virtual ~DTRecSegment4DReader() ;

/* Operations */ 
    void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

  protected:

  private:
    bool debug;
    std::string theRootFileName;
    TFile* theFile;
    //static std::string theAlgoName;
    std::string theRecHits4DLabel;
    std::string theRecHits1DLabel;
    std::string theSTAMuonLabel;
    TH1F*  histo(const char* name) const;



  TH2F *hDelT012_110;
  TH2F *hDelT0cut12_110;
 
  TH2F *T0Zedvst_PosX_1; 
  TH2F *T0Zedvst_PosX_2;  
  TH2F *T0Zedvst_PosX_3;  
  TH2F *T0Phivst_PosZ_1; 
  TH2F *T0Phivst_PosZ_2; 
  TH2F *T0Phivst_PosZ_3;

   TH1F *hLTCbits;
   TH1F *hNsegDT;
   TH1F *hNsegCSC;
   TH1F *hNsegRPC;

   TH2F *hPosXvsZ_1;  
   TH2F *hPosXvsZ_2;     
   TH2F *hPosXvsZ_3; 
   TH2F *hPosXvsZ_4; 
   TH1F *hPosXMB1;
   TH1F *hPosZMB1;
   TH1F *hPhiMB1;
   TH1F *hThMB1;
   TH1F *htPhiMB1;
   TH1F *hto_vdriftMB1;
   TH1F *htThMB1;
   TH1F *hctPhiMB1;
   TH1F *hctThMB1;
 
    TH1F *hPosXMB2;
   TH1F *hPosZMB2;
   TH1F *hPredXMB2;
   TH1F *hPredZMB2;
   TH1F *hPhiMB2;
   TH1F *hThMB2;
   TH1F *htPhiMB2;
   TH1F *hto_vdriftMB2;
   TH1F *htThMB2;
   TH1F *hctPhiMB2;
   TH1F *hctThMB2;

   TH1F *hPosXMB3;
   TH1F *hPosZMB3;
   TH1F *hPredXMB3;
   TH1F *hPredZMB3;
   TH1F *hPhiMB3;
   TH1F *hThMB3;
   TH1F *htPhiMB3;
   TH1F *hto_vdriftMB3;
   TH1F *htThMB3;
   TH1F *hctPhiMB3;
   TH1F *hctThMB3;

   TH1F *hPosXMB4;
   TH1F *hPhiMB4;      
   TH1F *htPhiMB4;
   TH1F *hto_vdriftMB4;
   TH1F *hctPhiMB4;

   TH1F* hResZedMB1_l2;
   TH1F* hResZedMB2_l2;
   TH1F* hResZedMB3_l2;
   TH1F* hResZedMB4_l2;
   TH1F* hResPhiMB2_l5;
   TH1F* hResPhiMB3_l5;
   TH1F* hResPhiMB4_l5;
   
   TH1F* hnptZedMB1;
   TH2F* hResZedMB1_vs_z;
   TH1F* hnptZedMB2;
   TH2F* hResZedMB2_vs_z;
   TH1F* hnptZedMB3;
   TH2F* hResZedMB3_vs_z;
   TH1F* hnptZedMB4;

   TH1F* hnptPhiMB1;
   TH2F* hResPhiMB1_vs_z;
   TH1F* hnptPhiMB2;
   TH2F* hResPhiMB2_vs_z;
   TH1F* hnptPhiMB3;
   TH2F* hResPhiMB3_vs_z;
   TH1F* hnptPhiMB4;

   TH1F* hResPhiMB1_l1;
   TH1F* hResPhiMB1_l2;
   TH1F* hResPhiMB1_l3;
   TH1F* hResPhiMB1_l4;
   TH1F* hResPhiMB1_l5;
   TH1F* hResPhiMB1_l6;
   TH1F* hResPhiMB1_l7;
   TH1F* hResPhiMB1_l8;
   TH1F* hResZedMB1_l3;
   TH1F* hResZedMB1_l4;

   TH1F *hDelx12_110;
   TH1F *hDelz12_110;
   TH1F *hDelx12_210;
   TH1F *hDelz12_210;
   TH1F *hDelx12_211;
   TH1F *hDelz12_211;
   TH1F *hDelx12_110_211;
   TH1F *hDelz12_110_211;

   TH1F *hDelx23_110;
   TH1F *hDelz23_110;
   TH1F *hDelx23_210;
   TH1F *hDelz23_210;
   TH1F *hDelx23_211;
   TH1F *hDelz23_211;
   TH1F *hDelx23_110_211;
   TH1F *hDelz23_110_211;

   TH1F *hDelx34_110;
   TH1F *hDelx34_210;
   TH1F *hDelx34_211;
   TH1F *hDelx34_110_211;

   TH1F *hDphi12_110;
   TH1F *hDphi12_210;   
   TH1F *hDphi12_211;
   TH1F *hDphi12_110_211;
   TH1F *hDth12_110;
   TH1F *hDth12_210;
   TH1F *hDth12_211;
   TH1F *hDth12_110_211;

   TH1F *hDphi23_110;
   TH1F *hDphi23_210;   
   TH1F *hDphi23_211;
   TH1F *hDphi23_110_211;
   TH1F *hDth23_110;
   TH1F *hDth23_210;
   TH1F *hDth23_211;
   TH1F *hDth23_110_211;

   TH1F *hDphi34_110;
   TH1F *hDphi34_210;   
   TH1F *hDphi34_211;
   TH1F *hDphi34_110_211;
   
   TH2F *hdphi_23vs12;
   TH2F *hdphi_34vs23;
   TH2F *hdelx_23vs12;
   TH2F *hdelx_34vs23;
   
   TH1F *hpt12;
   TH1F *hpt23;
   TH1F *hpt34;
   TH1F *hdpt1;
   TH1F *hdpt2; 

};
#endif // DTSegment_DTRecSegment4DReader_h

