//
//  Classifier.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 3/14/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

import Cocoa

public class Classifier: NSObject {
    
    var model : MLModel? = nil
    var classification: NSString = ""
    
    lazy var classificationRequest: VNCoreMLRequest? = {
        
        do {
            let vnModel = try VNCoreMLModel(for: model!)
            
            let request = VNCoreMLRequest(model: vnModel, completionHandler: { [weak self] request, error in
                self?.processClassifications(for: request, error: error)
            })
            request.imageCropAndScaleOption = .centerCrop
            return request
            
        } catch {
            print("Failed to load Vision ML model: \(error) \n")
            return nil
        }
    }()
    
    public init(with model: MLModel) {
        super.init()
        
        self.model = model
    }
    
    public override init() {
        super.init()
    }
    
    // Classify an image into one of the loaded model classes.
    @objc public func classify(_ image: NSImage) -> NSString {
        
        if self.model == nil {
            print("Failed to perform classification: model is null \n")
            return ""
        }
        
        guard let ciImage = CIImage(data: image.tiffRepresentation!) else {
            print("Failed to perform classification: unable to create \(CIImage.self) from \(image). \n")
            return ""
        }
        
        guard let classificationRequest = self.classificationRequest else {
            print("Failed to perform classification: request is null \n")
            return ""
        }
        
        let requestHandler = VNImageRequestHandler(ciImage: ciImage, orientation: .up)
        
        do {
            try requestHandler.perform([classificationRequest])
            if (self.classification != "") {
                return self.classification
            }
        } catch {
            print("Failed to perform classification: \(error.localizedDescription) \n")
        }
        
        return ""
    }
    
    func processClassifications(for request: VNRequest, error: Error?) {
        
        guard let results = request.results else {
            print("Unable to classify image: \(error!.localizedDescription) \n")
            return
        }
        
        let classifications = results as! [VNClassificationObservation]
        
        if classifications.isEmpty {
            self.classification = ""
        } else {
            // Display top classifications ranked by confidence in the UI.
            let topClassifications = classifications.prefix(1)
            let descriptions = topClassifications.map { classification in
                
                // Format the classification for display
                return classification.identifier
            }
            self.classification = NSString(string: descriptions[0])
        }
    }
    
}
