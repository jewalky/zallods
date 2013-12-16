#include "f_font.h"
#include "c_main.h"
#include "f_file.h"
#include "f_16.h"
#include "f_16a.h"
#include "f_256.h"

Font* Font1 = NULL;
Font* Font2 = NULL;
Font* Font3 = NULL;
Font* Font4 = NULL;

Font::Font(String base_filename, uint32_t lineHeight, uint32_t spaceWidth)
{
    memset(mSizes, 0, sizeof(mSizes));

    String dat_filename = "";
    size_t lastPos = base_filename.find_last_of(".");
    if(lastPos != String::npos)
        dat_filename = base_filename.substring(0, lastPos)+".dat";
    else C_Fatal("Font::Font: Invalid base filename: \"%s\".\n", base_filename.c_str());

    File f;
    if(!f.open(dat_filename))
        C_Fatal("Font::Font: Couldn't open font data file \"%s\".\n", dat_filename.c_str());

    f.seek(0);
    f.read(mSizes, sizeof(mSizes));
    f.close();

    mSizes[0] = spaceWidth;
    mLineHeight = lineHeight;

    String filetype = base_filename.substring(lastPos+1).toLowerCase();

    if(filetype == "16")
        mSprite = new Sprite_16(base_filename);
    else if(filetype == "16a")
        mSprite = new Sprite_16A(base_filename);
    else if(filetype == "256")
        mSprite = new Sprite_256(base_filename);
    else C_Fatal("Font::Font: Couldn't guess sprite's type from extension: \"%s\".\n", base_filename.c_str());
}

Font::~Font()
{
    if(mSprite != 0)
        delete mSprite;
}

uint32_t Font::measureWidth(const SDL_Rect& rect, String text, Font::Align align)
{
    return measureWidth(rect.w, text, align);
}

uint32_t Font::measureWidth(uint32_t width, String text, Font::Align align)
{
    return 0;
}

void Font::display(const SDL_Rect& rect, String text, Font::Align align, uint8_t r, uint8_t g, uint8_t b)
{

}

void LoadFonts()
{
    C_Printf("LoadFonts: Initializing fonts...\n");
    Font1 = new Font("graphics/font1/font1.16", 16, 8);
}
