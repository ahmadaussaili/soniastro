//
//  ImportImageView.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 2/14/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

import Cocoa

class CustomImageView: NSView {

    var imageView : NSImageView? = nil
    public var tracksMouseClicks : Bool = false
    var mouseLocation : NSPoint? = nil
    
    var mouseUpCompletionHandler: (()->Void)? = nil
    
    public func setImage(imagePath: String) {
        if imageView != nil {
            imageView!.image = NSImage.init(byReferencingFile: imagePath)
        }
    }
    
    public func getMouseLocation() -> NSPoint? {
        if !tracksMouseClicks {
            return nil
        }
        
        return mouseLocation
    }
    
    public func getImageDimensions() -> NSSize? {
        if let imageView = imageView {
            if let image = imageView.image {
                    return NSSize(width: image.representations[0].pixelsWide, height: image.representations[0].pixelsHigh)
            }
        }
        
        return nil
    }
    
    public func setImage(image: NSImage) {
        if imageView != nil {
            imageView!.image = image
        }
    }

    override var wantsUpdateLayer: Bool {
        return true
    }
    
    override func updateLayer() {
        self.layer?.backgroundColor = NSColor(red: 66.0 / 255, green: 67.0 / 255, blue: 76.0 / 255, alpha: 1.0).cgColor
    }
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
        imageView = NSImageView.init(frame: NSRect(x: 0, y: 0, width: self.frame.width, height: self.frame.height))
        if let imageView = imageView {
            self.addSubview(imageView)
            imageView.imageScaling = NSImageScaling.scaleProportionallyUpOrDown
        }
    }
    
    public func setMouseUpCompletionHandler(handler: @escaping (()->Void)) {
        mouseUpCompletionHandler = handler
    }
    
    override func mouseUp(with event: NSEvent) {
        if !tracksMouseClicks {
            return
        }
        
        updateMouseLocation(event: event)
        
        guard self.mouseLocation != nil else {
            return
        }
        
        if let handler = mouseUpCompletionHandler {
            handler()
        }
    }
    
    // This function is used to track the clicks of a mouse on an image and return the location
    // within that image.
    func updateMouseLocation(event: NSEvent) {
        
        if let imageView = imageView {
            let eventLocation = event.locationInWindow
            var location = imageView.convert(eventLocation, from: nil)
            
            guard let drawingRect = imageView.cell?.drawingRect(forBounds: imageView.bounds) else {
                return
            }
            
            location.x -= drawingRect.origin.x
            location.y -= drawingRect.origin.y
            
            let frameSize = drawingRect.size
            let frameAspect = frameSize.width/frameSize.height
            
            guard let image = imageView.image else {
                return
            }
            
            let imageSize = NSSize(width: image.representations[0].pixelsWide, height: image.representations[0].pixelsHigh)
            
            let imageAspect = imageSize.width/imageSize.height
            
            var scaleFactor = 1.0;
            
            if(imageAspect > frameAspect) {
                
                // Here, image.width == frame.width
                scaleFactor = Double(imageSize.width / frameSize.width)
                
                let imageHeightinFrame = Double(imageSize.height) / scaleFactor
                
                let imageOffsetInFrame = (Double(frameSize.height) - imageHeightinFrame)/2
                
                location.y -= CGFloat(imageOffsetInFrame)
                
            } else {
                // Here,  image.height == frame.height
                scaleFactor = Double(imageSize.height / frameSize.height)
                
                let imageWidthinFrame = Double(imageSize.width) / scaleFactor
                
                let imageOffsetInFrame = (Double(frameSize.width) - imageWidthinFrame)/2
                
                location.x -= CGFloat(imageOffsetInFrame)
            }
            
            location.x *= CGFloat(scaleFactor)
            location.y *= CGFloat(scaleFactor)
            
            // If mouse location is out of range, set it to null
            if !(0 ... imageSize.width).contains(location.x) || !(0 ... imageSize.height).contains(location.y) {
                return
            } else {
                mouseLocation = NSPoint(x: Int(location.x), y: Int(imageSize.height - location.y))
            }
        }
    }
    
    override func acceptsFirstMouse(for event: NSEvent?) -> Bool {
        return true;
    }
    
    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)

        // Drawing code here.
    }
    
}
