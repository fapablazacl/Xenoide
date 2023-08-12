
#include "CDocument.h"

namespace Xenoide {
    int CDocument::OnCreate(LPCREATESTRUCT lpCreateStruct) {
        RECT rcClient;
        this->GetClientRect(&rcClient);

        const DWORD dwStyle = ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL;

        m_edit.Create(m_hWnd, rcClient, _T("Test"), dwStyle);

        return 0;
    }

    void CDocument::OnSize(UINT nType, CSize size) {
        m_edit.ResizeClient(size.cx, size.cy, TRUE);
    }

    void CDocument::SetText(const CString &string) {
        m_edit.SetWindowText(string);
    }

    CString CDocument::GetText() const {
        return "";
    }
}


namespace Xenoide {
    void CDocument::setTitle(const std::string &title)
    {
    }


    std::string CDocument::getTitle() const
    {
        return "";
    }


    void CDocument::setContent(const std ::string &content)
    {
    }


    std::string CDocument::getContent() const
    {
        return "";
    }


    void CDocument::setConfig(const DocumentConfig &config)
    {
    }


    DocumentConfig CDocument::getConfig() const
    {
        return {};
    }


    void CDocument::clearAll()
    {
    }


    void CDocument::clearUndoBuffer()
    {
    }


    void CDocument::undo()
    {
    }


    void CDocument::redo()
    {
    }


    void CDocument::cut()
    {
    }


    void CDocument::copy()
    {
    }


    void CDocument::paste() {

    }
}
