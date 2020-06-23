//
//  Deblender.cpp
//  Astro Detector
//
//  Created by Ahmad Aussaili on 2/15/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#include "Deblender.hpp"

Deblender::Deblender () {
}

void Deblender::setTemplatesPath(string path) {
    this->templatesPath = path;
}

Mat Deblender::deblendImage(Mat orgImage, Mat bImage, unsigned int kernelSize, double standardDeviationSigma, DeblendingMethod deblendingMethod) {
    
    // Accept only char matrices (8-bits per channel)
    CV_Assert(bImage.depth() == CV_8U);
    
    Mat binaryImage = bImage.clone();
    
    // Detect objects
    Mat labeledImage; // each pixel is given a label (0 for background)
    Mat deblendedImage = bImage.clone();
    Mat stats; // [label] [0: x of bbox | 1: top left y of bbox | 2: width of bbox | 3: height of bbox | 4: area] , type: CV_32S (signed int)
    Mat centroids; // [label] [0: x center, 1: y center] , type: CV_64F (float)
    connectedComponentsWithStats(binaryImage, labeledImage, stats, centroids, 8);
    
    // Gaussian filter
    Mat smoothedOrgImg = orgImage.clone();
    GaussianBlur(orgImage, smoothedOrgImg, Size(kernelSize, kernelSize), standardDeviationSigma);
    
    // An additional label
    int newLabel = stats.size[0];
    
    // Skip background label
    for (int label = 1; label < stats.size[0]; label++) {
        
        int area = stats.at<int>(label, 4);
        
        // CAN USE HERE MEAN AREA OF OBJECTS
        if (area > 250) {
            
            // Bounding box properties
            int bboxCoordX = stats.at<int>(label, 0);
            int bboxCoordY = stats.at<int>(label, 1); // upper-left corner
            int bboxWidth = stats.at<int>(label, 2);
            int bboxHeight = stats.at<int>(label, 3);
            
            Rect boundingBox(bboxCoordX, bboxCoordY, bboxWidth, bboxHeight);
            
            // Cropped image of the object
            Mat objectImg = smoothedOrgImg(boundingBox).clone();
            Mat objectMask = (labeledImage == label);
            // Binary cropped image of the object
            Mat objectImgBinary = objectMask(boundingBox).clone();
            
            // Deblending object
            Mat deblendedObjectImg = deblendObject(objectImg, objectImgBinary, area, kernelSize, standardDeviationSigma, deblendingMethod);
            
            if (deblendedObjectImg.empty()) { // Deblending was not needed or not successful
                continue;
            }
            
            if (deblendingMethod == DistanceTransform) {
                // The deblended object image is a binary image
                deblendedObjectImg.copyTo(deblendedImage(boundingBox));
            } else {
                // The deblended object image is a labeled image, so add the new labels
                vector<int> labels = getUniqueElements(deblendedObjectImg);
                
                if (labels.size() > 2) { // 1 if no background label included
                    // Sort the labels in ascending mode
                    sort(labels.begin(), labels.end());
                    
                    // setTo(newValue, image == oldValue)
                    deblendedObjectImg.setTo(label, deblendedObjectImg == labels[1]); // labels[0] if no background label included
                    
                    for (int index = 2; index < labels.size(); index++) { // start loop from 1 if no background label included
                        deblendedObjectImg.setTo(newLabel, deblendedObjectImg == labels[index]);
                        newLabel += 1;
                    }
                    
                    Mat substitutionMatrix = deblendedImage(boundingBox).clone();
                    substitutionMatrix = substitutionMatrix * (1 - objectImgBinary) + deblendedObjectImg * objectImgBinary;
                    substitutionMatrix.copyTo(deblendedImage(boundingBox));
                }
            }
        }
    }
    
    return deblendedImage;
}

Mat Deblender::deblendObject(Mat objectImg, Mat binaryObjectImg, int area, unsigned int kernelSize, double standardDeviationSigma, DeblendingMethod deblendingMethod) {
    
    Mat deblendedImage = binaryObjectImg.clone();
    
    // Find the boundary of the object
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(binaryObjectImg.clone(), contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    
    // Check whether the object can be fitted by an ellipse or not
    bool canBeFittedByAnEllipse = false;
    vector<Point> contour = contours[0];
    
    vector<int> uniqueX;
    vector<int> uniqueY;
    for (int i = 0; i < contour.size(); i++) {
        Point point = contour[i];
        uniqueX.push_back(point.x);
        uniqueY.push_back(point.y);
    }
    uniqueX = getUniqueElements(uniqueX);
    uniqueY = getUniqueElements(uniqueY);
    
    if (contour.size() > 2 && uniqueX.size() > 1 && uniqueY.size() > 1) {
        vector<double> rePara;
        rePara.push_back( ((double) contour[0].x - contour[1].x) / (contour[0].y - contour[1].y) );
        rePara.push_back(contour[0].x - rePara[0] * contour[0].y);
        for (int i = 2; i < contour.size(); i++) {
            if (contour[i].x != (int) (rePara[0] * contour[i].y + rePara[1]) ) {
                canBeFittedByAnEllipse = true;
                break;
            }
        }
        
        if (canBeFittedByAnEllipse) {
            
            // Template matching for galaxies
            double galaxyTemplateMatchingScore = galaxyTemplateMatching(objectImg, this->templatesPath, 8);
            
            // If we are sure it's a single galaxy, then do not perform deblending
            if (galaxyTemplateMatchingScore > 0.5) {
                Mat emptyMatrix;
                return emptyMatrix;
            }
            
            // Kernel for sharpening the image
            Mat kernel = (Mat_<float>(3,3) <<
                          1,  1, 1,
                          1, -8, 1,
                          1,  1, 1);
            
            // Laplacian Filtering
            // We need to convert everything in something more deeper then CV_8U
            // because the kernel has some negative values
            // and we can expect in general to have a Laplacian image with negative values
            // BUT a 8 bits unsigned int (the one we are working with) can contain values from 0 to 255
            // so the possible negative number will be truncated
            Mat laplacianImage;
            filter2D(objectImg, laplacianImage, CV_32F, kernel);
            Mat sharpImage;
            objectImg.convertTo(sharpImage, CV_32F);
            Mat filteredImage = sharpImage - laplacianImage;
            // convert back to 8 bits gray scale
            filteredImage.convertTo(filteredImage, CV_8UC3);
            laplacianImage.convertTo(laplacianImage, CV_8UC3); // In case it is needed...
            
            // Create binary image from source image
            // Or used istead the given one
            //
            Mat binaryFilteredImage;
            cvtColor(filteredImage, binaryFilteredImage, COLOR_BGR2GRAY);
            //threshold(bw, bw, 40, 255, THRESH_BINARY | THRESH_OTSU);
            
            // Gaussian filter
            GaussianBlur(binaryFilteredImage, binaryFilteredImage, Size(11, 11), standardDeviationSigma);
            // Threshold to zero (lower bound clipping)
            double mint, maxt;
            minMaxLoc(binaryFilteredImage, &mint, &maxt);
            threshold(binaryFilteredImage, binaryFilteredImage, 0.8 * maxt, 255, THRESH_TOZERO);
            // Otsu threshold
            threshold(binaryFilteredImage, binaryFilteredImage, 0, 255, THRESH_BINARY | THRESH_OTSU);
            // Dilation
            kernel = Mat::ones(15, 15, CV_8U);
            dilate(binaryFilteredImage, binaryFilteredImage, kernel);
            
            // Perform the distance transform
            Mat distanceTransformImage;
            distanceTransform(binaryFilteredImage, distanceTransformImage, DIST_L2, 3);
            // Normalize the distance image for range = {0.0, 1.0}
            // so we can visualize and threshold it
            // RECHECK
            normalize(distanceTransformImage, distanceTransformImage, 0, 255, NORM_MINMAX);
            // Threshold to obtain the peaks
            // This will be the markers for the foreground objects
            threshold(distanceTransformImage, distanceTransformImage, 25, 255, THRESH_BINARY);
            // Dilate a bit the dist image
            Mat kernel1 = Mat::ones(3, 3, CV_8U);
            dilate(distanceTransformImage, distanceTransformImage, kernel1);
            // Create the CV_8U version of the distance image
            // It is needed for findContours()
            Mat dist_8u;
            distanceTransformImage.convertTo(dist_8u, CV_8U);
            
            if (deblendingMethod == DistanceTransform) {
                // Return the binary image, without applying the watershed method.
                return dist_8u;
            }
            
            // WATERSHED:
            
            // Find total markers
            vector<vector<Point> > contours;
            findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
            // Create the marker image for the watershed algorithm
            Mat markers = Mat::zeros(distanceTransformImage.size(), CV_32S);
            // Draw the foreground markers
            for (size_t i = 0; i < contours.size(); i++)
            {
                drawContours(markers, contours, static_cast<int>(i), Scalar(static_cast<int>(i)+1), -1);
            }
            // Draw the background marker
            circle(markers, Point(5,5), 3, Scalar(255), -1);
            
            Mat mark;
            markers.convertTo(mark, CV_8U);
            bitwise_not(mark, mark);
            //imshow("Markers_v2", mark); // uncomment this if you want to see how the mark
            // image looks like at that point
            
            // Perform the watershed algorithm
            watershed(filteredImage, markers);
            
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
            // Fill labeled objects with random colors
            for (int i = 0; i < markers.rows; i++)
            {
                for (int j = 0; j < markers.cols; j++)
                {
                    int index = markers.at<int>(i,j);
                    if (index > 0 && index <= static_cast<int>(contours.size()))
                    {
                        colouredImage.at<Vec3b>(i,j) = colors[index-1];
                    }
                }
            }
            
            // Return the relabeled image.
            return markers;
        }
    }

    // Deblending was not needed or not successful.
    Mat emptyMatrix;
    return emptyMatrix;
}
