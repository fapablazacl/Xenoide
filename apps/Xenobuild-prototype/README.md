# Xenobuild

Command-line tool to manage the git-hosted dependencies for CMake projects, caching them in a user folder for future reuse.

Currently, implements two subcommands:

- **setup**: Downloads, builds and installs the project dependencies, and generates locally the Debug and Relese build configurations, using the current Toolchain.

- **build**: Builds the project using the Debug and Release build configurations.
