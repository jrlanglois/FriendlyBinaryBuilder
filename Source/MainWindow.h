#ifndef JRL_MAIN_WINDOW_H
#define JRL_MAIN_WINDOW_H

#include "MainComponent.h"

class MainWindow : public juce::DocumentWindow
{
public:
    /** Constructor */
    MainWindow();

    //==============================================================================
    /** @internal */
    void closeButtonPressed() override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};

#endif //JRL_MAIN_WINDOW_H