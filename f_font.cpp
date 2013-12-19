#include "f_font.h"
#include "c_main.h"
#include "f_file.h"
#include "f_16.h"
#include "f_16a.h"
#include "f_256.h"
#include "r_main.h"

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

    if(spaceWidth < 2)
        spaceWidth = 2;
    mSizes[0] = spaceWidth-2;
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

uint32_t Font::measureWidth(String text, Font::Align align)
{
    uint32_t rwidth = 0;

    for(size_t i = 0; i < text.length(); i++)
    {
        uint8_t c = (uint8_t)convertEncoding(text[i]);
        if(c == 0xF0) continue;
        rwidth += mSizes[c]+2;
    }

    return rwidth;
}

void Font::display(const SDL_Rect& rect, String text, Font::Align align, uint8_t r, uint8_t g, uint8_t b, int16_t shadowpos)
{
    if(shadowpos != 0x7FFF)
    {
        SDL_Rect clipRect = rect;
        clipRect.w += shadowpos;
        clipRect.h += shadowpos;
        R_SetClip(clipRect);

        SDL_Rect rect2 = rect;
        rect2.x += shadowpos;
        rect2.y += shadowpos;
        display(rect2, text, align, 0, 0, 0, 0x7FFF);
    }

    int16_t offs = 0;
    int16_t yoffs = 0;
    // string is in cp-1251
    // fonts are cp-866 (-0x20 for every char)
    size_t spos = 0;
    while(spos < text.length())
    {
        // try peeking ahead
        size_t lastspace = spos;
        size_t nspos = spos;
        size_t lastwsize = 0;
        size_t wsize = 0;
        size_t lastwssize = 0;
        size_t wssize = 0;
        size_t lastnssize = 0;
        size_t nssize = 0;
        int skipcnt = 0;
        int spacecnt = 0;
        for(/*nspos*/; nspos < text.length(); nspos++)
        {
            uint8_t c = (uint8_t)convertEncoding(text[nspos]);
            wsize += mSizes[c]+2;

            if(text[nspos] == ' ' ||
               text[nspos] == '\t')
            {
                lastwsize = wsize;
                lastnssize = nssize;
                lastwssize = wssize;
                lastspace = nspos+1;
                spacecnt++;
            }
            else if(text[nspos] == '\n')
            {
                lastwsize = wsize;
                lastnssize = nssize;
                lastwssize = wssize;
                lastspace = nspos+1;
            }
            else if(text[nspos] == '.' ||
                    text[nspos] == '-' ||
                    text[nspos] == '_' ||
                    text[nspos] == '@' ||
                    text[nspos] == '{' ||
                    text[nspos] == '}' ||
                    text[nspos] == '|')
            {
                lastwsize = wsize;
                lastnssize = nssize;
                lastwssize = wssize;
                lastspace = nspos+1;
                wssize += mSizes[c]+2;
                nssize++;
            }
            else
            {
                wssize += mSizes[c]+2;
                nssize++;
            }

            if(wsize > rect.w || text[nspos] == '\n')
            {
                if(lastspace == spos) // no spaces
                    lastspace = nspos;
                nspos = lastspace;

                /*if(text[nspos] != ' ' && text[nspos] != '\t' && text[nspos] != '\n')
                {
                    wssize -= mSizes[c]+2;
                    nssize--;
                }*/

                if(text[nspos] != ' ' && text[nspos] != '\t')
                {
                    // ok if this char isn't space, then we should skip it and see if we have any spaces preceding it
                    for(size_t j = nspos-1; j > 0; j--)
                    {
                        if(text[j] == ' ' || text[j] == '\t')
                        {
                            lastspace--;
                            spacecnt--;
                            skipcnt++;
                        }
                        else break;
                    }
                }

                break;
            }
        }

        if(nspos >= text.length())
        {
            lastspace = nspos = text.length();
        }
        else
        {
            wssize = lastwssize;
            wsize = lastwsize;
            nssize = lastnssize;
        }

        String drawnText = text.substring(spos, lastspace);

        if(align == Align_Right)
            offs = rect.w-measureWidth(drawnText, align);
        else if(align == Align_Center)
            offs = (rect.w/2)-(measureWidth(drawnText, align)/2);
        else offs = 0;

        int spacewidth = ((lastspace != text.length() && align == Align_Both && spacecnt > 0) ? ((rect.w - wssize) / (spacecnt)) : (mSizes[0]+2));

        for(size_t i = spos; i < lastspace; i++)
        {
            uint8_t c = (uint8_t)convertEncoding(text[i]);
            if(c == 0xF0) continue;

            mSprite->displayColored(rect.x+offs, rect.y+yoffs, c, r, g, b, 255);
            offs += c>0 ? mSizes[c]+2 : spacewidth;
        }

        spos = lastspace+skipcnt;
        yoffs += mLineHeight;
    }
}

char Font::convertEncoding(char cc)
{
    uint8_t c = (uint8_t)cc;
    if(c >= 0xC0 && c <= 0xEF)
        c -= 0x40;
    else if(c >= 0xF0 && c <= 0xFF)
        c -= 0x10;

    if(c == '\n') c = 0xF0;
    else if(c < 0x20) c = 0x5F;
    else c -= 0x20;

    return c;
}

void LoadFonts()
{
    C_Printf("LoadFonts: Initializing fonts...\n");
    Font1 = new Font("graphics/font1/font1.16", 16, 7);
}
