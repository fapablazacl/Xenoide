
#pragma once 

#include <Xenoide/Gui/Document.h>

#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlmisc.h>
#include <atlcrack.h>

namespace Xenoide {

    /**
     * @brief Source code editor window
     */
    class CDocument : public CWindowImpl<CDocument>, public Document {
    public:
        DECLARE_WND_CLASS(_T("CDocument"))
        
        BEGIN_MSG_MAP(CDocument)
            MSG_WM_CREATE(OnCreate)
            MSG_WM_SIZE(OnSize)
        END_MSG_MAP()

    public:
        int OnCreate(LPCREATESTRUCT lpCreateStruct);

        void OnSize(UINT nType, CSize size);

        void SetText(const CString &string);

        CString GetText() const;


    public:
        virtual void setTitle(const std::string &title) override;
        virtual std::string getTitle() const override;

        virtual void setContent(const std::string &content) override;
        virtual std::string getContent() const override;

        virtual void setConfig(const DocumentConfig &config) override;
        virtual DocumentConfig getConfig() const override;

        virtual void clearAll() override;

        virtual void clearUndoBuffer() override;

        virtual void undo() override;
        virtual void redo() override;

        virtual void cut() override;
        virtual void copy() override;
        virtual void paste() override;

    private:
        CEdit m_edit;
    };
}
