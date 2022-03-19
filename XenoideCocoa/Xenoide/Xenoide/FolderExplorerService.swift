//
//  FolderExplorerService.swift
//  Xenoide
//
//  Created by Felipe Apablaza on 01-02-22.
//

import Foundation

public struct Path {
    public var name: String = ""
    
    public var children: [Path] = []
    
    public init(name: String, children: [Path]) {
        self.name = name
        self.children = children
    }
}


public class FolderExplorerService {
    
}
