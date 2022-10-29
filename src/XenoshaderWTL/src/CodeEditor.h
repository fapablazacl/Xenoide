
#pragma once 

#include <string>
#include <vector>
#include <tuple>
#include <optional>
#include <boost/filesystem/path.hpp>
#include <ILexer.h>
#include <Scintilla.h>
#include <Sci_Position.h>
#include <SciLexer.h>
#include <Lexilla.h>
#include <LexillaAccess.h>
#include <xeno/wtl/wtl.h>

#include "CodeEditorConfiguration.h"

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

    void SetStyleAttribs(const int style, const CodeEditorStyle& attribs);

    void ClearLanguage();

    void SetLanguage(const ILexer5* lexer, const CodeEditorConfiguration& config);

    void Configure(const CodeEditorConfiguration& config);

    void SetInitialContent(const char* textContent);

    std::string GetContent() const;

    void ClearSaveState();

private:
    std::string identifier;
    mutable CWindow mWndScintilla;
};
