//
//  Sonifier.hpp
//  soniastro
//
//  Created by Ahmad Aussaili on 3/5/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#ifdef __cplusplus

#ifndef Sonifier_hpp
#define Sonifier_hpp

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <vector>

#include "CombinedDetection.hpp"
#include "AstroSoundUtilities.hpp"
#include "AstroUtilities.hpp"
#include "AudioFile.h"

using namespace cv;
using namespace std;

class Sonifier {
    
    CombinedDetection *detector = NULL;
    
    // Logger
    string logIdentifier = "LOGGER: ";
    void logEvent(string event);
    
    // SOUND PREFERENCES
    
    float samplingFrequency = 44100.f; // Keep it even.
    int binSize = 5; // represents a frame in pixels.
    int FPS = 25; // Frames per second.
    int binSamples = samplingFrequency / FPS;
    int maximumSoundFreq = 1000;
    int minimumSoundFreq = 25;
    float objectSoundDurationFactor = 0.03; // As this gets bigger, the sound reduces in length.
    bool universalAmplitude = true;
    float amplitudeFactor = 30; // This is multiplied with the objects area. (only available when universal amplitude is off)
    float attenuationFactor = 0.0; // Should be between 0 and 1
    
    // Reverb:
    float delayinMilliSeconds = 500;
    float decayFactor = 0.9;
    int mixFactor = 70;
    int reverbIterations = 1;
    
    // Store the frequencies at each frame for future video creation
    Mat frequencies;
    
public:
    
    Sonifier(); // If you use this constructor, you should set the detector before calling any function.
    Sonifier(CombinedDetection &detector);
    
    // Produces a sound sample array from the detected image
    vector<double> sonify();
    
    void saveAudio(vector<double> sound, int durationInSeconds, string path);
    
    void createAndSaveVideo(vector<double> &sound, int durationInSeconds, string path);
    
    // Get the duration (in seconds) of the produced sound
    int getDurationInSeconds(vector<double> sound);
    
    // Sonifier property setters
    
    void setDetector(CombinedDetection &detector);
    
    void setObjectSoundDurationFactor(float value);
    void setMaxSoundFrequency(int freq);
    
    void setSamplingFrequency(float value);
    void setFPS(int value);
    void setbinSize(int value);
    void setUniversalAmplitude(bool value);
    void setAmplitudeFactor(float value);
    void setAttenuationFactor(float value);
    
    void setDelayInMilliSeconds(float value);
    void setDecayFactor(float value);
    void setMixFactor(int value);
    void setReverbIterations(int value);
};

#endif /* Sonifier_hpp */

#endif
