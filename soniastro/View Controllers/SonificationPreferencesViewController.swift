
//
//  SonificationPreferencesViewController.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 3/23/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

import Cocoa

class SonificationPreferencesViewController: NSViewController, NSTextFieldDelegate {

    // OUTLETS
    
    // AV PANEL
    
    @IBOutlet weak var AVContainerView: NSView!
    
    @IBOutlet weak var samplingFreqField: NSTextField!
    @IBOutlet weak var binSizeField: NSTextField!
    @IBOutlet weak var FPSField: NSTextField!
    @IBOutlet weak var universalAmplitudeButton: NSButton!
    @IBOutlet weak var amplitudeFactorField: NSTextField!
    @IBOutlet weak var attenuationFactorField: NSTextField!
    
    // REVERB
    
    @IBOutlet weak var reverbContainerView: NSView!
    
    @IBOutlet weak var delayField: NSTextField!
    @IBOutlet weak var decayFactorField: NSTextField!
    @IBOutlet weak var mixFactorField: NSTextField!
    @IBOutlet weak var reverbIterationsField: NSTextField!
    
    
    // ACTIONS
    
    // AV PANEL
    @IBAction func universalAmplitudeButtonPressed(_ sender: NSButton) {
        
        var enabled = false;
        
        if sender.state == .on {
            enabled = true
        }
        
        AstroDetector.setUniversalAmplitude(enabled)
    }
    
    func samplingFrequencyChanged(_ sender: NSTextField) {
        AstroDetector.setSamplingFrequency(sender.floatValue)
    }
    
    func binSizeChanged(_ sender: NSTextField) {
        AstroDetector.setbinSize(sender.intValue)
    }
    
    func FPSChanged(_ sender: NSTextField) {
        AstroDetector.setFPS(sender.intValue)
    }
    
    func amplitudeFactorChanged(_ sender: NSTextField) {
        AstroDetector.setAmplitudeFactor(sender.floatValue)
    }
    
    func attenuationFactorChanged(_ sender: NSTextField) {
        AstroDetector.setAttenuationFactor(sender.floatValue)
    }
    
    // REVERB
    
    func delayChanged(_ sender: NSTextField) {
        AstroDetector.setDelayInMilliSeconds(sender.floatValue)
    }
    
    func decayFactorChanged(_ sender: NSTextField) {
        AstroDetector.setDecayFactor(sender.floatValue)
    }
    
    func mixFactorChanged(_ sender: NSTextField) {
        AstroDetector.setMixFactor(sender.intValue)
    }
    
    func reverbIterationsChanged(_ sender: NSTextField) {
        AstroDetector.setReverbIterations(sender.intValue)
    }
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
        
        self.view.wantsLayer = true
        self.view.layer?.contents = NSImage(named: "dark_gray")
        
        // AV PANEL
        
        AVContainerView.wantsLayer = true
        AVContainerView.layer?.contents = NSImage(named: "button")
        AVContainerView.layer?.cornerRadius = 5
        
        samplingFreqField.floatValue = AstroDetector.getSamplingFrequency()
        samplingFreqField.delegate = self
        
        binSizeField.intValue = AstroDetector.getbinSize()
        binSizeField.delegate = self
        
        FPSField.intValue = AstroDetector.getFPS()
        FPSField.delegate = self
        
        universalAmplitudeButton.state = AstroDetector.getUniversalAmplitude() ? .on : .off
        
        amplitudeFactorField.floatValue = AstroDetector.getAmplitudeFactor()
        amplitudeFactorField.delegate = self
        
        attenuationFactorField.floatValue = AstroDetector.getAttenuationFactor()
        attenuationFactorField.delegate = self
        
        // REVERB
        
        reverbContainerView.wantsLayer = true
        reverbContainerView.layer?.contents = NSImage(named: "button")
        reverbContainerView.layer?.cornerRadius = 5
        
        delayField.floatValue = AstroDetector.getDelayInMilliSeconds()
        delayField.delegate = self
        
        decayFactorField.floatValue = AstroDetector.getDecayFactor()
        decayFactorField.delegate = self
        
        mixFactorField.intValue = AstroDetector.getMixFactor()
        mixFactorField.delegate = self
        
        reverbIterationsField.intValue = AstroDetector.getReverbIterations()
        reverbIterationsField.delegate = self
    }
    
    func controlTextDidChange(_ obj: Notification) {
        
        let textField = obj.object as! NSTextField
        
        switch textField {
            
            // AV PANEL
            
        case samplingFreqField:
            samplingFrequencyChanged(textField)
            break;
        case binSizeField:
            binSizeChanged(textField)
            break;
        case FPSField:
            FPSChanged(textField)
            break;
        case amplitudeFactorField:
            amplitudeFactorChanged(textField)
            break;
        case attenuationFactorField:
            attenuationFactorChanged(textField)
            break;
            
            // REVERB
            
        case delayField:
            delayChanged(textField)
            break;
        case decayFactorField:
            decayFactorChanged(textField)
            break;
        case mixFactorField:
            mixFactorChanged(textField)
            break;
        case reverbIterationsField:
            reverbIterationsChanged(textField)
            break;
            
        default:
            break;
        }
    }
    
}
