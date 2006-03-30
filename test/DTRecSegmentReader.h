#ifndef DTRecSegment_DTRecSegmentReader_h
#define DTRecSegment_DTRecSegmentReader_h

/** \class DTRecSegmentReader
 *
 * Description:
 *  
 * detailed description
 *
 * $Date: 28/02/2006 16:22:52 CET $
 * $Revision: 1.0 $
 * \author Stefano Lacaprara - INFN Legnaro <stefano.lacaprara@pd.infn.it>
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

/* C++ Headers */
#include <iostream>
using namespace std;

/* ====================================================================== */

/* Class DTRecSegmentReader Interface */

class DTRecSegmentReader : public edm::EDAnalyzer {

  public:

/// Constructor
    DTRecSegmentReader(const edm::ParameterSet& pset) ;

/// Destructor
    virtual ~DTRecSegmentReader() ;

/* Operations */ 
    void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

  protected:

  private:
    bool debug;
    string theRootFileName;
    TFile* theFile;

};
#endif // DTRecSegment_DTRecSegmentReader_h

