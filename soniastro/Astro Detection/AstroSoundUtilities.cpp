//
//  AstroSoundUtilities.cpp
//  soniastro
//
//  Created by Ahmad Aussaili on 3/5/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#include "AstroSoundUtilities.hpp"

// Method for Comb Filter
vector<double> combFilter(vector<double> samples, int samplesLength, float delayinMilliSeconds, float decayFactor, float sampleRate)
{
    // Calculating delay in samples from the delay in Milliseconds. Calculated from number of samples per millisecond
    int delaySamples = (int) ((float)delayinMilliSeconds * (sampleRate/1000));
    
    vector<double> combFilterSamples = samples;
    
    // Applying algorithm for Comb Filter
    for (int i=0; i<samplesLength - delaySamples; i++)
    {
        combFilterSamples[i+delaySamples] += ((double)combFilterSamples[i] * decayFactor);
    }
    
    return combFilterSamples;
}

// All Pass Filter
vector<double> allPassFilter(vector<double> samples, int samplesLength, float sampleRate)
{
    int delaySamples = (int) ((float)89.27f * (sampleRate/1000)); // Number of delay samples. Calculated from number of samples per millisecond
    vector<double> allPassFilterSamples(samplesLength, 0);
    double decayFactor = 0.131f;
    
    // Applying algorithm for All Pass Filter
    for(int i=0; i<samplesLength; i++)
    {
        allPassFilterSamples[i] = samples[i];
        
        if(i - delaySamples >= 0)
            allPassFilterSamples[i] += -decayFactor * allPassFilterSamples[i-delaySamples];
        
        if(i - delaySamples >= 1)
            allPassFilterSamples[i] += decayFactor * allPassFilterSamples[i+20-delaySamples];
    }
    
    
    // This smoothes out the samples and normalizing the audio. Without implementing this, the samples overflow may cause clipping
    double value = allPassFilterSamples[0];
    double max = 0.0f;
    
    for(int i=0; i < samplesLength; i++)
    {
        if(abs(allPassFilterSamples[i]) > max)
            max = abs(allPassFilterSamples[i]);
    }
    
    for(int i=0; i< allPassFilterSamples.size(); i++)
    {
        double currentValue = allPassFilterSamples[i];
        value = ((value + (currentValue - value))/max);
        
        allPassFilterSamples[i] = value;
    }
    
    return allPassFilterSamples;
}

vector<double> reverb(vector<double> sound, int bufferSize, float delayinMilliSeconds, int decayFactor, int mixFactor) {
    
    vector<double> combFilterSamples1 = combFilter(sound, bufferSize, delayinMilliSeconds, decayFactor, 44100);
    vector<double> combFilterSamples2 = combFilter(sound, bufferSize, (delayinMilliSeconds - 11.73f), (decayFactor - 0.1313f), 44100);
    vector<double> combFilterSamples3 = combFilter(sound, bufferSize, (delayinMilliSeconds + 19.31f), (decayFactor - 0.2743f), 44100);
    vector<double> combFilterSamples4 = combFilter(sound, bufferSize, (delayinMilliSeconds - 7.97f), (decayFactor - 0.31f), 44100);
    
    // Add the 4 Comb Filters
    vector<double> outputComb(bufferSize, 0);
    for( int i = 0; i < bufferSize; i++)
    {
        outputComb[i] = ((combFilterSamples1[i] + combFilterSamples2[i] + combFilterSamples3[i] + combFilterSamples4[i])) ;
    }
    
    // dry/wet mix effect
    vector<double> mixAudio(bufferSize, 0);
    for(int i=0; i<bufferSize; i++)
        mixAudio[i] = ((100 - mixFactor) * sound[i]) + (mixFactor * outputComb[i]);
    
    
    // Pass through 2 All Pass Filters
    vector<double> allPassFilterSamples1 = allPassFilter(mixAudio, bufferSize, 44100.f);
    vector<double> allPassFilterSamples2 = allPassFilter(allPassFilterSamples1, bufferSize, 44100.f);
    
    return allPassFilterSamples2;
}

void computeFFT(int bufferSize, double *in, fftw_complex *out) {
    
    fftw_plan plan = fftw_plan_dft_r2c_1d(bufferSize, in, out, FFTW_ESTIMATE);
    fftw_execute(plan);
}

vector<double> getMagnitudes(fftw_complex *fftOutputArray, int numOfSamples) {
    
    vector<double> magnitude(numOfSamples/2+1, 0);
    
    magnitude[0] = sqrt(fftOutputArray[0][REAL] * fftOutputArray[0][REAL]);  /* DC component */
    
    for (int k = 1; k < (numOfSamples+1)/2; ++k)  /* (k < N/2 rounded up) */ {
        magnitude[k] = sqrt(fftOutputArray[k][REAL] * fftOutputArray[k][REAL] + fftOutputArray[k][IMAG] * fftOutputArray[k][IMAG]);
    }
    
    if (numOfSamples % 2 == 0) /* N is even */ {
        magnitude[numOfSamples/2] = fftOutputArray[numOfSamples/2][REAL] * fftOutputArray[numOfSamples/2][REAL];  /* Nyquist freq. */
    }
    
    return magnitude;
}
