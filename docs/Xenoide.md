# Abstract

Modern IDEs are focused in mainstream tech stacks, like PHP, NodeJS, Java, etc, and neglecting the support for languages like C++, Rust or Haskell. On the other hand, the language support for those languages in Code Editors, like Visual Studio Code, is incomplete.

Xenoide is an IDE, focused on the development of software using non-mainstream programming languages, libraries and frameworks. It’s a niche application development IDE, enabling the development of the following software kinds:
- Native Desktop Application, for Windows, Linux, macOS, FreeBSD, and possibly others.
- Games and Applications for Legacy Consoles (Atari, NES, N64).
- Broodwar AI Bots.

# Project Vision


# Non-functional Requirements

## Cross platform
Must support all major desktop operating systems, including, but not limited to, the following:

- Windows 7+
- Linux
- macOS
- FreeBSD

## Native User Experience
Must use the native user interface toolkit for each platform. The reason is to provide a familiar UX for the User. The considered UI toolkit for each platform are the following:

- Windows: WTL/WinAPI.
- macOS: Cococa (Swift/objc)
- Linux/FreeBSD: GTK3 / Qt5

## Modular, Extensible and Testable
Must use a proper achitecture, one that provides the following quality attributes:

- Modular: The IDE must be separated in different Modules.
- Extensible: Must be extensible, via plugins, and the IDE must integrate that new functionality seamlessly.
- Testable: Each software unit must be testable.

# Functional Requirements
## User Interface Elements 
### Source Code Editor
El editor de codigo fuente debe ser capaz de proveer todas las funcionalidades minimas que se esperan durante las actividades de edició de codigo, ya que es el foco principal durante la fase de implementación. El editor de cdigo fuente debe proveer la siguiente información:

#### Syntax Coloring

#### Autocomplete Symbol

#### Show Symbol Tooltip

#### GoTo Symbol Definition

#### Language-Specific Actions

#### Source File Outline


### Folder Navigation


### Class Browsing


### Output Window

### IDE Logs


## Workspace Management
Workspace Management refers to the way the User interacts and manipulates the software solution, loaded in the IDE. The Workspace Management provides the following capabilities:

- Builds the software solution
- Detects the built artifacts.
- Detects all the files that compromise the solution
- Provides multiple views to the

These capabilities depends heavly on the build-system used by the software solution. The support must consider the following build-systems:

- CMake.

## Build System
