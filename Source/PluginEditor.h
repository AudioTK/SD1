/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"

#include <ATKJUCEComponents/JUCE/Slider.h>

//==============================================================================
/**
 */
class ATKSD1AudioProcessorEditor: public AudioProcessorEditor
{
public:
  ATKSD1AudioProcessorEditor(ATKSD1AudioProcessor& p, AudioProcessorValueTreeState& paramState);
  ~ATKSD1AudioProcessorEditor();

  //==============================================================================
  void paint(Graphics&) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  ATKSD1AudioProcessor& processor;
  AudioProcessorValueTreeState& paramState;

  ATK::juce::ImageLookAndFeel bigKnob;
  ATK::juce::ImageLookAndFeel smallKnob;

  ATK::juce::SliderComponent drive;
  ATK::juce::SliderComponent tone;
  ATK::juce::SliderComponent level;

  juce::Image bckgndImage;
};
