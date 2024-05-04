
#include "MainWindow.h"

#include <QLabel>
#include <QMenuBar>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <string>
#include <map>

#include "LanguageConfig.h"

namespace xenoide {
    MainWindow::MainWindow() {
        mScintilla = new QsciScintilla(this);

        createMenuBar();

        setupEditor(mScintilla);

        setCentralWidget(mScintilla);
    }

    void MainWindow::createMenuBar() {
        // Create the menu bar
        QMenuBar* menuBar = new QMenuBar(this);
        setMenuBar(menuBar);

        // Create the File menu
        QMenu* fileMenu = menuBar->addMenu(tr("File"));

        QAction* newAction = new QAction(tr("New"), this);
        QAction* openAction = new QAction(tr("Open"), this);
        QAction* saveAction = new QAction(tr("Save"), this);
        QAction* exitAction = new QAction(tr("Exit"), this);

        newAction->setShortcut(Qt::CTRL | Qt::Key_N);
        openAction->setShortcut(Qt::CTRL | Qt::Key_O);
        saveAction->setShortcut(Qt::CTRL | Qt::Key_S);
        exitAction->setShortcut(Qt::ALT | Qt::Key_F4);

        fileMenu->addAction(newAction);
        fileMenu->addSeparator();
        fileMenu->addAction(openAction);
        fileMenu->addAction(saveAction);
        fileMenu->addSeparator();
        fileMenu->addAction(exitAction);

        // Create the Edit menu
        QMenu* editMenu = menuBar->addMenu(tr("Edit"));
        QAction* cutAction = new QAction(tr("Cut"), this);
        QAction* copyAction = new QAction(tr("Copy"), this);
        QAction* pasteAction = new QAction(tr("Paste"), this);

        cutAction->setShortcut(Qt::CTRL | Qt::Key_X);
        copyAction->setShortcut(Qt::CTRL | Qt::Key_C);
        pasteAction->setShortcut(Qt::CTRL | Qt::Key_V);

        editMenu->addAction(cutAction);
        editMenu->addAction(copyAction);
        editMenu->addAction(pasteAction);

        // Create the Help menu
        QMenu* helpMenu = menuBar->addMenu(tr("Help"));
        QAction* aboutAction = new QAction(tr("About"), this);

        helpMenu->addAction(aboutAction);
    }

    void MainWindow::setLanguage(const LanguageKeywords &keywords) {
        mScintilla->SendScintilla(QsciScintilla::SCI_STYLECLEARALL);
        mScintilla->SendScintilla(QsciScintilla::SCI_SETKEYWORDS, 0, (void*)keywords.keywords.c_str());
        mScintilla->SendScintilla(QsciScintilla::SCI_SETKEYWORDS, 1, (void*)keywords.reservedKeywords.c_str());
        mScintilla->SendScintilla(QsciScintilla::SCI_STYLESETFORE, QsciLexerCPP::Comment, QColor(0, 255, 0));
        mScintilla->SendScintilla(QsciScintilla::SCI_STYLESETFORE, QsciLexerCPP::Keyword, QColor(0, 0, 255));
        mScintilla->SendScintilla(QsciScintilla::SCI_STYLESETFORE, QsciLexerCPP::DoubleQuotedString, QColor(255, 0, 0));
        mScintilla->SendScintilla(QsciScintilla::SCI_STYLESETFORE, QsciLexerCPP::SingleQuotedString, QColor(255, 0, 0));
        mScintilla->SendScintilla(QsciScintilla::SCI_STYLESETFORE, QsciLexerCPP::PreProcessor, QColor(127, 127, 255));
    }

    void MainWindow::setupEditor(QsciScintilla *scintilla) {
        const QFont defaultFont{ "Consolas", 10 };

        scintilla->SendScintilla(QsciScintilla::SCI_SETBUFFEREDDRAW, false);
        scintilla->setFont(defaultFont);
        scintilla->setCaretLineVisible(true);
        scintilla->setCaretLineBackgroundColor(QColor(240, 240, 240));
        scintilla->setTabWidth(4);
        scintilla->setIndentationsUseTabs(false);

        scintilla->setMarginWidth(1, QString("1000"));
        scintilla->setMarginType(1, QsciScintilla::NumberMargin);

        auto lexer = new QsciLexerCPP{};
        lexer->setDefaultFont(defaultFont);
        scintilla->setLexer(lexer);

        setLanguage(languageMap.at(LanguageDialect::GLSL_450));

    }
}
