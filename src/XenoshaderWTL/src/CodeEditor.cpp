
#include "CodeEditor.h"
#include <boost/filesystem/path.hpp>


CodeEditor::CodeEditor() {
    m_bMsgHandled = false;
}


LRESULT CodeEditor::OnCreate(LPCREATESTRUCT cs) {
    mWndScintilla.Create(_T("Scintilla"), m_hWnd, rcDefault, _T(""), WS_CHILD | WS_VISIBLE);
    mWndScintilla.SendMessage(SCI_SETUSETABS, 0);
    mWndScintilla.SendMessage(SCI_SETTABWIDTH, 4);
        
    mWndScintilla.SendMessage(SCI_SETMOUSEDWELLTIME, 2000);

    // configure for generic text
    SetStyleAttribs(STYLE_DEFAULT, {black, white, defaultFontSize, defaultFontName});

    SetMsgHandled(true);

    return 0;
}

LRESULT CodeEditor::OnNotify(int idCtrl, LPNMHDR pnmh) {
    if (pnmh->code == SCN_DWELLSTART) {
        auto notification = reinterpret_cast<SCNotification*>(pnmh);

        mWndScintilla.SendMessage(SCI_CALLTIPSHOW, notification->position, reinterpret_cast<LPARAM>("void glBegin(GLenum mode)"));
    }

    if (pnmh->code == SCN_CHARADDED) {
        auto notification = reinterpret_cast<SCNotification*>(pnmh);

        if (notification->ch == '(') {
            const auto pos = mWndScintilla.SendMessage(SCI_GETCURRENTPOS);
            mWndScintilla.SendMessage(SCI_CALLTIPSHOW, pos, reinterpret_cast<LPARAM>("The glBegin and glend functions delimit the vertices of a primitive or a group of like primitives.\nmode: The primitive or primitives that will be created from vertices presented between glBegin and the subsequent glEnd."));
            mWndScintilla.SendMessage(SCI_CALLTIPSETHLT, 99, 103);
                
        } else if (notification->ch == ' ') {
            identifier = "";
        } else if (std::isalpha(notification->ch)) {
            identifier += static_cast<char>(notification->ch);
                
            if (identifier.size() >= 3) {
                mWndScintilla.SendMessage(SCI_AUTOCSHOW, identifier.size(), reinterpret_cast<LPARAM>("void int float pointer test"));
                identifier = "";
            }
        } else {
            identifier = "";
        }
    }

    SetMsgHandled(false);
        
    return 0;
}


void CodeEditor::OnDestroy() {
    mWndScintilla.DestroyWindow();
    mWndScintilla.Detach();

    SetMsgHandled(false);
}

void CodeEditor::OnSize(UINT nType, CSize size) {
    const CRect rect = { 0, 0, size.cx, size.cy };

    mWndScintilla.SetWindowPos(NULL, rect, 0);
}

void CodeEditor::SetStyleAttribs(const int style, const CodeEditorStyle &attribs) {
    mWndScintilla.SendMessage(SCI_STYLESETFORE, style, attribs.fore);
    mWndScintilla.SendMessage(SCI_STYLESETBACK, style, attribs.back);

    if (attribs.size > 0) {
        mWndScintilla.SendMessage(SCI_STYLESETSIZE, style, attribs.size);
    }
        
    if (attribs.face) {
        mWndScintilla.SendMessage(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(attribs.face));
    }
}

void CodeEditor::ClearLanguage() {
    mWndScintilla.SendMessage(SCI_STYLECLEARALL);
    mWndScintilla.SendMessage(SCI_CLEARDOCUMENTSTYLE);
}

void CodeEditor::SetLanguage(const ILexer5 *lexer, const CodeEditorConfiguration &config) {
    mWndScintilla.SendMessage(SCI_STYLECLEARALL);
    mWndScintilla.SendMessage(SCI_CLEARDOCUMENTSTYLE);

    mWndScintilla.SendMessage(SCI_SETILEXER, 0, reinterpret_cast<LPARAM>(lexer));
    mWndScintilla.SendMessage(SCI_SETKEYWORDS, 0, reinterpret_cast<LPARAM>(config.keywords.c_str()));

    SetStyleAttribs(SCE_C_DEFAULT, config.defaultStyle);
    for (const auto styleColor : config.stylesColors) {
        SetStyleAttribs(styleColor.first, {styleColor.second, white, 0, nullptr});
    }

    mWndScintilla.SendMessage(SCI_STYLESETBOLD, SCE_C_WORD, 1);
    mWndScintilla.SendMessage(SCI_STYLESETBOLD, SCE_C_WORD2, 1);
}


void CodeEditor::Configure(const CodeEditorConfiguration &config) {
    mWndScintilla.SendMessage(SCI_STYLECLEARALL);
    mWndScintilla.SendMessage(SCI_CLEARDOCUMENTSTYLE);

    mWndScintilla.SendMessage(SCI_SETILEXER, 0, reinterpret_cast<LPARAM>(config.lexer));
    mWndScintilla.SendMessage(SCI_SETKEYWORDS, 0, reinterpret_cast<LPARAM>(config.keywords.c_str()));

    SetStyleAttribs(SCE_C_DEFAULT, config.defaultStyle);
    for (const auto styleColor : config.stylesColors) {
        SetStyleAttribs(styleColor.first, {styleColor.second, white, 0, nullptr});
    }

    mWndScintilla.SendMessage(SCI_STYLESETBOLD, SCE_C_WORD, 1);
    mWndScintilla.SendMessage(SCI_STYLESETBOLD, SCE_C_WORD2, 1);
}


void CodeEditor::SetInitialContent(const char *textContent) {
    mWndScintilla.SendMessage(SCI_SETTEXT, 0, reinterpret_cast<LPARAM>(textContent));
    mWndScintilla.SendMessage(SCI_EMPTYUNDOBUFFER);
    mWndScintilla.SendMessage(SCI_SETSAVEPOINT);
}

std::string CodeEditor::GetContent() const {
    std::string content;

    const LRESULT contentLength = mWndScintilla.SendMessage(SCI_GETLENGTH);

    content.resize(static_cast<size_t>(contentLength));

    mWndScintilla.SendMessage(SCI_GETTEXT, contentLength + 1, reinterpret_cast<LPARAM>(content.c_str()));

    return content;
}

void CodeEditor::ClearSaveState() {
    mWndScintilla.SendMessage(SCI_SETSAVEPOINT);
}
