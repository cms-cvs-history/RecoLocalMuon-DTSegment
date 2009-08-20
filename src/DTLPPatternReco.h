#ifndef DTSegment_DTLPPatternReco_h
#define DTSegment_DTLPPatternReco_h

/** \class DTLPPatternReco
 *
 * Algo for reconstructing 2d segment in DT using a linear programming approach
 *  
 * $Date: 2009/08/14 14:15:52 $
 * $Revision: 1.2 $
 * \author Enzo Busseti - SNS Pisa <enzo.busseti@sns.it>
 * 
 */

// Base Class Headers 
#include "RecoLocalMuon/DTSegment/src/DTRecSegment2DBaseAlgo.h"


//CMSSW Headers
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/DTRecHit/interface/DTChamberRecSegment2D.h"

/*Collaborating Class Headers*/
#include "RecoLocalMuon/DTSegment/src/DTSegmentUpdator.h"

//#include "DataFormats/GeometryVector/interface/LocalPoint.h"
//#include <DataFormats/GeometrySurface/interface/LocalError.h>

/* Collaborating Class Declarations */
namespace edm {
  class ParameterSet;
  class EventSetup;
}


/* Class DTLPPatternReco Interface */

class DTLPPatternReco : public DTRecSegment2DBaseAlgo {

public:

    // Constructor
  DTLPPatternReco(const edm::ParameterSet& pset) ;

    // Destructor
  virtual ~DTLPPatternReco() ;

    /* Operations */

    /// this function is called in the producer
  virtual edm::OwnVector<DTSLRecSegment2D>
    reconstruct(const DTSuperLayer* sl,
                  const std::vector<DTRecHit1DPair>& hits);

    /// return the algo name
  virtual std::string algoName() const { return "DTLPPatternReco"; }

  /// Through this function the EventSetup is percolated to the
    /// objs which request it
  virtual void setES(const edm::EventSetup& setup);

  edm::OwnVector<DTChamberRecSegment2D>
  reconstructSupersegment(const std::vector<DTRecHit1DPair>& pairs);

 class ResultLPAlgo{
  public:
    ResultLPAlgo(){ chi2Var =0;};
    ~ResultLPAlgo(){lambdas.clear();};
    double mVar;
    double qVar;
      double chi2Var;
    std::vector<int> lambdas;  
  };

 
private:

  enum ReconstructInSLOrChamber { ReconstructInSL, ReconstructInChamber };


  void * reconstructSegmentOrSupersegment( const std::vector<DTRecHit1DPair>& pairs,
					ReconstructInSLOrChamber & sl_chamber);

  void populateCoordinatesLists(std::list<double>& pz,
				std::list<double>& px,
				std::list<double>& pex,
				const DTSuperLayer* sl,
				const DTChamber* chamber,
				const std::vector<const DTRecHit1DPair*>& pairPointers,
				const ReconstructInSLOrChamber  sl_chamber);


  void removeUsedHits(const ResultLPAlgo& theAlgoResults,
		      std::vector<const DTRecHit1DPair*>& pairPointersx);

  edm::ESHandle<DTGeometry> theDTGeometry; // the DT geometry
  double theDeltaFactor, theMinimumM, theMaximumM, theMinimumQ, theMaximumQ, theBigM, theMaxAlphaTheta, theMaxAlphaPhi;
  int event_counter;
  DTSegmentUpdator * theUpdator;//the updator which updates the segments once they're reconstructed

 void  printGnuplot(edm::OwnVector<DTSLRecSegment2D> * theResults, const std::vector<DTRecHit1DPair>& pairs);
};

#endif // DTSegment_DTLPPatternReco_h
