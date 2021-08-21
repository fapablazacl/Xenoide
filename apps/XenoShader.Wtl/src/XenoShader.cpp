
#include <tuple>

#define STRICT
#define WIN32_LEAN_AND_MEAN
#define _WTL_USE_CSTRING

#include <atlbase.h>
#include <atlapp.h>

// WTL version of CComModule
extern CAppModule _Module;

#include <atlwin.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atluser.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlsplit.h>
#include <atldlgs.h>

#include <Scintilla.h>
#include <Sci_Position.h>
#include <SciLexer.h>
#include <ILexer.h>
#include <Lexilla.h>

#include <vector>
#include <string_view>
#include "LexillaAccess.h"
#include "resource.h"

#include <optional>
#include <set>
#include <boost/filesystem.hpp>
#include <boost/bimap.hpp>

#include <Xenoide/Core/FileService.hpp>


#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

class AppController {
public:
    virtual void openFile(const boost::filesystem::path &filePath) = 0;
};


// All Dialogs must inherit from the CDialogImpl template class.
// Must declar a class-scoped member IDD, wich haves the dialog's resource identifier.
class AboutDlg : public CDialogImpl<AboutDlg> {
public:
    enum {IDD = IDD_ABOUT};

public:
    BEGIN_MSG_MAP(AboutDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        COMMAND_ID_HANDLER(IDOK, OnOKCancel)
        COMMAND_ID_HANDLER(IDCANCEL, OnOKCancel)
    END_MSG_MAP()

public:
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        CenterWindow();

        // let the system set the focus
        return TRUE;
    }
 
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        EndDialog(IDCANCEL);
        return 0;
    }
 
    LRESULT OnOKCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
        EndDialog(wID);
        return 0;
    }
};


class FolderExplorerView {
public:
    virtual ~FolderExplorerView() {}

    virtual void clear() = 0;

    virtual int insert(const std::string &title, const std::optional<int> parentItemId = {}, const bool isDirectory = false) = 0;

    virtual void sort(const int itemId) = 0;

    virtual void sort(const int itemId, std::function<int (int, int)> cmp) = 0;
};


class FolderExplorerPresenter {
public:
    FolderExplorerPresenter(AppController *controller) : mController(controller) {}

    void attachView(FolderExplorerView *view) {
        mView = view;
    }

    void displayFolder(const boost::filesystem::path &folderPath) {
        mView->clear();

        mPathItemsCache.clear();
        mPopulatedItems.clear();

        const std::string folderName = folderPath.filename().string();
        const int itemId = mView->insert(folderName);

        mPathItemsCache.insert({itemId, folderPath});
    }

    void onItemActivated(const int itemId) {
        const auto pathIt = mPathItemsCache.left.find(itemId);

        if (pathIt == mPathItemsCache.left.end()) {
            return;
        }

        if (boost::filesystem::is_regular_file(pathIt->second)) {
            const auto filePath = pathIt->second;
            mController->openFile(filePath);
        }
    }

    void onItemExpanded(const int itemId) {
        if (! itemIsPopulated(itemId)) {
            populateItem(itemId);
        }
    }

public:
    void populateItem(const int parentItemId) {
        using boost::filesystem::recursive_directory_iterator;
        using boost::filesystem::directory_iterator;

        const auto folderPathIt = mPathItemsCache.left.find(parentItemId);

        if (folderPathIt == mPathItemsCache.left.end()) {
            return;
        }

        directory_iterator current{folderPathIt->second}, end;

        while (current != end) {
            const auto currentPath = current->path();

            const auto parentCacheIt = mPathItemsCache.right.find(currentPath.parent_path());

            if (parentCacheIt == mPathItemsCache.right.end()) {
                continue;
            }

            const bool isDirectory = boost::filesystem::is_directory(currentPath);
            const std::string name = currentPath.filename().string();
            const int itemId = mView->insert(name, parentItemId, isDirectory);
            mPathItemsCache.insert({itemId, currentPath});
            
            ++current;
        }

        mPopulatedItems.insert(parentItemId);

        mView->sort(parentItemId, [this](const int i1, const int i2) {
            return this->compare(i1, i2);
        });
    }

    bool itemIsPopulated(const int itemId) const {
        const auto it = mPopulatedItems.find(itemId); 

        return it != mPopulatedItems.end();
    }

    int compare(const int itemId1, const int itemId2) const {
        const auto path1 = mPathItemsCache.left.find(itemId1)->second;
        const auto path2 = mPathItemsCache.left.find(itemId2)->second;

        if (boost::filesystem::is_directory(path1) && !boost::filesystem::is_directory(path2)) {
            return -1;
        }

        if (!boost::filesystem::is_directory(path1) && boost::filesystem::is_directory(path2)) {
            return 1;
        }
        
        return path1.filename().string().compare(path2.filename().string());
    }

private:
    FolderExplorerView *mView = nullptr;
    AppController *mController = nullptr;
    boost::bimap<int, boost::filesystem::path> mPathItemsCache;
    std::set<int> mPopulatedItems;
};


int CALLBACK FolderView_CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
    auto &cmp = *reinterpret_cast<std::function<int (int, int)>*>(lParamSort);

    return cmp(static_cast<int>(lParam1), static_cast<int>(lParam2));
}


class FolderView : public CWindowImpl<FolderView>, public FolderExplorerView {
public:
    enum { ID_FOLDERVIEW_TREEVIEW = 10000 };

public:
    DECLARE_WND_CLASS(NULL)

    BEGIN_MSG_MAP_EX(FolderView)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_SIZE(OnSize)
        MSG_WM_NOTIFY(OnNotify)
    END_MSG_MAP()

public:
    FolderView (AppController *controller) : mPresenter(controller) {
        m_bMsgHandled = false;
    }

    void clear() override {
        mTreeView.DeleteAllItems();
    }

    int insert(const std::string &title, const std::optional<int> parentItemId, const bool hasChildren) override {
        const int itemId = ++mItemCount;

        HTREEITEM hItem = NULL;

        if (! parentItemId) {
            hItem = InsertTreeItem(mTreeView, title.c_str(), itemId);
        } else {
            const HTREEITEM hParentItem = mTreeItemBimap.right.find(*parentItemId)->second;

            hItem = InsertTreeItem(mTreeView, title.c_str(), itemId, hParentItem, hasChildren);
        }

        mTreeItemBimap.insert({hItem, itemId});

        return itemId;
    }

    void sort(const int itemId) override {
        const HTREEITEM hItem = mTreeItemBimap.right.find(itemId)->second;
        mTreeView.SortChildren(hItem);
    }

    void sort(const int itemId, std::function<int (int, int)> cmp) override {
        TVSORTCB sort = {};

        sort.hParent = mTreeItemBimap.right.find(itemId)->second;
        sort.lpfnCompare = FolderView_CompareFunc;
        sort.lParam = reinterpret_cast<LPARAM>(&cmp);

        mTreeView.SortChildrenCB(&sort, FALSE);
    }

public:
    LRESULT OnCreate(LPCREATESTRUCT cs) {
        SetMsgHandled(true);

        const DWORD dwStyle = TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | WS_VISIBLE | WS_CHILD;
        const DWORD dwExStyle = TVS_EX_DOUBLEBUFFER;

        mTreeView.Create(m_hWnd, rcDefault, "", dwStyle, dwExStyle, ID_FOLDERVIEW_TREEVIEW);

        mPresenter.attachView(this);

        return 0;
    }


    void OnDestroy() {
        SetMsgHandled(false);
    }


    void OnSize(UINT nType, CSize size) {
        const CRect rect = { 0, 0, size.cx, size.cy };

        mTreeView.SetWindowPos(NULL, rect, 0);
    }


    LRESULT OnNotify(int idCtrl, LPNMHDR pnmh) {
        SetMsgHandled(true);

        switch (pnmh->code) {
        case NM_DBLCLK: {
            const HTREEITEM selectedItem = mTreeView.GetSelectedItem();

            if (selectedItem) {
                const int itemId = mTreeItemBimap.left.find(selectedItem)->second;
                mPresenter.onItemActivated(itemId);
            }

            break;
        }

        case TVN_ITEMEXPANDING: {
            const auto &pnmtv = *reinterpret_cast<LPNMTREEVIEW>(pnmh);

            if (pnmtv.action == TVE_EXPAND) {
                mPresenter.onItemExpanded(static_cast<int>(pnmtv.itemNew.lParam));
            }

            break;
        }

        default:
            SetMsgHandled(false);
        }

        return 0;
    }

    void DisplayFolder(const boost::filesystem::path &folderPath) {
        mPresenter.displayFolder(folderPath);
    }

private:
    HTREEITEM InsertTreeItem(CTreeViewCtrl &treeView, const char *text, const int itemId) {
        const DWORD style = TVIF_PARAM | TVIF_TEXT | TVIF_CHILDREN /*| TVIF_IMAGE | TVIF_SELECTEDIMAGE*/;

        TVINSERTSTRUCT insertStruct = {};

        insertStruct.item.lParam = static_cast<LPARAM>(itemId);
        insertStruct.item.mask = style;
        insertStruct.item.pszText = const_cast<char*>(text);
        insertStruct.item.cChildren = 1;
        // insertStruct.item.iImage = 1;
        // insertStruct.item.iSelectedImage = 1;
        return treeView.InsertItem(&insertStruct);
    }

    HTREEITEM InsertTreeItem(CTreeViewCtrl &treeView, const char *text, const int itemId, const HTREEITEM parentItem, const bool hasChildren) {
        const DWORD style = TVIF_PARAM | TVIF_TEXT | (hasChildren ? TVIF_CHILDREN : 0) /*| TVIF_IMAGE | TVIF_SELECTEDIMAGE*/;

        TVINSERTSTRUCT insertStruct = {};

        insertStruct.hParent = parentItem;
        insertStruct.item.lParam = static_cast<LPARAM>(itemId);
        insertStruct.item.mask = style;
        insertStruct.item.pszText = const_cast<char*>(text);
        insertStruct.item.cChildren = hasChildren ? 1 : 0;
        // insertStruct.item.iImage = 1;
        // insertStruct.item.iSelectedImage = 1;
        return treeView.InsertItem(&insertStruct);
    }

private:
    int mItemCount = 0;

    CTreeViewCtrl mTreeView;
    boost::bimap<HTREEITEM, int> mTreeItemBimap;
    FolderExplorerPresenter mPresenter;
};


struct CodeViewStyleAttribs {
    COLORREF fore = RGB(255, 255, 255);
    COLORREF back = RGB(255, 255, 255);
    int size = 0;
    const char *face = nullptr;
};


constexpr COLORREF black = RGB(0, 0, 0);
constexpr COLORREF white = RGB(255, 255, 255);
constexpr char *defaultFontName = "Courier New";
constexpr int defaultFontSize = 10;

enum class CodeLanguage {
    TEXT,
    CPP,
    GLSL
};

static std::vector<std::string> extensionsC = {
    ".cpp", ".hpp", ".cxx", ".hxx", ".c++", ".h++", ".cc", ".hh", ".c", ".h"
};

static std::vector<std::string> extensionsGLSL = {
    ".vert", ".tesc", ".tese", ".geom", ".frag", ".comp", ".glsl"
};

CodeLanguage getCodeLanguage(const boost::filesystem::path &filePath) {
    if (filePath.has_extension()) {
        const auto ext = filePath.extension();

        for (const auto &extC : extensionsC) {
            if (ext == extC) {
                return CodeLanguage::CPP;
            }
        }

        for (const auto &extGLSL : extensionsGLSL) {
            if (ext == extGLSL) {
                return CodeLanguage::GLSL;
            }
        }
    }

    return CodeLanguage::TEXT;
}


struct CodeViewLanguageConfig {
    CodeViewStyleAttribs defaultStyle = {
        black, white, defaultFontSize, defaultFontName
    };

    std::string keywords;
    std::vector<std::pair<int, COLORREF>> stylesColors;
};

static CodeViewLanguageConfig languageConfigC = {
    { black, white, defaultFontSize, defaultFontName },
    "alignas alignof and and_eq asm atomic_cancel atomic_commit atomic_noexcept auto bitand bitor bool break case catch char "
	"char16_t char32_t class compl concept const constexpr const_cast continue decltype default delete do "
	"double dynamic_cast else enum explicit export extern false float for friend goto if inline int import long "
	"module mutable namespace new noexcept not not_eq nullptr operator or or_eq private protected public "
	"register reinterpret_cast requires return short signed sizeof static static_assert static_cast struct "
	"switch synchronized template this thread_local "
	"throw true try typedef typeid typename union unsigned "
	"using virtual void volatile wchar_t while xor xor_eq",
    {
        {SCE_C_COMMENT, RGB(0x00, 0x80, 0x00)}, 
        {SCE_C_COMMENTLINE, RGB(0x00, 0x80, 0x00)}, 
        {SCE_C_COMMENTDOC, RGB(0x00, 0x80, 0x00)}, 
        {SCE_C_NUMBER, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_WORD, RGB(0x00, 0x00, 0xFF)}, 
        {SCE_C_STRING, RGB(0x80, 0x00, 0x00)}, 
        {SCE_C_CHARACTER, RGB(0x80, 0x00, 0x00)}, 
        {SCE_C_UUID, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_PREPROCESSOR, RGB(0xA0, 0x00, 0xFF)}, 
        {SCE_C_OPERATOR, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_IDENTIFIER, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_STRINGEOL, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_VERBATIM, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_REGEX, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_COMMENTLINEDOC, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_WORD2, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_COMMENTDOCKEYWORD, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_COMMENTDOCKEYWORDERROR, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_GLOBALCLASS, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_STRINGRAW, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_TRIPLEVERBATIM, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_HASHQUOTEDSTRING, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_PREPROCESSORCOMMENT, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_PREPROCESSORCOMMENTDOC, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_USERLITERAL, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_TASKMARKER, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_ESCAPESEQUENCE, RGB(0x00, 0x00, 0x00)}
    }
};


static CodeViewLanguageConfig languageConfigGLSL = {
    { black, white, defaultFontSize, defaultFontName },
    // supported keywords
    "attribute const uniform varying "
    "layout "
    "centroid flat smooth noperspective "
    "patch sample "
    "break continue do for while switch case default "
    "if else "
    "subroutine "
    "in out inout "
    "float double int void bool true false "
    "invariant "
    "discard return "
    "mat2 mat3 mat4 dmat2 dmat3 dmat4 "
    "mat2x2 mat2x3 mat2x4 dmat2x2 dmat2x3 dmat2x4 "
    "mat3x2 mat3x3 mat3x4 dmat3x2 dmat3x3 dmat3x4 "
    "mat4x2 mat4x3 mat4x4 dmat4x2 dmat4x3 dmat4x4 "
    "vec2 vec3 vec4 ivec2 ivec3 ivec4 bvec2 bvec3 bvec4 dvec2 dvec3 dvec4 "
    "uint uvec2 uvec3 uvec4 "
    "lowp mediump highp precision "
    "sampler1D sampler2D sampler3D samplerCube "
    "sampler1DShadow sampler2DShadow samplerCubeShadow "
    "sampler1DArray sampler2DArray "
    "sampler1DArrayShadow sampler2DArrayShadow "
    "isampler1D isampler2D isampler3D isamplerCube "
    "isampler1DArray isampler2DArray "
    "usampler1D usampler2D usampler3D usamplerCube "
    "usampler1DArray usampler2DArray "
    "sampler2DRect sampler2DRectShadow isampler2DRect usampler2DRect "
    "samplerBuffer isamplerBuffer usamplerBuffer "
    "sampler2DMS isampler2DMS usampler2DMS "
    "sampler2DMSArray isampler2DMSArray usampler2DMSArray "
    "samplerCubeArray samplerCubeArrayShadow isamplerCubeArray usamplerCubeArray "
    "struct "
        
    // The following are the keywords reserved for future use. Using them will result in an error:
    "common partition active "
    "asm "
    "class union enum typedef template this packed "
    "goto "
    "inline noinline volatile public static extern external interface "
    "long short half fixed unsigned superp "
    "input output "
    "hvec2 hvec3 hvec4 fvec2 fvec3 fvec4 "
    "sampler3DRect "
    "filter "
    "image1D image2D image3D imageCube "
    "iimage1D iimage2D iimage3D iimageCube "
    "uimage1D uimage2D uimage3D uimageCube "
    "image1DArray image2DArray "
    "iimage1DArray iimage2DArray uimage1DArray uimage2DArray "
    "image1DShadow image2DShadow "
    "image1DArrayShadow image2DArrayShadow "
    "imageBuffer iimageBuffer uimageBuffer "
    "sizeof cast "
    "namespace using "
    "row_major",
    {
        {SCE_C_COMMENT, RGB(0x00, 0x80, 0x00)}, 
        {SCE_C_COMMENTLINE, RGB(0x00, 0x80, 0x00)}, 
        {SCE_C_COMMENTDOC, RGB(0x00, 0x80, 0x00)}, 
        {SCE_C_NUMBER, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_WORD, RGB(0x00, 0x00, 0xFF)}, 
        {SCE_C_STRING, RGB(0x80, 0x00, 0x00)}, 
        {SCE_C_CHARACTER, RGB(0x80, 0x00, 0x00)}, 
        {SCE_C_UUID, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_PREPROCESSOR, RGB(0xA0, 0x00, 0xFF)}, 
        {SCE_C_OPERATOR, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_IDENTIFIER, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_STRINGEOL, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_VERBATIM, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_REGEX, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_COMMENTLINEDOC, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_WORD2, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_COMMENTDOCKEYWORD, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_COMMENTDOCKEYWORDERROR, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_GLOBALCLASS, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_STRINGRAW, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_TRIPLEVERBATIM, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_HASHQUOTEDSTRING, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_PREPROCESSORCOMMENT, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_PREPROCESSORCOMMENTDOC, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_USERLITERAL, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_TASKMARKER, RGB(0x00, 0x00, 0x00)}, 
        {SCE_C_ESCAPESEQUENCE, RGB(0x00, 0x00, 0x00)}
    }
};


class CodeView : public CWindowImpl<CodeView> {
public:
    DECLARE_WND_CLASS(NULL)

    BEGIN_MSG_MAP_EX(CodeView)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_SIZE(OnSize)

        MSG_WM_NOTIFY(OnNotify)
    END_MSG_MAP()

public:
    CodeView() {
        m_bMsgHandled = false;
    }

    LRESULT OnCreate(LPCREATESTRUCT cs) {
        mWndScintilla.Create(_T("Scintilla"), m_hWnd, rcDefault, _T(""), WS_CHILD | WS_VISIBLE);
        mWndScintilla.SendMessage(SCI_SETUSETABS, 0);
        mWndScintilla.SendMessage(SCI_SETTABWIDTH, 4);
        
        mWndScintilla.SendMessage(SCI_SETMOUSEDWELLTIME, 2000);

        // configure for generic text
        SetStyleAttribs(STYLE_DEFAULT, {black, white, defaultFontSize, defaultFontName});

        SetMsgHandled(true);

        return 0;
    }

    LRESULT OnNotify(int idCtrl, LPNMHDR pnmh) {
        if (pnmh->code == SCN_DWELLSTART) {
            auto notification = reinterpret_cast<SCNotification*>(pnmh);

            mWndScintilla.SendMessage(SCI_CALLTIPSHOW, notification->position, reinterpret_cast<LPARAM>("void glBegin(GLenum mode)"));
        }

        if (pnmh->code == SCN_CHARADDED) {
            auto notification = reinterpret_cast<SCNotification*>(pnmh);

            if (notification->ch == '(') {
                const auto pos = mWndScintilla.SendMessage(SCI_GETCURRENTPOS);
                mWndScintilla.SendMessage(SCI_CALLTIPSHOW, pos, reinterpret_cast<LPARAM>("The glBegin and glend functions delimit the vertices of a primitive or a group of like primitives.\nmode: The primitive or primitives that will be created from vertices presented between glBegin and the subsequent glEnd."));
                mWndScintilla.SendMessage(SCI_CALLTIPSETHLT, 99, 103);
                
            } else if (notification->ch == ' ') {
                identifier = "";
            } else if (std::isalpha(notification->ch)) {
                identifier += static_cast<char>(notification->ch);
                
                if (identifier.size() >= 3) {
                    mWndScintilla.SendMessage(SCI_AUTOCSHOW, identifier.size(), reinterpret_cast<LPARAM>("void int float pointer test"));
                    identifier = "";
                }
            } else {
                identifier = "";
            }
        }

        SetMsgHandled(false);
        
        return 0;
    }


    void OnDestroy() {
        mWndScintilla.DestroyWindow();
        mWndScintilla.Detach();

        SetMsgHandled(false);
    }

    void OnSize(UINT nType, CSize size) {
        const CRect rect = { 0, 0, size.cx, size.cy };

        mWndScintilla.SetWindowPos(NULL, rect, 0);
    }

    void SetStyleAttribs(const int style, const CodeViewStyleAttribs &attribs) {
        mWndScintilla.SendMessage(SCI_STYLESETFORE, style, attribs.fore);
        mWndScintilla.SendMessage(SCI_STYLESETBACK, style, attribs.back);

        if (attribs.size > 0) {
            mWndScintilla.SendMessage(SCI_STYLESETSIZE, style, attribs.size);
        }
        
        if (attribs.face) {
            mWndScintilla.SendMessage(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(attribs.face));
        }
    }

    void ClearLanguage() {
        mWndScintilla.SendMessage(SCI_STYLECLEARALL);
        mWndScintilla.SendMessage(SCI_CLEARDOCUMENTSTYLE);
    }

    void SetLanguage(const ILexer5 *lexer, const CodeViewLanguageConfig &config) {
        mWndScintilla.SendMessage(SCI_STYLECLEARALL);
        mWndScintilla.SendMessage(SCI_CLEARDOCUMENTSTYLE);

        mWndScintilla.SendMessage(SCI_SETILEXER, 0, reinterpret_cast<LPARAM>(lexer));
        mWndScintilla.SendMessage(SCI_SETKEYWORDS, 0, reinterpret_cast<LPARAM>(config.keywords.c_str()));

        SetStyleAttribs(SCE_C_DEFAULT, config.defaultStyle);
        for (const auto styleColor : config.stylesColors) {
            SetStyleAttribs(styleColor.first, {styleColor.second, white, 0, nullptr});
        }

        mWndScintilla.SendMessage(SCI_STYLESETBOLD, SCE_C_WORD, 1);
        mWndScintilla.SendMessage(SCI_STYLESETBOLD, SCE_C_WORD2, 1);
    }

    void SetInitialContent(const char *textContent) {
        mWndScintilla.SendMessage(SCI_SETTEXT, 0, reinterpret_cast<LPARAM>(textContent));
        mWndScintilla.SendMessage(SCI_EMPTYUNDOBUFFER);
        mWndScintilla.SendMessage(SCI_SETSAVEPOINT);
    }

    std::string GetContent() const {
        std::string content;

        const LRESULT contentLength = mWndScintilla.SendMessage(SCI_GETLENGTH);

        content.resize(static_cast<size_t>(contentLength));

        mWndScintilla.SendMessage(SCI_GETTEXT, contentLength + 1, reinterpret_cast<LPARAM>(content.c_str()));

        return content;
    }

    void ClearSaveState() {
        mWndScintilla.SendMessage(SCI_SETSAVEPOINT);
    }

private:
    std::string identifier;
    mutable CWindow mWndScintilla;
};


// Frame Windows are defined with CFrameWindowImpl
class MainFrame :   public CFrameWindowImpl<MainFrame>,
                    public CUpdateUI<MainFrame>/*,
                    public CMessageFilter,
                    public CIdleHandler*/, 
                    public AppController {
public:
    DECLARE_FRAME_WND_CLASS("MainFrame", IDR_MAINFRAME)

    BEGIN_UPDATE_UI_MAP(MainFrame)
        // UPDATE_ELEMENT(ID_CLOCK_START, UPDUI_MENUPOPUP)
        // UPDATE_ELEMENT(ID_CLOCK_STOP, UPDUI_MENUPOPUP)
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(MainFrame)
        COMMAND_ID_HANDLER_EX(ID_FILE_NEW, OnFileMenu)
        COMMAND_ID_HANDLER_EX(ID_FILE_OPEN, OnFileMenu)
        COMMAND_ID_HANDLER_EX(ID_FILE_OPENFOLDER, OnFileMenu)
        COMMAND_ID_HANDLER_EX(ID_FILE_SAVE, OnFileMenu)
        COMMAND_ID_HANDLER_EX(ID_FILE_SAVE_AS, OnFileMenu)
        COMMAND_ID_HANDLER_EX(ID_FILE_EXIT, OnFileMenu)

        COMMAND_ID_HANDLER_EX(ID_EDIT_UNDO, OnEditMenu)
        COMMAND_ID_HANDLER_EX(ID_EDIT_REDO, OnEditMenu)
        COMMAND_ID_HANDLER_EX(ID_EDIT_CUT, OnEditMenu)
        COMMAND_ID_HANDLER_EX(ID_EDIT_COPY, OnEditMenu)
        COMMAND_ID_HANDLER_EX(ID_EDIT_PASTE, OnEditMenu)
        
        COMMAND_ID_HANDLER_EX(ID_HELP_ABOUT, OnAboutMenu)
        
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)

        CHAIN_MSG_MAP(CUpdateUI<MainFrame>)
        CHAIN_MSG_MAP(CFrameWindowImpl<MainFrame>)
    END_MSG_MAP()

public:
    MainFrame() : mFolderView(this) {}

    void openFile(const boost::filesystem::path &filePath) override {
        doOpenFile(filePath);
    }

public:
    LRESULT OnCreate(LPCREATESTRUCT cs) {
        CreateSimpleToolBar();
        CreateSimpleStatusBar();

        const DWORD dwClientStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        const DWORD dwClientExStyle = WS_EX_CLIENTEDGE;

        mSplitterWindow.Create(*this, rcDefault);

        mFolderView.Create(mSplitterWindow, rcDefault, NULL);
        mCodeView.Create(mSplitterWindow, rcDefault, NULL, dwClientStyle, dwClientExStyle);

        mSplitterWindow.SetSplitterPanes(mFolderView, mCodeView);

        m_hWndClient = mSplitterWindow;
        UpdateLayout();
        mSplitterWindow.SetSplitterPos(200);

        return 0;
    }

    void OnDestroy() {
        SetMsgHandled(false);
        PostQuitMessage(0);
    }

    void OnFileMenu(UINT uCode, int nID, HWND hwndCtrl) {
        if (nID == ID_FILE_OPEN) {
            auto dialog = CFileDialog{TRUE, _T("All Files\0*.*")};

            if (dialog.DoModal() != IDOK) {
                return;
            }

            doOpenFile(boost::filesystem::path{dialog.m_szFileName});
        }
        
        if (nID == ID_FILE_OPENFOLDER) {
            auto folderDialog = CFolderDialog(m_hWnd, _T("Open Folder"));

            if (folderDialog.DoModal() != IDOK) {
                return;
            }

            const boost::filesystem::path folderPath = folderDialog.GetFolderPath();

            mFolderView.DisplayFolder(folderPath);
        }

        if (nID == ID_FILE_SAVE) {
            if (mFilePath) {
                const auto fileService = Xenoide::FileService::create();
                const auto content = mCodeView.GetContent();
                fileService->save(mFilePath.value(), content);

                mCodeView.ClearSaveState();
            } else {
                OnFileMenu(uCode, ID_FILE_SAVE_AS, hwndCtrl);
            }
        }

        if (nID == ID_FILE_SAVE_AS) {
            int x = 0;
        }

        if (nID == ID_FILE_EXIT) {
            DestroyWindow();
        }
    }

    void OnEditMenu(UINT uCode, int nID, HWND hwndCtrl) {
        int x = 0;
    }

    void OnAboutMenu(UINT uCode, int nID, HWND hwndCtrl) {
        if (nID == ID_HELP_ABOUT) {
            AboutDlg dlg;
            dlg.DoModal();
        }
    }

private:
    void doOpenFile(const boost::filesystem::path &filePath) {
        mFilePath = filePath;

        // load the file and put the content into the 
        const auto fileService = Xenoide::FileService::create();
        const auto fileContent = fileService->load(*mFilePath);

        mCodeView.SetInitialContent(fileContent.c_str());

        // 
        const auto codeLang = getCodeLanguage(*mFilePath);

        switch (codeLang) {
            case CodeLanguage::CPP: 
                mCodeView.SetLanguage(Lexilla::MakeLexer("cpp"), languageConfigC); 
                break;

            case CodeLanguage::GLSL: 
                mCodeView.SetLanguage(Lexilla::MakeLexer("cpp"), languageConfigGLSL); 
                break;

            default: 
                mCodeView.ClearLanguage();
        }
    }


private:
    CodeView mCodeView;
    FolderView mFolderView;

    CString mFolderPath;
    CSplitterWindow mSplitterWindow;
    std::optional<boost::filesystem::path> mFilePath;
};


CAppModule _Module;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    ::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
    ::AtlInitCommonControls(ICC_COOL_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES);

    HMODULE hModScintilla = ::LoadLibrary(_T("Scintilla.dll"));

    if (hModScintilla == nullptr) {
        constexpr auto msg = _T("Required Scintilla.dll module wasn't found.");
        constexpr auto title = _T("XenoShader");

        ::MessageBox(nullptr, msg, title, MB_OK | MB_ICONERROR);
        return 1;
    }

    Lexilla::SetDefaultDirectory(".");
    Lexilla::Load(".");
    
    _Module.Init(NULL, hInstance);

    MainFrame mainFrame;
    MSG msg;

    if (NULL == mainFrame.CreateEx()) {
        return 1;
    }

    mainFrame.ShowWindow(nCmdShow);
    mainFrame.UpdateWindow();

    while (::GetMessage(&msg, NULL, 0, 0) > 0) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    _Module.Term();

    ::FreeLibrary(hModScintilla);
    
    return static_cast<int>(msg.wParam);
}
