#include <assert.h>
#include <TH1D.h>
#include <TMath.h>
#include <iostream>
#include <vector>
#include <TRandom.h>
#include <nll.h>

FFF::DataWrapper* NLL::GenerateData(const std::vector<double>& params) const
{  
  MyData *data = new MyData();
  for (size_t i=0; i<params.size(); i++) {
    float rate;
    if (fConstraints[i] > 0) {
      rate = gRandom->Gaus(params[i], fConstraints[i]);
    }
    else {
      rate = params[i];
    }
    int n = gRandom->Poisson(rate);
    for (int j=0; j<n; j++) {
      data->events.push_back(fPDFs[i]->GetRandom());
    }
  }
  return data;
}


double NLL::GetNLL(const std::vector<double>& params, const FFF::DataWrapper* data) const
{
    const MyData *mydata = dynamic_cast<const MyData *> (data);

    double result = 0;
    for (size_t i=0; i<params.size(); i++) {
        // hard prior
        if (params[i] < 0) {
            return 1e20;
        }

        result += params[i];

        // gaussian constraints
        if (fConstraints[i] > 0) {
            result += 0.5 * TMath::Power((params[i]-fExpectations[i]) /
                                          fConstraints[i], 2);
        }
    }

    // L -= sum over events ( log ( sum over pdfs ( N_j P_j(x_i)) ) )
    for (int i=0; i<mydata->events.size(); i++) {
        double s = 0;
        for (size_t j=0; j<params.size(); j++) {
            s += params[j] * fPDFs[j]->Interpolate(mydata->events[i]);
        }
        if (s != s) {
            s = 1e-20;
            assert(false);
        }
        result -= TMath::Log(s);
    }

    return result;
}

