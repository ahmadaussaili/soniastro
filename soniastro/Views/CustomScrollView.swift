//
//  CustomScrollView.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 2/13/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

import Cocoa

class CustomScrollView: NSScrollView {
    
    var cornerRadius = CGFloat.init(0)
    
    public func setCornerRadius(radius: Int) {
        cornerRadius = CGFloat.init(radius)
    }
    
    override var wantsUpdateLayer: Bool {
        return true
    }
    
    override func updateLayer() {
        
        self.layer?.cornerRadius = cornerRadius
        self.backgroundColor = NSColor.init(calibratedRed: 0.16, green: 0.16, blue: 0.18, alpha: 1.0)
    }
    
    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)

        // Drawing code here.
    }
    
}
