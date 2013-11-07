#ifndef __FFF_MIGRAD__
#define __FFF_MIGRAD__

#include <vector>
#include <Minuit2/FCNBase.h>

namespace fff {

class Fitter;
class Dataset;

class MigradFCN : public ROOT::Minuit2::FCNBase {
  public:
    MigradFCN(Fitter* _fitter, Dataset* _data)
        : fitter(_fitter), data(_data) {}

    double operator()(const std::vector<double>& params) const;

    double Up() const { return 0.5; }

  protected:
    Fitter* fitter;
    Dataset* data;
};

}  // namespace fff

#endif  // __FFF_MIGRAD__

