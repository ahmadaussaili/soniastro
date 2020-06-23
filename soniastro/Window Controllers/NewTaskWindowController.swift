//
//  NewTaskWindowController.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 2/14/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

import Cocoa

class NewTaskWindowController: NSWindowController {
    
    public static var shared: NSWindowController?
    
    override func windowDidLoad() {
        super.windowDidLoad()
        
        NewTaskWindowController.shared = self
    }

}
