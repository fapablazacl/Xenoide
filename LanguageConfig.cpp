
#include "LanguageConfig.h"

namespace xenoide {
    // glsl 4.5 keywords
    const char* keywordsGlsl = {
        // core keywords
        "const uniform buffer shared attribute varying coherent volatile restrict readonly writeonly atomic_uint layout centroid flat smooth noperspective patch sample invariant precise break continue do for while switch case default if else subroutine in out inout int void bool true false float double discard return vec2 vec3 vec4 ivec2 ivec3 ivec4 bvec2 bvec3 bvec4 uint uvec2 uvec3 uvec4 dvec2 dvec3 dvec4 mat2 mat3 mat4 mat2x2 mat2x3 mat2x4 mat3x2 mat3x3 mat3x4 mat4x2 mat4x3 mat4x4 dmat2 dmat3 dmat4 dmat2x2 dmat2x3 dmat2x4 dmat3x2 dmat3x3 dmat3x4 dmat4x2 dmat4x3 dmat4x4 lowp mediump highp precision sampler1D sampler1DShadow sampler1DArray sampler1DArrayShadow isampler1D isampler1DArray usampler1D usampler1DArray sampler2D sampler2DShadow sampler2DArray sampler2DArrayShadow isampler2D isampler2DArray usampler2D usampler2DArray sampler2DRect sampler2DRectShadow isampler2DRect usampler2DRect sampler2DMS isampler2DMS usampler2DMS sampler2DMSArray isampler2DMSArray usampler2DMSArray sampler3D isampler3D usampler3D samplerCube samplerCubeShadow isamplerCube usamplerCube samplerCubeArray samplerCubeArrayShadow isamplerCubeArray usamplerCubeArray samplerBuffer isamplerBuffer usamplerBuffer image1D iimage1D uimage1D image1DArray iimage1DArray uimage1DArray image2D iimage2D uimage2D image2DArray iimage2DArray uimage2DArray image2DRect iimage2DRect uimage2DRect image2DMS iimage2DMS uimage2DMS image2DMSArray iimage2DMSArray uimage2DMSArray image3D iimage3D uimage3D imageCube iimageCube uimageCube imageCubeArray iimageCubeArray uimageCubeArray imageBuffer iimageBuffer uimageBuffer struct "

        // vulkan keywords
        "texture1D texture1DArray itexture1D itexture1DArray utexture1D utexture1DArray texture2D texture2DArray itexture2D itexture2DArray utexture2D utexture2DArray texture2DRect itexture2DRect utexture2DRect texture2DMS itexture2DMS utexture2DMS texture2DMSArray itexture2DMSArray utexture2DMSArray texture3D itexture3D utexture3D textureCube itextureCube utextureCube textureCubeArray itextureCubeArray utextureCubeArray textureBuffer itextureBuffer utextureBuffer sampler samplerShadow subpassInput isubpassInput usubpassInput subpassInputMS isubpassInputMS usubpassInputMS "
    };

    // glsl 4.5 reserved keywords
    const char* keywordsGlslReserved = {
        // reserved for future use
        "common partition active asm class union enum typedef template this resource goto inline noinline public static extern external interface long short half fixed unsigned superp input output hvec2 hvec3 hvec4 fvec2 fvec3 fvec4 filter sizeof cast namespace using sampler3DRect "
    };

    const LanguageKeywords keywordsGlsl110{ keywordsGlsl, keywordsGlslReserved };
    const LanguageKeywords keywordsGlsl120{ keywordsGlsl, keywordsGlslReserved };
    const LanguageKeywords keywordsGlsl130{ keywordsGlsl, keywordsGlslReserved };
    const LanguageKeywords keywordsGlsl140{ keywordsGlsl, keywordsGlslReserved };
    const LanguageKeywords keywordsGlsl150{ keywordsGlsl, keywordsGlslReserved };
    const LanguageKeywords keywordsGlsl330{ keywordsGlsl, keywordsGlslReserved };
    const LanguageKeywords keywordsGlsl400{ keywordsGlsl, keywordsGlslReserved };
    const LanguageKeywords keywordsGlsl410{ keywordsGlsl, keywordsGlslReserved };
    const LanguageKeywords keywordsGlsl420{ keywordsGlsl, keywordsGlslReserved };
    const LanguageKeywords keywordsGlsl430{ keywordsGlsl, keywordsGlslReserved };
    const LanguageKeywords keywordsGlsl440{ keywordsGlsl, keywordsGlslReserved };
    const LanguageKeywords keywordsGlsl450{ keywordsGlsl, keywordsGlslReserved };

    const std::map<LanguageDialect, LanguageKeywords> languageMap = {
        { LanguageDialect::GLSL_110, keywordsGlsl110 },
        { LanguageDialect::GLSL_120, keywordsGlsl120 },
        { LanguageDialect::GLSL_130, keywordsGlsl130 },
        { LanguageDialect::GLSL_140, keywordsGlsl140 },
        { LanguageDialect::GLSL_150, keywordsGlsl150 },
        { LanguageDialect::GLSL_330, keywordsGlsl330 },
        { LanguageDialect::GLSL_400, keywordsGlsl400 },
        { LanguageDialect::GLSL_410, keywordsGlsl410 },
        { LanguageDialect::GLSL_420, keywordsGlsl420 },
        { LanguageDialect::GLSL_430, keywordsGlsl430 },
        { LanguageDialect::GLSL_440, keywordsGlsl440 },
        { LanguageDialect::GLSL_450, keywordsGlsl450 },
    };
}