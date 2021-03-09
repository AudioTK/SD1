/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

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

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(900, 200);
}

ATKSD1AudioProcessorEditor::~ATKSD1AudioProcessorEditor()
{
}

void ATKSD1AudioProcessorEditor::paint(Graphics& g)
{
  // (Our component is opaque, so we must completely fill the background with a solid colour)
  g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
  g.setFont(Font("Times New Roman", 30.0f, Font::bold | Font::italic));
  g.setColour(Colours::whitesmoke);
  g.drawText("Bass Preamp", 20, 10, 200, 30, Justification::verticallyCentred);
}

void ATKSD1AudioProcessorEditor::resized()
{
  drive.setBoundsRelative(0, 1. / 4, 1. / 6, 3. / 4);
  tone.setBoundsRelative(1. / 6, 1. / 4, 3. / 6, 3. / 4);
  level.setBoundsRelative(4. / 6, 1. / 4, 1. / 6, 3. / 4);
}
