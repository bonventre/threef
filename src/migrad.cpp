#include <vector>
#include <fff/fitter.h>
#include <fff/migrad.h>

namespace fff {

double MigradFCN::operator()(const std::vector<double>& params) const {
  return this->fitter->nll(params, this->data);
};

}  // namespace fff

