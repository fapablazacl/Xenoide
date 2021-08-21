
#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>

#include <Xenoide/Gui/FindReplaceDialog.hpp>
#include <Xenoide/Gui/Qt5/DocumentMdiSubWindowQt.hpp>

namespace Xenoide {
    class FindReplaceDialogQt : public QWidget, public FindReplaceDialog {
        Q_OBJECT

    public:
        explicit FindReplaceDialogQt(QWidget *parent, FindReplaceDialog::Presenter *presenter, DocumentMdiSubWindowQt *document);

        virtual ~FindReplaceDialogQt();

    private:
        void setupSignals();

        void setupLayout();

    public:
        void attachPresenter(FindReplaceDialog::Presenter *presenter) override;

        void show(const ViewData &viewData) override;

        void hide() override;

        void toggleReplaceControls(const bool status) override;

    private:
        QGroupBox *findReplaceGroupBox = nullptr;
        QLabel *findWhatLabel = nullptr;
        QLineEdit *findWhatEdit = nullptr;
        QCheckBox *replaceWithCheckBox = nullptr;
        QLineEdit *replaceWithEdit = nullptr;

        QGroupBox *matchGroupBox = nullptr;
        QCheckBox *matchCaseCheckBox = nullptr;
        QCheckBox *matchWholeWordCheckBox = nullptr;

        QGroupBox *scopeGroupBox = nullptr;
        QRadioButton *scopeSelectionRadio = nullptr;
        QRadioButton *scopeCurrentDocumentRadio = nullptr;

        QPushButton *findNextButton = nullptr;
        QPushButton *replaceNextButton = nullptr;
        QPushButton *replaceAllButton = nullptr;
        QPushButton *closeButton = nullptr;
    };
}
