//
//  SonifierViewController.swift
//  soniastro
//
//  Created by Ahmad Aussaili on 2/13/20.
//  Copyright © 2020 Ahmad Aussaili. All rights reserved.
//

import Cocoa
import AVKit
import KRActivityIndicatorView

class SonifierViewController: NSViewController {

    // DEFINITIONS
    
    var buttons : Array<CustomButton> = []
    
    var importedImagePath : String? = nil
    var importedImage : NSString? = nil
    
    let astroDetector = AstroDetector.init()
    
    var avComposition : AVMutableComposition? = nil
    
    // OUTLETS
    
    @IBOutlet weak var importImageButton: CustomButton!
    @IBOutlet weak var sonifyButton: CustomButton!
    @IBOutlet weak var exportAudioButton: CustomButton!
    @IBOutlet weak var exportVideoButton: CustomButton!
    @IBOutlet weak var exploreButton: CustomButton!
    
    @IBOutlet weak var importedImageNameLabel: NSTextField!
    
    @IBOutlet weak var importImageView: CustomImageView!
    
    @IBOutlet weak var consoleStackView: NSStackView!
    @IBOutlet var consoleTextView: NSTextView!
    @IBOutlet weak var consoleScrollView: CustomScrollView!
    
    @IBOutlet weak var playerViewContainer: NSView!
    @IBOutlet weak var playerView: AVPlayerView!
    @IBOutlet weak var noOutputLabel: NSTextField!
    @IBOutlet weak var sonifierActivityIndicator: KRActivityIndicatorView!
    
    @IBOutlet weak var soundFilterView: NSView!
    @IBOutlet weak var mysteriousSoundSliderView: NSView!
    @IBOutlet weak var euphoriaSoundSliderView: NSView!
    @IBOutlet weak var implSoundSliderView: NSView!
    
    @IBOutlet weak var mysteriousSoundSlider: NSSlider!
    @IBOutlet weak var euphoriaSoundSlider: NSSlider!
    @IBOutlet weak var reverbSoundSlider: NSSlider!
    
    // ACTIONS
    
    @IBAction func exportAudio(_ sender: Any) {
        
        let dialog = NSSavePanel()
        
        dialog.showsResizeIndicator    = true
        dialog.showsHiddenFiles        = false
        dialog.canCreateDirectories    = true
        dialog.allowedFileTypes = ["wav"]
        
        if let importPath = importedImagePath {
            dialog.nameFieldStringValue = importPath.components(separatedBy: "/").last!.components(separatedBy: ".").first! + "_audio.wav"
        } else {
            dialog.nameFieldStringValue = "audio.wav"
        }
        
        
        if (dialog.runModal() == NSApplication.ModalResponse.OK) {
            if let url = dialog.url {
                updateConsoleText(text: "Exporting audio file...\n")
                
                let exportPath = url.path
                
                let fileManager = FileManager.default
                do {
                    if fileManager.fileExists(atPath: exportPath) {
                        try fileManager.removeItem(atPath: exportPath)
                    }
                    try fileManager.copyItem(atPath: astroDetector.getAudioPath(), toPath: exportPath)
                } catch {
                    print("Could not save audio file: \(error)")
                    updateConsoleText(text: "Could not save audio file: \(error) \n")
                    return
                }
                
                updateConsoleText(text: "Audio file exported.\n")
            }
        } else {
            // User clicked on "Cancel"
            return
        }
    }
    
    @IBAction func exportVideo(_ sender: Any) {
        
        guard let avComposition = avComposition else {
            updateConsoleText(text: "No AV file to export.\n")
            return
        }
        
        let dialog = NSSavePanel()
        
        dialog.showsResizeIndicator    = true
        dialog.showsHiddenFiles        = false
        dialog.canCreateDirectories    = true
        dialog.allowedFileTypes = ["mp4"]
        
        if let importPath = importedImagePath {
            dialog.nameFieldStringValue = importPath.components(separatedBy: "/").last!.components(separatedBy: ".").first! + "_video.mp4"
        } else {
            dialog.nameFieldStringValue = "video.mp4"
        }
        
        
        if (dialog.runModal() == NSApplication.ModalResponse.OK) {
            if let url = dialog.url {
                
                updateConsoleText(text: "Exporting AV file, please wait...\n")
                
                Thread.detachNewThread { // It exits automatically as soon as the task is completed
                    self.exportComposition(composition: avComposition.copy() as! AVComposition, exportUrl: url, beforeCompletion: nil) { (message) in
                        
                        self.updateConsoleText(text: message + "\n")
                    }
                }
                
            }
        } else {
            // User clicked on "Cancel"
            return
        }
    }
    
    @IBAction func mysteriousSoundSliderChanged(_ sender: Any) {
        
        mysteriousSoundSlider.intValue = Int32(round(mysteriousSoundSlider.doubleValue))
        astroDetector.setMysteriousFactor(mysteriousSoundSlider.intValue)
    }
    
    @IBAction func euphoriaSoundSliderChanged(_ sender: Any) {
        
        euphoriaSoundSlider.intValue = Int32(round(euphoriaSoundSlider.doubleValue))
        astroDetector.setEuphoriaFactor(euphoriaSoundSlider.intValue)
    }
    
    @IBAction func reverbSoundSliderChanged(_ sender: Any) {
        reverbSoundSlider.intValue = Int32(round(reverbSoundSlider.doubleValue))
        astroDetector.setReverbFactor(reverbSoundSlider.intValue)
    }
    
    override func prepare(for segue: NSStoryboardSegue, sender: Any?) {
        if segue.destinationController is ExplorerWindowController {
            
            let explorerWindowController = segue.destinationController as? ExplorerWindowController
            let explorerViewController = explorerWindowController?.contentViewController as? ExplorerViewController
            
            if let path = importedImagePath {
                explorerViewController?.importedImagePath = path
                explorerViewController?.astroDetector = self.astroDetector
                explorerViewController?.handleDetectionImageLoading()
            }
        }
    }
    
    @IBAction func sonifyButtonPressed(_ sender: Any) {
        
        sonifyButton.isEnabled = false
        
        if let path = self.importedImagePath {
            self.updateConsoleText(text: "Detecting objects in image: " + path.components(separatedBy: "/").last! + " ...\n")
        }
        
        noOutputLabel.isHidden = true
        playerView.isHidden = true
        sonifierActivityIndicator.isHidden = false
        sonifierActivityIndicator.startAnimating()
        
        print("URLLL: \(astroDetector.getAudioPath())")
        
        Thread.detachNewThread { // It exits automatically as soon as the task is completed
            if let path = self.importedImagePath {
                if self.astroDetector.detectObjects(path) == true {
                    if self.astroDetector.sonifyWithVideo() == true {
                        self.handleSonifiedImageLoading()
                    }
                }
            }
        }
    }
    
    func handleSonifiedImageLoading() {
        
        self.sonifierActivityIndicator.stopAnimating()
        self.sonifierActivityIndicator.isHidden = true
        
        let combinedAudioVideo = mergeFilesWithUrl(videoUrl: astroDetector.getVideoURL(), audioUrl: astroDetector.getAudioURL())
        avComposition = combinedAudioVideo
        let playerItem = AVPlayerItem(asset: combinedAudioVideo)
        let player = AVPlayer(playerItem: playerItem)
        
        self.playerView.player = player
        self.playerView.isHidden = false
        
        self.exportAudioButton.isEnabled = true
        self.exportVideoButton.isEnabled = true
        self.sonifyButton.isEnabled = true
        
        self.exploreButton.isEnabled = true
    }
    
    // This function creates a mix composition by merging audio with video
    func mergeFilesWithUrl(videoUrl:URL, audioUrl:URL) -> AVMutableComposition
    {
        let mixComposition : AVMutableComposition = AVMutableComposition()
        var mutableCompositionVideoTrack : [AVMutableCompositionTrack] = []
        var mutableCompositionAudioTrack : [AVMutableCompositionTrack] = []
        
        
        // Start the merge
        
        let aVideoAsset : AVAsset = AVAsset(url: videoUrl)
        let aAudioAsset : AVAsset = AVAsset(url: audioUrl)
        mutableCompositionVideoTrack.append(mixComposition.addMutableTrack(withMediaType: AVMediaType.video, preferredTrackID: kCMPersistentTrackID_Invalid)!)
        mutableCompositionAudioTrack.append( mixComposition.addMutableTrack(withMediaType: AVMediaType.audio, preferredTrackID: kCMPersistentTrackID_Invalid)!)
        
        let aVideoAssetTrack : AVAssetTrack = aVideoAsset.tracks(withMediaType: AVMediaType.video)[0]
        let aAudioAssetTrack : AVAssetTrack = aAudioAsset.tracks(withMediaType: AVMediaType.audio)[0]
        
        do{
            try mutableCompositionVideoTrack[0].insertTimeRange(CMTimeRangeMake(start: CMTime.zero, duration: aVideoAssetTrack.timeRange.duration), of: aVideoAssetTrack, at: CMTime.zero)
            
            try mutableCompositionAudioTrack[0].insertTimeRange(CMTimeRangeMake(start: CMTime.zero, duration: aVideoAssetTrack.timeRange.duration), of: aAudioAssetTrack, at: CMTime.zero)
            
        } catch{
            
        }
        
        return mixComposition
    }
    
    func exportComposition(composition: AVComposition, exportUrl: URL, beforeCompletion: (()->Void)?, completion: ((_ message: String)->Void)?) {
        
        let assetExport: AVAssetExportSession = AVAssetExportSession(asset: composition, presetName: AVAssetExportPresetHighestQuality)!
        assetExport.outputFileType = AVFileType.mp4
        assetExport.outputURL = exportUrl
        assetExport.shouldOptimizeForNetworkUse = true

        if let beforeCompletion = beforeCompletion {
            beforeCompletion()
        }
        
        assetExport.exportAsynchronously { () -> Void in
            
            var exportMessage = ""
            
            switch assetExport.status {

            case AVAssetExportSession.Status.completed:

                // Uncomment this if u want to store the video in the assets folder

                // let assetsLib = ALAssetsLibrary()
                // assetsLib.writeVideoAtPathToSavedPhotosAlbum(savePathUrl, completionBlock: nil)

                exportMessage = "AV composition exported successfully."
                
            case  AVAssetExportSession.Status.failed:
                exportMessage = "AV compostion export failed: \(assetExport.error)"
            case AVAssetExportSession.Status.cancelled:
                exportMessage = "AV compostion export cancelled: \(assetExport.error)"
            default:
                exportMessage = "AV compostion export complete."
            }
            
            if let completion = completion {
                completion(exportMessage)
            }
        }
    }
    
    // LOGGING
    
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
    
    @IBAction func importImage(_ sender: Any) {
        
        let dialog = NSOpenPanel();
        
        dialog.showsResizeIndicator    = true;
        dialog.showsHiddenFiles        = false;
        dialog.canChooseDirectories    = false;
        dialog.canCreateDirectories    = false;
        dialog.allowsMultipleSelection = false;
        dialog.allowedFileTypes        = ["png", "jpg", "jpeg"];
        
        if (dialog.runModal() == NSApplication.ModalResponse.OK) {

            if let result = dialog.url {
                importedImagePath = result.path
                
                importImageView.setImage(imagePath: result.path)
                importImageView.isHidden = false
                importImageButton.isHidden = true
                
                importedImageNameLabel.stringValue = result.path.components(separatedBy: "/").last!
                
                sonifyButton.isEnabled = true
            }
        } else {
            // User clicked on "Cancel"
            return
        }
    }
    
    // MENU ACTIONS
    
    @IBAction func showConsole(_ sender: NSMenuItem) {
        consoleStackView.isHidden = !consoleStackView.isHidden
    }
    
    @IBAction func importImageFromMenu(_ sender: Any) {
        self.importImage(self)
    }
    
    func updateConsoleText(text : String) {
        consoleTextView.string += text
        consoleTextView.scrollToEndOfDocument(self)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
        
        // Set background color
        self.view.wantsLayer = true
        self.view.layer?.contents = NSImage(named: "dark_gray")
        self.view.needsDisplay = true
        
        // Add buttons
        buttons = [importImageButton, sonifyButton, exportAudioButton, exportVideoButton, exploreButton]
        setUpButtons(buttons: buttons)
        
        // UI elements
        setUpUiElements()
        
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
        }
    }
    
    override func mouseExited(with event: NSEvent) {
        
        // Identify which button triggered the mouseExited event
        if let buttonIndex = event.trackingArea?.userInfo?["buttonIndex"] as? Int {
            if buttonIndex == 0 {
                
                if let image = NSImage(named: "button") {
                    buttons[buttonIndex].updateImage(image: image)
                }
                
                let title = "No Image Imported"
                if let mutableAttributedTitle = buttons[buttonIndex].attributedTitle.mutableCopy() as? NSMutableAttributedString {
                    mutableAttributedTitle.replaceCharacters(in: NSMakeRange(0, mutableAttributedTitle.length), with: title)
                    buttons[buttonIndex].attributedTitle = mutableAttributedTitle
                }
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
            
            if let image = NSImage(named: "button_highlight") {
                button.updateHighlightImage(image: image)
            }
        }
        
        sonifyButton.setCornerRadius(radius: 5)
        exportAudioButton.setCornerRadius(radius: 5)
        exportVideoButton.setCornerRadius(radius: 5)
        exploreButton.setCornerRadius(radius: 5)
        
        exportAudioButton.isEnabled = false
        exportVideoButton.isEnabled = false
        exploreButton.isEnabled = false
        sonifyButton.isEnabled = false
    }
    
    func setUpUiElements() {
        
        // Imported image view
        importImageView.isHidden = true
        
        // Imported Image label
        importedImageNameLabel.stringValue = ""
        
        // Console
        
        consoleScrollView.setCornerRadius(radius: 10)
    
        // Uncomment the following line to display the same color as the buttons
//        consoleTextView.backgroundColor = NSColor(red: 66.0 / 255, green: 67.0 / 255, blue: 76.0 / 255, alpha: 1.0)
        consoleTextView.backgroundColor = NSColor(red: 66.0 / 255, green: 70.0 / 255, blue: 79.0 / 255, alpha: 1.0)
        consoleTextView.font = NSFont.systemFont(ofSize: 13)
        consoleTextView.textColor = NSColor.white.withAlphaComponent(0.9)
        consoleTextView.string = "Sonifier initialised \n"
        consoleTextView.scrollToEndOfDocument(self)
        
        // Player view
        
        playerViewContainer.wantsLayer = true
        playerViewContainer.layer?.contents = NSImage(named: "button")
        playerViewContainer.addSubview(noOutputLabel);
        playerViewContainer.addSubview(playerView);
        playerViewContainer.addSubview(sonifierActivityIndicator);
        playerView.isHidden = true
        playerView.showsFullScreenToggleButton = true
        sonifierActivityIndicator.isHidden = true
        sonifierActivityIndicator.type = .lineScale
        sonifierActivityIndicator.color = NSColor.systemGreen.withAlphaComponent(0.9)
        
        // Sound filters
        
        soundFilterView.wantsLayer = true
        mysteriousSoundSliderView.wantsLayer = true
        euphoriaSoundSliderView.wantsLayer = true
        implSoundSliderView.wantsLayer = true
        
        soundFilterView.layer?.backgroundColor = NSColor.black.cgColor
        soundFilterView.layer?.cornerRadius = 10
        mysteriousSoundSliderView.layer?.contents = NSImage(named: "button")
        euphoriaSoundSliderView.layer?.contents = NSImage(named: "button")
        implSoundSliderView.layer?.contents = NSImage(named: "button")
        
        mysteriousSoundSlider.minValue = 0;
        mysteriousSoundSlider.maxValue = 2;
        mysteriousSoundSlider.intValue = 1;
        astroDetector.setMysteriousFactor(mysteriousSoundSlider.intValue)
        
        euphoriaSoundSlider.minValue = 0;
        euphoriaSoundSlider.maxValue = 2;
        euphoriaSoundSlider.intValue = 1;
        astroDetector.setEuphoriaFactor(euphoriaSoundSlider.intValue)
        
        reverbSoundSlider.minValue = 0;
        reverbSoundSlider.maxValue = 2;
        reverbSoundSlider.intValue = 1;
        astroDetector.setReverbFactor(reverbSoundSlider.intValue)
    }
}
