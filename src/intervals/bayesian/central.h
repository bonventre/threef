#ifndef __FFF_INTERVALS_BAYESIAN_CENTRAL__
#define __FFF_INTERVALS_BAYESIAN_CENTRAL__

#include <utility>
#include <string>
#include <fff/neyman.h>

namespace fff {
  class LikelihoodSpace;
  namespace intervals {
    namespace bayesian {

/** Project the space onto the dimension of interest and integrate. */
class Central : public NeymanInterval {
  public:
    Central(float _cl, LikelihoodSpace* _likelihood_space)
        : NeymanInterval(_cl, _likelihood_space) {}
    virtual ~Central() {}

    virtual std::pair<double, double> operator()(std::string param);
};

    }  // namespace bayesian
  }  // namespace intervals
}  // namespace fff

#endif  // __FFF_INTERVALS_BAYESIAN_CENTRAL__

