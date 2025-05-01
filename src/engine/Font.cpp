#include <engine/Font.h>
#include <engine/Texture.h>
#include "external/easylogging++.h"

namespace Echo2D {

/**
 * @brief Constructs a Font object by loading glyphs from a font file.
 * 
 * Initializes FreeType, loads the specified font face, sets the desired pixel size,
 * and creates textures for the first 128 ASCII glyphs.
 * 
 * @param fontPath Path to the TrueType font (.ttf) file.
 * @param fontSize Pixel size for rendering the font.
 */
Font::Font(const char* fontPath, GLuint fontSize) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        LOG(ERROR) << "Failed to initialize FreeType";
        return;
    }

    if (FT_New_Face(ft, fontPath, 0, &Face)) {
        LOG(ERROR) << "Failed to load font: " << fontPath;
        FT_Done_FreeType(ft);
        return;
    }

    FT_Set_Pixel_Sizes(Face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

    // Load first 128 ASCII characters
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(Face, c, FT_LOAD_RENDER)) {
            LOG(WARNING) << "Failed to load Glyph: " << c;
            continue;
        }

        // Generate texture for the glyph
        Texture* glyphTex = new Texture(
            Face->glyph->bitmap.width,
            Face->glyph->bitmap.rows,
            Face->glyph->bitmap.buffer
        );

        Character character = {
            glyphTex,
            {Face->glyph->bitmap.width, Face->glyph->bitmap.rows},
            {Face->glyph->bitmap_left, Face->glyph->bitmap_top},
            static_cast<GLuint>(Face->glyph->advance.x)
        };
        Characters.insert({c, character});
    }

    FT_Done_Face(Face);
    FT_Done_FreeType(ft);
}

/**
 * @brief Destructor. Cleans up dynamically allocated glyph textures.
 */
Font::~Font() {
    for (auto& [c, ch] : Characters) {
        delete ch.Texture;
    }
}

/**
 * @brief Retrieves the Character struct associated with a given character code.
 * 
 * @param c The ASCII character to retrieve.
 * @return Reference to the corresponding Character object.
 * 
 * @throws std::out_of_range If the character does not exist in the Characters map.
 */
Character& Font::GetCharacter(char c) {
    return Characters.at(c);
}

} // namespace Echo2D

