
#pragma once

#include <QMainWindow>

class QsciScintilla;

namespace xenoide {

    struct LanguageKeywords;
    class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        MainWindow();

    private:
        void setupEditor(QsciScintilla *scintilla);

        void createMenuBar();

        void setLanguage(const LanguageKeywords &keywords);

        QsciScintilla *mScintilla = nullptr;
    };
}
