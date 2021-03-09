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
  , bigKnob(juce::ImageFileFormat::loadFrom(BinaryData::BigKnob_png, BinaryData::BigKnob_pngSize), 120, 120, 131)
  , smallKnob(juce::ImageFileFormat::loadFrom(BinaryData::SmallKnob_png, BinaryData::SmallKnob_pngSize), 100, 100, 131)
  , drive(paramState, "drive", "Drive", &bigKnob)
  , tone(paramState, "tone", "Tone", &smallKnob)
  , level(paramState, "level", "Level", &bigKnob)

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
  drive.setBounds(250, 20, 120, 120);
  tone.setBounds(150, 70, 100, 100);
  level.setBounds(30, 20, 120, 120);
}
