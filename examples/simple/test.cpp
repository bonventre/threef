#include <iostream>
#include <vector>
#include <string>
#include <TMath.h>
#include <Minuit2/FCNBase.h>
#include <Minuit2/MnUserParameters.h>
#include <Minuit2/MnMigrad.h>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MnMinos.h>
#include <fff/fitter.h>
#include <fff/likelihood.h>
#include <fff/neyman.h>
#include <fff/sampler.h>
#include <fff/samplers/metropolis.h>
#include <fff/intervals/bayesian/central.h>

class MyData : public fff::Dataset {
  public:
    MyData(double _truth) : truth(_truth) {}
    virtual ~MyData() {}

    double truth;
};


double myNLL(const std::vector<double>& params, fff::Dataset* data) {
  MyData* mydata = dynamic_cast<MyData*>(data);
  
  if (params[0] < 0 || params[0] > TMath::Power(mydata->truth, 2)) {
    return 1e9;
  }

  return -1.0 * TMath::Log(1 - TMath::Power((params[0] - mydata->truth) /
                                             mydata->truth, 2));
}

fff::Dataset* myMC(const std::vector<double>& params) {
  return new MyData(params[0]);
}

int main(int argc, char* argv[]) {
  std::vector<double> initial_params(1, 5.0);
  std::vector<std::string> param_names(1, "a");

  fff::FitSimple fit(param_names, &myNLL, &myMC);
  fff::Dataset* data = fit.generate_data(initial_params);
  std::cout << "The correct answer is 5.0, with a likelihood of 0.0"
            << std::endl << std::endl;

  // Fit with MIGRAD
  {
    fff::Fitter::BestFit* results = fit.migrad(initial_params, data);
    std::cout << "migrad: " << results->parameters[0]
              << ", nll = " << results->value << std::endl;
    delete results;
  }

  // Use the Minuit2 wrapper to get MINOS errors
  ROOT::Minuit2::FCNBase* migradfunc = fit.get_minuit2_fcn(data);
  std::vector<double> errors(initial_params.size(), 0);
  ROOT::Minuit2::MnUserParameters mn_params(initial_params, errors);
  ROOT::Minuit2::MnMigrad migrad(*migradfunc, mn_params);  
  ROOT::Minuit2::FunctionMinimum minimum = migrad(); //FIXME maxfcn, tol
  ROOT::Minuit2::MnUserParameters migradresult = minimum.UserParameters();
  ROOT::Minuit2::MnMinos minos(*migradfunc, minimum);
  std::pair<double, double> minos_errors = minos(0);

  std::cout << "Minos: " << migradresult.Params()[0]
            << " -" << -1 * minos_errors.first
            << " +" << minos_errors.second
            << std::endl << std::endl;

  // Fit with a Markov Chain Monte Carlo, using the Metropolis algorithm
  {
    std::vector<double> jump_sigma(1, 5);
    fff::samplers::Metropolis metropolis(jump_sigma);
    fff::LikelihoodSpace* lspace;
    fff::Fitter::BestFit* results = \
      fit.markov(initial_params, data, metropolis, 50000, 0.1, &lspace);
    std::cout << "markov: " << results->parameters[0]
              << ", nll = " << results->value << std::endl;

    // Calculate some intervals
    fff::intervals::bayesian::Central central(lspace);
    std::pair<double, double> interval = central(0.9,"a");
    std::cout << "Bayesian central: " << results->parameters[0]
              << " -" << results->parameters[0] - interval.first
              << " +" << interval.second - results->parameters[0]
              << std::endl;
  }

  return 0;
}

