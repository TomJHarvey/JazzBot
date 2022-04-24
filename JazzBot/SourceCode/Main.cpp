// Comments will make this look better with a bit of extra formatting too

#ifdef GUI_APP

#include "GuiApp/MainGuiApp.hpp"
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MainGuiApp)

#else

#include <juce_core/juce_core.h>
#include "CommandLineApp/CommandLineApplication.hpp"

int main (int argc, char* argv[])
{
    juce::ignoreUnused(argc,argv);
    CommandLineApplication command_line_app; // need to pass in the args
    return 0;
}

#endif


