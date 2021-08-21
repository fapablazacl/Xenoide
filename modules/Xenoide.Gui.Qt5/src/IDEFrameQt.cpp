
#include <Xenoide/Gui/Qt5/IDEFrameQt.hpp>

#include <iostream>
#include <vector>
#include <functional>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QCloseEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <Xenoide/Gui/IDEFrame.hpp>
#include <Xenoide/Gui/Qt5/DialogManagerQt.hpp>
#include <Xenoide/Gui/Qt5/FolderBrowserQt.hpp>
#include <Xenoide/Gui/Qt5/UtilitiesQt.hpp>

namespace Xenoide {
    IDEFrameQt::IDEFrameQt(IDEFrame::Presenter *presenter) : IDEFrame(presenter) {
        mDialogManager = std::make_unique<DialogManagerQt>(this);

        this->setupDocumentManager();
        this->setupDockUI();

        QDesktopWidget desktopWidget;
        
        const QRect screenSize = desktopWidget.availableGeometry(desktopWidget.primaryScreen());
        
        this->setMinimumSize(screenSize.size() * 0.4);
        this->resize(screenSize.size() * 0.7);

        mPresenter->onInitialized(this, mDialogManager.get(), this);
    }

    void IDEFrameQt::setupMenuBar(const Menu &menu) {
        auto menuBar = createMenuBar(this, menu);

        this->setMenuBar(menuBar);
    }

    void IDEFrameQt::setupDocumentManager() {
        mDocumentManager = new DocumentManagerMdiQt(this, mPresenter->getDocumentManagerPresenter());
        this->setCentralWidget(mDocumentManager);
    }
    
    void IDEFrameQt::setupDockUI() {
        const auto areas = QFlags<Qt::DockWidgetArea>(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        
        // setup folder browser dock widget
        mFolderBrowserDock = new QDockWidget("Folder Browser", this);
        
        mFolderBrowser = new FolderBrowserQt(mFolderBrowserDock, mPresenter->getFolderBrowserPresenter(), mDialogManager.get());
        mFolderBrowserDock->setAllowedAreas(areas);
        mFolderBrowserDock->setWidget(mFolderBrowser);

        this->addDockWidget(Qt::LeftDockWidgetArea, mFolderBrowserDock);
    }
    
    void IDEFrameQt::closeEvent(QCloseEvent *evt) {
        if (mPresenter->onCloseRequested()) {
            evt->accept();
        } else {
            evt->ignore();
        }
    }
}

namespace Xenoide {
    DocumentManager* IDEFrameQt::getDocumentManager() {
        return mDocumentManager;
    }

    DialogManager* IDEFrameQt::getDialogManager() {
        return mDialogManager.get();
    }
    
    FolderBrowser* IDEFrameQt::getFolderBrowser() {
        return mFolderBrowser;
    }
    
    void IDEFrameQt::close() {
        QMainWindow::close();
    }

    void IDEFrameQt::show() {
        QMainWindow::show();
    }

    void IDEFrameQt::showPanel(const Panel panel) {
        switch (panel) {
        case IDEFrame::FOLDER_BROWSER:
            const bool visible = !mFolderBrowserDock->isVisible();
            mFolderBrowserDock->setVisible(visible);
            break;
        }
    }
}
