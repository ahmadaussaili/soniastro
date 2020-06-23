//
//  ObjectDetectionPreferencesViewController.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 3/23/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

import Cocoa

class ObjectDetectionPreferencesViewController: NSViewController, NSTextFieldDelegate {

    // OUTLETS
    
    // Detection
    
    @IBOutlet weak var detectionContainerView: NSView!
    
    @IBOutlet weak var localDetectionButton: NSButton!
    
    @IBOutlet weak var deblendingButton: NSButton!
    
    @IBOutlet weak var starGalaxyClassificationButton: NSButton!
    
    @IBOutlet weak var galaxyMorphologyClassificationButton: NSButton!
    
    // Global Detection
    
    @IBOutlet weak var globalDetectionContainerView: NSView!
    
    @IBOutlet weak var globalDetectionKernelSizeField: NSTextField!
    
    @IBOutlet weak var globalDetectionStdDevSigmaField: NSTextField!
    
    @IBOutlet weak var globalDetectionLocalAreaExpansionField: NSTextField!
    
    @IBOutlet weak var globalDetectionMinCleaningObjSizeField: NSTextField!
    
    @IBOutlet weak var globalDetectionCleaningIterationsField: NSTextField!
    
    @IBOutlet weak var globalDetectionBGSubScalingFactorField: NSTextField!
    
    // Local Detection
    
    @IBOutlet weak var localDetectionContainerView: NSView!
    
    @IBOutlet weak var localDetectionKernelSizeField: NSTextField!
    
    @IBOutlet weak var localDetectionStdDevSigmaField: NSTextField!
    
    @IBOutlet weak var localDetectionLocalAreaExpansionField: NSTextField!
    
    @IBOutlet weak var localDetectionMinCleaningObjSizeField: NSTextField!
    
    @IBOutlet weak var localDetectionCleaningIterationsField: NSTextField!
    
    @IBOutlet weak var localDetectionBGSubScalingFactorField: NSTextField!
    
    @IBOutlet weak var localDetectionNumOfGlobalObjectsField: NSTextField!
    
    @IBOutlet weak var localDetectionSigmoidSlopeField: NSTextField!
    
    @IBOutlet weak var localDetectionSigmoidCenterParamField: NSTextField!
    
    @IBOutlet weak var localDetectionLayeredDetectionThresholdField: NSTextField!
    
    // ACTIONS
    
    // Detection
    
    @IBAction func localDetectionButtonPressed(_ sender: NSButton) {
        
        var enabled = false;
        
        if sender.state == .on {
            enabled = true
        }
        
        AstroDetector.setLocalDetectionEnabled(enabled)
    }
    
    @IBAction func deblendingButtonPressed(_ sender: NSButton) {
        
        var enabled = false;
        
        if sender.state == .on {
            enabled = true
        }
        
        AstroDetector.setDeblendingEnabled(enabled)
    }
    
    @IBAction func starGalaxyClassificationButtonPressed(_ sender: NSButton) {
        
        var enabled = false;
        
        if sender.state == .on {
            enabled = true
            galaxyMorphologyClassificationButton.isEnabled = true
        } else {
            galaxyMorphologyClassificationButton.isEnabled = false
            galaxyMorphologyClassificationButton.state = .off
        }
        
        AstroDetector.setStarGalaxyClassificationEnabled(enabled)
    }
    
    @IBAction func galaxyMorphologyClassificationButtonPressed(_ sender: NSButton) {
        
        var enabled = false;
        
        if sender.state == .on {
            enabled = true
        }
        
        AstroDetector.setGalaxyMorphologyClassificationEnabled(enabled)
    }
    
    // Global Detection
    
    func globalDetectionKernelSizeChanged(_ sender: NSTextField) {
        
        AstroDetector.setGlobalDetectionKernelSize(sender.intValue)
    }
    
    func globalDetectionStdDevSigmaChanged(_ sender: NSTextField) {
        AstroDetector.setGlobalDetectionStdDevSigma(sender.floatValue)
    }
    
    func globalDetectionLocalAreaExpansionChanged(_ sender: NSTextField) {
        AstroDetector.setGlobalDetectionLocalAreaExpansion(sender.intValue)
    }
    
    func globalDetectionMinCleaningObjSizeChanged(_ sender: NSTextField) {
        AstroDetector.setGlobalDetectionMinCleaningObjSize(sender.intValue)
    }
    
    func globalDetectionCleaningIterationsChanged(_ sender: NSTextField) {
        AstroDetector.setGlobalDetectionCleaningIterations(sender.intValue)
    }
    
    func globalDetectionBGSubScalingFactorChanged(_ sender: NSTextField) {
        AstroDetector.setGlobalDetectionBGSubScalingFactor(sender.floatValue)
    }
    
    // Local Detection
    
    func localDetectionKernelSizeChanged(_ sender: NSTextField) {
        
        AstroDetector.setLocalDetectionKernelSize(sender.intValue)
    }
    
    func localDetectionStdDevSigmaChanged(_ sender: NSTextField) {
        AstroDetector.setLocalDetectionStdDevSigma(sender.floatValue)
    }
    
    func localDetectionLocalAreaExpansionChanged(_ sender: NSTextField) {
        AstroDetector.setLocalDetectionLocalAreaExpansion(sender.intValue)
    }
    
    func localDetectionMinCleaningObjSizeChanged(_ sender: NSTextField) {
        AstroDetector.setLocalDetectionMinCleaningObjSize(sender.intValue)
    }
    
    func localDetectionCleaningIterationsChanged(_ sender: NSTextField) {
        AstroDetector.setLocalDetectionCleaningIterations(sender.intValue)
    }
    
    func localDetectionBGSubScalingFactorChanged(_ sender: NSTextField) {
        AstroDetector.setLocalDetectionBGSubScalingFactor(sender.floatValue)
    }
    
    func localDetectionSigmoidSlopeChanged(_ sender: NSTextField) {
        AstroDetector.setLocalDetectionSigmoidSlope(sender.intValue);
    }
    
    func localDetectionSigmoidCenterParamChanged(_ sender: NSTextField) {
        AstroDetector.setLocalDetectionSigmoidCenterParam(sender.intValue);
    }
    
    func localDetectionNumOfGlobalObjectsChanged(_ sender: NSTextField) {
        AstroDetector.setLocalDetectionNumOfGlobalObjects(sender.intValue);
    }
    
    func localDetectionLayeredDetectionThresholdChanged(_ sender: NSTextField) {
        AstroDetector.setLocalDetectionLayeredDetectionThreshold(sender.intValue);
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
        
        self.view.wantsLayer = true
        self.view.layer?.contents = NSImage(named: "dark_gray")
        
        // Detection
        
        detectionContainerView.wantsLayer = true
        detectionContainerView.layer?.contents = NSImage(named: "button")
        detectionContainerView.layer?.cornerRadius = 5
        
        localDetectionButton.state = AstroDetector.isLocalDetectionEnabled() ? .on : .off
        deblendingButton.state = AstroDetector.isDeblendingEnabled() ? .on : .off
        starGalaxyClassificationButton.state = AstroDetector.isStarGalaxyClassificationEnabled() ? .on : .off
        galaxyMorphologyClassificationButton.state = AstroDetector.isGalaxyMorphologyClassificationEnabled() ? .on : .off
        
        // Global Detection
        
        globalDetectionContainerView.wantsLayer = true
        globalDetectionContainerView.layer?.contents = NSImage(named: "button")
        globalDetectionContainerView.layer?.cornerRadius = 5
        
        globalDetectionKernelSizeField.intValue = AstroDetector.getGlobalDetectionKernelSize()
        globalDetectionKernelSizeField.delegate = self
        
        globalDetectionStdDevSigmaField.floatValue = AstroDetector.getGlobalDetectionStdDevSigma()
        globalDetectionStdDevSigmaField.delegate = self
        
        globalDetectionLocalAreaExpansionField.intValue = AstroDetector.getGlobalDetectionLocalAreaExpansion()
        globalDetectionLocalAreaExpansionField.delegate = self
        
        globalDetectionMinCleaningObjSizeField.intValue = AstroDetector.getGlobalDetectionMinCleaningObjSize()
        globalDetectionMinCleaningObjSizeField.delegate = self
        
        globalDetectionCleaningIterationsField.intValue = AstroDetector.getGlobalDetectionCleaningIterations()
        globalDetectionCleaningIterationsField.delegate = self
        
        globalDetectionBGSubScalingFactorField.floatValue = AstroDetector.getGlobalDetectionBGSubScalingFactor()
        globalDetectionBGSubScalingFactorField.delegate = self
        
        // Local Detection
        
        localDetectionContainerView.wantsLayer = true
        localDetectionContainerView.layer?.contents = NSImage(named: "button")
        localDetectionContainerView.layer?.cornerRadius = 5
        
        localDetectionKernelSizeField.intValue = AstroDetector.getLocalDetectionKernelSize()
        localDetectionKernelSizeField.delegate = self
        
        localDetectionStdDevSigmaField.floatValue = AstroDetector.getLocalDetectionStdDevSigma()
        localDetectionStdDevSigmaField.delegate = self
        
        localDetectionLocalAreaExpansionField.intValue = AstroDetector.getLocalDetectionLocalAreaExpansion()
        localDetectionLocalAreaExpansionField.delegate = self
        
        localDetectionMinCleaningObjSizeField.intValue = AstroDetector.getLocalDetectionMinCleaningObjSize()
        localDetectionMinCleaningObjSizeField.delegate = self
        
        localDetectionCleaningIterationsField.intValue = AstroDetector.getLocalDetectionCleaningIterations()
        localDetectionCleaningIterationsField.delegate = self
        
        localDetectionBGSubScalingFactorField.floatValue = AstroDetector.getLocalDetectionBGSubScalingFactor()
        localDetectionBGSubScalingFactorField.delegate = self
        
        localDetectionNumOfGlobalObjectsField.intValue = AstroDetector.getLocalDetectionNumOfGlobalObjects()
        localDetectionNumOfGlobalObjectsField.delegate = self
        
        localDetectionSigmoidSlopeField.intValue = AstroDetector.getLocalDetectionSigmoidSlope()
        localDetectionSigmoidSlopeField.delegate = self
        
        localDetectionSigmoidCenterParamField.intValue = AstroDetector.getLocalDetectionSigmoidCenterParam()
        localDetectionSigmoidCenterParamField.delegate = self
        
        localDetectionLayeredDetectionThresholdField.intValue = AstroDetector.getLocalDetectionLayeredDetectionThreshold()
        localDetectionLayeredDetectionThresholdField.delegate = self
        
    }
    
    func controlTextDidChange(_ obj: Notification) {
        
        let textField = obj.object as! NSTextField
        
        switch textField {
            
        // GLOBAL DETECTION
            
        case globalDetectionKernelSizeField:
            globalDetectionKernelSizeChanged(textField)
            break;
        case globalDetectionStdDevSigmaField:
            globalDetectionStdDevSigmaChanged(textField)
            break;
        case globalDetectionLocalAreaExpansionField:
            globalDetectionLocalAreaExpansionChanged(textField)
            break;
        case globalDetectionMinCleaningObjSizeField:
            globalDetectionMinCleaningObjSizeChanged(textField)
            break;
        case globalDetectionCleaningIterationsField:
            globalDetectionCleaningIterationsChanged(textField)
            break;
        case globalDetectionBGSubScalingFactorField:
            globalDetectionBGSubScalingFactorChanged(textField)
            break;
            
        // LOCAL DETECTION
            
        case localDetectionKernelSizeField:
            localDetectionKernelSizeChanged(textField)
            break;
        case localDetectionStdDevSigmaField:
            localDetectionStdDevSigmaChanged(textField)
            break;
        case localDetectionLocalAreaExpansionField:
            localDetectionLocalAreaExpansionChanged(textField)
            break;
        case localDetectionMinCleaningObjSizeField:
            localDetectionMinCleaningObjSizeChanged(textField)
            break;
        case localDetectionCleaningIterationsField:
            localDetectionCleaningIterationsChanged(textField)
            break;
        case localDetectionBGSubScalingFactorField:
            localDetectionBGSubScalingFactorChanged(textField)
            break;
        case localDetectionNumOfGlobalObjectsField:
            localDetectionNumOfGlobalObjectsChanged(textField)
            break;
        case localDetectionSigmoidSlopeField:
            localDetectionSigmoidSlopeChanged(textField)
            break;
        case localDetectionSigmoidCenterParamField:
            localDetectionSigmoidCenterParamChanged(textField)
            break;
        case localDetectionLayeredDetectionThresholdField:
            localDetectionLayeredDetectionThresholdChanged(textField)
            break;
            
        default:
            break;
        }
    }
    
}
