#ifndef __FFF_NEYMAN__
#define __FFF_NEYMAN__

namespace fff {

class LikelihoodSpace;

/** Abstract base for interval-computing functors. */
class NeymanInterval {
  public:
    NeymanInterval(LikelihoodSpace* _likelihood_space)
        : likelihood_space(_likelihood_space) {}
    virtual ~NeymanInterval() {}

    virtual std::pair<double, double> operator()(float cl, std::string param) = 0;

  protected:
    LikelihoodSpace* likelihood_space;
};

}  // namespace fff

#endif  // __FFF_NEYMAN__

