//
//  AstroUtilities.cpp
//  Astro Detector
//
//  Created by Ahmad Aussaili.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#include "AstroUtilities.hpp"

double medianMatrix(Mat image, int numOfIntensityValues) {
    
    // Accept only 1-channel matrices
    CV_Assert(image.channels() == 1);
    
    // Compute the pixel intensity histogram
    Mat histogram;
    int histogramSize = numOfIntensityValues;
    float range[] = {0, static_cast<float>(numOfIntensityValues)};
    const float* histogramRange = { range };
    bool uniform = true;
    bool accumulate = false;
    calcHist(&image, 1, 0, Mat(), histogram, 1, &histogramSize, &histogramRange, uniform, accumulate);
    
    // Compute the cumulative distibution function (CDF)
    Mat cdf;
    histogram.copyTo(cdf);
    for (int valueIndex = 1; valueIndex < numOfIntensityValues; valueIndex++) {
        cdf.at<float>(valueIndex) += cdf.at<float>(valueIndex - 1);
    }
    cdf /= image.total();
    
    // Compute median value
    double median = 0;
    for (int valueIndex = 0; valueIndex < numOfIntensityValues; valueIndex++) {
        if (cdf.at<float>(valueIndex) >= 0.5) {
            median = valueIndex;
            break;
        }
    }
    return median;
}

double medianMatrix(Mat image) {
    
    // Accept only 1-channel matrices
    CV_Assert(image.channels() == 1);
    
    std::vector<uchar> imageVector;
    
    // Insert the values of the image matrix into a vector
    if (image.isContinuous()) {
        imageVector.assign(image.data, image.data + image.total());
    } else {
        for (int i = 0; i < image.rows; ++i) {
            imageVector.insert(imageVector.end(), image.ptr<uchar>(i), image.ptr<uchar>(i) + image.cols);
        }
    }
    
    // Sort the vector in ascending order
    sort(imageVector.begin(), imageVector.end());
    
    // Return the median:
    
    // Odd case
    unsigned long imageSize = imageVector.size();
    if (imageSize % 2 != 0)
        return (double) imageVector[imageSize/2];
    
    // Even case
    return (double) (imageVector[(imageSize-1)/2] + imageVector[imageSize/2])/2.0;
}

bool containsValuesOutsideRange(Mat image, double minValue, double maxValue) {
    
    // Accept only char matrices (8-bits per channel)
    CV_Assert(image.depth() == CV_8U);
    
    // Accept only 1-channel matrices
    CV_Assert(image.channels() == 1);
    
    int numOfRows = image.rows;
    int numOfCols = image.cols;
    
    // Check if matrix is stored in a continues manner
    if (image.isContinuous()) {
        numOfCols *= numOfRows;
        numOfRows = 1;
    }
    
    uchar* rowPointer;
    for (int row = 0; row < numOfRows; ++row) {
        rowPointer = image.ptr<uchar>(row);
        for (int col = 0; col < numOfCols; ++col) {
            if ((int) rowPointer[col] == 0) {
                continue;
            }
            if ((int) rowPointer[col] < minValue || (int) rowPointer[col] > maxValue) {
                return true;
            }
        }
    }
    
    return false;
}

Mat sqrtMatrix(Mat image) {
    
    // Accept only char matrices (8-bits per channel)
    CV_Assert(image.depth() == CV_8U);
    
    // Accept only 1-channel matrices
    CV_Assert(image.channels() == 1);
    
    Mat sqrtMatrix = image.clone();
    
    int numOfRows = sqrtMatrix.rows;
    int numOfCols = sqrtMatrix.cols;
    
    // Check if matrix is stored in a continues manner
    if (sqrtMatrix.isContinuous()) {
        numOfCols *= numOfRows;
        numOfRows = 1;
    }
    
    uchar* rowPointer;
    for (int row = 0; row < numOfRows; ++row) {
        rowPointer = sqrtMatrix.ptr<uchar>(row);
        for (int col = 0; col < numOfCols; ++col) {
            
            double intensity = 1.0 * (int) rowPointer[col];
            intensity /= 255;
            intensity = sqrt(intensity);
            intensity *= 255;
            
            rowPointer[col] = (uchar) intensity;
        }
    }
    
    return sqrtMatrix;
}

vector<int> getUniqueElements(Mat matrix) {
    
    // The unique elements wil be stored in this vector
    vector<int> uniqueElements;
    
    CV_Assert(matrix.depth() == CV_32S);
    
    // Accept only 1-channel matrices
    CV_Assert(matrix.channels() == 1);
    
    int numOfRows = matrix.rows;
    int numOfCols = matrix.cols;
    
    // Check if matrix is stored in a continues manner
    if (matrix.isContinuous()) {
        numOfCols *= numOfRows;
        numOfRows = 1;
    }
    
    int* rowPointer;
    for (int row = 0; row < numOfRows; ++row) {
        rowPointer = matrix.ptr<int>(row);
        for (int col = 0; col < numOfCols; ++col) {
            int label = rowPointer[row];
            if (find(uniqueElements.begin(), uniqueElements.end(), label) == uniqueElements.end()) {
                uniqueElements.push_back(label);
            }
        }
    }
    return uniqueElements;
}

vector<int> getUniqueElements(vector<int> vec) {
    
    // The unique elements wil be stored in this vector
    vector<int> uniqueElements;

    for (int i = 0; i < vec.size(); i++) {
        int element = vec[i];
        if (find(uniqueElements.begin(), uniqueElements.end(), element) == uniqueElements.end()) {
            uniqueElements.push_back(element);
        }
    }
    return uniqueElements;
}

Mat drawBoundingBoxes(Mat orgImage, Mat binaryImage) {
    
    CV_Assert(binaryImage.depth() == CV_8U);
    
    // Image with bounding boxes around each detected object
    Mat detectionImage = orgImage.clone();
    
    Mat labeledImage; // each pixel is given a label (0 for background)
    Mat stats; // [label] [0: x of bbox | 1: top left y of bbox | 2: width of bbox | 3: height of bbox | 4: area] , type: CV_32S (signed int)
    Mat centroids; // [label] [0: x center, 1: y center] , type: CV_64F (float)
    connectedComponentsWithStats(binaryImage, labeledImage, stats, centroids, 8);
    
    int minimumObjectAreaForDrawing = 16;
    
    for (int label = 1; label < stats.size[0]; label++) {
        
        int area = stats.at<int>(label, 4);
        if (area < minimumObjectAreaForDrawing) {
            continue;
        }
        
        // Bounding box property
        int bboxCoordX = stats.at<int>(label, 0);
        int bboxCoordY = stats.at<int>(label, 1); // upper-left corner
        int bboxWidth = stats.at<int>(label, 2);
        int bboxHeight = stats.at<int>(label, 3);
        
        Rect boundingBox(bboxCoordX, bboxCoordY, bboxWidth, bboxHeight);
        rectangle(detectionImage, boundingBox, cv::Scalar(0, 255, 0), 2);
    }
    
    return detectionImage;
}

double galaxyTemplateMatching(Mat image, string templatesDirectory, int numOfTemplates) {
    
    double matchingScore = 0;
    
    for (int i = 1; i <= numOfTemplates; i++) {
        
        // Path to image template
        string imagePath = templatesDirectory + "/template";
        imagePath = imagePath + to_string(i);
        imagePath += ".png";
        
        Mat imageTemplate = imread(imagePath);
        
        if (!imageTemplate.data) {
            cout <<  "Could not open or find the template '" << imagePath << "'" << endl;
            return matchingScore;
        }
        
        // Convert to grayscale
        cvtColor(imageTemplate, imageTemplate, COLOR_BGR2GRAY);
        Mat grayScaleImage = image.clone();
        if (grayScaleImage.channels() != 1) {
            cvtColor(grayScaleImage, grayScaleImage, COLOR_BGR2GRAY);
        }
        
        if (grayScaleImage.cols >= imageTemplate.cols && grayScaleImage.rows >= imageTemplate.rows) {
            Mat result;
            
            int result_cols =  grayScaleImage.cols - imageTemplate.cols + 1;
            int result_rows = grayScaleImage.rows - imageTemplate.rows + 1;
            
            result.create( result_rows, result_cols, CV_32FC1 );
            
            matchTemplate(grayScaleImage, imageTemplate, result, TM_CCOEFF_NORMED);
            normalize( result, result, 0.0, 1.0, NORM_MINMAX, -1, Mat() );

            // Compute the matching score as the mean of the result matrix values
            matchingScore += mean(result)[0];
        }
    }
    
    return matchingScore / numOfTemplates;
}

vector<double> normalizeVector(vector<double> arr, double maxNormalizationValue) {
    double maxValue = 0;
    for (int i = 0; i < arr.size(); i++) {
        //arr[i] = (double)(int)(arr[i] * 100000) / 100000;
        if (arr[i] > maxValue) {
            maxValue = arr[i];
        }
    }
    
    if (maxValue != 0) {
        for (int i = 0; i < arr.size(); i++) {
            arr[i] /= maxValue;
            arr[i] *= maxNormalizationValue;
        }
    }
    
    return arr;
}

void checkAndResizeImage(Mat inputImage, Mat &outputImage, int maxCols) {
    
    int rows = inputImage.rows;
    int cols = inputImage.cols;
    if (cols > maxCols) {
        int offset = cols - maxCols;
        cv::Size resizedImageSize = cv::Size(0,0);
        resizedImageSize.width = cols - offset;
        resizedImageSize.height = rows - offset;
        resize(inputImage, outputImage, resizedImageSize, 0, 0, INTER_AREA);
    }
}

int estimateBackground(Mat image, float scalingFactor) {
    
    Scalar imgStdDev, imgMean;
    meanStdDev(image, imgMean, imgStdDev);
    
    if (imgStdDev[0] == 0) {
        return 0;
    }
    
    // Compute the min and max intensity values of the original image
    double median = medianMatrix(image, 256);
    double minIntensityValue = median - (3 * imgStdDev[0]);
    double maxIntensityValue = median + (3 * imgStdDev[0]);
    
    // Remove values which are not witihin (median -/+ 3*stddev)
    Mat clippedImage = clipIntensity(image, minIntensityValue, maxIntensityValue);
    
    // Compute the min and max intensity values of the clipped image
    Scalar clippedImgStdDev, clippedImgMean;
    meanStdDev(clippedImage, clippedImgMean, clippedImgStdDev);
    median = medianMatrix(clippedImage, 256);
    minIntensityValue = median - (3 * clippedImgStdDev[0]);
    maxIntensityValue = median + (3 * clippedImgStdDev[0]);
    
    // The differences between the std deviations in intensities between the iterations of clipping the image
    vector<double> stdDevDifferences;
    stdDevDifferences.push_back( (imgStdDev[0] - clippedImgStdDev[0]) / imgStdDev[0] );
    
    // Keep clipping the image inentsities until it contains no values outside the specified range
    while (containsValuesOutsideRange(clippedImage, minIntensityValue, maxIntensityValue)) {
        imgStdDev = clippedImgStdDev;
        clippedImage = clipIntensity(clippedImage, minIntensityValue, maxIntensityValue);
        meanStdDev(clippedImage, clippedImgMean, clippedImgStdDev);
        median = medianMatrix(clippedImage, 256);
        minIntensityValue = median - (3 * clippedImgStdDev[0]);
        maxIntensityValue = median + (3 * clippedImgStdDev[0]);
        stdDevDifferences.push_back( (imgStdDev[0] - clippedImgStdDev[0]) / imgStdDev[0] );
    }
    
    // This is to check whether the image represents a crowded cluster or not
    double maxStdDevDiff = *max_element(stdDevDifferences.begin(), stdDevDifferences.end());
    
    // NON_CROWDED FIELD
    if ( maxStdDevDiff < 0.2) {
        // The value of the background in a non-crowded field is estimated as the mean of the clipped histogram of pixel distribution
        return scalingFactor * round(clippedImgMean[0]);
        
    } else { // CROWDED FIELD
        return scalingFactor * round( (2.5 * median - 1.5 * clippedImgMean[0]) );
    }
}

Mat clipIntensity(Mat image, double minValue, double maxValue) {
    
    // Accept only char matrices (8-bits per channel)
    CV_Assert(image.depth() == CV_8U);
    
    // Accept only 1-channel matrices
    CV_Assert(image.channels() == 1);
    
    // If values are outside of range [minValue, maxValue], then set them to 0 (black)
    Mat clippedImage = image.clone();
    
    int numOfRows = clippedImage.rows;
    int numOfCols = clippedImage.cols;
    
    // Check if matrix is stored in a continues manner
    if (clippedImage.isContinuous()) {
        numOfCols *= numOfRows;
        numOfRows = 1;
    }
    
    uchar* rowPointer;
    for (int row = 0; row < numOfRows; ++row) {
        rowPointer = clippedImage.ptr<uchar>(row);
        for (int col = 0; col < numOfCols; ++col) {
            if ((int) rowPointer[col] == 0) {
                continue;
            }
            if ((int) rowPointer[col] < minValue || (int) rowPointer[col] > maxValue) {
                rowPointer[col] = (uchar) 0;
            }
        }
    }
    return clippedImage;
}

Mat cleanArtifacts(Mat image, Mat bImage, int localAreaExpansion, int minDetectionObjPixelSize) {
    
    Mat orgImage = image.clone();
    Mat binaryImage = bImage.clone();
    Mat cleanImage = bImage.clone(); // binary
    
    // Accept only char matrices (8-bits per channel)
    CV_Assert(orgImage.depth() == CV_8U);
    CV_Assert(binaryImage.depth() == CV_8U);
    
    // Accept only 1-channel binary matrices
    CV_Assert(binaryImage.channels() == 1);
    
    // Detect objects in image
    Mat labeledImage; // each pixel is given a label (0 for background)
    Mat stats; // [label] [0: x of bbox | 1: top left y of bbox | 2: width of bbox | 3: height of bbox | 4: area] , type: CV_32S (signed int)
    Mat centroids; // [label] [0: x center, 1: y center] , type: CV_64F (float)
    connectedComponentsWithStats(binaryImage, labeledImage, stats, centroids, 8);
    
    // Compute the mean area of the detected objects (skip background)
    int meanArea = 0;
    for (int objIndex = 1; objIndex < stats.size[0]; objIndex++) {
        int area = stats.at<int>(objIndex, 4);
        meanArea += area;
    }
    if (stats.size[0] - 1 != 0) {
        meanArea /= (stats.size[0] - 1); // -1 to skip the background label
    }
    
    // Clean the noise around large objects
    // (skip the background label again)
    for (int objIndex = 1; objIndex < stats.size[0]; objIndex++) {
        
        int area = stats.at<int>(objIndex, 4);
        
        // If area > threshold * mean area
        if (area > minDetectionObjPixelSize * meanArea) {
            
            // Expand the area of large objects:
            
            // Bounding box properties
            int bboxCoordX = stats.at<int>(objIndex, 0);
            int bboxCoordY = stats.at<int>(objIndex, 1); // upper-left corner
            int bboxWidth = stats.at<int>(objIndex, 2);
            int bboxHeight = stats.at<int>(objIndex, 3);
            
            // Compute expended bounding box
            int areaCoordX1 = max(bboxCoordX - localAreaExpansion, 0);
            int areaCoordX2 = min(bboxCoordX + bboxWidth + localAreaExpansion, orgImage.cols - 1);
            int areaCoordY1 = max(bboxCoordY - localAreaExpansion, 0);
            int areaCoordY2 = min(bboxCoordY + bboxHeight + localAreaExpansion, orgImage.rows - 1);
            
            // Expanded area without refactoring (fine-tuning).
            // Used for comparison with the refactored one.
            Mat orgExpandedArea = binaryImage.colRange(areaCoordX1, areaCoordX2).rowRange(areaCoordY1, areaCoordY2);
            
            // Compute expanded area with refactoring
            Mat refactoredExpandedArea = orgImage.colRange(areaCoordX1, areaCoordX2).rowRange(areaCoordY1, areaCoordY2).clone();
            
            // Gray scaling
            cvtColor(refactoredExpandedArea, refactoredExpandedArea, COLOR_BGR2GRAY);
            // Background subtraction
            int backgroundSubScalingFactor = 1.0;
            if (area > minDetectionObjPixelSize * minDetectionObjPixelSize * meanArea) {
                backgroundSubScalingFactor = 1.5;
            }
            int background = estimateBackground(refactoredExpandedArea, backgroundSubScalingFactor);
            refactoredExpandedArea = refactoredExpandedArea - background;
            // Histogram equalization
            equalizeHist(refactoredExpandedArea, refactoredExpandedArea);
            // Gaussian filter
            int kernelSize = 7;
            float standardDeviationSigma = 1.5;
            GaussianBlur(refactoredExpandedArea, refactoredExpandedArea, Size(kernelSize, kernelSize), standardDeviationSigma);
            // Otsu thresholding
            Mat binaryRefactoredExpandedArea;
            int otsuThreshold = threshold(refactoredExpandedArea, binaryRefactoredExpandedArea, 0, 255, THRESH_BINARY | THRESH_OTSU);
            // Keep increasing the threshold until satisfactory results
            while (sum(binaryRefactoredExpandedArea)[0] - sum(orgExpandedArea)[0] > sum(orgExpandedArea)[0] / 3) {
                otsuThreshold += 1;
                threshold(refactoredExpandedArea, binaryRefactoredExpandedArea, otsuThreshold, 255, THRESH_BINARY);
            }
            // Image opening and closing
            Mat structuringElement = getStructuringElement(MORPH_ELLIPSE, Size(9, 9)); // similar to disk structuring element in matlab
            morphologyEx(binaryRefactoredExpandedArea, binaryRefactoredExpandedArea, MORPH_OPEN, structuringElement);
            morphologyEx(binaryRefactoredExpandedArea, binaryRefactoredExpandedArea, MORPH_CLOSE, structuringElement);
            
            // Compute the contracted area (the area without the surrounding noise)
            int contractedAreaCoordX1 = max(bboxCoordX - localAreaExpansion, 0) + minDetectionObjPixelSize;
            int contractedAreaCoordX2 = min(bboxCoordX + bboxWidth + localAreaExpansion, orgImage.cols - 1) - minDetectionObjPixelSize;
            
            int contractedAreaCoordY1 = max(bboxCoordY - localAreaExpansion, 0) + minDetectionObjPixelSize;
            int contractedAreaCoordY2 = min(bboxCoordY + bboxHeight + localAreaExpansion, orgImage.rows - 1) - minDetectionObjPixelSize;
            
            if (contractedAreaCoordX2 < contractedAreaCoordX1 || contractedAreaCoordY2 < contractedAreaCoordY1) {
                continue;
            }
            
            // Substitute the portion of the image containing the noisy object with the refactored area:
            
            Mat cleanImageSubArea = cleanImage.colRange(contractedAreaCoordX1, contractedAreaCoordX2).rowRange(contractedAreaCoordY1, contractedAreaCoordY2);
            
            Mat substitutionMatrix = binaryRefactoredExpandedArea.colRange(minDetectionObjPixelSize, binaryRefactoredExpandedArea.cols - minDetectionObjPixelSize).rowRange(minDetectionObjPixelSize, binaryRefactoredExpandedArea.rows - minDetectionObjPixelSize).clone();
            
            substitutionMatrix.copyTo(cleanImageSubArea);
        }
    }
    
    return cleanImage;
}
