
#include <Xenoide/Gui/mvp/Document.hpp>

#include <cassert>

namespace Xenoide {
    Document::View::View(Document::Presenter *presenter) : presenter(presenter) {
        assert(presenter);
    }
}


namespace Xenoide {
    Document::Presenter::Presenter(Document::Model *model) : model(model) {
        assert(model);
    }

    void Document::Presenter::handleInitializedView(Document::View *view) {
        assert(view);
        assert(model);

        // TODO: Perform view initialization

        this->view = view;
    }
}
