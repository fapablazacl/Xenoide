
#include "CDocumentManager.h"

namespace Xenoide {
    int CDocumentManager::OnCreate(LPCREATESTRUCT lpCreateStruct) {
        const DWORD dwStyle = WS_CHILD | WS_VISIBLE;

        RECT clientRect = {};
        this->GetClientRect(&clientRect);

        tabControl.Create(m_hWnd, clientRect, _T(""), dwStyle);

        tabControl.AddItem(_T("Test1"));
        tabControl.AddItem(_T("Test2"));
        tabControl.AddItem(_T("Test3"));

        return 0;
    }

    void CDocumentManager::OnSize(UINT nType, CSize size) {
        
    }
}


namespace Xenoide {
    Document* CDocumentManager::getDocument(const std::size_t index)
    {
        return nullptr;
    }


    void CDocumentManager::closeDocument(Document* editor)
    {
    }


    void CDocumentManager::showDocument(Document* editor)
    {
    }


    void CDocumentManager::closeDocuments(const std::vector<Document*>& documents)
    {
    }


    std::vector<Document*> CDocumentManager::enumerateDocuments()
    {
        return std::vector<Document*>();
    }


    CDocumentManager::CDocumentManager(DocumentManager::Presenter* presenter) : DocumentManager(presenter)
    {
    }


    CDocumentManager::~CDocumentManager()
    {
    }


    Document* CDocumentManager::appendDocument(Document::Presenter* presenter)
    {
        return nullptr;
    }


    Document* CDocumentManager::getCurrentDocument()
    {
        return nullptr;
    }


    void CDocumentManager::setCurrentDocument(Document* document)
    {
    }


    std::size_t CDocumentManager::getDocumentCount() const
    {
        return std::size_t();
    }
}
