//
//  GlobalDetection.cpp
//  Astro Detector
//
//  Created by Ahmad Aussaili.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#include "GlobalDetection.hpp"

GlobalDetection::GlobalDetection () {
}

void GlobalDetection::setTemplatesPath(string path) {
    this->templatesPath = path;
}

void GlobalDetection::logEvent(string event) {
    cout << logIdentifier << event;
}

string GlobalDetection::getLogIdentifier() {
    return this->logIdentifier;
}

void GlobalDetection::setLogIdentifier(string identifier) {
    this->logIdentifier = identifier;
}

Mat GlobalDetection::detect(Mat image, bool deblending) {
    
    ostringstream eventMsgStream;
    logEvent("Detection started \n");
    
    // Original image
    Mat orgImage = image.clone();
    
    // Detection image
    Mat dImage = image.clone();
    
    // Gaussian filter
    logEvent("Applying Gaussian filering... \n");
    GaussianBlur(dImage, dImage, Size(kernelSize, kernelSize), standardDeviationSigma);
    
    // Gray scaling
    logEvent("Applying gray scaling... \n");
    cvtColor(dImage, dImage, COLOR_BGR2GRAY);
    
    // Background subtraction
    // EXPERIMENTAL: Mat sqrtImage = sqrtMatrix(dImage);
    //dImage = sqrtMatrix(dImage);
    logEvent("Applying background estimation and subtraction... \n");
    int background = estimateBackground(dImage, backgroundSubtractionScalingFactor);
    dImage = dImage - background;
    
    // Histogram Equalization: it improves the contrast in the image, in order to stretch out the intensity range.
    logEvent("Applying histogram equalization... \n");
    equalizeHist(dImage, dImage);
    
    // Convert image to binary by applying Otsu's threshold method
    logEvent("Applying image thresholding using Otsu's method... \n");
    threshold(dImage, dImage, 0, 255, THRESH_BINARY | THRESH_OTSU);
    
    // Artifact cleaning around large objects
    eventMsgStream << "Cleaning artifacts around large objects (iterations: " << numOfCleaningIterations<< ")... \n";
    logEvent(eventMsgStream.str());
    for (int i = 0; i < numOfCleaningIterations; i++) {
        dImage = cleanArtifacts(orgImage, dImage, localAreaExpansion, minDetectionObjPixelSize);
    }
    
    // Deblending (Optional)
    logEvent("Deblending local objects... \n");
    if (deblending == true) {
        Deblender deblender = Deblender();
        deblender.setTemplatesPath(this->templatesPath);
        dImage = deblender.deblendImage(orgImage, dImage, kernelSize, standardDeviationSigma, Deblender::DeblendingMethod::DistanceTransform);
    }
    
    //imwrite("/Users/ahmadaussaili/Desktop/deblend.jpg", dImage);
    
    this->binaryImage = dImage.clone();
    this->originalImage = image.clone();
    
    logEvent("Detection done. \n");
    
    return dImage;
}

bool GlobalDetection::setDetectedObjectsStatistics() {
    
    if (this->binaryImage.empty()) {
        cout << "OpenCV: no binary image found for statistics setting";
        return false;
    }
    
    connectedComponentsWithStats(this->binaryImage, this->labeledImage, this->stats, this->centroids, 8);
    
    this->statsWereSet = true;
    
    return true;
}

Mat GlobalDetection::getOriginalImage() {
    return  this->originalImage;
}

Mat GlobalDetection::getBinaryImage() {
    return  this->binaryImage;
}

Mat GlobalDetection::getLabeledImage() {
    return  this->labeledImage;
}

Mat GlobalDetection::getStats() {
    return  this->stats;
}

Mat GlobalDetection::getCentroids() {
    return  this->centroids;
}

void GlobalDetection::setGlobalDetectionKernelSize(int value) {
    this->kernelSize = value;
}

void GlobalDetection::setGlobalDetectionStdDevSigma(float value) {
    this->standardDeviationSigma = value;
}

void GlobalDetection::setGlobalDetectionLocalAreaExpansion(int value) {
    this->localAreaExpansion = value;
}

void GlobalDetection::setGlobalDetectionMinCleaningObjSize(int value) {
    this->minDetectionObjPixelSize = value;
}

void GlobalDetection::setGlobalDetectionCleaningIterations(int value) {
    this->numOfCleaningIterations = value;
}

void GlobalDetection::setGlobalDetectionBGSubScalingFactor(float value) {
    this->backgroundSubtractionScalingFactor = value;
}
