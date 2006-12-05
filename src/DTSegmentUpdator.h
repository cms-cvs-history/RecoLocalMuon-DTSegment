#ifndef DTSegment_DTSegmentUpdator_h
#define DTSegment_DTSegmentUpdator_h

/** \class DTSegmentUpdator
 *
 * Perform linear fit and hits update for DT segments.
 * Update a segment by improving the hits thanks to the refined knowledge of
 * impact angle and position (also along the wire) and perform linear fit on
 * improved hits.
 *
 * $Date: 2006/11/20 18:56:22 $
 * $Revision: 1.3.2.1 $
 * \author Stefano Lacaprara - INFN Legnaro <stefano.lacaprara@pd.infn.it>
 * \author Riccardo Bellan - INFN TO <riccardo.bellan@cern.ch>
 *
 */

/* Base Class Headers */

/* Collaborating Class Declarations */

class DTSegmentCand;
class DTRecSegment2D;
class DTRecSegment4D;
class DTLinearFit;
class DTRecHitBaseAlgo;

namespace edm{class EventSetup; class ParameterSet;}

/* C++ Headers */
#include <vector>
#include "Geometry/Vector/interface/GlobalPoint.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CommonDetAlgo/interface/AlgebraicObjects.h"
/* ====================================================================== */

/* Class DTSegmentUpdator Interface */

class DTSegmentUpdator{

  public:

/// Constructor
    DTSegmentUpdator(const edm::ParameterSet& config) ;

/// Destructor
    ~DTSegmentUpdator() ;

/* Operations */ 

    /** do the linear fit on the hits of the segment candidate and update it.
     * Returns false if the segment candidate is not good() */
    bool fit(DTSegmentCand* seg);

    /** ditto for true segment: since the fit is applied on a true segment, by
     * definition the segment is "good", while it's not the case for just
     * candidates */
    void fit(DTRecSegment2D* seg);

    /** ditto for true segment 4D, the fit is done on either projection and then
     * the 4D direction and position is built. Since the fit is applied on a
     * true segment, by definition the segment is "good", while it's not the
     * case for just candidates */
    void fit(DTRecSegment4D* seg);

    /// recompute hits position and refit the segment4D
    void update(DTRecSegment4D* seg);

    /// recompute hits position and refit the segment2D
    void update(DTRecSegment2D* seg);

    /// set the setup
    void setES(const edm::EventSetup& setup);

  protected:

  private:
    DTLinearFit* theFitter; // the linear fitter
    DTRecHitBaseAlgo* theAlgo; // the algo for hit reconstruction
    edm::ESHandle<DTGeometry> theGeom; // the geometry

  private:

    void updateHits(DTRecSegment2D* seg);

    void updateHits(DTRecSegment2D* seg,
                    GlobalPoint &gpos,
                    GlobalVector &gdir,
                    int step=2);

    /// interface to LinearFit
    void fit(const std::vector<float>& x,
             const std::vector<float>& y, 
             const std::vector<float>& sigy,
             LocalPoint& pos,
             LocalVector& dir,
             AlgebraicSymMatrix& covMat,
             double& chi2);

   bool fitT0_seg(DTSegmentCand* seg);
   
   void fitT0_seg(DTRecSegment2D* seg,float& t0_cor );
   
   void fitT0_seg(DTRecSegment4D* seg);
	     
    /// interface to LinearFit
    void fitT0(
            const std::vector<float>& xfit,
            const std::vector<float>& yfit,
            const std::vector<float>& sigy,
            const std::vector<int>& lfit,
            int& nptfit,
            int& nppar,
            LocalPoint& pos,
            LocalVector& dir,
           float& aminf,
           float& bminf,
           float& cminf,
           double& chi2fit);
	     
	   
 
    void Fit4Var(
            const std::vector<float>& xfit,
            const std::vector<float>& yfit,
            const std::vector<float>& sigy,
            const std::vector<int>& lfit,
	    const std::vector<double>& tfit,
            int& nptfit,
            int& nppar,
           float& aminf,
           float& bminf,
           float& cminf,
	   double& vminf,
       	   double& chi2fit,
	   bool debug);

  bool T0_seg;
  bool vdrift_4parfit;
  bool T0_seg_debug;

};
#endif // DTSegment_DTSegmentUpdator_h
