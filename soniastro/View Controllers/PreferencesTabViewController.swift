//
//  PreferencesTabViewController.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 3/23/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

import Cocoa

class PreferencesTabViewController: NSTabViewController {

    @IBOutlet weak var objectDetectionTab: NSTabViewItem!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
        
        self.view.wantsLayer = true
        self.view.layer?.contents = NSImage(named: "dark_gray")
    }
    
}
