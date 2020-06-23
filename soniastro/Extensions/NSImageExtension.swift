//
//  NSImageExtension.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 3/27/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

import Foundation

extension NSImage {
    
    // Write the contents of an NSImage to the specified path.
    func writeToFile(file: String, atomically: Bool, usingType type: NSBitmapImageRep.FileType) -> Bool {
        let properties = [NSBitmapImageRep.PropertyKey.compressionFactor: 1.0]
        
        guard
            let imageData = tiffRepresentation,
            let imageRep = NSBitmapImageRep(data: imageData),
            let fileData : NSData = imageRep.representation(using: type, properties: properties) as! NSData else {
                return false
        }
        
        return fileData.write(toFile: file, atomically: atomically)
    }
}
