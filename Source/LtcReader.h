/*
  ==============================================================================

    LtcReader.h
    Created: 29 Jan 2024 11:35:34am
    Author:  joris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <decoder.h>

class LTCReader 
{
public:
    LTCReader();
    ~LTCReader();
    
    void processAudio( const float* inBuffer, const size_t length );

    int getCurrentFrame();
    String getCurrentFrameAsString();

    class JUCE_API  Listener
    {
    public:
        /** Destructor. */
        virtual ~Listener() = default;

        virtual void newFrameReceived() = 0;
    };
    void addListener( Listener* newListener ) { listeners.add( newListener ); };
    void removeListener( Listener* listener ) { listeners.remove( listener ); };
    
private:
    LTCDecoder* decoder;
    LTCFrameExt frame;
    SMPTETimecode stime;

    ListenerList<LTCReader::Listener> listeners;
};
