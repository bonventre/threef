#ifndef __FFF_NEYMAN__
#define __FFF_NEYMAN__

namespace fff {

class LikelihoodSpace;

/** Abstract base for interval-computing functors. */
class NeymanInterval {
  public:
    NeymanInterval(float _cl, LikelihoodSpace* _likelihood_space)
        : cl(_cl), likelihood_space(_likelihood_space) {}
    virtual ~NeymanInterval() {}

    virtual std::pair<double, double> operator()(std::string param) = 0;

  protected:
    float cl;  //!< Confidence level
    LikelihoodSpace* likelihood_space;
};

}  // namespace fff

#endif  // __FFF_NEYMAN__

