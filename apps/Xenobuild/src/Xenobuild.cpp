

#include <iostream>
#include <vector>
#include <string>
#include <map>

namespace model {
    struct IncludeDir {
        std::string path;
    };

    using PreprocessorDefinitions = std::map<std::string, std::string>;

    namespace package {
        struct Dependency {
            std::string identifier;
        };

        struct SourceFile {
            std::string path;
        };

        enum class ModuleType {
            Executable, 
            StaticLibrary,
            SharedLibrary
        };

        struct Module {
            std::string name;
            ModuleType type;
            std::vector<Dependency> dependencies;
            std::vector<SourceFile> sourceFiles;
            std::vector<IncludeDir> includeDirs;
        };

        struct Package {
            std::string name;
            std::string version;
            std::vector<Module> modules;
        };
    }

    namespace toolchain {
        struct Configuration {
            std::string name;
            std::string triplet;
        };

        struct CompilerInput {
            
        };

        struct Compiler {
            std::string name;
        };

        struct Linker {

        };

        struct Toolchain {
            std::string identifier;
        };
    }


    namespace build {
        class Hola {
        };
    }


    // low level layer
    struct Command {
        std::string name;
        std::vector<std::string> args;
    };
}



int main() {

    return 0;
}
