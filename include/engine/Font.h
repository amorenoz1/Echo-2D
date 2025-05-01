#pragma once

#include <map>
#include <core/core.h>
#include <glm/glm.hpp>
#include <engine/Texture.h>

namespace Echo2D {

/**
 * @brief Holds all state information relevant to a single glyph character.
 */
struct Character {
    Texture* Texture;    /**< Pointer to the glyph's texture. */
    glm::ivec2 Size;     /**< Size of the glyph (width and height). */
    glm::ivec2 Bearing;  /**< Offset from baseline to top-left of glyph. */
    GLuint Advance;      /**< Horizontal offset to advance to next glyph (in pixels). */
};

/**
 * @brief Manages font loading and provides access to glyph Characters.
 */
class Font {
public:
    /**
     * @brief Constructs a Font by loading a font file at a specific pixel size.
     * 
     * @param fontPath Path to the TrueType font (.ttf) file.
     * @param fontSize Size (in pixels) to load the font at.
     */
    Font(const char* fontPath, GLuint fontSize);

    /**
     * @brief Destructor. Frees associated font resources.
     */
    ~Font();

    /**
     * @brief Retrieves the Character struct corresponding to a given character.
     * 
     * @param c ASCII character to look up.
     * @return Reference to the Character object.
     */
    Character& GetCharacter(char c);

private:
    FT_Face Face; /**< FreeType font face object. */
    std::map<char, Character> Characters; /**< Map from character codes to their glyph information. */
};

} // namespace Echo2D

