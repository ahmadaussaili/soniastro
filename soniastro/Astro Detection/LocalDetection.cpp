//
//  LocalDetection.cpp
//  soniastro
//
//  Created by Ahmad Aussaili on 3/26/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#include "LocalDetection.hpp"

LocalDetection::LocalDetection () {
}

void LocalDetection::logEvent(string event) {
    cout << logIdentifier << event;
}

string LocalDetection::getLogIdentifier() {
    return this->logIdentifier;
}

void LocalDetection::setTemplatesPath(string path) {
    this->templatesPath = path;
}


Mat LocalDetection::detect(Mat originalImage, Mat seedpointImage, bool deblending) {
    
    ostringstream eventMsgStream;
    logEvent("Local detection started...\n");
    
    // Original image
    Mat orgImage = originalImage.clone();
    
    // Detection image
    Mat dImage = seedpointImage.clone();
    
    // Irregular Sub-region Division
    logEvent("Performing irregular sub-region division...\n");
    
    // Find total markers
    vector<vector<Point> > contours;
    findContours(dImage.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    // Create the marker image for the watershed algorithm
    Mat markers = Mat::zeros(dImage.size(), CV_32S);
    // Draw the foreground markers
    for (size_t i = 0; i < contours.size(); i++)
    {
        drawContours(markers, contours, static_cast<int>(i), Scalar(static_cast<int>(i)+1), -1);
    }
    // Draw the background marker
    circle(markers, Point(5,5), 3, Scalar(255), -1);
    
    // Perform the watershed algorithm
    watershed(orgImage, markers);
    
    // Generate random colors
    vector<Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = theRNG().uniform(0, 256);
        int g = theRNG().uniform(0, 256);
        int r = theRNG().uniform(0, 256);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    
    // Create a randomly coloured image
    Mat colouredImage = Mat::zeros(markers.size(), CV_8UC3);
    
    Mat backgroundRegions = Mat::zeros(markers.size(), CV_8U);
    bool foundBackgroundRegions = false;
    
    vector<int> regionLabels;
    
    // Fill labeled objects with random colors
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i,j);
            
            if (index > 0 && index <= static_cast<int>(contours.size()))
            {
                regionLabels.push_back(index);
                
                if (dImage.at<uchar>(i,j) == (uchar) 255) {
                    colouredImage.at<Vec3b>(i,j) = Vec3b(255, 255, 255);
                } else {
                    colouredImage.at<Vec3b>(i,j) = colors[index-1];
                }
            } else if (index > static_cast<int>(contours.size())) {
                foundBackgroundRegions = true;
                backgroundRegions.at<uchar>(i,j) = (uchar) 255;
            }
        }
    }
    
    // Label background regions (marked with -1)
    regionLabels = getUniqueElements(regionLabels);
    sort(regionLabels.begin(), regionLabels.end());
    
    if (foundBackgroundRegions) {
        
        Mat labeledBackgroundRegions; // each pixel is given a label (0 for background)
        Mat stats; // [label] [0: x of bbox | 1: top left y of bbox | 2: width of bbox | 3: height of bbox | 4: area] , type: CV_32S (signed int)
        Mat centroids; // [label] [0: x center, 1: y center] , type: CV_64F (float)
        
        connectedComponentsWithStats(backgroundRegions, labeledBackgroundRegions, stats, centroids, 8);
        
        for (int i = 0; i < markers.rows; i++)
        {
            for (int j = 0; j < markers.cols; j++)
            {
                int index = markers.at<int>(i,j);
                
                if (index > static_cast<int>(contours.size()) && labeledBackgroundRegions.at<int>(i,j) != 0) {
                    int newLabel = regionLabels[regionLabels.size() - 1] + labeledBackgroundRegions.at<int>(i,j);
                    markers.at<int>(i,j) = newLabel;
                }
            }
        }
        
        int maxRegionLabel = regionLabels[regionLabels.size() - 1];
        for (int label = 1; label < stats.size[0]; label++) {
            regionLabels.push_back(maxRegionLabel + label);
        }
    }
    
    // Detect objects in each sub-region
    
    logEvent("Detecting objects in each sub-region...\n");
    logEvent("Performing layered object detection...\n");
    
    Mat histogramEqImage = Mat::zeros(originalImage.size(), CV_8U);
    Mat filteredNoiseImage = Mat::zeros(originalImage.size(), CV_8U);
    Mat localDetectionResult = Mat::zeros(originalImage.size(), CV_8U);
    
    for (int regionIndex = 0; regionIndex < regionLabels.size(); regionIndex++) {
        
        int regionLabel = regionLabels[regionIndex];
        Mat regionMask = (markers == regionLabel);
        
        Mat labeledRegion; // each pixel is given a label (0 for background)
        Mat stats; // [label] [0: x of bbox | 1: top left y of bbox | 2: width of bbox | 3: height of bbox | 4: area] , type: CV_32S (signed int)
        Mat centroids; // [label] [0: x center, 1: y center] , type: CV_64F (float)
        
        connectedComponentsWithStats(regionMask, labeledRegion, stats, centroids, 8);
        
        int bboxCoordX = stats.at<int>(1, 0);
        int bboxCoordY = stats.at<int>(1, 1); // upper-left corner
        int bboxWidth = stats.at<int>(1, 2);
        int bboxHeight = stats.at<int>(1, 3);
        
        int maskAreaExpansion = 2;
        
        if (bboxCoordX - maskAreaExpansion >= 0) {
            bboxCoordX -= maskAreaExpansion;
            bboxWidth += maskAreaExpansion;
        }
        
        if (bboxCoordY - maskAreaExpansion >= 0) {
            bboxCoordY -= maskAreaExpansion;
            bboxHeight += maskAreaExpansion;
        }
        
        if (bboxCoordX + bboxWidth + maskAreaExpansion < dImage.cols) {
            bboxWidth += maskAreaExpansion;
        }
        
        if (bboxCoordY + bboxHeight + maskAreaExpansion < dImage.rows) {
            bboxHeight += maskAreaExpansion;
        }
        
        Rect boundingBox(bboxCoordX, bboxCoordY, bboxWidth, bboxHeight);
        
        int maskArea = bboxWidth * bboxHeight;
        
        Mat regionImage = markers(boundingBox).clone();
        Mat localRegionMask = (regionImage == regionLabel);
        localRegionMask /= 255;
        Mat orgRegionImage = originalImage(boundingBox).clone();
        
        // Grayscale stretching
        Mat processingRegionImage = orgRegionImage.clone();
        cvtColor(orgRegionImage, processingRegionImage, COLOR_BGR2GRAY);
        processingRegionImage = sqrtMatrix(processingRegionImage);
        
        double median = medianMatrix(processingRegionImage, 256);
        median /= 255;
        double sigmoidCenter = 1;
        if (sigmoidCenterParameter * median < sigmoidCenter) {
            sigmoidCenter = sigmoidCenterParameter * median;
        }
        for (int row = 0; row < processingRegionImage.rows; row++) {
            for (int col = 0; col < processingRegionImage.cols; col++) {
                double intensity = (double) processingRegionImage.at<uchar>(row, col);
                intensity /= 255;
                processingRegionImage.at<uchar>(row, col) = (uchar) (int) (1.0 / (1.0 + exp( -sigmoidSlope * (intensity - median) + DBL_EPSILON)) * 255);
            }
        }
        
        // Background estimation
        int background = estimateBackground(processingRegionImage, backgroundSubtractionScalingFactor);
        processingRegionImage = processingRegionImage - background;
        Mat backgroundSubtractionImage = processingRegionImage.clone();
        
        // Histogram equalization
        equalizeHist(processingRegionImage, processingRegionImage);
        histogramEqImage(boundingBox) += processingRegionImage.mul(localRegionMask);
        
        // Noise removal
        GaussianBlur(processingRegionImage, processingRegionImage, Size(kernelSize, kernelSize), standardDeviationSigma);
        filteredNoiseImage(boundingBox) += processingRegionImage.mul(localRegionMask);
        
        // Otsu's threshold
        threshold(processingRegionImage, processingRegionImage, 0, 255, THRESH_BINARY | THRESH_OTSU);
        
        // Layered object detection to detect more faint objects
        
        Mat labeledLocalRegion; // each pixel is given a label (0 for background)
        Mat localStats; // [label] [0: x of bbox | 1: top left y of bbox | 2: width of bbox | 3: height of bbox | 4: area] , type: CV_32S (signed int)
        Mat localCentroids; // [label] [0: x center, 1: y center] , type: CV_64F (float)
        
        connectedComponentsWithStats(processingRegionImage, labeledLocalRegion, localStats, localCentroids, 8);
        
        Mat bgSubImageWithoutLargeObjects = backgroundSubtractionImage.clone();
        Mat faintObjectsBGSubImage = backgroundSubtractionImage.clone();
        
        Mat largeObjects = Mat::zeros(processingRegionImage.size(), CV_8U);
        
        // Compute background subtraction image without large objects
        for (int objLabel = 1; objLabel < localStats.size[0]; objLabel++) {
            
            int objbboxCoordX = localStats.at<int>(objLabel, 0);
            int objbboxCoordY = localStats.at<int>(objLabel, 1); // upper-left corner
            int objbboxWidth = localStats.at<int>(objLabel, 2);
            int objbboxHeight = localStats.at<int>(objLabel, 3);
            
            cv::Rect boundingBox(objbboxCoordX, objbboxCoordY, objbboxWidth, objbboxHeight);
            Mat objectImg = originalImage(boundingBox).clone();
            
            int area = stats.at<int>(objLabel, 4);
            
            if (area > layerdDetectionObjectSizeThreshold) {
                bgSubImageWithoutLargeObjects -= (labeledLocalRegion == objLabel);
                largeObjects += (labeledLocalRegion == objLabel);
            }
        }
        
        // Set the intesities of large objects to mean intensity of the background subtracted image without the large objects
        int meanIntensity = medianMatrix(bgSubImageWithoutLargeObjects, 256);
        for (int objLabel = 1; objLabel < localStats.size[0]; objLabel++) {
            
            int area = stats.at<int>(objLabel, 4);
            
            if (area > layerdDetectionObjectSizeThreshold) {
                Mat mask = (labeledLocalRegion == objLabel);
                faintObjectsBGSubImage -= mask;
                mask /= 255;
                mask *= meanIntensity;
                faintObjectsBGSubImage += mask;
            }
        }
        
        // Histogram equalization
        equalizeHist(faintObjectsBGSubImage, faintObjectsBGSubImage);
        histogramEqImage(boundingBox) += faintObjectsBGSubImage.mul(localRegionMask);
        
        // Noise removal
        GaussianBlur(faintObjectsBGSubImage, faintObjectsBGSubImage, Size(kernelSize, kernelSize), standardDeviationSigma);
        filteredNoiseImage(boundingBox) += faintObjectsBGSubImage.mul(localRegionMask);
        
        // Otsu's threshold
        threshold(faintObjectsBGSubImage, faintObjectsBGSubImage, 0, 255, THRESH_BINARY | THRESH_OTSU);
        
        // Combine the detected faint objects and large objects
        
        Mat structuringElement = getStructuringElement(MORPH_ELLIPSE, Size(17, 17)); // similar to disk structuring element in matlab
        //erode(largeObjects, largeObjects, structuringElement);
        dilate(largeObjects, largeObjects, structuringElement);
        
        processingRegionImage = faintObjectsBGSubImage + largeObjects;
        
        localDetectionResult(boundingBox) += processingRegionImage.mul(localRegionMask);
    }
    
    // Artifact cleaning
    
    eventMsgStream << "Cleaning artifacts around large objects (iterations: " << numOfCleaningIterations<< ")... \n";
    logEvent(eventMsgStream.str());
    
    for (int i = 0; i < numOfCleaningIterations; i++) {
        localDetectionResult = cleanArtifacts(originalImage, localDetectionResult, localAreaExpansion, minDetectionObjPixelSize);
    }
    
    imwrite("/Users/ahmadaussaili/Desktop/layered_detection.jpg", localDetectionResult);
    
    // Deblending (Optional)
    logEvent("Deblending local objects... \n");
    
    if (deblending == true) {
        Deblender deblender = Deblender();
        deblender.setTemplatesPath(this->templatesPath);
        localDetectionResult = deblender.deblendImage(originalImage, localDetectionResult, kernelSize, standardDeviationSigma, Deblender::DeblendingMethod::DistanceTransform);
    }
    
    this->binaryImage = localDetectionResult.clone();
    this->originalImage = originalImage.clone();
    
    return localDetectionResult;
}

Mat LocalDetection::getSeedPoints(Mat globalObjects, int numberOfGlobalObjects) {
    
    CV_Assert(globalObjects.depth() == CV_8U);
    
    Mat binaryImage = globalObjects.clone();
    Mat structuringElement = getStructuringElement(MORPH_ELLIPSE, Size(15, 15)); // similar to disk structuring element in matlab
    erode(binaryImage, binaryImage, structuringElement);
    dilate(binaryImage, binaryImage, structuringElement);
    
    Mat labeledImage; // each pixel is given a label (0 for background)
    Mat stats; // [label] [0: x of bbox | 1: top left y of bbox | 2: width of bbox | 3: height of bbox | 4: area] , type: CV_32S (signed int)
    Mat centroids; // [label] [0: x center, 1: y center] , type: CV_64F (float)
    
    connectedComponentsWithStats(binaryImage, labeledImage, stats, centroids, 8);
    
    vector<int> globalObjectsAreas;
    
    for (int label = 1; label < stats.size[0]; label++) {
        
        int area = stats.at<int>(label, 4);
        globalObjectsAreas.push_back(area);
    }
    
    if (globalObjectsAreas.size() <= numberOfGlobalObjects) {
        return binaryImage;
    }
    
    sort(globalObjectsAreas.begin(), globalObjectsAreas.end(), greater<int>());
    
    // Iterate and only keep the large objects
    
    int numOfRows = labeledImage.rows;
    int numOfCols = labeledImage.cols;
    
    int* rowPointer;
    for (int row = 0; row < numOfRows; ++row) {
        
        rowPointer = labeledImage.ptr<int>(row);
        for (int col = 0; col < numOfCols; ++col) {
            
            int label = (int) rowPointer[col];
            
            // If the object is the background ignore it.
            if (label == 0) {
                continue;
            }
            
            // If the the object is not in the first "numberOfGlobalObjects"
            // objects, then set it's intensity to zero.
            int area = stats.at<int>(label, 4);
            if (area <= globalObjectsAreas[numberOfGlobalObjects]) {
                uchar* binaryImageRowPointer = binaryImage.ptr<uchar>(row);
                binaryImageRowPointer[col] = (uchar) 0;
            }
        }
    }
    
    return binaryImage;
}

Mat LocalDetection::getOriginalImage() {
    return  this->originalImage;
}

Mat LocalDetection::getBinaryImage() {
    return  this->binaryImage;
}

void LocalDetection::setLocalDetectionKernelSize(int value) {
    this->kernelSize = value;
}

void LocalDetection::setLocalDetectionStdDevSigma(float value) {
    this->standardDeviationSigma = value;
}

void LocalDetection::setLocalDetectionLocalAreaExpansion(int value) {
    this->localAreaExpansion = value;
}

void LocalDetection::setLocalDetectionMinCleaningObjSize(int value) {
    this->minDetectionObjPixelSize = value;
}

void LocalDetection::setLocalDetectionCleaningIterations(int value) {
    this->numOfCleaningIterations = value;
}

void LocalDetection::setLocalDetectionBGSubScalingFactor(float value) {
    this->backgroundSubtractionScalingFactor = value;
}

void LocalDetection::setLocalDetectionNumOfGlobalObjects(int value) {
    this->numberOfGlobalObjects = value;
}

void LocalDetection::setLocalDetectionSigmoidSlope(int value) {
    this->sigmoidSlope = value;
}

void LocalDetection::setLocalDetectionSigmoidCenterParam(int value) {
    this->sigmoidCenterParameter = value;
}

void LocalDetection::setLocalDetectionLayeredDetectionThreshold(int value) {
    this->layerdDetectionObjectSizeThreshold = value;
}
