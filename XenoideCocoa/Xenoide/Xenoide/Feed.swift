//
//  Feed.swift
//  Xenoide
//
//  Created by Felipe Apablaza on 11-02-22.
//

import Cocoa


class Feed: NSObject {
    public var name: String
    public var children: [Feed] = []
    
    public init(name: String) {
        self.name = name
    }
    
    public init (name: String, children: [Feed]) {
        self.name = name
        self.children = children
    }
}
