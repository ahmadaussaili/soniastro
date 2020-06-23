//
//  AstroDetector.h
//  soniastro
//
//  Created by Ahmad Aussaili on 2/16/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <CoreML/CoreML.h>
#import <Vision/Vision.h>
#import "GlobalDetection.hpp"
#import "CombinedDetection.hpp"
#import "Sonifier.hpp"

// Star Galaxy Classifier
@class SGCModel;
// Galaxy Morphology Classifier
@class GMCModel;

NS_ASSUME_NONNULL_BEGIN

struct CPPMembers;

@interface AstroDetector : NSObject {
    // opaque pointer to store cpp members
    struct CPPMembers *_cppMembers;
    SGCModel *sgcModel;
    GMCModel *gmcModel;
}

// Star-Galaxy enum
typedef NS_ENUM(NSUInteger, ObjectType) {
    UnknownType, Star, Galaxy
};

// Galaxy morphology enum
typedef NS_ENUM(NSUInteger, ObjectMorphology) {
    UnknownMorph, Edgeon, Elliptical, Spiral, Merge
};

// Star-Galaxy Classification method
// Option 1: Template Matching
// Option 2: Machine Learning
typedef NS_ENUM(NSUInteger, ClassificationMethod) {
    TemplateMatching, ML
};

/// Main Detection Properties

// getters

+ (bool) isLocalDetectionEnabled;

+ (bool) isDeblendingEnabled;

+ (bool) isStarGalaxyClassificationEnabled;

+ (bool) isGalaxyMorphologyClassificationEnabled;

// setters

+ (void) setLocalDetectionEnabled: (bool) enabled;

+ (void) setDeblendingEnabled: (bool) enabled;

+ (void) setStarGalaxyClassificationEnabled: (bool) enabled;

+ (void) setGalaxyMorphologyClassificationEnabled: (bool) enabled;

/// Global Detection Properties

// getters

+ (int) getGlobalDetectionKernelSize;

+ (float) getGlobalDetectionStdDevSigma;

+ (int) getGlobalDetectionLocalAreaExpansion;

+ (int) getGlobalDetectionMinCleaningObjSize;

+ (int) getGlobalDetectionCleaningIterations;

+ (float) getGlobalDetectionBGSubScalingFactor;

// setters

+ (void) setGlobalDetectionKernelSize: (int) value;

+ (void) setGlobalDetectionStdDevSigma: (float) value;

+ (void) setGlobalDetectionLocalAreaExpansion: (int) value;

+ (void) setGlobalDetectionMinCleaningObjSize: (int) value;

+ (void) setGlobalDetectionCleaningIterations: (int) value;

+ (void) setGlobalDetectionBGSubScalingFactor: (float) value;

/// Local Detection Properties

// getters

+ (int) getLocalDetectionKernelSize;

+ (float) getLocalDetectionStdDevSigma;

+ (int) getLocalDetectionLocalAreaExpansion;

+ (int) getLocalDetectionMinCleaningObjSize;

+ (int) getLocalDetectionCleaningIterations;

+ (float) getLocalDetectionBGSubScalingFactor;

+ (int) getLocalDetectionNumOfGlobalObjects;

+ (int) getLocalDetectionSigmoidSlope;

+ (int) getLocalDetectionSigmoidCenterParam;

+ (int) getLocalDetectionLayeredDetectionThreshold;

// setters

+ (void) setLocalDetectionKernelSize: (int) value;

+ (void) setLocalDetectionStdDevSigma: (float) value;

+ (void) setLocalDetectionLocalAreaExpansion: (int) value;

+ (void) setLocalDetectionMinCleaningObjSize: (int) value;

+ (void) setLocalDetectionCleaningIterations: (int) value;

+ (void) setLocalDetectionBGSubScalingFactor: (float) value;

+ (void) setLocalDetectionNumOfGlobalObjects: (int) value;

+ (void) setLocalDetectionSigmoidSlope: (int) value;

+ (void) setLocalDetectionSigmoidCenterParam: (int) value;

+ (void) setLocalDetectionLayeredDetectionThreshold: (int) value;

// Sonification Properties

// getters

+ (float) getSamplingFrequency;

+ (int) getFPS;

+ (int) getbinSize;

+ (bool) getUniversalAmplitude;

+ (float) getAmplitudeFactor;

+ (float) getAttenuationFactor;

+ (float) getDelayInMilliSeconds;

+ (float) getDecayFactor;

+ (int) getMixFactor;

+ (int) getReverbIterations;

// setters

+ (void) setSamplingFrequency: (float) value;

+ (void) setFPS: (int) value;

+ (void) setbinSize: (int) value;

+ (void) setUniversalAmplitude: (bool) value;

+ (void) setAmplitudeFactor: (float) value;

+ (void) setAttenuationFactor: (float) value;

+ (void) setDelayInMilliSeconds: (float) value;

+ (void) setDecayFactor: (float) value;

+ (void) setMixFactor: (int) value;

+ (void) setReverbIterations: (int) value;

/// Bundled Files paths

- (NSString *) getObjectsFilePath;

/// Bundled Image paths

- (NSString *) getDetectionImagePath;

- (NSString *) getSelectedObjectImagePath;

- (NSString *) getSelectedObjectBBoxImagePath;

/// Bundled AV paths

- (NSString *) getAudioPath;

- (NSString *) getVideoPath;

- (NSURL *) getAudioURL;

- (NSURL *) getVideoURL;

- (NSURL *) getCombinedAudioVideoURL;

/// Selected Object Properties

- (int) getMaxObjectArea;
- (int) getMinObjectArea;

- (NSString *) getSelectedObjectType;

- (NSString *) getSelectedObjectMorphology;

- (int) getSelectedObjectArea;

- (NSPoint) getSelectedObjectLocation;

- (NSPoint) getSelectedObjectBBoxLocation;

- (NSSize) getSelectedObjectBBoxSize;

/// Detected Objects Filters:

- (void) setDetectedObjectsFilter: (bool) isEnabled;

- (void) setStarFilter: (bool) isEnabled;

- (void) setGalaxyFilter: (bool) isEnabled;

- (void) setGalaxyClassificationFilter: (bool) isEnabled;

- (void) setObjectAreaFilter: (int) area;

/// Sonification Filters

- (void) setMysteriousFactor: (int) factor;

- (void) setEuphoriaFactor: (int) factor;

- (void) setReverbFactor: (int) factor;

/// Utilities

- (NSString *) getLogIdentifier;

/// Main Functionality

// Detects the astronomical objects and computes their statistics.
// Results are saved in the detector object.
- (bool) detectObjects: (NSString *) imagePath;

// Loads an image with bounding boxes around the detected objects.
- (bool) drawObjects;

// Draws the selected object in the object panel
- (bool) drawSelectedObject: (int) xCoord and: (int) yCoord;

// Produces sound from the detected image
- (bool) sonify;

// Produces sound and video from the detected image
- (bool) sonifyWithVideo;

@end

NS_ASSUME_NONNULL_END
