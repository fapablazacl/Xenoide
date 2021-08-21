//
//  AppDelegate.swift
//  Xenoide
//
//  Created by Felipe Apablaza on 18-05-21.
//

import Cocoa

@main
class AppDelegate: NSObject, NSApplicationDelegate {
    @IBOutlet var window: NSWindow!

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Insert code here to initialize your application
        
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }
    
    @IBAction func openDocument(_ sender: Any) {
        let openPanel = NSOpenPanel()
        
        openPanel.title = "Open C Source File ..."
        openPanel.allowedFileTypes = ["c"]
        
        openPanel.begin(completionHandler: { (result) -> Void in
            if result ==  NSApplication.ModalResponse.OK {
                let fileUrl: URL? = openPanel.url
                
                if fileUrl != nil {
                    do {
                        let filePath: String = fileUrl!.path
                        let content = try String(contentsOfFile: filePath)
                        
                        print(content)
                        
                        
                        // self.codeTextView.string = content
                    } catch {
                        print("Some error happened!")
                    }
                }
            }
        })
    }
}
