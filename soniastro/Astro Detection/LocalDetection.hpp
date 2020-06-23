//
//  LocalDetection.hpp
//  soniastro
//
//  Created by Ahmad Aussaili on 3/26/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#ifdef __cplusplus

#ifndef LocalDetection_hpp
#define LocalDetection_hpp

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

class LocalDetection {
    
    int numberOfGlobalObjects = 30;
    int sigmoidSlope = 20; // For grayscale stretching
    int sigmoidCenterParameter = 1; // For grayscale stretching
    // The following defines the object size that needs to be processed by the layered detection scheme
    int layerdDetectionObjectSizeThreshold = 3000;
    float noiseFilterWindowSize1 = 0.08; // Experimental
    float noiseFilterWindowSize2 = 0.05; // Experimental
    
    unsigned int kernelSize = 7; // Gaussian filter kernel size: should be odd and positive
    double standardDeviationSigma = 1.5; // Used for gaussian filer and possibly other filters
    float backgroundSubtractionScalingFactor = 1.6;
    
    // Artifact cleaning:
    int numOfCleaningIterations = 1;
    int localAreaExpansion = 30;
    int minDetectionObjPixelSize = 10;
    
    // The original and detection image
    Mat originalImage;
    Mat binaryImage;
    
    // This should be set when creating a Global Detector object.
    string templatesPath = ""; // for galaxyTemplateMatching
    
    // Logging
    string logIdentifier = "LOGGER: "; // change this
    void logEvent(string event);
    
public:
    
    // Constructor: returns a Global Detector object
    LocalDetection();
    
    string getLogIdentifier();
    
    // Returns a binary image containing only large objects to be used as seedpoints for the watershed algorithm
    Mat getSeedPoints(Mat globalObjects, int numberOfGlobalObjects);
    
    Mat getOriginalImage();
    Mat getBinaryImage();
    
    // Template matching
    void setTemplatesPath(string path);
    
    // Detects cosmic objects
    // Returns a binary image, where the background is black (0) and the objects are white (max intensity)
    Mat detect(Mat originalImage, Mat seedpointImage, bool deblending);
    
    // Paramter setters
    
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

#endif /* LocalDetection_hpp */

#endif
