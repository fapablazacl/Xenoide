
#include <Xenoide/Gui/mvp/FindReplace.hpp>

#include <cassert>

namespace Xenoide {
    FindReplace::View::View(FindReplace::Presenter *presenter) : presenter(presenter) {
        assert(presenter);
    }
}


namespace Xenoide {
    FindReplace::Presenter::Presenter(FindReplace::Model *model, Document::View *document) : model(model), document(document) {
        assert(model);
        assert(document);
    }

    void FindReplace::Presenter::handleInitializedView(FindReplace::View *view) {
        assert(view);
        assert(model);
        assert(document);

        auto data = model->getFindReplaceData();

        view->show("Find/Replace ...", data);

        this->view = view;
    }

    void FindReplace::Presenter::handleFindReplaceDataChanged(const FindReplace::FindReplaceData &data) {
        assert(view);
        assert(model);
        assert(document);

        view->toggleReplaceControls(data.replaceWith.has_value());

        findReplaceData = data;
    }

    void FindReplace::Presenter::handleCommand(const FindReplace::Command command) {
        assert(view);
        assert(model);
        assert(document);

        switch (command) {
            case FindReplace::C_FIND_NEXT: {
                size_t startOffset = 0;

                if (const auto selection = document->getSelection(); selection) {
                    startOffset = selection->end;
                }

                // TODO: add implementation

                break;
            }
                
                

            case FindReplace::C_REPLACE_NEXT:
                // TODO: add implementation
                break;

            case FindReplace::C_REPLACE_ALL:
                // TODO: add implementation
                break;

            case FindReplace::C_CLOSE:
                model->setFindReplaceData(findReplaceData);
                view->hide();
                break;
        }
    }
}
