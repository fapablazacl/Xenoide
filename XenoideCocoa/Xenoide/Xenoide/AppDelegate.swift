//
//  AppDelegate.swift
//  Xenoide
//
//  Created by Felipe Apablaza on 12-01-22.
//

import Cocoa


@main
class AppDelegate: NSObject, NSApplicationDelegate {
    @IBOutlet var window: NSWindow!
    
    @IBOutlet weak var outlineView: NSOutlineView!
    
    private var feeds: [Feed] = []
    
    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // mock data
        feeds = [
            Feed(
                name: "Cathegory",
                children: [
                    Feed(name: "Sub Cathegory 1"),
                    Feed(name: "Sub Cathegory 2")
                ]
            ),
            Feed(
                name: "Cathegory2 ",
                children: [
                    Feed(name: "Mini 1"),
                    Feed(name: "Mini 2"),
                    Feed(name: "Mini 3")
                ]
            )
        ]
        
        //
        // self.outlineView.dataSource = self
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }

    func applicationSupportsSecureRestorableState(_ app: NSApplication) -> Bool {
        return true
    }
    
    func newDocument() {
        
    }
    
    @IBAction func folderOpened(_ sender: Any) {
        let panel = NSOpenPanel()
        
        panel.canChooseFiles = false
        panel.canChooseDirectories = true
        panel.runModal()
        
        if panel.directoryURL == nil {
            return
        }
        
        let url : URL = panel.directoryURL!
        
        self.openProject(url: url)
        
    }
    
    func openProject(url: URL) {
        print(url)
    }
}


extension AppDelegate : NSOutlineViewDataSource {
    // returns the number of children of the specified item
    func outlineView(_ outlineView: NSOutlineView, numberOfChildrenOfItem item: Any?) -> Int {
        //1
        if let feed = item as? Feed {
            return feed.children.count
        }

        //2
        return feeds.count
    }
    
    // returns the child Item of the specified Item, based on its index
    func outlineView(_
                     outlineView: NSOutlineView,
                     child index: Int,
                     ofItem item: Any?) -> Any {
        
        if let feed = item as? Feed {
            return feed.children[index]
        }

        return feeds[index]
    }
    
    // checks if the specified item is expandable
    func outlineView(_ outlineView: NSOutlineView, isItemExpandable item: Any) -> Bool {
        if let feed = item as? Feed {
            return feed.children.count > 0
        }

        return false
    }
}
