//
//  ViewController.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 2/12/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

import Cocoa

class NewTaskViewController: NSViewController {
    
    // DEFINITIONS
    
    var buttons : Array<CustomButton> = []
    
    // OUTLETS
    
    @IBOutlet weak var mainStackView: NSStackView!
    @IBOutlet weak var produceSoundButton: CustomButton!
    @IBOutlet weak var exploreObjectsButton: CustomButton!
    
    // VIEW FUNCTIONS
    
    override func viewDidLoad() {
        
        super.viewDidLoad()
        
        // Do any additional setup after loading the view.
        
        // Set background color to dark gray
        self.view.wantsLayer = true;
        self.view.layer?.contents = NSImage(named: "dark_gray")
        self.view.needsDisplay = true;
        
        // Add buttons to an array.
        buttons = [produceSoundButton, exploreObjectsButton]
        
        setUpButtons(buttons: buttons)
    }
    
    override func shouldPerformSegue(withIdentifier identifier: NSStoryboardSegue.Identifier, sender: Any?) -> Bool {
        // Hide window after opening a task.
        self.view.window?.orderOut(self)
        return true
    }
    
    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }
    
}

extension NewTaskViewController {
    
    override func mouseEntered(with event: NSEvent) {
        
        // Identify which button triggered the mouseEntered event
        if let buttonIndex = event.trackingArea?.userInfo?["buttonIndex"] as? Int {
            if let image = NSImage(named: "button_highlight") {
                buttons[buttonIndex].updateImage(image: image)
            }
        }
    }
    
    override func mouseExited(with event: NSEvent) {
        
        // Identify which button triggered the mouseExited event
        if let buttonIndex = event.trackingArea?.userInfo?["buttonIndex"] as? Int {
            if let image = NSImage(named: "button") {
                buttons[buttonIndex].updateImage(image: image)
            }
        }
    }
    
    func setUpButtons(buttons: Array<CustomButton>) {
        
        for button in buttons {
            // Set up tracking area
            let trackingArea = NSTrackingArea.init(rect: button.bounds, options: [.mouseEnteredAndExited, .activeAlways], owner: self, userInfo: ["buttonIndex": buttons.index(of: button)!])
            button.addTrackingArea(trackingArea)
            
            // Change title properties
            if let mutableAttributedTitle = button.attributedTitle.mutableCopy() as? NSMutableAttributedString {
                mutableAttributedTitle.addAttribute(.foregroundColor, value: NSColor.white, range: NSRange(location: 0, length: mutableAttributedTitle.length))
                button.attributedTitle = mutableAttributedTitle
            }
            
            // Set images
            if let image = NSImage(named: "button") {
                button.updateImage(image: image)
            }
        }
    }
}
