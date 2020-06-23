//
//  Sonifier.cpp
//  soniastro
//
//  Created by Ahmad Aussaili on 3/5/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

#include "Sonifier.hpp"

Sonifier::Sonifier() {
}

Sonifier::Sonifier(CombinedDetection &detector) {
    this->detector = &detector;
}

void Sonifier::setDetector(CombinedDetection &detector) {
    this->detector = &detector;
}

void Sonifier::logEvent(string event) {
    cout << logIdentifier << event;
}

void Sonifier::setObjectSoundDurationFactor(float value) {
    this->objectSoundDurationFactor = value;
}

void Sonifier::setMaxSoundFrequency(int freq){
    this->maximumSoundFreq = freq;
}

vector<double> Sonifier::sonify() {
    
    // Validation
    
    if (detector == NULL) {
        
        logEvent("Could not sonify image - no detector set.\n");
        
        vector<double> emptyVector;
        return emptyVector;
        
    } else if (!detector->statsWereSet) {
        
        logEvent("Could not sonify image - detector stats not set.\n");
        
        vector<double> emptyVector;
        return emptyVector;
    }
    
    logEvent("Sonifying image...\n");
    
    Mat orgImage = detector->getOriginalImage().clone();
    
    // The sound duration
    int duration = orgImage.cols / (FPS * binSize); // in seconds (may want to play with this)
    int numOfBins = FPS * duration; // total number of frames
    int numOfSamples = samplingFrequency * duration;
    
    frequencies = Mat(maximumSoundFreq, numOfBins, CV_16S, Scalar(0));
    
    // The sound samples array, normalized to be between 0 and 1.
    vector<double> soundf(numOfSamples, attenuationFactor);
    
    // Assign a sound to each object.
    for (int label = 1; label < detector->getStats().size[0]; label++) {
        
        int area = detector->getStats().at<int>(label, 4);
        
        // Bounding box property
        float bboxCoordX = detector->getStats().at<int>(label, 0);
        float bboxCoordY = detector->getStats().at<int>(label, 1); // upper-left corner
        float bboxWidth = detector->getStats().at<int>(label, 2);
        float bboxHeight = detector->getStats().at<int>(label, 3);
        
        // Frequency is represented by y coordinate (changes from top to bottom, 25 to 600-900 Hertz, based on which filter is used in the app)
        int freq = maximumSoundFreq - (((bboxCoordY + bboxHeight/2) / (orgImage.rows - 1) ) * (maximumSoundFreq - minimumSoundFreq));
        
        // Compute diameter of object
        Point topLeft = Point(bboxCoordX, bboxCoordY);
        Point bottomRight = Point(bboxCoordX + bboxWidth, bboxCoordY + bboxHeight);
        float diameter = sqrt(pow(topLeft.x - bottomRight.x, 2) + pow(topLeft.y - bottomRight.y, 2));
        
        // Compute sound duration
        float soundDuration = diameter / ((orgImage.cols - 1) * objectSoundDurationFactor);
        
        // Store the frequency magnitude of this object
        
        int frequencyMagnitude = bboxWidth + (0.01 / objectSoundDurationFactor) * 50;
        int objectBin = bboxCoordX / binSize;
        int objectLastBin = (bboxCoordX + bboxWidth) / binSize;
        int numOfObjectBins = objectLastBin - objectBin + 1;
        
        int currentScheduledBin = numOfObjectBins;
        for (int bin = objectBin; bin <= objectLastBin; bin++) {
            frequencies.at<short>(freq - 1, bin) = (frequencyMagnitude * 1.0 / numOfObjectBins) * currentScheduledBin;
            currentScheduledBin--;
            
            // skip lines (to create the spikes which have the base as wide as the object)
            int stopSkipFreq = maximumSoundFreq - ((bboxCoordY / (orgImage.rows - 1) ) * (maximumSoundFreq - minimumSoundFreq)) - 1;
            int startSkipFreq = maximumSoundFreq - (((bboxCoordY + bboxHeight) / (orgImage.rows - 1) ) * (maximumSoundFreq - minimumSoundFreq)) - 1;

            for (int frequency = startSkipFreq; frequency <= stopSkipFreq; frequency++) {
                if (frequency != freq - 1) {
                    frequencies.at<short>(frequency, bin) = -1;
                }
            }
        }
        
        // Produce sound
        
        int amplitude = 0;
        
        // Universal amplitude: amplitude depends less on the size of the object
        if (universalAmplitude) {
            
            if (freq > maximumSoundFreq / 3) {
                amplitude = 8000;
            } else {
                amplitude = 12000;
            }
            
        } else {
            
            if (freq > maximumSoundFreq / 2) {
                amplitude = diameter * (amplitudeFactor / 8);
            } else {
                amplitude = diameter * amplitudeFactor;
            }
        }

        
        // Nr of samples must be even
        int bufferSize = soundDuration * samplingFrequency;
        
        int sampleIndexOffset = bboxCoordX * (int) ((numOfSamples) / (orgImage.cols - 1));
        
        int lastSampleIndex = sampleIndexOffset + bufferSize;
        if (lastSampleIndex > samplingFrequency * duration) {
            bufferSize -= (lastSampleIndex - (numOfSamples));
        }
        
        if (bufferSize < 0) {
            bufferSize = 0;
        }
        
        if (bufferSize % 2 == 1) {
            bufferSize--;
        }
        
        for(int j = 0; j < bufferSize; ++j) {
            
            int sample = amplitude * sin( 2.f * float(M_PI) * freq * j / samplingFrequency );
            
            double a0Coeff = 0.6;
            double hannWindowMultiplier = a0Coeff - (1 - a0Coeff) * cos(2*M_PI*j/(bufferSize - 1));
            double sineWindowMultiplier = sin(M_PI*j/(bufferSize - 1));
            
            sample = hannWindowMultiplier * sample;
            // Can also apply a sinw window:
            //sample = sineWindowMultiplier * sample;
            int soundSampleIndex = sampleIndexOffset + j;
            soundf.at(soundSampleIndex) += sample;
        }
    }
    
    // Normalise the sample array between 0 and 1
    soundf = normalizeVector(soundf, 1);
    
    // Add reverb
    
    for (int iteration = 0; iteration <= this->reverbIterations; iteration++) {
        soundf = reverb(soundf, numOfSamples, delayinMilliSeconds, decayFactor, mixFactor);
    }
    
    for (int i = 0; i < soundf.size(); i++) {
        double a0Coeff = 0.6;
        double hannWindowMultiplier = a0Coeff - (1 - a0Coeff) * cos(2*M_PI*i/(soundf.size() - 1));
        soundf.at(i) *= hannWindowMultiplier;
    }
    
    logEvent("Sound produced.\n");
    logEvent("Sonification done.\n");
    
    return soundf;
}

void Sonifier::saveAudio(vector<double> sound, int duration, string path) {
    
    logEvent("Saving audio...\n");
    
    AudioFile<double> audioFile = AudioFile<double>();
    
    AudioFile<double>::AudioBuffer buffer;
    
    buffer.resize (2);
    buffer[0].resize (samplingFrequency * duration);
    buffer[1].resize (samplingFrequency * duration);
    int numChannels = 2;
    int numSamplesPerChannel = samplingFrequency * duration;
    
    for (int i = 0; i < numSamplesPerChannel; i++)
    {
        for (int channel = 0; channel < numChannels; channel++)
            buffer[channel][i] = sound.at(i);
    }
    
    audioFile.setAudioBuffer (buffer);
    
    // Set both the number of channels and number of samples per channel
    audioFile.setAudioBufferSize (numChannels, numSamplesPerChannel);
    
    // Set the number of samples per channel
    audioFile.setNumSamplesPerChannel (numSamplesPerChannel);
    
    // Set the number of channels
    audioFile.setNumChannels (numChannels);
    
    audioFile.setBitDepth (16);
    audioFile.setSampleRate (samplingFrequency);
    
    audioFile.save (path, AudioFileFormat::Wave);
    
    logEvent("Audio saved.\n");
}

void Sonifier::createAndSaveVideo(vector<double> &sound, int duration, string path) {
    
    if (detector == NULL) {
        
        logEvent("Could not create video - no detector set.\n");
        
        return;
    }
    
    logEvent("Producing video...\n");
    
    Mat orgImage = detector->getOriginalImage().clone();
    
    Size frameSize = Size(0, 0);
    frameSize.width = orgImage.cols;
    frameSize.height = orgImage.rows;
    
    VideoWriter out(path, VideoWriter::fourcc('m', 'p', '4', 'v'), 25, frameSize, true);
    
    if (!out.isOpened()) {
        logEvent("Could not open video for writing \n");
        return;
    }
    
    int numOfBins = FPS * duration;
    
    for (int i = 0; i < numOfBins; i++) {
        Mat frameImage = orgImage.clone();
        
        if (i == numOfBins - 1) {
            line(frameImage, Point(i * binSize,0), Point(i * binSize, frameImage.rows), Scalar(255, 255, 255), 5);
            out.write(frameImage);
            break;
        }

        vector<double> magnitudes(maximumSoundFreq, 0);
        int count = 0;
        for (int j = 0; j < maximumSoundFreq; j++) {
            magnitudes[j] = frequencies.at<short>(j, i); // freq, bin
        }
        
        normalizeVector(magnitudes, 100);
        
        vector<cv::Point> magPoints;
        vector<cv::Point> magPoints2;
        
        int maxFreqIndex = maximumSoundFreq - 1;
        int minFreqIndex = minimumSoundFreq - 1;
        for (int j = maxFreqIndex; j >= minFreqIndex; j--) { // from 25 to 800 Hertz
            float stepSize = (orgImage.rows - 1) * 1.0 / (maxFreqIndex - minFreqIndex);
            int y = (maxFreqIndex - j) * stepSize;
            
            if (j == minFreqIndex && y < (orgImage.rows - 1)) {
                y = (orgImage.rows - 1);
            }
            
            // The following can be used with a FFT method
            cv::Point point = cv::Point(y,magnitudes[j]); // invert the x,y coordinates
            magPoints.push_back(point);
            
            // The following can be used without a FFT method
            if (magnitudes[j] >= 0) {
                cv::Point point2 = cv::Point((i * binSize) + magnitudes[j], y);
                magPoints2.push_back(point2);
            }
        }
        
        // Plot the frequency magnitudes at this frame
        for (int j = 0; j < magPoints2.size() - 1; j++) {
            Point point1 = magPoints2[j];
            Point point2 = magPoints2[j+1];
            line(frameImage, point1, point2, Scalar(255,255,255), 5);
        }
        
        out.write(frameImage);
    }
    
    logEvent("Video produced.\n");
}

int Sonifier::getDurationInSeconds(vector<double> sound) {
    return sound.size() / samplingFrequency;
}


void Sonifier::setSamplingFrequency(float value) {
    this->samplingFrequency = value;
    binSamples = samplingFrequency / FPS;
}

void Sonifier::setFPS(int value) {
    this->FPS = value;
    binSamples = samplingFrequency / FPS;
}

void Sonifier::setbinSize(int value) {
    this->binSize = value;
}

void Sonifier::setUniversalAmplitude(bool value) {
    this->universalAmplitude = value;
}

void Sonifier::setAmplitudeFactor(float value) {
    this->amplitudeFactor = value;
}

void Sonifier::setAttenuationFactor(float value) {
    this->attenuationFactor = value;
}

void Sonifier::setDelayInMilliSeconds(float value) {
    this->delayinMilliSeconds = value;
}

void Sonifier::setDecayFactor(float value){
    this->decayFactor = value;
}

void Sonifier::setMixFactor(int value) {
    this->mixFactor = value;
}

void Sonifier::setReverbIterations(int value) {
    this->reverbIterations = value;
}
