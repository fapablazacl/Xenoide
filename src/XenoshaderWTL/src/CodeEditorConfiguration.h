
#pragma once

#include <string>
#include <vector>
#include <optional>
#include <ILexer.h>
#include <Scintilla.h>
#include <Sci_Position.h>
#include <SciLexer.h>
#include <Lexilla.h>
#include <LexillaAccess.h>
#include <boost/filesystem/path.hpp>
#include <xeno/wtl/wtl.h>

struct CodeEditorStyle {
    COLORREF fore = RGB(255, 255, 255);
    COLORREF back = RGB(255, 255, 255);
    int size = 0;
    const char *face = nullptr;
};


constexpr COLORREF black = RGB(0, 0, 0);
constexpr COLORREF white = RGB(255, 255, 255);
constexpr char *defaultFontName = "Courier New";
constexpr int defaultFontSize = 10;

enum class CodeEditorLanguage {
    TEXT,
    CPP,
    GLSL
};


struct CodeEditorConfiguration {
    CodeEditorStyle defaultStyle;

    std::string keywords;
    std::vector<std::pair<int, COLORREF>> stylesColors;

    const ILexer5* lexer = nullptr;

    CodeEditorConfiguration();

    CodeEditorConfiguration(const CodeEditorStyle &defaultStyle, 
                            const std::string &keywords, 
                            const std::vector<std::pair<int, COLORREF>> &stylesColors);

    static std::optional<CodeEditorConfiguration> detect(const boost::filesystem::path& filePath);
};
