//
//  ExplorerViewController.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 2/16/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

import Cocoa
import KRActivityIndicatorView

class ExplorerViewController: NSViewController {

    // DEFINITIONS
    
    var buttons : Array<CustomButton> = []
    var filterButtons : Array<CustomButton> = []
    var propertyLabels : Array<NSTextField> = []
    var objectWasSelected : Bool = false
    
    var importedImagePath : String? = nil
    
    var astroDetector = AstroDetector.init()
    
    // OUTLETS
    
    @IBOutlet weak var importedImageNameLabel: NSTextField!
    
    @IBOutlet weak var importImageView: CustomImageView!
    
    // Buttons
    @IBOutlet weak var importImageButton: CustomButton!
    @IBOutlet weak var detectObjectsButton: CustomButton!
    @IBOutlet weak var downloadObjectsFileButton: CustomButton!
    @IBOutlet weak var exportDetectionImageButton: CustomButton!
    
    @IBOutlet weak var consoleStackView: NSStackView!
    @IBOutlet weak var consoleScrollView: CustomScrollView!
    @IBOutlet var consoleTextView: NSTextView!
    
    // Filter
    @IBOutlet weak var filterStackView: NSStackView!
    @IBOutlet weak var filterView: NSView!
    @IBOutlet weak var objectAreaSliderView: NSView!
    
    @IBOutlet weak var detectedObjectsFilter: CustomButton!
    @IBOutlet weak var starsFilter: CustomButton!
    @IBOutlet weak var galaxyFilter: CustomButton!
    @IBOutlet weak var galaxyClassificationFilter: CustomButton!
    
    @IBOutlet weak var objectAreaSlider: NSSlider!
    @IBOutlet weak var objPropBBoxButton: CustomButton!
    @IBOutlet weak var objectAreaLabel: NSTextField!
    
    // Object properties panel
    @IBOutlet weak var objPropImageView: CustomImageView!
    @IBOutlet weak var noObjSelectedLabel: NSTextField!
    
    @IBOutlet weak var objPropView: NSView!
    @IBOutlet weak var typePropertyLabel: NSTextField!
    @IBOutlet weak var morphologyPropertyLabel: NSTextField!
    @IBOutlet weak var areaPropertyLabel: NSTextField!
    @IBOutlet weak var objXPropertyLabel: NSTextField!
    @IBOutlet weak var objYPropertyLabel: NSTextField!
    @IBOutlet weak var bboxWidthPropertyLabel: NSTextField!
    @IBOutlet weak var bboxHeightPropertyLabel: NSTextField!
    @IBOutlet weak var bboxXPropertyLabel: NSTextField!
    @IBOutlet weak var bboxYPropertyLabel: NSTextField!
    
    @IBOutlet weak var exportObjImgButton: CustomButton!
    
    
    @IBOutlet weak var detectObjectsActivityIndicator: KRActivityIndicatorView!
    
    // The container of the activity indicator.
    @IBOutlet weak var detecObjectsAIContainer: NSView!
    
    // ACTIONS
    
    @IBAction func downloadObjectsButtonPressed(_ sender: Any) {
        
        let dialog = NSSavePanel()
        
        dialog.showsResizeIndicator    = true
        dialog.showsHiddenFiles        = false
        dialog.canCreateDirectories    = true
        dialog.allowedFileTypes = ["txt"]
        
        if let importPath = importedImagePath {
            dialog.nameFieldStringValue = importPath.components(separatedBy: "/").last!.components(separatedBy: ".").first! + "_objects.txt"
        } else {
            dialog.nameFieldStringValue = "objects.txt"
        }
        
        
        if (dialog.runModal() == NSApplication.ModalResponse.OK) {
            if let url = dialog.url {
                let exportPath = url.path
                
                let fileManager = FileManager.default
                do {
                    if fileManager.fileExists(atPath: exportPath) {
                        try fileManager.removeItem(atPath: exportPath)
                    }
                    try fileManager.copyItem(atPath: astroDetector.getObjectsFilePath(), toPath: exportPath)
                } catch {
                    print("Could not download objects file: \(error)")
                }
            }
        } else {
            // User clicked on "Cancel"
            return
        }
    }
    
    @IBAction func importImage(_ sender: Any) {
        
        let dialog = NSOpenPanel()
        
        dialog.showsResizeIndicator    = true
        dialog.showsHiddenFiles        = false
        dialog.canChooseDirectories    = false
        dialog.canCreateDirectories    = false
        dialog.allowsMultipleSelection = false
        dialog.allowedFileTypes        = ["png", "jpg", "jpeg"]
        
        if (dialog.runModal() == NSApplication.ModalResponse.OK) {
            
            if let result = dialog.url {
                importedImagePath = result.path
                
                importImageView.setImage(imagePath: result.path)
                importImageView.isHidden = false
                importImageButton.isHidden = true
                detectObjectsButton.isEnabled = true
                
                importedImageNameLabel.stringValue = result.path.components(separatedBy: "/").last!
            }
        } else {
            // User clicked on "Cancel"
            return
        }
    }
    
    @IBAction func exportDetectionImage(_ sender: Any) {
        
        let dialog = NSSavePanel()
        
        dialog.showsResizeIndicator    = true
        dialog.showsHiddenFiles        = false
        dialog.canCreateDirectories    = true
        
        if let importPath = importedImagePath {
            dialog.nameFieldStringValue = importPath.components(separatedBy: "/").last!.components(separatedBy: ".").first! + "_detection.jpg"
        } else {
            dialog.nameFieldStringValue = "detection.jpg"
        }
    
        
        if (dialog.runModal() == NSApplication.ModalResponse.OK) {
            if let url = dialog.url {
                let exportPath = url.path
                
                let fileManager = FileManager.default
                do {
                    if fileManager.fileExists(atPath: exportPath) {
                        try fileManager.removeItem(atPath: exportPath)
                    }
                    try fileManager.copyItem(atPath: astroDetector.getDetectionImagePath(), toPath: exportPath)
                } catch {
                    print("Could not save detection image: \(error)")
                }
            }
        } else {
            // User clicked on "Cancel"
            return
        }
    }
    
    @IBAction func exportSelectedObjectImage(_ sender: Any) {
        
        let dialog = NSSavePanel()
        
        dialog.showsResizeIndicator    = true
        dialog.showsHiddenFiles        = false
        dialog.canCreateDirectories    = true
        
        if let importPath = importedImagePath {
            dialog.nameFieldStringValue = importPath.components(separatedBy: "/").last!.components(separatedBy: ".").first! + "_object.jpg"
        } else {
            dialog.nameFieldStringValue = "detection.jpg"
        }
        
        
        if (dialog.runModal() == NSApplication.ModalResponse.OK) {
            if let url = dialog.url {
                let exportPath = url.path
                
                let fileManager = FileManager.default
                do {
                    if fileManager.fileExists(atPath: exportPath) {
                        try fileManager.removeItem(atPath: exportPath)
                    }
                    
                    var objectImagePath = ""
                    if objPropBBoxButton.isSelected {
                        objectImagePath = astroDetector.getSelectedObjectBBoxImagePath()
                    } else {
                        objectImagePath = astroDetector.getSelectedObjectImagePath()
                    }
                
                    try fileManager.copyItem(atPath: objectImagePath, toPath: exportPath)
                } catch {
                    print("Could not save detection image: \(error)")
                }
            }
        } else {
            // User clicked on "Cancel"
            return
        }
    }
    
    @IBAction func loadDetectionImage(_ sender: Any) {
        
        self.detectObjectsButton.isEnabled = false
        
        if let path = self.importedImagePath {
            self.updateConsoleText(text: "Detecting objects in image: " + path.components(separatedBy: "/").last! + " ...\n")
        }
        
        detecObjectsAIContainer.isHidden = false
        importImageView.isHidden = true
        detectObjectsActivityIndicator.startAnimating();
        
        Thread.detachNewThread { // It exits automatically as soon as the task is completed
            
            if let path = self.importedImagePath {
                if self.astroDetector.detectObjects(path) == true {
                    self.handleDetectionImageLoading()
                }
            }
        }
    }
    
    func handleDetectionImageLoading() {
        
        if !self.astroDetector.drawObjects() {
            return
        }
        
        self.importImageView.isHidden = false
        self.importImageButton.isHidden = true
        self.detectObjectsButton.isEnabled = true
        self.downloadObjectsFileButton.isEnabled = true
        
        self.importedImageNameLabel.stringValue = self.importedImagePath!.components(separatedBy: "/").last!
        
        self.importImageView.setImage(imagePath: self.astroDetector.getDetectionImagePath())
        
        exportDetectionImageButton.isEnabled = true
        
        // Enable filter options
        self.enableFilterOptions()
        
        // Set min and max values of area slider
        self.objectAreaSlider.maxValue = Double(self.astroDetector.getMaxObjectArea())
        self.objectAreaSlider.minValue = Double(self.astroDetector.getMinObjectArea())
        self.objectAreaSlider.doubleValue = self.objectAreaSlider.minValue
        self.objectAreaLabel.stringValue = String(self.objectAreaSlider.intValue) + " px"
        
        self.detectObjectsActivityIndicator.stopAnimating()
        self.importImageView.isHidden = false
        self.detecObjectsAIContainer.isHidden = true
        
        // Object properties panel
        
        let objPropImageHandler : ()->Void = {
            () -> Void in
            guard let mouseLocation = self.importImageView.getMouseLocation() else {
                return
            }
            
            if self.astroDetector.drawSelectedObject(Int32(mouseLocation.x), and: Int32(mouseLocation.y)) {
                self.noObjSelectedLabel.isHidden = true
                self.objPropBBoxButton.isEnabled = true
                self.exportObjImgButton.isEnabled = true
                
                if (self.objPropBBoxButton.isSelected) {
                    self.objPropImageView.setImage(imagePath: self.astroDetector.getSelectedObjectBBoxImagePath())
                } else {
                    self.objPropImageView.setImage(imagePath: self.astroDetector.getSelectedObjectImagePath())
                }
                
                // Display object statistics
                self.typePropertyLabel.stringValue = self.astroDetector.getSelectedObjectType()
                self.morphologyPropertyLabel.stringValue = self.astroDetector.getSelectedObjectMorphology()
                self.areaPropertyLabel.stringValue = String(self.astroDetector.getSelectedObjectArea()) + " px"
                self.objXPropertyLabel.stringValue = String(Int(self.astroDetector.getSelectedObjectLocation().x)) + " px"
                self.objYPropertyLabel.stringValue = String(Int(self.astroDetector.getSelectedObjectLocation().y)) + " px"
                self.bboxXPropertyLabel.stringValue = String(Int(self.astroDetector.getSelectedObjectBBoxLocation().x)) + " px"
                self.bboxYPropertyLabel.stringValue = String(Int(self.astroDetector.getSelectedObjectBBoxLocation().y)) + " px"
                self.bboxWidthPropertyLabel.stringValue = String(Int(self.astroDetector.getSelectedObjectBBoxSize().width)) + " px"
                self.bboxHeightPropertyLabel.stringValue = String(Int(self.astroDetector.getSelectedObjectBBoxSize().height)) + " px"
            }
        }
        self.importImageView.setMouseUpCompletionHandler(handler: objPropImageHandler)
    }
    
    func updateDetectorFilters() {
        astroDetector.setDetectedObjectsFilter(detectedObjectsFilter.isSelected)
        astroDetector.setObjectAreaFilter(objectAreaSlider.intValue)
        astroDetector.setStarFilter(starsFilter.isSelected)
        astroDetector.setGalaxyFilter(galaxyFilter.isSelected)
        astroDetector.setGalaxyClassificationFilter(galaxyClassificationFilter.isSelected)
    }
    
    // DETECTED OBJECTS FILTERS
    
    @IBAction func objectAreaSliderChanged(_ sender: Any) {
        objectAreaLabel.stringValue = String(objectAreaSlider.intValue) + " px"
        updateDetectorFilters()
        if astroDetector.drawObjects() {
            importImageView.setImage(imagePath: astroDetector.getDetectionImagePath())
        }
    }
    
    @IBAction func detectedObjectsFilterButtonPressed(_ sender: Any) {
        updateButtonAfterSelection(button: detectedObjectsFilter, isSelected: !detectedObjectsFilter.isSelected)
        
        // If de-selected, de-select all other options.
        if !detectedObjectsFilter.isSelected {
            for button in buttons {
                if button.isSelected {
                    updateButtonAfterSelection(button: button, isSelected: false)
                }
            }
        } else {
            updateButtonAfterSelection(button: starsFilter, isSelected: true)
            updateButtonAfterSelection(button: galaxyFilter, isSelected: true)
        }
        
        updateDetectorFilters()
        if astroDetector.drawObjects() {
            importImageView.setImage(imagePath: astroDetector.getDetectionImagePath())
        }
    }
    
    @IBAction func starsFilterButtonPressed(_ sender: Any) {
        updateButtonAfterSelection(button: starsFilter, isSelected: !starsFilter.isSelected)
        updateDetectorFilters()
        if astroDetector.drawObjects() {
            importImageView.setImage(imagePath: astroDetector.getDetectionImagePath())
        }
    }
    
    @IBAction func galaxyFilterButtonPressed(_ sender: Any) {
        updateButtonAfterSelection(button: galaxyFilter, isSelected: !galaxyFilter.isSelected)
        updateDetectorFilters()
        if astroDetector.drawObjects() {
            importImageView.setImage(imagePath: astroDetector.getDetectionImagePath())
        }
    }
    
    @IBAction func galaxyClassificationFilterButtonPressed(_ sender: Any) {
        updateButtonAfterSelection(button: galaxyClassificationFilter, isSelected: !galaxyClassificationFilter.isSelected)
        updateDetectorFilters()
        if astroDetector.drawObjects() {
            importImageView.setImage(imagePath: astroDetector.getDetectionImagePath())
        }
    }
    
    @IBAction func objPropBBoxButtonPressed(_ sender: Any) {
        updateButtonAfterSelection(button: objPropBBoxButton, isSelected: !objPropBBoxButton.isSelected, normalImagePath: "button_bbox", highlightImagePath: "button_bbox_highlight")
        if (objPropBBoxButton.isSelected) {
            objPropImageView.setImage(imagePath: self.astroDetector.getSelectedObjectBBoxImagePath())
        } else {
            objPropImageView.setImage(imagePath: self.astroDetector.getSelectedObjectImagePath())
        }
    }
    
    // MENU ACTIONS
    
    @IBAction func showConsole(_ sender: NSMenuItem) {
        consoleStackView.isHidden = !consoleStackView.isHidden
    }
    
    @IBAction func importImageFromMenu(_ sender: Any) {
        self.importImage(self)
        
    }

    var inputConsolePipe : Pipe? = nil
    var outputConsolePipe : Pipe? = nil
    
    func openConsolePipe() {
        
        // A new Pipe to consume the messages on STDOUT and STDERR
        inputConsolePipe = Pipe()
        
        // A pipe to output messages back to STDOUT
        outputConsolePipe = Pipe()
        
        guard let inputPipe = inputConsolePipe, let outputPipe = outputConsolePipe else {
            return
        }
        
        let pipeReadHandle = inputPipe.fileHandleForReading
        
        // Copy the STDOUT file descriptor into the output pipe file descriptor to write logs back to STDOUT.
        dup2(STDOUT_FILENO, outputPipe.fileHandleForWriting.fileDescriptor)
        
        dup2(inputPipe.fileHandleForWriting.fileDescriptor, STDOUT_FILENO)
        dup2(inputPipe.fileHandleForWriting.fileDescriptor, STDERR_FILENO)
        
        // Observe the readHandle notification
        NotificationCenter.default.addObserver(self, selector: #selector(self.handleConsolePipeNotification), name: FileHandle.readCompletionNotification, object: pipeReadHandle)
        
        // We want to be notified of any data coming across the pipe.
        pipeReadHandle.readInBackgroundAndNotify()
    }
    
    @objc func handleConsolePipeNotification(notification: Notification) {
        
        // This has to be called continuosly when receiving a message.
        inputConsolePipe?.fileHandleForReading.readInBackgroundAndNotify()
        
        if let data = notification.userInfo![NSFileHandleNotificationDataItem] as? Data,
            let log = String(data: data, encoding: String.Encoding.ascii) {
            
            if (log.starts(with: astroDetector.getLogIdentifier())) {
                updateConsoleText(text: log.replacingOccurrences(of: astroDetector.getLogIdentifier(), with: ""))
            } else {
                // Write the data back into the output pipe (to show the logs on the xcode console).
                outputConsolePipe?.fileHandleForWriting.write(data)
            }
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
        
        self.view.window?.acceptsMouseMovedEvents = true
    
        // Set background color
        self.view.wantsLayer = true
        self.view.layer?.contents = NSImage(named: "dark_gray")
        self.view.needsDisplay = true
        
        // Add buttons
        buttons = [importImageButton, detectedObjectsFilter, starsFilter, galaxyFilter, galaxyClassificationFilter, detectObjectsButton, downloadObjectsFileButton, exportDetectionImageButton, exportObjImgButton, objPropBBoxButton]
        filterButtons = [detectedObjectsFilter, starsFilter, galaxyFilter, galaxyClassificationFilter]
        setUpButtonsUI(buttons: buttons)
        
        // Update filters
        updateDetectorFilters()
        
        // Update object properties
        updateObjectPropertiesPanel()
        
        // UI elements
        setUpUiElements()
        
        // Open pipe for console logging
        openConsolePipe()
    }
    
    override func mouseEntered(with event: NSEvent) {
        
        // Identify which button triggered the mouseEntered event
        if let buttonIndex = event.trackingArea?.userInfo?["buttonIndex"] as? Int {
            if buttonIndex == 0 { // importImageButton
                
                if let image = NSImage(named: "button_highlight") {
                    buttons[buttonIndex].updateImage(image: image)
                }
                
                let title = "Import Image"
                if let mutableAttributedTitle = buttons[buttonIndex].attributedTitle.mutableCopy() as? NSMutableAttributedString {
                    mutableAttributedTitle.replaceCharacters(in: NSMakeRange(0, mutableAttributedTitle.length), with: title)
                    buttons[buttonIndex].attributedTitle = mutableAttributedTitle
                }
            }
            
            // Filter buttons
            if case 1 ... 4 = buttonIndex {
                if let image = NSImage(named: "button_highlight") {
                    buttons[buttonIndex].updateImage(image: image)
                }
            }
        }
    }
    
    override func mouseExited(with event: NSEvent) {
        
        // Identify which button triggered the mouseExited event
        if let buttonIndex = event.trackingArea?.userInfo?["buttonIndex"] as? Int {
            if buttonIndex == 0 { // importImageButton
                
                if let image = NSImage(named: "button") {
                    buttons[buttonIndex].updateImage(image: image)
                }
                
                let title = "No Image Imported"
                if let mutableAttributedTitle = buttons[buttonIndex].attributedTitle.mutableCopy() as? NSMutableAttributedString {
                    mutableAttributedTitle.replaceCharacters(in: NSMakeRange(0, mutableAttributedTitle.length), with: title)
                    buttons[buttonIndex].attributedTitle = mutableAttributedTitle
                }
            }
            
            // Filter buttons
            if case 1 ... 4 = buttonIndex {
                if !buttons[buttonIndex].isSelected {
                    if let image = NSImage(named: "button") {
                        buttons[buttonIndex].updateImage(image: image)
                    }
                }
            }
        }
    }
    
    func setUpButtonsUI(buttons: Array<CustomButton>) {
        
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
            
            if let image = NSImage(named: "button_highlight") {
                button.updateHighlightImage(image: image)
            }
        }
        
        // Selected object bounding box button
        if let image = NSImage(named: "button_bbox") {
            objPropBBoxButton.updateImage(image: image)
        }
        
        if let image = NSImage(named: "button_bbox_highlight") {
            objPropBBoxButton.updateHighlightImage(image: image)
        }
        
        // Main buttons
        
        detectObjectsButton.setCornerRadius(radius: 5)
        downloadObjectsFileButton.setCornerRadius(radius: 5)
        exportDetectionImageButton.setCornerRadius(radius: 5)
        exportObjImgButton.setCornerRadius(radius: 5)
        objPropBBoxButton.setCornerRadius(radius: 5)
        
        detectObjectsButton.isEnabled = false
        downloadObjectsFileButton.isEnabled = false
        exportDetectionImageButton.isEnabled = false
        
        // Disable filter buttons
        
        detectedObjectsFilter.isEnabled = false
        starsFilter.isEnabled = false
        galaxyFilter.isEnabled = false
        galaxyClassificationFilter.isEnabled = false
        
        objPropBBoxButton.isEnabled = false
        
        // Update filter buttons with the correct states
        
        updateButtonAfterSelection(button: detectedObjectsFilter, isSelected: true)
        updateButtonAfterSelection(button: starsFilter, isSelected: true)
        updateButtonAfterSelection(button: galaxyFilter, isSelected: true)
        
        updateButtonAfterSelection(button: objPropBBoxButton, isSelected: false, normalImagePath: "button_bbox", highlightImagePath: "button_bbox_highlight")
    }
    
    func setUpUiElements() {
        
        // Imported image view
        importImageView.isHidden = true
        importImageView.tracksMouseClicks = true
        
        // Imported Image label
        importedImageNameLabel.stringValue = ""
        
        // Console
        
        consoleScrollView.setCornerRadius(radius: 10)
        
        // Uncomment the following line to display the same color as the buttons
        //        consoleTextView.backgroundColor = NSColor(red: 66.0 / 255, green: 67.0 / 255, blue: 76.0 / 255, alpha: 1.0)
        consoleTextView.backgroundColor = NSColor(red: 66.0 / 255, green: 70.0 / 255, blue: 79.0 / 255, alpha: 1.0)
        consoleTextView.font = NSFont.systemFont(ofSize: 13)
        consoleTextView.textColor = NSColor.white.withAlphaComponent(0.9)
        consoleTextView.string = "Explorer initialised \n"
        consoleTextView.scrollToEndOfDocument(self)
        
        // Filter view
        filterView.wantsLayer = true
        filterView.layer?.cornerRadius = 15
        filterView.layer?.backgroundColor = NSColor.black.cgColor
        
        // Filter elements
        objectAreaSlider.intValue = 0
        objectAreaSlider.isEnabled = false
        objectAreaLabel.stringValue = String(objectAreaSlider.intValue) + " px"
        objectAreaSliderView.wantsLayer = true
        objectAreaSliderView.layer?.contents = NSImage(named: "button")
        
        // Object properties panel
        objPropImageView.wantsLayer = true
        objPropImageView.layer?.cornerRadius = 15
        objPropView.wantsLayer = true
        objPropView.layer?.cornerRadius = 15
        if let image = NSImage(named: "button") {
            objPropImageView.setImage(image: image)
            objPropView.layer?.contents = image
        }
        objPropImageView.addSubview(noObjSelectedLabel)
        objPropImageView.addSubview(objPropBBoxButton)
        
        // Activity indicators
        detectObjectsActivityIndicator.type = .ballGridPulse
        detectObjectsActivityIndicator.color = NSColor.systemGreen.withAlphaComponent(0.9)
        detecObjectsAIContainer.wantsLayer = true
        detecObjectsAIContainer.layer?.contents = NSImage(named: "button")
        detecObjectsAIContainer.isHidden = true
    }
    
    func updateConsoleText(text : String) {
        consoleTextView.string += text
        consoleTextView.scrollToEndOfDocument(self)
    }
    
    func enableFilterOptions() {
        detectedObjectsFilter.isEnabled = true
        starsFilter.isEnabled = true
        galaxyFilter.isEnabled = true
        galaxyClassificationFilter.isEnabled = true
        objectAreaSlider.isEnabled = true
    }
    
    func updateButtonAfterSelection(button : CustomButton, isSelected : Bool) {
        button.isSelected = isSelected
        
        let imagePath : String
        
        if (button.isSelected) {
            imagePath = "button_highlight"
        } else {
            imagePath = "button"
        }
        
        if let image = NSImage(named: imagePath) {
            button.updateImage(image: image)
        }
    }
    
    func updateButtonAfterSelection(button : CustomButton, isSelected : Bool, normalImagePath : String, highlightImagePath : String) {
        button.isSelected = isSelected
        
        let imagePath : String
        
        if (button.isSelected) {
            imagePath = highlightImagePath
        } else {
            imagePath = normalImagePath
        }
        
        if let image = NSImage(named: imagePath) {
            button.updateImage(image: image)
        }
    }
    
    func updateObjectPropertiesPanel() {
        
        if !objectWasSelected {
            for label in propertyLabels {
                label.stringValue = ""
                label.placeholderString = ""
            }
            exportObjImgButton.isEnabled = false
            return
        }
        
        // SET OBJECT PROPERTY LABELS...
        noObjSelectedLabel.isHidden = true
    }
}
