#include "MainComponent.h"

static const int window_size = 1200;
static const int window_limit_size = 500;
static const int main_menu_y_position_scaling = 6;

//==============================================================================
MainComponent::MainComponent()
    : m_main_menu(this)
    , m_training_data_view(this)
    , m_generate_sequence_view(this)
{
    setSize(window_size, window_size);
    addAndMakeVisible(m_main_menu);
}

//==============================================================================
void
MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::purple);
}

void
MainComponent::menuItemSelected(const MenuItem& menu_selection)
{
    removeChildComponent(&m_main_menu);
    switch (menu_selection) {
        case MenuItem::view_test_data:
            m_training_data_view.setBounds(0, 0, getWidth(), getHeight());
            addAndMakeVisible(m_training_data_view);
            break;
        case MenuItem::generate_sequence:
            m_generate_sequence_view.setBounds(0, 0, getWidth(), getHeight());
            addAndMakeVisible(m_generate_sequence_view);
            break;
        default:
            break;
    }
}

void
MainComponent::returnToMainMenu()
{
    // this should pass in the menu item so it knows what to close
    removeChildComponent(&m_training_data_view);
    addAndMakeVisible(m_main_menu);
}

void
MainComponent::exitGenerateSequence()
{
    removeChildComponent(&m_generate_sequence_view);
    addAndMakeVisible(m_main_menu);
}

void
MainComponent::resized()
{
    m_main_menu.setBounds((getWidth()/2) - (m_main_menu.getWidth()/2) ,
                                            getHeight()/main_menu_y_position_scaling,
                                            m_main_menu.getWidth(),
                                            450);
    m_training_data_view.setBounds(0, 0, getWidth(), getHeight());
    m_generate_sequence_view.setBounds(0, 0, getWidth(), getHeight());
    
    int w = getWidth();
    int h = getHeight();
    if (w < window_limit_size) w = window_limit_size;
    if (h < window_limit_size) h = window_limit_size;
    if (w != getWidth() || h != getHeight())
        setSize(w, h);
}
