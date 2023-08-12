
#include "CodeEditorConfiguration.h"
#include "FileNamePatternMatcher.h"

CodeEditorConfiguration languageConfigC = {
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


CodeEditorConfiguration languageConfigGLSL = {
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


class ScintillaNotificationHandlerGLSL : public ScintillaNotificationHandler {
public:
    void handleNotification(CWindow &scintilla, const SCNotification &notification) override {

    }
} notificationHandlerGLSL;



class ScintillaNotificationHandlerCPP : public ScintillaNotificationHandler {
public:
    void handleNotification(CWindow &scintilla, const SCNotification &notification) override {

        const auto code = static_cast<Scintilla::Notification>(notification.nmhdr.code);

        switch (code) {
        case Scintilla::Notification::DwellStart:
            scintilla.SendMessage(SCI_CALLTIPSHOW, notification.position, reinterpret_cast<LPARAM>("void glBegin(GLenum mode)"));
            break;

        case Scintilla::Notification::CharAdded:
            if (notification.ch == 13) {
                // return
            }

            if (notification.ch == '(') {
                const auto pos = scintilla.SendMessage(SCI_GETCURRENTPOS);
                scintilla.SendMessage(SCI_CALLTIPSHOW, pos, reinterpret_cast<LPARAM>("The glBegin and glend functions delimit the vertices of a primitive or a group of like primitives.\nmode: The primitive or primitives that will be created from vertices presented between glBegin and the subsequent glEnd."));
                scintilla.SendMessage(SCI_CALLTIPSETHLT, 99, 103);

            } else if (notification.ch == '.') {
                scintilla.SendMessage(SCI_AUTOCSHOW, 0, reinterpret_cast<LPARAM>("void int float pointer test"));
            }
        }
    }
} 
notificationHandlerCPP;



static std::vector<std::pair<FileNamePatternMatcher, CodeEditorLanguage>> matcherLanguagePairs = {
    {
        FileNamePatternMatcher{".cpp", ".hpp", ".cxx", ".hxx", ".c++", ".h++", ".cc", ".hh", ".c", ".h"}, 
        CodeEditorLanguage::CPP
    },  {
        FileNamePatternMatcher{".vert", ".tesc", ".tese", ".geom", ".frag", ".comp", ".glsl"}, 
        CodeEditorLanguage::GLSL
    }
};


CodeEditorLanguage detectCodeLanguage(const boost::filesystem::path &filePath) {
    for (const auto &pair : matcherLanguagePairs) {
        if (pair.first.match(filePath)) {
            return pair.second;
        }
    }

    return CodeEditorLanguage::TEXT;
}


CodeEditorConfiguration::CodeEditorConfiguration() : 
    defaultStyle {black, white, defaultFontSize, defaultFontName} {}


CodeEditorConfiguration::CodeEditorConfiguration(   
    const CodeEditorStyle &defaultStyle, 
    const std::string &keywords, 
    const std::vector<std::pair<int, COLORREF>> &stylesColors) 
    : defaultStyle{defaultStyle}, keywords{keywords}, stylesColors{stylesColors} {}


std::optional<CodeEditorConfiguration> CodeEditorConfiguration::detect(const boost::filesystem::path& filePath) {
    CodeEditorConfiguration config;

    const auto codeLang = detectCodeLanguage(filePath);

    switch (codeLang) {
    case CodeEditorLanguage::CPP:
        config = languageConfigC;
        config.lexer = Lexilla::MakeLexer("cpp");
        config.notificationHandler = &notificationHandlerCPP;
        break;

    case CodeEditorLanguage::GLSL: 
        config = languageConfigGLSL;
        config.lexer = Lexilla::MakeLexer("cpp");
        config.notificationHandler = &notificationHandlerGLSL;
        break;

    default:
        return {};
    }

    return config;
}
