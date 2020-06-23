//
//  CombinedDetection.cpp
//  soniastro
//
//  Created by Ahmad Aussaili on 3/5/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#include "CombinedDetection.hpp"

CombinedDetection::CombinedDetection() {
    this->globalDetector = GlobalDetection();
    this->localDetector = LocalDetection();
}

void CombinedDetection::setTemplatesPath(string path) {
    this->templatesPath = path;
    this->globalDetector.setTemplatesPath(path);
    this->localDetector.setTemplatesPath(path);
}

void CombinedDetection::logEvent(string event) {
    cout << logIdentifier << event;
}

string CombinedDetection::getLogIdentifier() {
    return this->logIdentifier;
}

void CombinedDetection::setLogIdentifier(string identifier) {
    this->logIdentifier = identifier;
}

Mat CombinedDetection::detect(Mat image, bool localDetection, bool deblending) {

    Mat detectionImage;
    
    if (localDetection) {
        Mat globalDetectionImage = this->globalDetector.detect(image, false);
        Mat localDetectionImage = this->localDetector.detect(image, globalDetectionImage, deblending);
        
        detectionImage = localDetectionImage;
        
        this->originalImage = this->localDetector.getOriginalImage();
        this->binaryImage = this->localDetector.getBinaryImage();
        
    } else {
        Mat globalDetectionImage = this->globalDetector.detect(image, deblending);
        detectionImage = globalDetectionImage;
        
        this->originalImage = this->globalDetector.getOriginalImage();
        this->binaryImage = this->globalDetector.getBinaryImage();
    }
    
    return detectionImage;
}

bool CombinedDetection::setDetectedObjectsStatistics() {
    
    if (this->binaryImage.empty()) {
        cout << "OpenCV: no binary image found for statistics setting";
        return false;
    }
    
    connectedComponentsWithStats(this->binaryImage, this->labeledImage, this->stats, this->centroids, 8);
    
    this->statsWereSet = true;
    
    return true;
}

Mat CombinedDetection::getOriginalImage() {
    return  this->originalImage;
}

Mat CombinedDetection::getBinaryImage() {
    return  this->binaryImage;
}

Mat CombinedDetection::getLabeledImage() {
    return  this->labeledImage;
}

Mat CombinedDetection::getStats() {
    return  this->stats;
}

Mat CombinedDetection::getCentroids() {
    return  this->centroids;
}


// Global Detection

void CombinedDetection::setGlobalDetectionKernelSize(int value) {
    this->globalDetector.setGlobalDetectionKernelSize(value);
}

void CombinedDetection::setGlobalDetectionStdDevSigma(float value) {
    this->globalDetector.setGlobalDetectionStdDevSigma(value);
}

void CombinedDetection::setGlobalDetectionLocalAreaExpansion(int value) {
    this->globalDetector.setGlobalDetectionLocalAreaExpansion(value);
}
                                                      
void CombinedDetection::setGlobalDetectionMinCleaningObjSize(int value) {
    this->globalDetector.setGlobalDetectionMinCleaningObjSize(value);
}
                                                      
void CombinedDetection::setGlobalDetectionCleaningIterations(int value) {
    this->globalDetector.setGlobalDetectionCleaningIterations(value);
}
                                                      
void CombinedDetection::setGlobalDetectionBGSubScalingFactor(float value) {
    this->globalDetector.setGlobalDetectionBGSubScalingFactor(value);
}

// Local Detection

void CombinedDetection::setLocalDetectionKernelSize(int value) {
    this->localDetector.setLocalDetectionKernelSize(value);
}

void CombinedDetection::setLocalDetectionStdDevSigma(float value) {
    this->localDetector.setLocalDetectionStdDevSigma(value);
}

void CombinedDetection::setLocalDetectionLocalAreaExpansion(int value) {
    this->localDetector.setLocalDetectionLocalAreaExpansion(value);
}

void CombinedDetection::setLocalDetectionMinCleaningObjSize(int value) {
    this->localDetector.setLocalDetectionMinCleaningObjSize(value);
}

void CombinedDetection::setLocalDetectionCleaningIterations(int value) {
    this->localDetector.setLocalDetectionCleaningIterations(value);
}

void CombinedDetection::setLocalDetectionBGSubScalingFactor(float value) {
    this->localDetector.setLocalDetectionBGSubScalingFactor(value);
}

void CombinedDetection::setLocalDetectionNumOfGlobalObjects(int value) {
    this->localDetector.setLocalDetectionNumOfGlobalObjects(value);
}

void CombinedDetection::setLocalDetectionSigmoidSlope(int value) {
    this->localDetector.setLocalDetectionSigmoidSlope(value);
}

void CombinedDetection::setLocalDetectionSigmoidCenterParam(int value) {
    this->localDetector.setLocalDetectionSigmoidCenterParam(value);
}

void CombinedDetection::setLocalDetectionLayeredDetectionThreshold(int value) {
    this->localDetector.setLocalDetectionLayeredDetectionThreshold(value);
}
