#include "stdinc.h"
#include "Bitmap.h"

HFCLOUD_BEGIN

class Font{
    TTF_Font *_font;                /**< SDL_ttf object.             */
    std::string _filename;          
    int _size;
    unsigned _style;

    void _constrcut(const std::string &fn, int size);
    void _destruct();

public:
    const std::string &filename = _filename;            /**< Filename of the font file.  */
    const int &size = _size;                            /**< Font's ptsize.              */
    const int &style = _style;                          /**< Font's stype.               */

    Font();
    Font(const std::string &fn, int size);
    void reload(const std::string &fn);     
    void reload(int size);
    void reload(const std::string &fn, int size);
    //---------------------------------------------------
    static const int STYLE_NORMAL          =       TTF_STYLE_NORMAL;
    static const int STYLE_BOLD            =       TTF_STYLE_BOLD;
    static const int STYLE_ITALIC          =       TTF_STYLE_ITALIC;
    static const int STYLE_UNDERLINE       =       TTF_STYLE_UNDERLINE;
    static const int STYLE_STRIKETHROUGH   =       TTF_STYLE_STRIKETHROUGH;

    void set_style(int nstyle);                         /**< Set the font's style.       */
    //---------------------------------------------------
    Bitmap *render_solid_text(const std::string &text, HfCloud::Color color);

};


HFCLOUD_END