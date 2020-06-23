//
//  GMCModel.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 3/15/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//
// Galaxy Morphology Classifier
//

import Cocoa

public class GMCModel: Classifier {
    
    public override init() {
        super.init(with: GalaxyMorphologyClassifier().model)
    }
}
