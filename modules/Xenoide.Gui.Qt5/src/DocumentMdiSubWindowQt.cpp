
#include <Xenoide/Gui/Qt5/DocumentMdiSubWindowQt.hpp>

#include <cassert>
#include <iostream>

#include <QEvent>
#include <QGridLayout>
#include <QAction>
#include <QMenu>


namespace Xenoide {
    DocumentMdiSubWindowQt::DocumentMdiSubWindowQt(Document::Presenter *presenter) : mDialogManager(this) {
        mPresenter = presenter;

        mScintilla = new QsciScintilla(this);
        mScintilla->SendScintilla(QsciScintilla::SCI_SETBUFFEREDDRAW, false);
        mScintilla->setMarginWidth(1, QString("1000"));

        this->setWidget(mScintilla);

        presenter->onInitialized(this, &mDialogManager);

        this->setupContextMenu();

        // HACK: This prevents the raise of the textChanged signal, just only for the 1st time.
        this->setupSignals();
    }


    DocumentMdiSubWindowQt::~DocumentMdiSubWindowQt() {}


    void DocumentMdiSubWindowQt::closeEvent(QCloseEvent *evt) {
        emit closeRequested(this, evt);
    }


    void DocumentMdiSubWindowQt::setupSignals() {
        connect(mScintilla, &QsciScintilla::textChanged, [this]() {
            this->mPresenter->onContentChanged();
        });
    }


    void DocumentMdiSubWindowQt::setupLayout() {
        QGridLayout *layout = new QGridLayout(this);
        layout->addWidget(mScintilla);
        this->setLayout(layout);
    }


    void DocumentMdiSubWindowQt::setupContextMenu() {
        systemMenu()->clear();

        auto closeAction = new QAction{"Close", this};
        systemMenu()->addAction(closeAction);
        connect(closeAction, &QAction::triggered, [this]() {
            // this->presenter->onCloseDocument(editor);
        });

        auto closeAllButThisAction = new QAction("Close all but this", this);
        systemMenu()->addAction(closeAllButThisAction);
        connect(closeAllButThisAction, &QAction::triggered, [this]() {
            // this->presenter->onCloseOtherDocuments(editor);
        });

        auto closeAllAction = new QAction("Close all", this);
        systemMenu()->addAction(closeAllAction);
        connect(closeAllAction, &QAction::triggered, [this]() {
            // this->presenter->onCloseAllDocuments();
        });

        auto closeToTheRightAction = new QAction("Close to the right", this);
        systemMenu()->addAction(closeToTheRightAction);
        connect(closeToTheRightAction, &QAction::triggered, [this]() {
            // this->presenter->onCloseDocumentsToTheRight(editor);
        });
    }


    void DocumentMdiSubWindowQt::setTitle(const std::string &title) {
        this->setWindowTitle(title.c_str());
        // HACK: this fix the modified mark in the window title
        this->setWindowModified(true);
    }


    std::string DocumentMdiSubWindowQt::getTitle() const {
        return this->windowTitle().toStdString();
    }


    void DocumentMdiSubWindowQt::setContent(const std::string &content) {
        mScintilla->setText(content.c_str());
    }


    std::string DocumentMdiSubWindowQt::getContent() const {
        return mScintilla->text().toStdString();
    }


    void DocumentMdiSubWindowQt::setConfig(const DocumentConfig &config) {
        assert(mScintilla);

        auto font = QFont{config.fontName.c_str(), config.fontSize};

        mScintilla->setFont(font);
        mScintilla->setCaretLineVisible(config.caretLineVisible);
        mScintilla->setTabWidth(config.tabWidth);

        if (config.showLineNumbers) {
            mScintilla->setMarginType(1, QsciScintilla::NumberMargin);
        }

        mConfig = config;
    }


    DocumentConfig DocumentMdiSubWindowQt::getConfig() const {
        assert(mScintilla);

        return mConfig;
    }


    void DocumentMdiSubWindowQt::undo() {
        assert(mScintilla);
        mScintilla->undo();
    }


    void DocumentMdiSubWindowQt::redo() {
        assert(mScintilla);
        mScintilla->redo();
    }


    void DocumentMdiSubWindowQt::cut() {
        assert(mScintilla);
        mScintilla->cut();
    }


    void DocumentMdiSubWindowQt::copy() {
        assert(mScintilla);
        mScintilla->copy();
    }


    void DocumentMdiSubWindowQt::paste() {
        assert(mScintilla);
        mScintilla->paste();
    }


    void DocumentMdiSubWindowQt::clearAll() {
        // TODO: Add implementation
    }


    void DocumentMdiSubWindowQt::clearUndoBuffer() {
        // TODO: Add implementation
    }


    void DocumentMdiSubWindowQt::setSelection(const TextSelection &selection) {
        // TODO: Add implementation
    }


    void DocumentMdiSubWindowQt::selectAll() {
        // TODO: Add implementation
    }


    void DocumentMdiSubWindowQt::clearSelection() {
        // TODO: Add implementation
    }


    TextSelection DocumentMdiSubWindowQt::getSelection() const {
        // TODO: Add implementation
        return TextSelection::all();
    }
}
