
#include <Xenoide/Gui/Qt5/FolderBrowserQt.hpp>

#include <QVBoxLayout>
#include <QBoxLayout>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <iostream>
#include <Xenoide/Gui/FolderBrowser.hpp>

#include <Xenoide/Gui/Qt5/DialogManagerQt.hpp>
#include <Xenoide/Gui/Qt5/UtilitiesQt.hpp>

#include <boost/filesystem.hpp>

namespace Xenoide {
    namespace fs = boost::filesystem;

    // TODO: Refactor file handling logic into another layer
    class FolderBrowserQtTreeModel : public QFileSystemModel {
    public:
        FolderBrowserQtTreeModel(QWidget *parent, FolderBrowser::Presenter *presenter) : QFileSystemModel(parent) {
            this->presenter = presenter;
        }

        virtual Qt::ItemFlags flags(const QModelIndex &index) const override {
            if (!index.isValid()) {
                return 0;
            }

            const auto path = fs::path(this->filePath(index).toStdString());

            Qt::ItemFlags flags = Qt::ItemFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);

            if (fs::is_directory(path)) {
                flags |= Qt::ItemIsDropEnabled;
            }

            return flags;
        }

        virtual Qt::DropActions supportedDropActions() const override {
            return Qt::DropActions(Qt::CopyAction | Qt::MoveAction);
        }

        virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override {
            if (action == Qt::MoveAction) {
                const QString absFilePath = this->fileInfo(parent).absoluteFilePath();

                presenter->onMoveSelectedPath(absFilePath.toStdString());
            }

            return QFileSystemModel::dropMimeData(data, action, row, column, parent);
        }

    private:
        FolderBrowser::Presenter *presenter = nullptr;
    };
}

namespace Xenoide {
    void FolderBrowserQt::displayFolder(const std::string &folder) {
        this->setProjectFolder(folder.c_str());
    }

    boost::optional<std::string> FolderBrowserQt::getSelectedPath() const {
        const QModelIndex index = mTreeView->currentIndex();
        const QString path = mFileSystemModel->fileInfo(index).absoluteFilePath();

        return path.toStdString();
    }
}

namespace Xenoide {
    FolderBrowserQt::FolderBrowserQt(QWidget *parent, FolderBrowser::Presenter *presenter, DialogManagerQt *dialogManager) : QWidget(parent), FolderBrowser(presenter) {
        mTreeView = new QTreeView(this);

        this->setLayout(new QVBoxLayout(this));
        this->layout()->addWidget(mTreeView);

        // instance file system model
        mFileSystemModel = new FolderBrowserQtTreeModel(this, presenter);
        mFileSystemModel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);

        // enable treeview support
        mTreeView->setDragDropMode(QAbstractItemView::InternalMove);
        // m_treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        mTreeView->setDragEnabled(true);
        mTreeView->setAcceptDrops(true);
        // m_treeView->setDropIndicatorShown(true);

        // bind view and model together
        mTreeView->setModel(mFileSystemModel);
        mTreeView->setVisible(false);
        mTreeView->setHeaderHidden(true);
        mTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

        for (int i=1; i<mFileSystemModel->columnCount(); ++i) {
            mTreeView->hideColumn(i);
        }

        // connect signal handlers
        connect(mTreeView, &QTreeView::doubleClicked, [this](const QModelIndex index) {
            mPresenter->onOpenSelectedFile();
        });

        connect(mTreeView, &QTreeView::customContextMenuRequested, [this](const QPoint &pos) {
            mPresenter->onContextMenuRequested({pos.x(), pos.y()});
        });

        mPresenter->onInitialized(this, dialogManager);
    }

    FolderBrowserQt::~FolderBrowserQt() {}

    void FolderBrowserQt::setProjectFolder(const QString &projectFolder) {
        mTreeView->setVisible(true);

        mProjectFolder = projectFolder;
        mFileSystemModel->setRootPath(projectFolder);

        QModelIndex index = mFileSystemModel->index(projectFolder);

        mTreeView->setRootIndex(index);
    }

    QString FolderBrowserQt::projectFolder() const {
        return mProjectFolder;
    }

    void FolderBrowserQt::displayContextualMenu(const Point &point, const Menu &menu) {
        QPoint pos;
        pos.setX(point.x);
        pos.setY(point.y);

        QMenu contextMenu(menu.text.c_str(), this);
        for (const Menu &childMenu : menu.childs) {
            setupMenu(&contextMenu, childMenu);
        }

        contextMenu.exec(this->mapToGlobal(pos));
    }
}
