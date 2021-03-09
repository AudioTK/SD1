/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ATKSD1AudioProcessor::ATKSD1AudioProcessor()
  :
#ifndef JucePlugin_PreferredChannelConfigurations
  AudioProcessor(BusesProperties()
#  if !JucePlugin_IsMidiEffect
#    if !JucePlugin_IsSynth
                     .withInput("Input", AudioChannelSet::stereo(), true)
#    endif
                     .withOutput("Output", AudioChannelSet::stereo(), true)
#  endif
          )
  ,
#endif
  inFilter(nullptr, 1, 0, false)
  , overdriveFilter(ATK::Triode2Filter<double,
        ATK::DempwolfTriodeFunction<double>>::build_standard_filter(100e3, 1e6, 1e6, 820, 300, 22e-9, 250e-6))
  , toneFilter(ATK::ToneStackCoefficients<double>::buildBassmanStack())
  , outFilter(nullptr, 1, 0, false)
  , parameters(*this,
        nullptr,
        juce::Identifier("ATKSD1"),
        {std::make_unique<juce::AudioParameterFloat>("drive", "Drive", NormalisableRange<float>(0, 100), 0, "%"),
            std::make_unique<juce::AudioParameterFloat>("tone", "Tone", NormalisableRange<float>(-100, 100), 0, "%"),
            std::make_unique<juce::AudioParameterFloat>("level", "Level", NormalisableRange<float>(100, 100), 0, "%")})
{
  oversamplingFilter.set_input_port(0, &inFilter, 0);
  overdriveFilter.set_input_port(0, &oversamplingFilter, 0);
  lowpassFilter.set_input_port(0, &overdriveFilter, 0);
  decimationFilter.set_input_port(0, &lowpassFilter, 0);
  toneFilter.set_input_port(0, &decimationFilter, 0);
  highpassFilter.set_input_port(0, &toneFilter, 0);
  volumeFilter.set_input_port(0, &highpassFilter, 0);
  outFilter.set_input_port(0, &volumeFilter, 0);

  lowpassFilter.set_cut_frequency(20000);
  lowpassFilter.set_order(6);
  highpassFilter.select(2);
  highpassFilter.set_cut_frequency(20);
  highpassFilter.set_attenuation(1);

  old_drive = *parameters.getRawParameterValue("drive");
  old_tone = *parameters.getRawParameterValue("tone");
  old_level = *parameters.getRawParameterValue("level");
}

ATKSD1AudioProcessor::~ATKSD1AudioProcessor()
{
}

//==============================================================================
const String ATKSD1AudioProcessor::getName() const
{
  return JucePlugin_Name;
}

bool ATKSD1AudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool ATKSD1AudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

double ATKSD1AudioProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int ATKSD1AudioProcessor::getNumPrograms()
{
  return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
            // so this should be at least 1, even if you're not really implementing programs.
}

int ATKSD1AudioProcessor::getCurrentProgram()
{
  return 0;
}

void ATKSD1AudioProcessor::setCurrentProgram(int index)
{
}

const String ATKSD1AudioProcessor::getProgramName(int index)
{
  return {};
}

void ATKSD1AudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void ATKSD1AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  auto intsamplerate = std::lround(sampleRate);

  inFilter.set_input_sampling_rate(intsamplerate);
  inFilter.set_output_sampling_rate(intsamplerate);
  oversamplingFilter.set_input_sampling_rate(intsamplerate);
  oversamplingFilter.set_output_sampling_rate(intsamplerate * OVERSAMPLING);
  lowpassFilter.set_input_sampling_rate(intsamplerate * OVERSAMPLING);
  lowpassFilter.set_output_sampling_rate(intsamplerate * OVERSAMPLING);
  overdriveFilter.set_input_sampling_rate(intsamplerate * OVERSAMPLING);
  overdriveFilter.set_output_sampling_rate(intsamplerate * OVERSAMPLING);
  decimationFilter.set_input_sampling_rate(intsamplerate * OVERSAMPLING);
  decimationFilter.set_output_sampling_rate(intsamplerate);
  toneFilter.set_input_sampling_rate(intsamplerate);
  toneFilter.set_output_sampling_rate(intsamplerate);
  highpassFilter.set_input_sampling_rate(intsamplerate);
  highpassFilter.set_output_sampling_rate(intsamplerate);
  volumeFilter.set_input_sampling_rate(intsamplerate);
  volumeFilter.set_output_sampling_rate(intsamplerate);
  outFilter.set_input_sampling_rate(intsamplerate);
  outFilter.set_output_sampling_rate(intsamplerate);
}

void ATKSD1AudioProcessor::releaseResources()
{
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ATKSD1AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#  if JucePlugin_IsMidiEffect
  ignoreUnused(layouts);
  return true;
#  else
  // This is the place where you check if the layout is supported.
  if(layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#    if !JucePlugin_IsSynth
  if(layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#    endif

  return true;
#  endif
}
#endif

void ATKSD1AudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
  if(*parameters.getRawParameterValue("drive") != old_drive)
  {
    old_drive = *parameters.getRawParameterValue("drive");
    overdriveFilter.set_drive(old_drive / 100.);
  }
  if(*parameters.getRawParameterValue("tone") != old_tone)
  {
    old_tone = *parameters.getRawParameterValue("tone");
    toneFilter.set_tone((old_tone + 100) / 200.);
  }
  if(*parameters.getRawParameterValue("level") != old_level)
  {
    old_level = *parameters.getRawParameterValue("level");
    volumeFilter.set_volume(old_level / 150.)
  }

  const int totalNumInputChannels = getTotalNumInputChannels();
  const int totalNumOutputChannels = getTotalNumOutputChannels();

  assert(totalNumInputChannels == totalNumOutputChannels);
  assert(totalNumOutputChannels == 1);

  inFilter.set_pointer(buffer.getReadPointer(0), buffer.getNumSamples());
  outFilter.set_pointer(buffer.getWritePointer(0), buffer.getNumSamples());

  outFilter.process(buffer.getNumSamples());
}

//==============================================================================
bool ATKSD1AudioProcessor::hasEditor() const
{
  return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ATKSD1AudioProcessor::createEditor()
{
  return new ATKSD1AudioProcessorEditor(*this, parameters);
}

//==============================================================================
void ATKSD1AudioProcessor::getStateInformation(MemoryBlock& destData)
{
  MemoryOutputStream store(destData, true);
  store.writeInt(0); // version ID
  store.writeFloat(*parameters.getRawParameterValue("gain"));
  store.writeFloat(*parameters.getRawParameterValue("bass"));
  store.writeFloat(*parameters.getRawParameterValue("medium"));
  store.writeFloat(*parameters.getRawParameterValue("high"));
  store.writeFloat(*parameters.getRawParameterValue("volume"));
  store.writeFloat(*parameters.getRawParameterValue("drywet"));
}

void ATKSD1AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
  MemoryInputStream store(data, static_cast<size_t>(sizeInBytes), false);
  int version = store.readInt(); // version ID
  *parameters.getRawParameterValue("gain") = store.readFloat();
  *parameters.getRawParameterValue("bass") = store.readFloat();
  *parameters.getRawParameterValue("medium") = store.readFloat();
  *parameters.getRawParameterValue("high") = store.readFloat();
  *parameters.getRawParameterValue("volume") = store.readFloat();
  *parameters.getRawParameterValue("drywet") = store.readFloat();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
  return new ATKSD1AudioProcessor();
}