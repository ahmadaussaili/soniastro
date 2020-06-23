//
//  AstroDetector.m
//  soniastro
//
//  Created by Ahmad Aussaili on 2/16/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#import "AstroDetector.h"
#include <fstream>
#include "soniastro-Swift.h"

struct CPPMembers {
    
    // Detector and sonifier
    CombinedDetection detector = CombinedDetection();
    Sonifier sonifier = Sonifier();
    
    // Classification
    std::map<int, int> classification;
    std::map<int, int> morphology;
    bool classesWereSet = false;
    
    // Classification method
    int classificationMethod = ML;
    
    // Objects smaller than this size are still visible in the objects file
    int minimumObjectAreaForDrawing = 16;
    
    // For use in determining filter boundaries
    int maxObjectArea = 0;
    int minObjectArea = 0;
    
    // Filters
    bool detectedObjectsFilter = true;
    bool starFilter = true;
    bool galaxyFilter = true;
    bool galaxyClassificationFilter = false;
    int objectArea = 0;
    
    // Selected object statistics
    int selectedObjectType = UnknownType;
    int selectedObjectMorphology = UnknownMorph;
    int selectedObjectArea = 0;
    cv::Point selectedObjectLocation = cv::Point(0,0);
    cv::Point selectedObjectBBoxLocation = cv::Point(0,0);
    cv::Size selectedObjectBBoxSize = cv::Size(0,0);
};

// Detection Properties

static bool localDetectionEnabled = false;
static bool deblendingEnabled = true;
static bool starGalaxyClassificationEnabled = true;
static bool galaxyMorphologyClassificationEnabled = true;

// Global Detection Properties

static int globalDetectionKernelSize = 7;
static float globalDetectionStdDevSigma = 1.5;
static int globalDetectionLocalAreaExpansion = 30;
static int globalDetectionMinDetectionObjSize = 10;
static int globalDetectionCleaningIterations = 7;
static float globalDetectionBGSubScalingFactor = 2;

// Local Detection Properties

static int localDetectionKernelSize = 7;
static float localDetectionStdDevSigma = 1.5;
static int localDetectionLocalAreaExpansion = 30;
static int localDetectionMinDetectionObjSize = 10;
static int localDetectionCleaningIterations = 1;
static float localDetectionBGSubScalingFactor = 1.6;
static int localDetectionNumbOfGlobalObjects = 30;
static int localDetectionSigmoidSlope = 20;
static int localDetectionSigmoidCenterParameter = 1;
static int localDetectionLayerdDetectionObjectSizeThreshold = 3000;

// Sonification Properties

static float samplingFrequency = 44100.f; // Keep it even.
static int binSize = 5; // Represents a frame in pixels.
static int FPS = 25; // Frames per second.
static bool universalAmplitude = true;
static float amplitudeFactor = 30; // This is multiplied with the objects area. (only available when universal amplitude is off)
static float attenuationFactor = 0.0; // Should be between 0 and 1

// Reverb
static float delayinMilliSeconds = 500;
static float decayFactor = 0.9;
static int mixFactor = 70;
static int reverbIterations = 1;

@implementation AstroDetector

- (id)init
{
    self = [super init];
    if (self) {
        
        // Allocate storage for cpp members
        _cppMembers = new CPPMembers;
        
        string templatesPath = string([[self getTemplatesPath] UTF8String]);
        _cppMembers->detector.setTemplatesPath(templatesPath);
        
        [self updateDetectorParameters];
        
        _cppMembers->sonifier.setDetector(_cppMembers->detector);
        
        [self updateSonifierParameters];
        
        sgcModel = [SGCModel new];
        gmcModel = [GMCModel new];
    }
    
    return self;
}

- (void)dealloc
{
    // Free members even if ARC (automatic reference counting).
    delete _cppMembers;
    
    // If not ARC uncomment the following line
    // [super dealloc];
}

- (void) updateDetectorParameters {
    
    // Global detection
    _cppMembers->detector.setGlobalDetectionKernelSize(globalDetectionKernelSize);
    _cppMembers->detector.setGlobalDetectionStdDevSigma(globalDetectionStdDevSigma);
    _cppMembers->detector.setGlobalDetectionLocalAreaExpansion(globalDetectionLocalAreaExpansion);
    _cppMembers->detector.setGlobalDetectionMinCleaningObjSize(globalDetectionMinDetectionObjSize);
    _cppMembers->detector.setGlobalDetectionCleaningIterations(globalDetectionCleaningIterations);
    _cppMembers->detector.setGlobalDetectionBGSubScalingFactor(globalDetectionBGSubScalingFactor);
    
    // Local detection
    _cppMembers->detector.setLocalDetectionKernelSize(localDetectionKernelSize);
    _cppMembers->detector.setLocalDetectionStdDevSigma(localDetectionStdDevSigma);
    _cppMembers->detector.setLocalDetectionLocalAreaExpansion(localDetectionLocalAreaExpansion);
    _cppMembers->detector.setLocalDetectionMinCleaningObjSize(localDetectionMinDetectionObjSize);
    _cppMembers->detector.setLocalDetectionCleaningIterations(localDetectionCleaningIterations);
    _cppMembers->detector.setLocalDetectionBGSubScalingFactor(localDetectionBGSubScalingFactor);
    _cppMembers->detector.setLocalDetectionNumOfGlobalObjects(localDetectionNumbOfGlobalObjects);
    _cppMembers->detector.setLocalDetectionSigmoidSlope(localDetectionSigmoidSlope);
    _cppMembers->detector.setLocalDetectionSigmoidCenterParam(localDetectionSigmoidCenterParameter);
    _cppMembers->detector.setLocalDetectionLayeredDetectionThreshold(localDetectionLayerdDetectionObjectSizeThreshold);
}

- (void) updateSonifierParameters {
    
    // AV
    _cppMembers->sonifier.setSamplingFrequency(samplingFrequency);
    _cppMembers->sonifier.setbinSize(binSize);
    _cppMembers->sonifier.setFPS(FPS);
    _cppMembers->sonifier.setUniversalAmplitude(universalAmplitude);
    _cppMembers->sonifier.setAmplitudeFactor(amplitudeFactor);
    _cppMembers->sonifier.setAttenuationFactor(attenuationFactor);
    
    // REVERB
    _cppMembers->sonifier.setDelayInMilliSeconds(delayinMilliSeconds);
    _cppMembers->sonifier.setDecayFactor(decayFactor);
    _cppMembers->sonifier.setMixFactor(mixFactor);
    _cppMembers->sonifier.setReverbIterations(reverbIterations);
}

// DETECTION PROPERTY FUNCTIONS

// getters

+ (bool) isLocalDetectionEnabled {
    return localDetectionEnabled;
}

+ (bool) isDeblendingEnabled {
    return deblendingEnabled;
}

+ (bool) isStarGalaxyClassificationEnabled {
    return starGalaxyClassificationEnabled;
}

+ (bool) isGalaxyMorphologyClassificationEnabled {
    return galaxyMorphologyClassificationEnabled;
}

// setters

+ (void) setLocalDetectionEnabled: (bool) enabled {
    localDetectionEnabled = enabled;
}

+ (void) setDeblendingEnabled: (bool) enabled {
    deblendingEnabled = enabled;
}

+ (void) setStarGalaxyClassificationEnabled: (bool) enabled {
    starGalaxyClassificationEnabled = enabled;
}

+ (void) setGalaxyMorphologyClassificationEnabled: (bool) enabled {
    galaxyMorphologyClassificationEnabled = enabled;
}

// GLOBAL DETECTION PROPERTY FUNCTIONS

// getters

+ (int) getGlobalDetectionKernelSize {
    return globalDetectionKernelSize;
}

+ (float) getGlobalDetectionStdDevSigma {
    return globalDetectionStdDevSigma;
}

+ (int) getGlobalDetectionLocalAreaExpansion {
    return globalDetectionLocalAreaExpansion;
}

+ (int) getGlobalDetectionMinCleaningObjSize {
    return globalDetectionMinDetectionObjSize;
}

+ (int) getGlobalDetectionCleaningIterations {
    return globalDetectionCleaningIterations;
}

+ (float) getGlobalDetectionBGSubScalingFactor {
    return globalDetectionBGSubScalingFactor;
}

// setters

+ (void) setGlobalDetectionKernelSize: (int) value {
    globalDetectionKernelSize = value;
}

+ (void) setGlobalDetectionStdDevSigma: (float) value {
    globalDetectionStdDevSigma = value;
}

+ (void) setGlobalDetectionLocalAreaExpansion: (int) value {
    globalDetectionLocalAreaExpansion = value;
}

+ (void) setGlobalDetectionMinCleaningObjSize: (int) value {
    globalDetectionMinDetectionObjSize = value;
}

+ (void) setGlobalDetectionCleaningIterations: (int) value {
    globalDetectionCleaningIterations = value;
}

+ (void) setGlobalDetectionBGSubScalingFactor: (float) value {
    globalDetectionBGSubScalingFactor = value;
}

// LOCAL DETECTION PROPERTY FUNCTIONS

// getters

+ (int) getLocalDetectionKernelSize {
    return localDetectionKernelSize;
}

+ (float) getLocalDetectionStdDevSigma {
    return localDetectionStdDevSigma;
}

+ (int) getLocalDetectionLocalAreaExpansion {
    return localDetectionLocalAreaExpansion;
}

+ (int) getLocalDetectionMinCleaningObjSize {
    return localDetectionMinDetectionObjSize;
}

+ (int) getLocalDetectionCleaningIterations {
    return localDetectionCleaningIterations;
}

+ (float) getLocalDetectionBGSubScalingFactor {
    return localDetectionBGSubScalingFactor;
}

+ (int) getLocalDetectionNumOfGlobalObjects {
    return localDetectionNumbOfGlobalObjects;
}

+ (int) getLocalDetectionSigmoidSlope {
    return localDetectionSigmoidSlope;
}

+ (int) getLocalDetectionSigmoidCenterParam {
    return localDetectionSigmoidCenterParameter;
}

+ (int) getLocalDetectionLayeredDetectionThreshold {
    return localDetectionLayerdDetectionObjectSizeThreshold;
}

// setters

+ (void) setLocalDetectionKernelSize: (int) value {
    localDetectionKernelSize = value;
}

+ (void) setLocalDetectionStdDevSigma: (float) value {
    localDetectionStdDevSigma = value;
}

+ (void) setLocalDetectionLocalAreaExpansion: (int) value {
    localDetectionLocalAreaExpansion = value;
}

+ (void) setLocalDetectionMinCleaningObjSize: (int) value {
    localDetectionMinDetectionObjSize = value;
}

+ (void) setLocalDetectionCleaningIterations: (int) value {
    localDetectionCleaningIterations = value;
}

+ (void) setLocalDetectionBGSubScalingFactor: (float) value {
    localDetectionBGSubScalingFactor = value;
}

+ (void) setLocalDetectionNumOfGlobalObjects: (int) value {
    localDetectionNumbOfGlobalObjects = value;
}

+ (void) setLocalDetectionSigmoidSlope: (int) value {
    localDetectionSigmoidSlope = value;
}

+ (void) setLocalDetectionSigmoidCenterParam: (int) value {
    localDetectionSigmoidCenterParameter = value;
}

+ (void) setLocalDetectionLayeredDetectionThreshold: (int) value {
    localDetectionLayerdDetectionObjectSizeThreshold = value;
}


// SONIFICATION PROPERTY FUNCTIONS

// getters

+ (float) getSamplingFrequency {
    return samplingFrequency;
}

+ (int) getFPS {
    return FPS;
}

+ (int) getbinSize {
    return binSize;
}

+ (bool) getUniversalAmplitude {
    return universalAmplitude;
}

+ (float) getAmplitudeFactor {
    return amplitudeFactor;
}

+ (float) getAttenuationFactor {
    return attenuationFactor;
}

+ (float) getDelayInMilliSeconds {
    return delayinMilliSeconds;
}

+ (float) getDecayFactor {
    return decayFactor;
}

+ (int) getMixFactor {
    return mixFactor;
}

+ (int) getReverbIterations {
    return reverbIterations;
}

// setters

+ (void) setSamplingFrequency: (float) value {
    samplingFrequency = value;
}

+ (void) setFPS: (int) value {
    FPS = value;
}

+ (void) setbinSize: (int) value {
    binSize = value;
}

+ (void) setUniversalAmplitude: (bool) value {
    universalAmplitude = value;
}

+ (void) setAmplitudeFactor: (float) value {
    amplitudeFactor = value;
}

+ (void) setAttenuationFactor: (float) value {
    attenuationFactor = value;
}

+ (void) setDelayInMilliSeconds: (float) value {
    delayinMilliSeconds = value;
}

+ (void) setDecayFactor: (float) value {
    decayFactor = value;
}

+ (void) setMixFactor: (int) value {
    mixFactor = value;
}

+ (void) setReverbIterations: (int) value {
    reverbIterations = value;
}


// SELECTED OBJECT PROPERTIES

// getters

- (int) getMaxObjectArea {
    return _cppMembers->maxObjectArea;
}

- (int) getMinObjectArea {
    return _cppMembers->minObjectArea;
}

- (NSString *) getSelectedObjectType {
    if (!_cppMembers->detector.statsWereSet) {
        [self logEvent:@"OpenCV: Could not get object property because stats were not set\n"];
        return @"";
    }
    
    int type = _cppMembers->selectedObjectType;
    switch (type) {
        case UnknownType:
            return @"Unknown";
            break;
        case Star:
            return @"Star";
            break;
        case Galaxy:
            return @"Galaxy";
            break;
        default:
            return @"";
            break;
    }
}

- (NSString *) getSelectedObjectMorphology {
    if (!_cppMembers->detector.statsWereSet) {
        [self logEvent:@"OpenCV: Could not get object property because stats were not set\n"];
        return @"";
    }
    
    int morphology = _cppMembers->selectedObjectMorphology;
    return [self getObjectMorphologyStringRepresentation: morphology];
}

- (NSString *) getObjectMorphologyStringRepresentation: (int) morphology {
    
    switch (morphology) {
        case UnknownMorph:
            return @"Unknown";
            break;
        case Edgeon:
            return @"Edge-on";
            break;
        case Elliptical:
            return @"Elliptical";
            break;
        case Spiral:
            return @"Spiral";
            break;
        case Merge:
            return @"Merge";
            break;
        default:
            return @"";
            break;
    }
}

- (int) getSelectedObjectArea {
    if (!_cppMembers->detector.statsWereSet) {
        [self logEvent:@"OpenCV: Could not get object property because stats were not set\n"];
        return 0;
    }
    
    return _cppMembers->selectedObjectArea;
}

- (NSPoint) getSelectedObjectLocation {
    if (!_cppMembers->detector.statsWereSet) {
        [self logEvent:@"OpenCV: Could not get object property because stats were not set\n"];
        return NSMakePoint(0, 0);
    }
    
    return NSMakePoint(_cppMembers->selectedObjectLocation.x, _cppMembers->selectedObjectLocation.y);
}

- (NSPoint) getSelectedObjectBBoxLocation {
    if (!_cppMembers->detector.statsWereSet) {
        [self logEvent:@"OpenCV: Could not get object property because stats were not set\n"];
        return NSMakePoint(0, 0);
    }
    
    return NSMakePoint(_cppMembers->selectedObjectBBoxLocation.x, _cppMembers->selectedObjectBBoxLocation.y);
}

- (NSSize) getSelectedObjectBBoxSize {
    if (!_cppMembers->detector.statsWereSet) {
        [self logEvent:@"OpenCV: Could not get object property because stats were not set\n"];
        return NSMakeSize(0, 0);
    }
    
    return NSMakeSize(_cppMembers->selectedObjectBBoxSize.width, _cppMembers->selectedObjectBBoxSize.height);
}

// DETECTED OBJECTS FILTERS

- (void) setObjectAreaFilter: (int) area {
    _cppMembers->objectArea = area;
}

- (void) setDetectedObjectsFilter: (bool) isEnabled {
    _cppMembers->detectedObjectsFilter = isEnabled;
}

- (void) setStarFilter: (bool) isEnabled {
    _cppMembers->starFilter = isEnabled;
}

- (void) setGalaxyFilter: (bool) isEnabled {
    _cppMembers->galaxyFilter = isEnabled;
}

- (void) setGalaxyClassificationFilter: (bool) isEnabled {
    _cppMembers->galaxyClassificationFilter = isEnabled;
}

// SONIFICATION FILTERS

- (void) setMysteriousFactor: (int) factor {
    
    float defaultFactorValue = 0.03;
    
    float objectSoundDurationFactor = defaultFactorValue;
    
    switch (factor) {
        case 0:
            objectSoundDurationFactor = defaultFactorValue;
            break;
        case 1:
            objectSoundDurationFactor = 0.02;
            break;
        case 2:
            objectSoundDurationFactor = 0.01;
            break;
            
        default:
            return;
    }
    
    _cppMembers->sonifier.setObjectSoundDurationFactor(objectSoundDurationFactor);
}

- (void) setEuphoriaFactor: (int) factor {
    
    int defaultMaxFrequency = 900;
    
    int maxSoundFrequency = defaultMaxFrequency;
    
    switch (factor) {
        case 0:
            maxSoundFrequency = 600;
            break;
        case 1:
            maxSoundFrequency = 800;
            break;
        case 2:
            maxSoundFrequency = defaultMaxFrequency;
            break;
            
        default:
            return;
    }
    
    _cppMembers->sonifier.setMaxSoundFrequency(maxSoundFrequency);
}

- (void) setReverbFactor: (int) factor {
    
    int defaultReverbIterations = 1;
    
    int reverbIterations = defaultReverbIterations;
    
    switch (factor) {
        case 0:
            reverbIterations = 0;
            break;
        case 1:
            reverbIterations = 1;
            break;
        case 2:
            reverbIterations = 3;
            break;
            
        default:
            return;
    }
    
    _cppMembers->sonifier.setReverbIterations(reverbIterations);
}

// BUNDLED IMAGE PATHS

- (NSString *) getTemplatesPath {
    
    NSString *resourcePathObjC = [[NSBundle mainBundle] resourcePath];
    string templatesPath = string([resourcePathObjC UTF8String]) + "/templates";
    
    return [NSString stringWithCString:templatesPath.c_str() encoding:[NSString defaultCStringEncoding]];
}

- (NSString *) getStarTemplatesPath {
    
    NSString *resourcePathObjC = [[NSBundle mainBundle] resourcePath];
    string templatesPath = string([resourcePathObjC UTF8String]) + "/star_templates";
    
    return [NSString stringWithCString:templatesPath.c_str() encoding:[NSString defaultCStringEncoding]];
}

- (NSString *) getBinaryDetectionImagePath {
    
    NSString *resourcePathObjC = [[NSBundle mainBundle] resourcePath];
    string detectionImagePath = string([resourcePathObjC UTF8String]) + "/results/detection_image_binary.jpg";
    
    return [NSString stringWithCString:detectionImagePath.c_str() encoding:[NSString defaultCStringEncoding]];
}

- (NSString *) getDetectionImagePath {
    
    NSString *resourcePathObjC = [[NSBundle mainBundle] resourcePath];
    string detectionImagePath = string([resourcePathObjC UTF8String]) + "/results/detection_image.jpg";
    
    return [NSString stringWithCString:detectionImagePath.c_str() encoding:[NSString defaultCStringEncoding]];
}

- (NSString *) getSelectedObjectImagePath {
    
    NSString *resourcePathObjC = [[NSBundle mainBundle] resourcePath];
    string selectedObjectImagePath = string([resourcePathObjC UTF8String]) + "/results/selected_object_image.jpg";
    
    return [NSString stringWithCString:selectedObjectImagePath.c_str() encoding:[NSString defaultCStringEncoding]];
}

- (NSString *) getClassificationObjectImagePath {
    
    NSString *resourcePathObjC = [[NSBundle mainBundle] resourcePath];
    string selectedObjectImagePath = string([resourcePathObjC UTF8String]) + "/results/classification_object_image.jpg";
    
    return [NSString stringWithCString:selectedObjectImagePath.c_str() encoding:[NSString defaultCStringEncoding]];
}

- (NSString *) getSelectedObjectBBoxImagePath {
    NSString *resourcePathObjC = [[NSBundle mainBundle] resourcePath];
    string selectedObjectBBoxImagePath = string([resourcePathObjC UTF8String]) + "/results/selected_object_bbox_image.jpg";
    
    return [NSString stringWithCString:selectedObjectBBoxImagePath.c_str() encoding:[NSString defaultCStringEncoding]];
}

// BUNDLED AV PATHS

- (NSURL *) getCombinedAudioVideoURL {
    return [[[NSBundle mainBundle] resourceURL]  URLByAppendingPathComponent:@"results/combinedAudioVideo.mp4"];
}

- (NSString *) getAudioPath {
    NSString *audioPathC = [[NSBundle mainBundle] resourcePath];
    string audioPath = string([audioPathC UTF8String]) + "/results/audio.wav";
    
    return [NSString stringWithCString:audioPath.c_str() encoding:[NSString defaultCStringEncoding]];
}

- (NSString *) getVideoPath {
    NSString *videoPathC = [[NSBundle mainBundle] resourcePath];
    string videoPath = string([videoPathC UTF8String]) + "/results/video.mp4";
    
    return [NSString stringWithCString:videoPath.c_str() encoding:[NSString defaultCStringEncoding]];
}

- (NSURL *) getAudioURL {
    return [[[NSBundle mainBundle] resourceURL]  URLByAppendingPathComponent:@"results/audio.wav"];
}

- (NSURL *) getVideoURL {
    return [[[NSBundle mainBundle] resourceURL]  URLByAppendingPathComponent:@"results/video.mp4"];
}

// BUNDLED FILES PATHS

- (NSString *) getObjectsFilePath {
    NSString *resourcePathObjC = [[NSBundle mainBundle] resourcePath];
    string objectsFilePath = string([resourcePathObjC UTF8String]) + "/results/objects.txt";
    
    return [NSString stringWithCString:objectsFilePath.c_str() encoding:[NSString defaultCStringEncoding]];
}

// UTILITIES

- (void) checkAndResizeImage: (Mat) inputImage outputImage: (Mat*) outputImage maxCols: (int) maxCols {
    
    int rows = inputImage.rows;
    int cols = inputImage.cols;
    if (cols > 3000) {
        int offset = cols - 3000;
        cv::Size resizedImageSize = cv::Size(0,0);
        resizedImageSize.width = cols - offset;
        resizedImageSize.height = rows - offset;
        resize(inputImage, *outputImage, resizedImageSize, 0, 0, INTER_AREA);
    }
}

- (NSString *) getResultsPath {
    
    NSString *resourcePathObjC = [[NSBundle mainBundle] resourcePath];
    string resultsPath = string([resourcePathObjC UTF8String]) + "/results";
    
    return [NSString stringWithCString:resultsPath.c_str() encoding:[NSString defaultCStringEncoding]];
}

- (NSString *) getLogIdentifier {
    string logIdentifier = "LOGGER: ";
    return [NSString stringWithCString:logIdentifier.c_str() encoding:[NSString defaultCStringEncoding]];
}

- (void) logEvent: (NSString *) event {
    string logId = string([[self getLogIdentifier] UTF8String]);
    string eventString = string([event UTF8String]);
    cout << logId << eventString;
}

// MAIN FUNCTIONALITY

- (bool) detectObjects: (NSString *) imagePath {
    
    string path = string([imagePath UTF8String]);
    Mat orgImage = imread(path, IMREAD_COLOR);
    checkAndResizeImage(orgImage, orgImage, 3000);

    if (!orgImage.data) {
        [self logEvent:@"OpenCV: Could not open or find the image\n"];
        return false;
    }
    
    _cppMembers->classification.clear();
    _cppMembers->classesWereSet = false;
    _cppMembers->morphology.clear();
    
    string templatesPath = string([[self getTemplatesPath] UTF8String]);
    _cppMembers->detector.setTemplatesPath(templatesPath);
    
    [self updateDetectorParameters];

    // Detect objects in image
    Mat binaryDetectionImage = _cppMembers->detector.detect(orgImage, localDetectionEnabled, deblendingEnabled);
    _cppMembers->detector.setDetectedObjectsStatistics();
    
    // Save binary detection image
    string binaryDetectionImagePath = string([[self getBinaryDetectionImagePath] UTF8String]);
    imwrite(binaryDetectionImagePath, binaryDetectionImage);
    
    // Set min and max object areas.
    _cppMembers->minObjectArea = orgImage.rows * orgImage.cols;
    _cppMembers->maxObjectArea = 0;
    
    int count = 0;
    for (int label = 1; label < _cppMembers->detector.getStats().size[0]; label++) {
        count++;
        int area = _cppMembers->detector.getStats().at<int>(label, 4);
        
        if (area < _cppMembers->minimumObjectAreaForDrawing) {
            continue;
        }
 
        if (area > _cppMembers->maxObjectArea) {
            _cppMembers->maxObjectArea = area;
        }
        
        if (area < _cppMembers->minObjectArea) {
            _cppMembers->minObjectArea = area;
        }
    }
    
    cout << "NUMBER OF OBJECTS: " << count << endl;
    
    [self setObjectAreaFilter: _cppMembers->minObjectArea];
    
    return true;
}

- (bool) drawObjects {
    
    if (!_cppMembers->detector.statsWereSet) {
        [self logEvent:@"OpenCV: Could not open or find the binary image for drawing\n"];
        return false;
    }
    
    if (starGalaxyClassificationEnabled && _cppMembers->classesWereSet == false) {
        [self logEvent:@"Classifying objects...\n"];
    }
    
    Mat detectionImage = _cppMembers->detector.getOriginalImage().clone();
    
    // Draw objects while checking for filters
    if (_cppMembers->detectedObjectsFilter == true) {
        
        ofstream objectsFile;
        string objectsFilePath = string([[self getObjectsFilePath] UTF8String]);
        objectsFile.open(objectsFilePath);
        
        for (int label = 1; label < _cppMembers->detector.getStats().size[0]; label++) {
            
            int area = _cppMembers->detector.getStats().at<int>(label, 4);
            
            if (area < _cppMembers->minimumObjectAreaForDrawing) {
                continue;
            }
            
            // Min object area filter
            if (area < _cppMembers->objectArea) {
                continue;
            }
            
            // Bounding box property
            float bboxCoordX = _cppMembers->detector.getStats().at<int>(label, 0);
            float bboxCoordY = _cppMembers->detector.getStats().at<int>(label, 1); // upper-left corner
            float bboxWidth = _cppMembers->detector.getStats().at<int>(label, 2);
            float bboxHeight = _cppMembers->detector.getStats().at<int>(label, 3);
            
            cv::Rect boundingBox(bboxCoordX, bboxCoordY, bboxWidth, bboxHeight);
        
            // Cropping box (2x bigger than bounding box)
            int cboxSideSize = max(bboxWidth, bboxHeight);
            int cboxCoordX = max(0, (int) bboxCoordX - cboxSideSize / 2);
            int cboxCoordY = max(0, (int) bboxCoordY - cboxSideSize / 2);
            int cboxWidth = cboxSideSize * 2;
            if (cboxCoordX + cboxWidth > detectionImage.cols - 1) {
                cboxWidth -= ((cboxCoordX + cboxWidth) - (detectionImage.cols - 1));
            }
            int cboxHeight = cboxWidth;
            if (cboxCoordY + cboxHeight > detectionImage.rows - 1) {
                cboxHeight -= ((cboxCoordY + cboxHeight) - (detectionImage.rows - 1));
            }
            cv::Rect croppingBox(cboxCoordX, cboxCoordY, cboxWidth, cboxHeight);
            
            // Can either select the object by the bounding box or the bigger cropping box
            Mat objectImage = detectionImage(boundingBox).clone();
            //Mat objectImage = detectionImage(croppingBox).clone();
            
            // Save object image for classification
            string classificationObjectImagePath = string([[self getClassificationObjectImagePath] UTF8String]);
            imwrite(classificationObjectImagePath, objectImage);
            
            if (starGalaxyClassificationEnabled && _cppMembers->classesWereSet == false) {
                
                // Star Galaxy Classification
                
                int type = UnknownType;
                
                NSImage *classificationObjectImage = [[NSImage alloc] initWithContentsOfFile:[self getClassificationObjectImagePath]];
                
                if (_cppMembers->classificationMethod == TemplateMatching) {
                    
                    string templatesPath = string([[self getTemplatesPath] UTF8String]);
                    double galaxyTemplateMatchingScore = galaxyTemplateMatching(objectImage, templatesPath, 8);
                    
                    if (galaxyTemplateMatchingScore > 0.5) {
                        type = Galaxy;
                    } else {
                        type = Star;
                    }
                    
                } else { // Machine Learning
                    
                    NSString *sgcModelResult = [sgcModel classify: classificationObjectImage];
                    
                    if ([sgcModelResult isEqualToString:@"galaxy"]) {
                        type = Galaxy;
                    } else {
                        type = Star;
                    }
                }
            
                // Galaxy morphology classification
                if (galaxyMorphologyClassificationEnabled) {
                    
                    if (type == Galaxy) {
                        
                        _cppMembers->classification[label] = Galaxy;
                        
                        int morphology = UnknownMorph;
                        
                        NSString *gmcModelResult = [gmcModel classify: classificationObjectImage];
                        
                        if ([gmcModelResult isEqualToString:@"spiral"]) {
                            morphology = Spiral;
                        } else if ([gmcModelResult isEqualToString:@"elliptical"]) {
                            morphology = Elliptical;
                        } else if ([gmcModelResult isEqualToString:@"merge"]) {
                            morphology = Merge;
                        } else {
                            morphology = Edgeon;
                        }
                        
                        _cppMembers->morphology[label] = morphology;
                        
                    } else {
                        _cppMembers->classification[label] = Star;
                    }
                }
        
            }
            
            // Star filter
            if (_cppMembers->starFilter == false && _cppMembers->classification[label] == Star) {
                continue;
            }
            
            // Galaxy filter
            if (_cppMembers->galaxyFilter == false && _cppMembers->classification[label] == Galaxy) {
                continue;
            }
            
            // Galaxy classification filter
            if (_cppMembers->galaxyClassificationFilter == true && _cppMembers->classification[label] == Galaxy) {
                
                string morphology = string([[self getObjectMorphologyStringRepresentation:_cppMembers->morphology[label]] UTF8String]);
                
                // Write the morphology of each galaxy on the image
                putText(detectionImage, morphology, cv::Point(bboxCoordX - bboxWidth / 2, bboxCoordY),
                        FONT_HERSHEY_DUPLEX, 3, cv::Scalar(255,255,255), 2);
            }
            
            // Download objects file option
            if (_cppMembers->classification[label] == Galaxy) {
                
                string morphology = string([[self getObjectMorphologyStringRepresentation:_cppMembers->morphology[label]] UTF8String]);
                
                int x = bboxCoordX + bboxWidth / 2;
                int y = bboxCoordY + bboxHeight / 2;
                
                objectsFile << "galaxy " << morphology << " " << area << " " << x << " " << y << " " << bboxWidth << " " << bboxHeight << "\n";
            } else {
                
                int x = bboxCoordX + bboxWidth / 2;
                int y = bboxCoordY + bboxHeight / 2;
                
                objectsFile << "star " << "unknown" << " " << area << " " << x << " " << y << " " << bboxWidth << " " << bboxHeight << "\n";
            }
            
            // Draw bounding box around current object
            rectangle(detectionImage, boundingBox, cv::Scalar(0, 255, 0), 2);
        }
        
        objectsFile.close();
        _cppMembers->classesWereSet = true;
    }
    
    string detectionImagePath = string([[self getDetectionImagePath] UTF8String]);
    imwrite(detectionImagePath, detectionImage);
    
    return true;
}

- (bool) drawSelectedObject: (int) xCoord and: (int) yCoord {
    
    if (!_cppMembers->detector.statsWereSet) {
        [self logEvent:@"OpenCV: Could not open or find the binary image for drawing\n"];
        return false;
    }
    
    Mat originalImage = _cppMembers->detector.getOriginalImage().clone();
    Mat selectedObjectImage;

    int label = _cppMembers->detector.getLabeledImage().at<int>(yCoord, xCoord);
    if (label == 0) { // i.e. if background was selected
        return false;
    }
    
    // Bounding box
    int bboxCoordX = _cppMembers->detector.getStats().at<int>(label, 0);
    int bboxCoordY = _cppMembers->detector.getStats().at<int>(label, 1); // upper-left corner
    int bboxWidth = _cppMembers->detector.getStats().at<int>(label, 2);
    int bboxHeight = _cppMembers->detector.getStats().at<int>(label, 3);
    
    // Cropping box (2x bigger than bounding box)
    int cboxSideSize = max(bboxWidth, bboxHeight);
    int cboxCoordX = max(0, bboxCoordX - cboxSideSize / 2);
    int cboxCoordY = max(0, bboxCoordY - cboxSideSize / 2);
    int cboxWidth = cboxSideSize * 2;
    if (cboxCoordX + cboxWidth > originalImage.cols - 1) {
        cboxWidth -= ((cboxCoordX + cboxWidth) - (originalImage.cols - 1));
    }
    int cboxHeight = cboxWidth;
    if (cboxCoordY + cboxHeight > originalImage.rows - 1) {
        cboxHeight -= ((cboxCoordY + cboxHeight) - (originalImage.rows - 1));
    }
    
    cv::Rect croppingBox(cboxCoordX, cboxCoordY, cboxWidth, cboxHeight);
    selectedObjectImage = originalImage(croppingBox).clone();
    
    // Save selected object image.
    string selectedObjectImagePath = string([[self getSelectedObjectImagePath] UTF8String]);
    imwrite(selectedObjectImagePath, selectedObjectImage);
    
    // Save selected object image with bounding box.
    cv::Rect boundingBox(bboxCoordX, bboxCoordY, bboxWidth, bboxHeight);
    rectangle(originalImage, boundingBox, cv::Scalar(0, 255, 0), 2);
    selectedObjectImage = originalImage(croppingBox).clone();
    string selectedObjectBBoxImagePath = string([[self getSelectedObjectBBoxImagePath] UTF8String]);
    imwrite(selectedObjectBBoxImagePath, selectedObjectImage);
    
    // Compute object properties
    _cppMembers->selectedObjectArea = _cppMembers->detector.getStats().at<int>(label, 4);
    _cppMembers->selectedObjectLocation.x = bboxCoordX + bboxWidth / 2;
    _cppMembers->selectedObjectLocation.y = bboxCoordY + bboxHeight / 2;
    _cppMembers->selectedObjectBBoxLocation.x = bboxCoordX;
    _cppMembers->selectedObjectBBoxLocation.y = bboxCoordY;
    _cppMembers->selectedObjectBBoxSize.width = bboxWidth;
    _cppMembers->selectedObjectBBoxSize.height = bboxHeight;
    _cppMembers->selectedObjectType = _cppMembers->classification[label];
    
    // Retrieve morphology if object is a galaxy
    if (_cppMembers->selectedObjectType == Galaxy) {
        _cppMembers->selectedObjectMorphology = _cppMembers->morphology[label];
    } else {
        _cppMembers->selectedObjectMorphology = UnknownMorph;
    }
    
    return true;
}

- (bool) sonify {
    
    if (!_cppMembers->detector.statsWereSet) {
        [self logEvent:@"OpenCV: Could not open or find the binary image for sonifying\n"];
        return false;
    }
    
    [self updateSonifierParameters];
    
    // Retrieve and save sound produced by the detected image
    vector<double> sound = _cppMembers->sonifier.sonify();
    string soundPath = string([[self getAudioPath] UTF8String]);
    int durationInSeconds = _cppMembers->sonifier.getDurationInSeconds(sound);
    _cppMembers->sonifier.saveAudio(sound, durationInSeconds, soundPath);
    
    return true;
}

- (bool) sonifyWithVideo {
    
    if (!_cppMembers->detector.statsWereSet) {
        [self logEvent:@"OpenCV: Could not open or find the binary image for sonifying\n"];
        return false;
    }
    
    [self updateSonifierParameters];
    
    // Retrieve and save sound produced by the detected image
    vector<double> sound = _cppMembers->sonifier.sonify();
    string soundPath = string([[self getAudioPath] UTF8String]);
    int durationInSeconds = _cppMembers->sonifier.getDurationInSeconds(sound);
    _cppMembers->sonifier.saveAudio(sound, durationInSeconds, soundPath);
    
    // Compute video
    string videoPath = string([[self getVideoPath] UTF8String]);
    _cppMembers->sonifier.createAndSaveVideo(sound, durationInSeconds, videoPath);
    
    return true;
}

@end
