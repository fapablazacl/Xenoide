
#pragma once 

#include <string>
#include <optional>

namespace Xenoide {
    class Document final {
    public:
        Document() = delete;

        enum ConfigFlags {
            CF_NONE                 = 0x00000000,
            CF_SHOW_CARET_LINE      = 0x00000001,
            CF_SHOW_LINE_NUMBERS    = 0x00000002,
            CF_INDENT_SPACES        = 0x00000004,
            CF_INDENT_TABS          = 0x00000008,
        };

        struct Font {
            std::string name = "";
            int size = 12;
        };
        
        struct VisualConfig {
            Font font = {};
            int tabWidth = 4;
            ConfigFlags flags = CF_NONE;
        };

        struct Selection {
            int start = 0;
            int end = 0;
        };

        class Presenter;
        class View {
        public:
            explicit View(Presenter *presenter);

            virtual ~View() {}

            virtual void setTitle(const std::string &title) = 0;

            virtual std::string getTitle() const = 0;

            virtual void setContent(const std::string &content) = 0;

            virtual std::string getContent() const = 0;

            virtual std::optional<Selection> getSelection() const = 0;

            virtual void setSelection(std::optional<Selection> selection) = 0;

            virtual void setVisualConfig(const VisualConfig &config) = 0;

        protected:
            Presenter *presenter = nullptr;
        };

        class Model {
        public:
            virtual ~Model() {}

            virtual VisualConfig getDefaultConfig() const = 0;
        };


        class Presenter {
        public:
            explicit Presenter(Document::Model *model);

            void handleInitializedView(Document::View *view);

        private:
            View *view = nullptr;
            Model *model = nullptr;
        };
    };
}
