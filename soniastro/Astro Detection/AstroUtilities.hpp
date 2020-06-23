//
//  AstroUtilities.hpp
//  Astro Detector
//
//  Created by Ahmad Aussaili.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#ifdef __cplusplus


#ifndef AstroUtilities_hpp
#define AstroUtilities_hpp

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <map>

using namespace cv;
using namespace std;

// Compute the median value of the intensities of a given image (1-channel)
// (Uses the cumulative distibution function of the pixel intensity histogram)
double medianMatrix(Mat image, int numOfIntensityValues);

// (Overload) Compute the median value of the intensities of a given image (1-channel)
// (Uses a sorted vector approach)
double medianMatrix(Mat image);

// Checks whether a matrix contains values outside the given range
bool containsValuesOutsideRange(Mat image, double minValue, double maxValue);

// Computes the square root of each element in the matrix
Mat sqrtMatrix(Mat image);

// Returns a vector containing the unique elements in a matrix
vector<int> getUniqueElements(Mat matrix);

// Returns a vector containing the unique elements in a vector
vector<int> getUniqueElements(vector<int> vec);

// Estimates the background of an astronomical image
int estimateBackground(Mat image, float scalingFactor);

// Computes a new image which only retains the intensity between the given
// minimum value and maximum value of the original image.
// NOTE: the input image should have only one channel
Mat clipIntensity(Mat image, double minValue, double maxValue);

// Removes noise around large objects.
// Returns a modified binary image version of the given binary image.
Mat cleanArtifacts(Mat image, Mat binaryImage, int localAreaExpansion, int minDetectionObjPixelSize);

// Draws bounding boxes around the detected objects in the original image
Mat drawBoundingBoxes(Mat orgImage, Mat binaryImage);

// Returns a score as the result of template matching a given image with galaxy templates
// Template images should be named as: template1, template2, ...
// and should have the .png format.
// NOTE: do not include a '/' at the end of the templates directory name.
double galaxyTemplateMatching(Mat image, string templatesDirectory, int numOfTemplates);

vector<double> normalizeVector(vector<double> arr, double maxNormalizationValue);

void checkAndResizeImage(Mat inputImage, Mat &outputImage, int maxCols);

#endif


#endif // cpp
