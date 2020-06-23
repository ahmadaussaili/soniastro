//
//  AppDelegate.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 2/12/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

import Cocoa

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {
    
    func applicationDidFinishLaunching(_ aNotification: Notification) {
    }

    func applicationWillTerminate(_ aNotification: Notification) {
    }
    
    @IBAction func newTask(_ sender: Any) {
        if let newTaskWindowController = NewTaskWindowController.shared {
            if let window = newTaskWindowController.window {
                if !window.isVisible {
                    window.makeKeyAndOrderFront(self)
                }
            }
        }
    }
    
    // Uncomment the following function if you want to import images from any place in the app.
//    @IBAction func importImageFromMenu(_ sender: Any) {
//    }
}

