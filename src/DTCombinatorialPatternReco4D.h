#ifndef DTSegment_DTCombinatorialPatternReco4D_h
#define DTSegment_DTCombinatorialPatternReco4D_h

/** \class DTCombinatorialPatternReco4D
 *
 * Algo for reconstructing 4d segment in DT using a combinatorial approach
 *  
 * $Date:  $
 * $Revision: $
 * \author Stefano Lacaprara - INFN Legnaro <stefano.lacaprara@pd.infn.it>
 * \author Riccardo Bellan - INFN TO <riccardo.bellan@cern.ch>
 *
 */

// Base Class Headers
#include "RecoLocalMuon/DTSegment/src/DTRecSegment4DBaseAlgo.h"

class DTRecSegment2DBaseAlgo;

// Collaborating Class Declarations
namespace edm {
  class ParameterSet;
  class EventSetup;
}
class DTSegmentUpdator;
//class DTSegmentCleaner;

// C++ Headers
#include <vector>
//#include <utility>

#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "FWCore/Framework/interface/ESHandle.h"

// ====================================================================== 

// Class DTCombinatorialPatternReco4D Interface 

class DTCombinatorialPatternReco4D : public DTRecSegment4DBaseAlgo {

 public:

  /// Constructor
  DTCombinatorialPatternReco4D(const edm::ParameterSet& pset) ;
  
  /// Destructor
  virtual ~DTCombinatorialPatternReco4D(){};
    
  /// Operations  
  virtual edm::OwnVector<DTRecSegment4D>
    reconstruct(const DTChamber* chamber,
		const std::vector<DTRecSegment2D>& segments2DPhi1,
		const std::vector<DTRecSegment2D>& segments2DTheta,
		const std::vector<DTRecSegment2D>& segments2DPhi2);
    
  virtual std::string algoName() const { return theAlgoName; }
    
  virtual void setES(const edm::EventSetup& setup);
 protected:

 private:
  std::string theAlgoName;

  bool debug;
  // DTSegmentUpdator* theUpdator; // the updator and fitter
  // DTSegmentCleaner* theCleaner; // the cleaner
    
  edm::ESHandle<DTGeometry> theDTGeometry; // the DT geometry

  // The reconstruction 2D algorithm
  DTRecSegment2DBaseAlgo* the2DAlgo;

  // the updator
  DTSegmentUpdator *theUpdator;

};
#endif
