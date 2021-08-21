
#pragma once

#include <QObject>
#include <QMdiSubWindow>
#include <QCloseEvent>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>
#include <Qsci/qscilexercpp.h>

#include <Xenoide/Gui/Document.hpp>
#include <Xenoide/Gui/Qt5/DialogManagerQt.hpp>


namespace Xenoide {
    // TODO: Consider promote this class to be the Document interface implementation
    class DocumentMdiSubWindowQt : public QMdiSubWindow, public Document {
        Q_OBJECT

    public:
        explicit DocumentMdiSubWindowQt(Document::Presenter *presenter);

        virtual ~DocumentMdiSubWindowQt();

    signals:
        void closeRequested(DocumentMdiSubWindowQt *subWindow, QCloseEvent *evt);

    public:
        void closeEvent(QCloseEvent *evt) override;

    private:
        void setupSignals();

        void setupLayout();

        void setupContextMenu();

    public:
        virtual void setTitle(const std::string &title) override;
        virtual std::string getTitle() const override;

        virtual void setContent(const std::string &content) override;
        virtual std::string getContent() const override;

        virtual void setConfig(const DocumentConfig &config) override;
        virtual DocumentConfig getConfig() const override;

        virtual void undo() override;
        virtual void redo() override;

        virtual void cut() override;
        virtual void copy() override;
        virtual void paste() override;

        virtual void clearAll() override;

        virtual void clearUndoBuffer() override;

        virtual void setSelection(const TextSelection &selection) override;

        virtual void selectAll() override;

        virtual void clearSelection() override;

        virtual TextSelection getSelection() const override;

    private:
        DialogManagerQt mDialogManager;
        Document::Presenter *mPresenter = nullptr;
        QsciScintilla *mScintilla = nullptr;
        DocumentConfig mConfig;
    };
}
