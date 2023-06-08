
#include "CodeEditor.h"
#include <boost/filesystem/path.hpp>


CodeEditor::CodeEditor() {
    m_bMsgHandled = false;
}


LRESULT CodeEditor::OnCreate(LPCREATESTRUCT cs) {
    mWndScintilla.Create(_T("Scintilla"), m_hWnd, rcDefault, _T(""), WS_CHILD | WS_VISIBLE);

    const LRESULT marginWidth = mWndScintilla.SendMessage(SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)"_99999");

    mWndScintilla.SendMessage(SCI_SETCARETLINEVISIBLE, 1);
    mWndScintilla.SendMessage(SCI_SETMARGINWIDTHN, 0, marginWidth);
    mWndScintilla.SendMessage(SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);

    mWndScintilla.SendMessage(SCI_SETUSETABS, 0);
    mWndScintilla.SendMessage(SCI_SETTABWIDTH, 4);
    mWndScintilla.SendMessage(SCI_SETMOUSEDWELLTIME, 2000);

    // configure for generic text
    SetStyleAttribs(STYLE_DEFAULT, {black, white, defaultFontSize, defaultFontName});

    SetMsgHandled(true);

    return 0;
}

LRESULT CodeEditor::OnNotify(int idCtrl, LPNMHDR pnmh) {
    if (pnmh->code >= 2000 && pnmh->code < 3000) {
        auto notification = *reinterpret_cast<SCNotification*>(pnmh);

        if (config.notificationHandler) {
            config.notificationHandler->handleNotification(mWndScintilla, notification);
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

    config = {};
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
    
    this->config = config;
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
