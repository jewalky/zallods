#include "core.hpp"
#include "config.hpp"
#include "display.hpp"

int main(int argc, char* argv[])
{
    core::printf(" * Initializing system...%n");
    core::init_args(argc, argv);

    // parse command-line arguments
    if(core::has_arg("-w") && core::has_arg("-h"))
    {
        uint16_t nW = core::sub_arg("-w", 1).toInteger();
        uint16_t nH = core::sub_arg("-h", 1).toInteger();
        if(nW > 0 && nH > 0)
        {
            config::display::width = nW;
            config::display::height = nH;
        }
    }
    else if(core::has_arg("-1024"))
    {
        config::display::width = 1024;
        config::display::height = 768;
    }
    else if(core::has_arg("-800"))
    {
        config::display::width = 800;
        config::display::height = 600;
    }
    else if(core::has_arg("-640"))
    {
        config::display::width = 640;
        config::display::height = 480;
    }

    if(core::has_arg("-serve"))
        config::is_server = true;

    if(!config::is_server)
    {
        core::printf(" * Initializing display (at %ux%ux32)...%n", config::display::width, config::display::height);
        if(!display::init())
        {
            core::printf(" - Failed.%n");
            return 1;
        }
    }

    core::printf(" * Initializing resources...%n");
    core::printf(" * Entering main loop...%n");
    core::printf(" * Exiting.%n");
    return 0;
}
