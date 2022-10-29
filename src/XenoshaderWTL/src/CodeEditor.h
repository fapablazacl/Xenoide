
#pragma once 

#include <string>
#include <vector>
#include <tuple>
#include <ILexer.h>
#include <Scintilla.h>
#include <Sci_Position.h>
#include <SciLexer.h>
#include <Lexilla.h>
#include <LexillaAccess.h>
#include <boost/filesystem/path.hpp>
#include <xeno/wtl/wtl.h>


struct CodeViewStyleAttribs {
    COLORREF fore = RGB(255, 255, 255);
    COLORREF back = RGB(255, 255, 255);
    int size = 0;
    const char *face = nullptr;
};


constexpr COLORREF black = RGB(0, 0, 0);
constexpr COLORREF white = RGB(255, 255, 255);
constexpr char *defaultFontName = "Courier New";
constexpr int defaultFontSize = 10;

enum class CodeLanguage {
    TEXT,
    CPP,
    GLSL
};


struct CodeViewLanguageConfig {
    CodeViewStyleAttribs defaultStyle = {
        black, white, defaultFontSize, defaultFontName
    };

    std::string keywords;
    std::vector<std::pair<int, COLORREF>> stylesColors;
};


extern CodeViewLanguageConfig languageConfigC;

extern CodeViewLanguageConfig languageConfigGLSL;

extern std::vector<std::string> extensionsC;

extern std::vector<std::string> extensionsGLSL;

extern CodeLanguage getCodeLanguage(const boost::filesystem::path& filePath);


class CodeEditor : public CWindowImpl<CodeEditor> {
public:
    DECLARE_WND_CLASS(NULL)

    BEGIN_MSG_MAP_EX(CodeEditor)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_SIZE(OnSize)

        MSG_WM_NOTIFY(OnNotify)
    END_MSG_MAP()

public:
    CodeEditor();

    LRESULT OnCreate(LPCREATESTRUCT cs);

    LRESULT OnNotify(int idCtrl, LPNMHDR pnmh);

    void OnDestroy();

    void OnSize(UINT nType, CSize size);

    void SetStyleAttribs(const int style, const CodeViewStyleAttribs& attribs);

    void ClearLanguage();

    void SetLanguage(const ILexer5* lexer, const CodeViewLanguageConfig& config);

    void SetInitialContent(const char* textContent);

    std::string GetContent() const;

    void ClearSaveState();

private:
    std::string identifier;
    mutable CWindow mWndScintilla;
};
