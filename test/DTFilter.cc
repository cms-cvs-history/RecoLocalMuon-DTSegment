/******* \class DTFilter *******
 *
 * Description:
 *
 * \author : Stefano Lacaprara - INFN LNL <stefano.lacaprara@pd.infn.it>
 * $date   : 22/11/2007 14:11:34 CET $
 *
 * Modification:
 *
 *********************************/

/* This Class Header */
#include "RecoLocalMuon/DTSegment/test/DTFilter.h"

/* Collaborating Class Header */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/LTCDigi/interface/LTCDigi.h"
using namespace edm;

/* C++ Headers */
#include <iostream>
using namespace std;

/* ====================================================================== */

/* Constructor */ 
DTFilter::DTFilter(const edm::ParameterSet& pset) {
  // Get the debug parameter for verbose output
  debug = pset.getUntrackedParameter<bool>("debug");

  // Use LTC
  useLTC = pset.getParameter<bool>("useLTC");
  LTC_RPC = pset.getParameter<bool>("LTC_RPC");
  LTC_DT = pset.getParameter<bool>("LTC_DT");
  LTC_CSC = pset.getParameter<bool>("LTC_CSC");
}

/* Destructor */ 
DTFilter::~DTFilter() {
}

/* Operations */ 
bool DTFilter::filter(edm::Event& event, const edm::EventSetup& setup) {

  bool result = false;
  // Trigger analysis
  if (debug) cout << "Use LTC " << useLTC << endl;
  result = (useLTC)? result | selectLTC(event): result | true;
  return result;
}

bool DTFilter::selectLTC(edm::Event& event) {
  Handle<LTCDigiCollection> ltcdigis;
  event.getByType(ltcdigis);
  //cout << "ltcdigis " << ltcdigis << endl;

  for (std::vector<LTCDigi>::const_iterator ltc= ltcdigis->begin(); ltc!=
       ltcdigis->end(); ++ltc) {
    //if (debug) cout << (*ltc) << endl;
    for (int i = 0; i < 6; i++) 
      if ((*ltc).HasTriggered(i)) {
        LTC.set(i);
      }
  }

  bool trigger=false;
  if (LTC_DT) trigger = trigger || getLTC(DT);
  if (LTC_RPC) trigger = trigger || (getLTC(RPC_W1) || getLTC(RPC_W2));
  if (LTC_CSC) trigger = trigger || getLTC(CSC);
  if (debug) cout << "LTC " << trigger << endl;
  return trigger;
}

bool DTFilter::getLTC(LTCType t) const {
  return LTC.test(t);
}

DEFINE_FWK_MODULE(DTFilter);
