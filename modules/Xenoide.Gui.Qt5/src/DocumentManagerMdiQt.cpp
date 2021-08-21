
#include <Xenoide/Gui/Qt5/DocumentManagerMdiQt.hpp>

#include <iostream>
#include <QGridLayout>
#include <QAction>
#include <QMenu>
#include <QMdiSubWindow>
#include <QTextEdit>

#include <Xenoide/Gui/Document.hpp>
#include <Xenoide/Gui/Qt5/DocumentMdiSubWindowQt.hpp>

namespace Xenoide {
    DocumentManagerMdiQt::DocumentManagerMdiQt(QWidget *parent, DocumentManager::Presenter *presenter) : QWidget(parent), DocumentManager(presenter), mDialogManager(this) {
        mMdiArea = new QMdiArea(this);
        mMdiArea->setViewMode(QMdiArea::TabbedView);
        mMdiArea->setTabsClosable(true);
        mMdiArea->setTabsMovable(true);
        mMdiArea->setDocumentMode(true);

        mMdiArea->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(mMdiArea, &QMdiArea::customContextMenuRequested, [this](const QPoint &pos) {
            bool found = true;
            int index = 0;

            // determine the tab
            auto list = mMdiArea->subWindowList();

            for (int i=0; i<list.count(); i++) {
                const auto rect = list[i]->rect();
                if (rect.contains(pos)) {
                    found = true;
                    index = i;
                    break;
                }
            }

            // TODO: Generate menu Dynamically
            // trigger context menu on that tab
            if (Document *editor = this->getDocument(index); found && editor) {
                QMenu contextMenu("Context Menu", this);

                QAction closeAction("Close", this);
                contextMenu.addAction(&closeAction);
                this->connect(&closeAction, &QAction::triggered, [this, editor]() {
                    this->presenter->onCloseDocument(editor);
                });

                QAction closeAllButThisAction("Close all but this", this);
                contextMenu.addAction(&closeAllButThisAction);
                this->connect(&closeAllButThisAction, &QAction::triggered, [this, editor]() {
                    this->presenter->onCloseOtherDocuments(editor);
                });

                QAction closeAllAction("Close all", this);
                contextMenu.addAction(&closeAllAction);
                this->connect(&closeAllAction, &QAction::triggered, [this]() {
                    this->presenter->onCloseAllDocuments();
                });

                QAction closeToTheRightAction("Close to the right", this);
                contextMenu.addAction(&closeToTheRightAction);
                this->connect(&closeToTheRightAction, &QAction::triggered, [this, editor]() {
                    this->presenter->onCloseDocumentsToTheRight(editor);
                });

                contextMenu.exec(this->mapToGlobal(pos));
            }
        });

        QGridLayout *layout = new QGridLayout(this);
        layout->addWidget(mMdiArea);
        this->setLayout(layout);

        presenter->onInitialized(this, &mDialogManager);
    }


    DocumentManagerMdiQt::~DocumentManagerMdiQt() {}
    

    boost::optional<int> DocumentManagerMdiQt::getDocumentIndex(const DocumentMdiSubWindowQt *documentQt) {
        assert(mMdiArea);
        assert(documentQt);

        auto list = mMdiArea->subWindowList();

        for (int i = 0; i < list.count(); i++) {
            const QMdiSubWindow *subWindow = list[i];

            if (subWindow->widget() == documentQt) {
                return i;
            }
        }

        return {};
    }
    

    void DocumentManagerMdiQt::changeDocumentTitle(DocumentMdiSubWindowQt *documentSubWindowQt, const std::string &title) {
        assert(documentSubWindowQt);

        documentSubWindowQt->setWindowTitle(title.c_str());
    }
}

namespace Xenoide {
    Document* DocumentManagerMdiQt::appendDocument(Document::Presenter *documentPresenter) {
        assert(mMdiArea);
        assert(documentPresenter);

        // TODO: This document-tab initialization logic is private to the CustomMdiSubWindow. Consider refactor it later
        auto documentSubWindowQt = new DocumentMdiSubWindowQt(documentPresenter);

        documentSubWindowQt->setAttribute(Qt::WA_DeleteOnClose, true);

        // handle the tab close request
        this->connect(documentSubWindowQt, &DocumentMdiSubWindowQt::closeRequested, [=](DocumentMdiSubWindowQt *subWindow, QCloseEvent *evt) {
            std::cout << "triggered DocumentMdiSubWindowQt::closeRequested" << std::endl;

            if (documentPresenter->onCloseRequested() == Document::Presenter::UserResponse::Accept) {
                presenter->onCloseDocument(subWindow);

                evt->accept();
            } else {
                evt->ignore();
            }
        });

        mMdiArea->addSubWindow(documentSubWindowQt);

        documentSubWindowQt->show();

        return documentSubWindowQt;
    }


    void DocumentManagerMdiQt::setCurrentDocument(Document *document) {
        assert(mMdiArea);
        assert(document);
        
        auto documentSubWindowQt = static_cast<DocumentMdiSubWindowQt*>(document);
        
        mMdiArea->setActiveSubWindow(documentSubWindowQt);
    }

    Document* DocumentManagerMdiQt::getCurrentDocument() {
        assert(mMdiArea);

        if (auto documentSubWindowQt = mMdiArea->activeSubWindow(); documentSubWindowQt) {
            return static_cast<DocumentMdiSubWindowQt*>(documentSubWindowQt);
        }

        return nullptr;
    }


    std::size_t DocumentManagerMdiQt::getDocumentCount() const {
        assert(mMdiArea);

        return mMdiArea->subWindowList().count();
    }


    Document* DocumentManagerMdiQt::getDocument(const std::size_t index) {
        assert(mMdiArea);

        auto list = mMdiArea->subWindowList();
        auto documentSubWindowQt = list[index];

        assert(documentSubWindowQt);
        
        return static_cast<DocumentMdiSubWindowQt*>(documentSubWindowQt);
    }

    
    void DocumentManagerMdiQt::closeDocument(Document *document) {
        assert(mMdiArea);
        assert(document);

        auto documentSubWindowQt = static_cast<DocumentMdiSubWindowQt*>(document);
        
        documentSubWindowQt->close();
    }

    void DocumentManagerMdiQt::showDocument(Document *document) {
        assert(mMdiArea);
        assert(document);

        auto documentSubWindowQt = static_cast<DocumentMdiSubWindowQt*>(document);

        documentSubWindowQt->show();
    }
}
