//
//  GlobalDetection.hpp
//  Astro Detector
//
//  Created by Ahmad Aussaili.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#ifdef __cplusplus


#ifndef GlobalDetection_hpp
#define GlobalDetection_hpp

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <map>

#include "AstroUtilities.hpp"
#include "Deblender.hpp"

using namespace cv;
using namespace std;

class GlobalDetection {
    
    unsigned int kernelSize = 7; // Gaussian filter kernel size: should be odd and positive
    double standardDeviationSigma = 1.5; // Used for gaussian filer and possibly other filters
    double thresholdScalingFactor = 0.55; // currently experimental
    
    float backgroundSubtractionScalingFactor = 2;
    
    int numOfCleaningIterations = 7; // Artifact cleaning
    int localAreaExpansion = 30; // Parameter controlling local area expansion
    int minDetectionObjPixelSize = 10; // Minimum pixel object size for detection
    
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
    
    // Constructor: returns a Global Detector object
    GlobalDetection();
    
    // Logging
    string getLogIdentifier();
    void setLogIdentifier(string identifier);
    
    // Template matching
    void setTemplatesPath(string path);
    
    // Statistics
    
    bool statsWereSet = false;
    
    Mat getOriginalImage();
    Mat getBinaryImage();
    Mat getLabeledImage();
    Mat getStats();
    Mat getCentroids();
    
    // Initialise the variables holding information about the detected objects
    bool setDetectedObjectsStatistics();
    
    // Detects cosmic objects
    // Returns a binary image, where the background is black (0) and the objects are white (max intensity)
    Mat detect(Mat image, bool deblending);
    
    // Parameter setters
    
    void setGlobalDetectionKernelSize(int value);
    
    void setGlobalDetectionStdDevSigma(float value);
    
    void setGlobalDetectionLocalAreaExpansion(int value);
    
    void setGlobalDetectionMinCleaningObjSize(int value);
    
    void setGlobalDetectionCleaningIterations(int value);
    
    void setGlobalDetectionBGSubScalingFactor(float value);

};

#endif


#endif
