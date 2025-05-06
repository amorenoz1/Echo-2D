#ifndef FONT_H
#define FONT_H

#include <map>
#include <core/core.h>
#include <glm/glm.hpp>
#include <engine/Texture.h>

namespace Echo2D {

/**
 * @struct Character
 * @brief Holds all state information relevant to a single glyph character.
 *
 * The `Character` struct stores all the necessary information for rendering
 * an individual character, such as its texture, size, bearing, and the
 * advance offset used when rendering text.
 */
struct Character {
    Texture* m_Texture;    /**< Pointer to the glyph's texture. */
    glm::ivec2 m_Size;     /**< Size of the glyph (width and height). */
    glm::ivec2 m_Bearing;  /**< Offset from baseline to top-left of glyph. */
    GLuint m_Advance;      /**< Horizontal offset to advance to next glyph (in pixels). */
};

/**
 * @class Font
 * @brief Manages font loading and provides access to glyph Characters.
 *
 * The `Font` class loads a font from a TrueType font file and makes it
 * available for use in rendering. It provides access to individual glyph
 * characters and stores the associated textures and metadata.
 */
class Font {
public:
    /**
     * @brief Constructs a Font by loading a font file at a specific pixel size.
     *
     * Initializes FreeType, loads the font file, sets the desired pixel size,
     * and generates textures for the first 128 ASCII characters.
     *
     * @param fontPath Path to the TrueType font (.ttf) file.
     * @param fontSize Size (in pixels) to load the font at.
     */
    Font(const char* fontPath, GLuint fontSize);

    /**
     * @brief Destructor. Frees associated font resources.
     *
     * Cleans up dynamically allocated textures and releases font resources.
     */
    ~Font();

    /**
     * @brief Retrieves the Character struct corresponding to a given character.
     *
     * This function returns a reference to the `Character` object that contains
     * the texture and metadata for the specified character.
     *
     * @param c ASCII character to look up.
     * @return Reference to the Character object.
     */
    Character& GetCharacter(char c);

private:
    FT_Face m_Face; /**< FreeType font face object. */
    std::map<char, Character> m_Characters; /**< Map from character codes to their glyph information. */
};

} // namespace Echo2D

#endif

