//
//  AstroSoundUtilities.hpp
//  soniastro
//
//  Created by Ahmad Aussaili on 3/5/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#ifdef __cplusplus

#ifndef AstroSoundUtilities_hpp
#define AstroSoundUtilities_hpp

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <vector>

#define REAL 0
#define IMAG 0

using namespace cv;
using namespace std;

vector<double> reverb(vector<double> sound, int bufferSize, float delayinMilliSeconds, int decayFactor, int mixFactor);

void computeFFT(int bufferSize, double *in, fftw_complex *out);

vector<double> getMagnitudes(fftw_complex *fftOutputArray, int numOfSamples);

#endif /* AstroSoundUtilities_hpp */

#endif
