
#include "QMainFrame.hpp"

#include <QDesktopWidget>
#include <QCloseEvent>
#include <QMessageBox>

namespace Xenoide {
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

    QMainFrame::QMainFrame(MainFrame::Presenter *presenter) : MainFrame::View(presenter) {
        this->setMinimumSize(this->computeFrameSize());

        outputWindowDockWidget = new QDockWidget("Output Window", this);
        outputWindowTextEdit = new QTextEdit(this);
        outputWindowDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
        outputWindowDockWidget->setWidget(outputWindowTextEdit);
        this->addDockWidget(Qt::BottomDockWidgetArea, outputWindowDockWidget);
        
        folderBrowserDockWidget = new QDockWidget("Folder Browser", this);
        folderBrowserTreeView = new QTreeView(this);
        folderBrowserDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
        folderBrowserDockWidget->setWidget(folderBrowserTreeView);
        this->addDockWidget(Qt::LeftDockWidgetArea, folderBrowserDockWidget);

        this->setCentralWidget(createCodeEditor());

        presenter->handleInitializedView(this);
    }

    QMainFrame::~QMainFrame() {}

    QsciScintilla* QMainFrame::createCodeEditor() {
        QsciScintilla* scintilla = new QsciScintilla(this);

        scintilla->SendScintilla(QsciScintilla::SCI_SETBUFFEREDDRAW, false);
        scintilla->setMarginWidth(1, QString("1000"));

        // auto font = QFont{"Monospace", 24};
        // scintilla->setFont(font);
        scintilla->setCaretLineVisible(true);
        scintilla->setTabWidth(4);
        // scintilla->setIndentationsUseTabs(false);
        scintilla->setMarginType(1, QsciScintilla::NumberMargin);

        // setup C++
        scintilla->setLexer(new QsciLexerCPP{});

        scintilla->SendScintilla(QsciScintilla::SCI_STYLECLEARALL);
        scintilla->SendScintilla(QsciScintilla::SCI_SETKEYWORDS, 0, (void*)keywords);
        scintilla->SendScintilla(QsciScintilla::SCI_STYLESETFORE, QsciLexerCPP::Comment, QColor(0, 255, 0));
        scintilla->SendScintilla(QsciScintilla::SCI_STYLESETFORE, QsciLexerCPP::Keyword, QColor(0, 0, 255));

        return scintilla;
    }

    void QMainFrame::displayTitle(const std::string &value) {
        this->setWindowTitle(value.c_str());
    }

    void QMainFrame::changeWindowState(const MainFrame::Window window, const MainFrame::VisibleState state) {
        const bool visible = (state == MainFrame::VS_SHOW);
        
        switch (window) {
        case MainFrame::W_OUTPUT:
            outputWindowDockWidget->setVisible(visible);
            break;

        case MainFrame::W_FOLDERBROWSER:
            folderBrowserDockWidget->setVisible(visible);
            break;
        }
    }

    void QMainFrame::show() {
        assert(presenter);

        QMainWindow::show();
    }

    MainFrame::AskResult QMainFrame::askCloseConfirmation(const std::string &title, const std::string &prompt) {
        assert(presenter);

        const auto button = QMessageBox::Yes | QMessageBox::No;
        const int result = QMessageBox::question(this, title.c_str(), prompt.c_str(), button);

        if (result == QMessageBox::Yes) {
            return MainFrame::AR_OK;
        }

        return MainFrame::AR_CANCEL;
    }
}

namespace Xenoide {
    void QMainFrame::closeEvent(QCloseEvent *event) {
        assert(presenter);

        if (presenter->handleCloseRequest() == MainFrame::CRA_CLOSE) {
            event->accept();
        } else {
            event->ignore();
        }
    }

    QSize QMainFrame::computeFrameSize() const {
        const double factor = 0.4;
        const QDesktopWidget desktopWidget;
        const QRect screenSize = desktopWidget.availableGeometry(desktopWidget.primaryScreen());

        return screenSize.size() * factor;
    }
}
