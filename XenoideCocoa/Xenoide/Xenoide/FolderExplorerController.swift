//
//  FolderExplorerController.swift
//  Xenoide
//
//  Created by Felipe Apablaza on 01-02-22.
//

import Foundation
import Cocoa

public class FolderExplorerController : NSController {
    public func setPath(path: String) -> Void {
        self.path = path
    }
    
    private var path: String = ""
}


extension FolderExplorerController : NSOutlineViewDataSource {
    
}
