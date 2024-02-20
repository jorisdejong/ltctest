#pragma once

#include <JuceHeader.h>

#include "LtcReader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : 
    public juce::AudioAppComponent,
    public ComponentListener,
    public LTCReader::Listener

{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    LTCReader ltcReader;

    ComboBox channelComboBox;
    Label channelLabel;
    std::unique_ptr<AudioDeviceSelectorComponent> audioPrefsComponent;
    Label ltcLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)

        // Inherited via Listener
        virtual void newFrameReceived() override;
};
