
#pragma once 

#include <string>
#include <vector>
#include <optional>
#include <tuple>

#include "Document.hpp"

namespace Xenoide {
    class FindReplace final {
    public:
        FindReplace() = delete;

        enum SearchScope {
            Selection = 0,
            CurrentDocument = 1
        };

        struct FindReplaceData {
            std::string findWhat;
            std::optional<std::string> replaceWith;

            bool matchCase = false;
            bool matchWholeWorld = false;

            SearchScope scope = CurrentDocument;
        };

        class Presenter;
        class View {
        public:
            explicit View(Presenter *presenter);

            virtual ~View() {}

            virtual void show(const std::string &title, const FindReplaceData &data) = 0;

            virtual void hide() = 0;

            virtual void toggleReplaceControls(const bool status) = 0;

        protected:
            Presenter *presenter = nullptr;
        };

        class Model {
        public:
            virtual ~Model() {}

            virtual FindReplaceData getFindReplaceData() const = 0;

            virtual void setFindReplaceData(const FindReplaceData &findReplaceData) const = 0;

            virtual std::optional<size_t> findNext(const FindReplaceData &data, std::optional<size_t> startOffset) = 0;

            virtual std::optional<size_t> replaceNext(const FindReplaceData &data, std::optional<size_t> startOffset) = 0;
        };

        enum Command {
            C_CLOSE,
            C_FIND_NEXT,
            C_REPLACE_NEXT,
            C_REPLACE_ALL
        };

        class Presenter {
        public:
            explicit Presenter(FindReplace::Model *model, Document::View *document);

            void handleInitializedView(FindReplace::View *view);

            void handleFindReplaceDataChanged(const FindReplaceData &data);

            void handleCommand(const Command command);

        private:
            View *view = nullptr;
            Model *model = nullptr;
            Document::View *document = nullptr;
            FindReplaceData findReplaceData = {};
        };
    };
}
