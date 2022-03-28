# Abstract

Xenoide is an IDE, focused on the development of software using non-mainstream programming languages, libraries and frameworks. It’s a niche application development IDE, enabling the development of the following kinds of software:
- Native Desktop Applications, for Windows, Linux, macOS, FreeBSD, and possibly others.
- Games and Applications for Legacy Consoles. For example: The Atari 8-bit family, NES, and N64.
- Broodwar AI Bots.

# Non-functional Requirements

## Cross platform
Must support all major desktop operating systems, including, but not limited to, the following:

- Windows XP*, 7, 10, 11+
- Linux
- macOS
- FreeBSD

## Native User Experience
Must use the native user interface toolkit for each platform. Altrough this desicion may seem controversial, due to the existence of several open-source, cross-platform UI frameworks, there are still some valid reasons to use these platform specific toolkits.

### Advantages to this approach
- UX: These platform specific toolkits will provide a seamless and familiar experience to the User. The App will be pertectly integrated with the rest of the system, due to the direct access to many API's.
- Performance: 

### Disavantages
However, we have some disavantages:
- Increased development complexity: We will have to implement the UI for each supported toolkit.
- Increased learning time: Learn to code with the Windows UI API, GTK, Cocoa, etc.

### Mitigations
- Create an UI Design System.
- Create a highly reusable UI component library for those platforms, that follows those UI conventions.
- Refactor common IDE logic, like interaction with build system, language services, etc into an cross-platform internal libraries. This layer has to be highly testable, in order to decrease the later effort of manual testing for each platform.

### Considered UI Toolkits, per Platform
The considered UI toolkit for each host platform are the following:
- Windows: WTL/WinAPI.
- macOS: Cococa (Swift/objc)
- Linux/FreeBSD: GTK3 and/or Qt5.

## Modular, Extensible and Testable
Must have a proper achitecture, one that provides the following quality attributes:

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
