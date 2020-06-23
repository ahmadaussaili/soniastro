//
//  CustomButton.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 2/12/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

import Cocoa

public class CustomButton: NSButton {
    
    public var isSelected : Bool = false
    
    var normalImage : NSImage? = nil
    var highlightImage : NSImage? = nil
    var cornerRadius = CGFloat.init(0)
 
    public func setCornerRadius(radius: Int) {
        cornerRadius = CGFloat.init(radius)
    }
    
    public func updateHighlightImage(image: NSImage) {
        highlightImage = image
        self.updateLayer()
    }
    
    public func updateImage(image: NSImage) {
        normalImage = image
        self.updateLayer()
    }
    
    override public var wantsUpdateLayer: Bool {
        return true
    }
    
    override public func updateLayer() {
        
        self.layer?.cornerRadius = cornerRadius
        
        if (self.cell?.isHighlighted)! {
            if let image = highlightImage {
                self.layer?.contents = image
            }
        } else {
            if let image = normalImage {
                self.layer?.contents = image
            }
        }
    }
    
    override public func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)

        // Drawing code here.
        
    }
}
