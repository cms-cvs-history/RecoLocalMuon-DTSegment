#include <vector>
#include <list>

namespace DTLPPatternReco {
  class ResultLPAlgo{
  public:
    ResultLPAlgo(){ chi2Var =0;};
    ~ResultLPAlgo(){lambdas.clear();};
    double mVar;
    double qVar;
    double chi2Var;
    std::vector<int> lambdas;  
  };
}

bool lpAlgorithm(DTLPPatternReco::ResultLPAlgo& theAlgoResults,
		 const std::list<double>& pz,
		 const std::list<double>& px,
		 const std::list<double>& pex,
		 const double m_min, const double m_max,
		 const double q_min, const double q_max,
		 const double BIG_M, const double theDeltaFactor);

void printGLPReturnCode(int returnCode);

void printGLPSolutionStatus(int status);

void printLPXReturnCode(int returnCode);