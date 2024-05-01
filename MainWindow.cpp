
#include "MainWindow.h"

#include <QLabel>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>


static const char *keywords = {
    "alignas alignof and and_eq asm atomic_cancel atomic_commit" " "
    "atomic_noexcept auto bitand bitor bool break case catch char" " "
    "char16_t char32_t class compl concept const constexpr const_cast" " "
    "continue decltype default delete do" " "
    "double dynamic_cast else enum explicit export extern false float" " "
    "for friend goto if inline int import long" " "
    "module mutable namespace new noexcept not not_eq nullptr operator" " "
    "or or_eq private protected public" " "
    "register reinterpret_cast requires return short signed sizeof static" " "
    "static_assert static_cast struct" " "
    "switch synchronized template this thread_local" " "
    "throw true try typedef typeid typename union unsigned" " "
    "using virtual void volatile wchar_t while xor xor_eq"
};


namespace xenoide {
    MainWindow::MainWindow() {
        mScintilla = new QsciScintilla(this);

        setupEditor(mScintilla);

        setCentralWidget(mScintilla);
    }

    void MainWindow::setupEditor(QsciScintilla *scintilla)
    {
        const QFont defaultFont{ "Consolas", 10 };

        scintilla->SendScintilla(QsciScintilla::SCI_SETBUFFEREDDRAW, false);
        scintilla->setFont(defaultFont);
        scintilla->setCaretLineVisible(true);
        scintilla->setCaretLineBackgroundColor(QColor(240, 240, 240));
        scintilla->setTabWidth(4);
        scintilla->setIndentationsUseTabs(false);

        scintilla->setMarginWidth(1, QString("1000"));
        scintilla->setMarginType(1, QsciScintilla::NumberMargin);

        // setup C++
        auto lexer = new QsciLexerCPP{};
        lexer->setDefaultFont(defaultFont);
        scintilla->setLexer(lexer);
        scintilla->SendScintilla(QsciScintilla::SCI_STYLECLEARALL);
        scintilla->SendScintilla(QsciScintilla::SCI_SETKEYWORDS, 0, (void*)keywords);
        scintilla->SendScintilla(QsciScintilla::SCI_STYLESETFORE, QsciLexerCPP::Comment, QColor(0, 255, 0));
        scintilla->SendScintilla(QsciScintilla::SCI_STYLESETFORE, QsciLexerCPP::Keyword, QColor(0, 0, 255));
        scintilla->SendScintilla(QsciScintilla::SCI_STYLESETFORE, QsciLexerCPP::DoubleQuotedString, QColor(255, 0, 0));
        scintilla->SendScintilla(QsciScintilla::SCI_STYLESETFORE, QsciLexerCPP::SingleQuotedString, QColor(255, 0, 0));
        scintilla->SendScintilla(QsciScintilla::SCI_STYLESETFORE, QsciLexerCPP::PreProcessor, QColor(127, 127, 255));
    }
}
