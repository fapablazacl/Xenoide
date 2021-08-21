
#ifndef __XENOIDE_UI_QT5_MAINWINDOW_HPP__
#define __XENOIDE_UI_QT5_MAINWINDOW_HPP__

#include <map>

#include <QMainWindow>
#include <QDockWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include <Xenoide/Gui/IDEFrame.hpp>
#include <Xenoide/Gui/FolderBrowser.hpp>
#include <Xenoide/Gui/DocumentManager.hpp>
#include <Xenoide/Gui/Menu.hpp>
#include <Xenoide/Gui/MenuPanel.hpp>

#include <Xenoide/Gui/Qt5/DialogManagerQt.hpp>
#include <Xenoide/Gui/Qt5/DocumentManagerMdiQt.hpp>

namespace Xenoide {
    class FolderBrowserQt;
    class IDEFrameQt : public QMainWindow, public IDEFrame, public MenuPanel {
        Q_OBJECT;

    public:
        explicit IDEFrameQt(IDEFrame::Presenter *controller);

    public:
        virtual DocumentManager* getDocumentManager() override;

        virtual DialogManager* getDialogManager() override;
        
        virtual FolderBrowser* getFolderBrowser() override;
        
        virtual void close() override;

        virtual void show() override;

        virtual void showPanel(const Panel panel) override;

        virtual void setupMenuBar(const Menu &menu) override;

    private:
        void setupDocumentManager();
        void setupDockUI();

    private:
        virtual void closeEvent(QCloseEvent *evt) override;
        
    private:
        DocumentManagerMdiQt *mDocumentManager = nullptr;
        std::unique_ptr<DialogManagerQt> mDialogManager;
        FolderBrowserQt *mFolderBrowser = nullptr;
        QDockWidget *mFolderBrowserDock = nullptr;
    };
}

#endif
