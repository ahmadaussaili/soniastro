//
//  Deblender.hpp
//  Astro Detector
//
//  Created by Ahmad Aussaili on 2/15/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#ifdef __cplusplus


#ifndef Deblender_hpp
#define Deblender_hpp

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <vector>

#include "AstroUtilities.hpp"

using namespace cv;
using namespace std;

class Deblender {
    
    // This should be set when creating a Global Detector object.
    string templatesPath = ""; // for galaxyTemplateMatching
    
public:
    
    enum DeblendingMethod {Watershed, DistanceTransform};
    
    // Constructor: returns a Deblender object
    Deblender();
    
    void setTemplatesPath(string path);
    
    // Relabels objects using a deblending method.
    // If watershed method used, returns a labeled image.
    // If distance transform is only used, returns a binary image
    Mat deblendImage(Mat orgImage, Mat binaryImage, unsigned int kernelSize, double standardDeviationSigma, DeblendingMethod deblendingMethod);
    
private:
    
    // Deblends an object using a deblending method.
    // If watershed method used, returns a labeled image.
    // If distance transform is only used, returns a binary image
    Mat deblendObject(Mat objectImg, Mat binaryObjectImg, int area, unsigned int kernelSize, double standardDeviationSigma, DeblendingMethod deblendingMethod);
};

#endif


#endif
