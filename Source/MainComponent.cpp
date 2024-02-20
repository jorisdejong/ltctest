#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
	audioPrefsComponent =
		std::make_unique<AudioDeviceSelectorComponent>( deviceManager, 0, 2, 0, 0, false, false, false, false );
	addAndMakeVisible( audioPrefsComponent.get() );
	audioPrefsComponent->addComponentListener( this );

	ltcLabel.setFont( Font( "Lucida Console", 100.0f, Font::bold ) );
	ltcLabel.setText( "00:00:00:00", dontSendNotification );
	ltcLabel.setJustificationType( Justification::centred );
	addAndMakeVisible( ltcLabel );

	channelComboBox.addItemList( { "Left Channel", "Right Channel" }, 1 );
	channelComboBox.setSelectedItemIndex( 0, dontSendNotification );
	addAndMakeVisible( channelComboBox );
	channelLabel.setText( "Input Channel:", dontSendNotification );
	channelLabel.setJustificationType( Justification::right );
	addAndMakeVisible( channelLabel );

	// Make sure you set the size of the component after
	// you add any child components.
	setSize( 800, 500 );

	// Some platforms require permissions to open input channels so request that here
	if ( juce::RuntimePermissions::isRequired( juce::RuntimePermissions::recordAudio )
		&& !juce::RuntimePermissions::isGranted( juce::RuntimePermissions::recordAudio ) )
	{
		juce::RuntimePermissions::request( juce::RuntimePermissions::recordAudio,
			[ & ] ( bool granted ) { setAudioChannels( granted ? 2 : 0, 0 ); } );
	}
	else
	{
		// Specify the number of input and output channels that we want to open
		setAudioChannels( 2, 0 );
	}

	ltcReader.addListener( this );
}

MainComponent::~MainComponent()
{
	// This shuts down the audio device and clears the audio source.
	shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay( int samplesPerBlockExpected, double sampleRate )
{
	// This function will be called when the audio device is started, or when
	// its settings (i.e. sample rate, block size, etc) are changed.

	// You can use this function to initialise any resources you might need,
	// but be careful - it will be called on the audio thread, not the GUI thread.

	// For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock( const juce::AudioSourceChannelInfo& bufferToFill )
{
	auto* device = deviceManager.getCurrentAudioDevice();
	//DBG( device->getName() );
	auto activeInputChannels = device->getActiveInputChannels();
	auto maxInputChannels = activeInputChannels.getHighestBit() + 1;

	int channel = channelComboBox.getSelectedItemIndex();
	if ( ( !activeInputChannels[ channel ] ) || maxInputChannels == 0 )
	{
		//bufferToFill.buffer->clear( channel, bufferToFill.startSample, bufferToFill.numSamples );
	}
	else
	{
		const float* inBuffer = bufferToFill.buffer->getReadPointer( channel, bufferToFill.startSample );
		ltcReader.processAudio( inBuffer, bufferToFill.numSamples );
	}
}

void MainComponent::releaseResources()
{
	
}

//==============================================================================
void MainComponent::paint( juce::Graphics& g )
{
	g.fillAll( getLookAndFeel().findColour( juce::ResizableWindow::backgroundColourId ) );
}

void MainComponent::resized()
{
	auto bounds = getLocalBounds();
	ltcLabel.setBounds( bounds.removeFromTop( 200 ) );
	auto channelStrip = bounds.removeFromTop( audioPrefsComponent->getItemHeight() );
	channelLabel.setBounds( channelStrip.removeFromLeft( 280 ) );
	channelComboBox.setBounds( channelStrip.withWidth ( 480 ) ) ;
	audioPrefsComponent->setBounds( bounds );
}

void MainComponent::newFrameReceived()
{
	const MessageManagerLock mmLock;
	ltcLabel.setText( ltcReader.getCurrentFrameAsString(), dontSendNotification);
}
