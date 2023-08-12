
#pragma once 

#include <string>
#include <vector>

#include "Document.hpp"

namespace Xenoide {
    class DocumentManager final {
    public:
        DocumentManager() = delete;

        class Presenter;
        class View {
        public:
            explicit View(Presenter *presenter);

            virtual ~View() {}

            virtual std::vector<Document::View*> enumerate() = 0;

            virtual size_t getCount() const = 0;

            virtual void setCurrent(const size_t index) = 0;

            virtual size_t getCurrent() const = 0;

        protected:
            Presenter *presenter = nullptr;
        };

        class Model {
        public:
            virtual ~Model() {}
        };

        class Presenter {
        public:
            explicit Presenter(DocumentManager::Model *model);

            void handleInitializedView(DocumentManager::View *view);

        private:
            View *view = nullptr;
            Model *model = nullptr;
        };
    };
}
