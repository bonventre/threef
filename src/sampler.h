#ifndef __FFF_SAMPLER__
#define __FFF_SAMPLER__

#include <vector>

namespace fff {

/** A PDF sampler. */
class Sampler {
  public:
    Sampler() {}
    virtual ~Sampler() {}

    virtual std::vector<double> propose_jump(const std::vector<double>& coordinates) const = 0;
    virtual bool accept(const double current, const double proposed) const = 0;
};

}  // namespace fff

#endif  // __FFF_SAMPLER__

