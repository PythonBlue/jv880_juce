/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
Jv880_juceAudioProcessorEditor::Jv880_juceAudioProcessorEditor(
    Jv880_juceAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p), lcd(p),
      tabs(juce::TabbedButtonBar::TabsAtTop), patchBrowser(p), editTab(p),
      settingsTab(p) {
  addAndMakeVisible(lcd);
  addAndMakeVisible(tabs);
  setSize(820, 1000);

  tabs.addTab(
      "Browse",
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId),
      &patchBrowser, false);
  tabs.addTab(
      "Edit",
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId),
      &editTab, false);
  tabs.addTab(
      "Settings",
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId),
      &settingsTab, false);

  if (!audioProcessor.loaded) {
    juce::AlertWindow::showAsync(
        juce::MessageBoxOptions()
            .withIconType(juce::MessageBoxIconType::WarningIcon)
            .withTitle("Error")
            .withMessage("Cannot load ROMs. Please copy the rom files to the "
                         "ROM folder and restart the plugin to continue.")
            .withButton("Open ROMs Folder")
            .withAssociatedComponent(this)
            .withParentComponent(this),
        [](int param) {
          juce::File romsDir(juce::File::getSpecialLocation(
                                 juce::File::userApplicationDataDirectory)
                                 .getChildFile("JV880"));
          if (romsDir.exists()) {
            juce::Process::openDocument(romsDir.getFullPathName(), "");
          }
        });
  }
}

Jv880_juceAudioProcessorEditor::~Jv880_juceAudioProcessorEditor() {}

void Jv880_juceAudioProcessorEditor::resized() {
  lcd.setBounds(0, 0, 820, 100);
  tabs.setBounds(0, 100, 820, 900);
}
