/** \file
 *
 * $Date: 2006/03/30 16:53:18 $
 * $Revision: 1.1 $
 * \author Stefano Lacaprara - INFN Legnaro <stefano.lacaprara@pd.infn.it>
 */

/* This Class Header */
#include "RecoLocalMuon/DTSegment/test/DTRecSegmentReader.h"

/* Collaborating Class Header */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/DTRecHit/interface/DTRecSegment2DCollection.h"
#include "DataFormats/MuonDetId/interface/DTSuperLayerId.h"

#include "TFile.h"

/* C++ Headers */
#include <string>

/* ====================================================================== */

/// Constructor
DTRecSegmentReader::DTRecSegmentReader(const edm::ParameterSet& pset) {
  // Get the debug parameter for verbose output
  debug = pset.getUntrackedParameter<bool>("debug");
  theRootFileName = pset.getUntrackedParameter<string>("rootFileName");

  if(debug)
    cout << "[DTRecHitReader] Constructor called" << endl;

  // Create the root file
  theFile = new TFile(theRootFileName.c_str(), "RECREATE");
  theFile->cd();
}

/// Destructor
DTRecSegmentReader::~DTRecSegmentReader() {
  if(debug) 
    cout << "[DTRecHitReader] Destructor called" << endl;

  // Write the histos to file
  theFile->cd();
  theFile->Close();
}

/* Operations */ 
void DTRecSegmentReader::analyze(const edm::Event & event, const
                                 edm::EventSetup& eventSetup) {
  cout << "--- [DTRecSegmentReader] Event analysed #Run: " << event.id().run()
       << " #Event: " << event.id().event() << endl;

  // Get the rechit collection from the event
  edm::Handle<DTRecSegment2DCollection> coll;
  event.getByLabel("recseg2dbuilder", coll);


  // RB, TODO:
//   for (DTRecSegment2DCollection::const_iterator det=coll->begin() ;
//        // RB, was
//   //  for (DTRecSegment2DCollection::DigiRangeIterator det=coll->begin() ;
//        det!=coll->end() ; ++det) {
//     //const DTSuperLayerId& slID = (*detUnitIt).first;
//     const DTRecSegment2DCollection::range& segs = (*det).second;
//     // Loop over the rechits of this DetUnit
//     for (DTRecSegment2DCollection::const_iterator seg = segs.first;
// 	 seg!=segs.second; ++seg){
//       cout << *seg << endl;
//     }
//   }


}

DEFINE_FWK_MODULE(DTRecSegmentReader)
