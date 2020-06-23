//
//  CombinedDetection.hpp
//  soniastro
//
//  Created by Ahmad Aussaili on 3/5/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#ifdef __cplusplus

#ifndef CombinedDetection_hpp
#define CombinedDetection_hpp

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <map>

#include "GlobalDetection.hpp"
#include "LocalDetection.hpp"

using namespace cv;
using namespace std;

class CombinedDetection {
    
    // Global and local detectors
    GlobalDetection globalDetector;
    LocalDetection localDetector;
    
    // This should be set when creating a Global Detector object.
    string templatesPath = ""; // for galaxyTemplateMatching
    
    // Detected image statistics
    Mat originalImage;
    Mat binaryImage;
    Mat labeledImage; // each pixel is given a label (0 for background)
    Mat stats; // [label] [0: x of bbox | 1: top left y of bbox | 2: width of bbox | 3: height of bbox | 4: area] , type: CV_32S (signed int)
    Mat centroids; // [label] [0: x center, 1: y center] , type: CV_64F (float)
    
    // Logging
    string logIdentifier = "LOGGER: "; // change this
    void logEvent(string event);
    
public:
    
    // Constructor: returns a Combined Detection object
    CombinedDetection();
    
    // Logging
    string getLogIdentifier();
    void setLogIdentifier(string identifier);
    
    // Statistics
    
    bool statsWereSet = false;
    
    Mat getOriginalImage();
    Mat getBinaryImage();
    Mat getLabeledImage();
    Mat getStats();
    Mat getCentroids();
    
    // Initialise the variables holding information about the detected objects
    bool setDetectedObjectsStatistics();
    
    // Template mathing
    void setTemplatesPath(string path);
    
    // Detects cosmic objects
    // Returns a binary image, where the background is black (0) and the objects are white (max intensity)
    Mat detect(Mat image, bool localDetection, bool deblending);
    
    // Parameter setters
    
    void setGlobalDetectionKernelSize(int value);
    
    void setGlobalDetectionStdDevSigma(float value);
    
    void setGlobalDetectionLocalAreaExpansion(int value);
    
    void setGlobalDetectionMinCleaningObjSize(int value);
    
    void setGlobalDetectionCleaningIterations(int value);
    
    void setGlobalDetectionBGSubScalingFactor(float value);
    
    void setLocalDetectionKernelSize(int value);
    
    void setLocalDetectionStdDevSigma(float value);
    
    void setLocalDetectionLocalAreaExpansion(int value);
    
    void setLocalDetectionMinCleaningObjSize(int value);
    
    void setLocalDetectionCleaningIterations(int value);
    
    void setLocalDetectionBGSubScalingFactor(float value);
    
    void setLocalDetectionNumOfGlobalObjects(int value);
    
    void setLocalDetectionSigmoidSlope(int value);
    
    void setLocalDetectionSigmoidCenterParam(int value);
    
    void setLocalDetectionLayeredDetectionThreshold(int value);
};

#endif /* CombinedDetection_hpp */

#endif
