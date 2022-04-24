#include "MainComponent.h"

static const int window_size = 1200;
static const int window_limit_size = 500;
static const int main_menu_y_position_scaling = 6;

//==============================================================================
MainComponent::MainComponent()
    : m_main_menu(this)
{
    setSize (window_size, window_size);
    addAndMakeVisible(m_main_menu);
}

//==============================================================================
void
MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::purple);
    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Gui app functionality to be completed", getLocalBounds(), juce::Justification::centred, true);
}

void
MainComponent::menuItemSelected(const MenuItem& menu_selection)
{
    removeChildComponent(&m_main_menu);
    switch (menu_selection) {
        case MenuItem::view_test_data:
            break;
        case MenuItem::generate_sequence:
            break;
        case MenuItem::view_sequence:
            break;
        default:
            break;
    }
}

void
MainComponent::resized()
{
    m_main_menu.setBounds((getWidth()/2) - (m_main_menu.getWidth()/2) ,
                                            getHeight()/main_menu_y_position_scaling,
                                            m_main_menu.getWidth(),
                                            m_main_menu.getHeight());
    
    int w = getWidth();
    int h = getHeight();
    if (w < window_limit_size) w = window_limit_size;
    if (h < window_limit_size) h = window_limit_size;
    if (w != getWidth() || h != getHeight())
        setSize(w, h);
}
