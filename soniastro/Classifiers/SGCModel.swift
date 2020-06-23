//
//  SGCModel.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 3/15/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//
// Star Galaxy Classifier
//


import Cocoa

public class SGCModel: Classifier {
    
    public override init() {
        super.init(with: StarGalaxyClassifier().model)
    }
}
