
#include <QApplication>

#include "Xenoide/Gui/mvp/MainFrame.hpp"
#include "QMainFrame.hpp"


class MainFrameModel : public Xenoide::MainFrame::Model {
public:
    std::string getTitle() const override {
        return "test-";
    }

    bool hasModifiedFiles() const override {
        return true;

    }
};


int main(int argc, char **argv) {
    QApplication app(argc, argv);

    using namespace Xenoide;

    MainFrameModel model{};
    MainFrame::Presenter presenter{&model};
    QMainFrame view{&presenter};

    view.show();

    return app.exec();
}
