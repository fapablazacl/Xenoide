
#pragma once

#include <QMainWindow>

class QsciScintilla;

namespace xenoide {

    
    class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        MainWindow();

    private:
        void setupEditor(QsciScintilla *scintilla);

        void createMenuBar();

        QsciScintilla *mScintilla = nullptr;
    };
}
