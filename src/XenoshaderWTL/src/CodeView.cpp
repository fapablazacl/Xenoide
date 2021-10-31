
#include "CodeView.h"

#include <boost/filesystem/path.hpp>



CodeViewLanguageConfig languageConfigC = {
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


CodeViewLanguageConfig languageConfigGLSL = {
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


std::vector<std::string> extensionsC = {
    ".cpp", ".hpp", ".cxx", ".hxx", ".c++", ".h++", ".cc", ".hh", ".c", ".h"
};


std::vector<std::string> extensionsGLSL = {
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


CodeView::CodeView() {
    m_bMsgHandled = false;
}


LRESULT CodeView::OnCreate(LPCREATESTRUCT cs) {
    mWndScintilla.Create(_T("Scintilla"), m_hWnd, rcDefault, _T(""), WS_CHILD | WS_VISIBLE);
    mWndScintilla.SendMessage(SCI_SETUSETABS, 0);
    mWndScintilla.SendMessage(SCI_SETTABWIDTH, 4);
        
    mWndScintilla.SendMessage(SCI_SETMOUSEDWELLTIME, 2000);

    // configure for generic text
    SetStyleAttribs(STYLE_DEFAULT, {black, white, defaultFontSize, defaultFontName});

    SetMsgHandled(true);

    return 0;
}

LRESULT CodeView::OnNotify(int idCtrl, LPNMHDR pnmh) {
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


void CodeView::OnDestroy() {
    mWndScintilla.DestroyWindow();
    mWndScintilla.Detach();

    SetMsgHandled(false);
}

void CodeView::OnSize(UINT nType, CSize size) {
    const CRect rect = { 0, 0, size.cx, size.cy };

    mWndScintilla.SetWindowPos(NULL, rect, 0);
}

void CodeView::SetStyleAttribs(const int style, const CodeViewStyleAttribs &attribs) {
    mWndScintilla.SendMessage(SCI_STYLESETFORE, style, attribs.fore);
    mWndScintilla.SendMessage(SCI_STYLESETBACK, style, attribs.back);

    if (attribs.size > 0) {
        mWndScintilla.SendMessage(SCI_STYLESETSIZE, style, attribs.size);
    }
        
    if (attribs.face) {
        mWndScintilla.SendMessage(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(attribs.face));
    }
}

void CodeView::ClearLanguage() {
    mWndScintilla.SendMessage(SCI_STYLECLEARALL);
    mWndScintilla.SendMessage(SCI_CLEARDOCUMENTSTYLE);
}

void CodeView::SetLanguage(const ILexer5 *lexer, const CodeViewLanguageConfig &config) {
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

void CodeView::SetInitialContent(const char *textContent) {
    mWndScintilla.SendMessage(SCI_SETTEXT, 0, reinterpret_cast<LPARAM>(textContent));
    mWndScintilla.SendMessage(SCI_EMPTYUNDOBUFFER);
    mWndScintilla.SendMessage(SCI_SETSAVEPOINT);
}

std::string CodeView::GetContent() const {
    std::string content;

    const LRESULT contentLength = mWndScintilla.SendMessage(SCI_GETLENGTH);

    content.resize(static_cast<size_t>(contentLength));

    mWndScintilla.SendMessage(SCI_GETTEXT, contentLength + 1, reinterpret_cast<LPARAM>(content.c_str()));

    return content;
}

void CodeView::ClearSaveState() {
    mWndScintilla.SendMessage(SCI_SETSAVEPOINT);
}
