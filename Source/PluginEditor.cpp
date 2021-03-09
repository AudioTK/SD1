/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "BinaryData.h"

//==============================================================================
ATKSD1AudioProcessorEditor::ATKSD1AudioProcessorEditor(ATKSD1AudioProcessor& p,
    AudioProcessorValueTreeState& paramState)
  : AudioProcessorEditor(&p)
  , processor(p)
  , paramState(paramState)
  , drive(paramState, "drive", "Drive")
  , tone(paramState, "tone", "Tone")
  , level(paramState, "level", "Level")

{
  addAndMakeVisible(drive);
  addAndMakeVisible(tone);
  addAndMakeVisible(level);

  bckgndImage = juce::ImageFileFormat::loadFrom(BinaryData::Background_png, BinaryData::Background_pngSize);
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(400, 200);
}

ATKSD1AudioProcessorEditor::~ATKSD1AudioProcessorEditor() = default;

void ATKSD1AudioProcessorEditor::paint(Graphics& g)
{
  g.drawImageAt(bckgndImage, 0, 0);
}

void ATKSD1AudioProcessorEditor::resized()
{
  drive.setBoundsRelative(0, 1. / 4, 1. / 6, 3. / 4);
  tone.setBoundsRelative(1. / 6, 1. / 4, 3. / 6, 3. / 4);
  level.setBoundsRelative(4. / 6, 1. / 4, 1. / 6, 3. / 4);
}
