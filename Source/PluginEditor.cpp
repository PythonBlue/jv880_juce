/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "rom.h"
#include <algorithm>

//==============================================================================
VirtualJVEditor::VirtualJVEditor(
    VirtualJVProcessor &p)
    : AudioProcessorEditor(&p), processor(p),
      lcd(p), tabs(juce::TabbedButtonBar::TabsAtTop), patchBrowser(p), editCommonTab(p),
      editTone1Tab(p, this, 0U), editTone2Tab(p, this, 1U), editTone3Tab(p, this, 2U), editTone4Tab(p, this, 3U), editRhythmTab(p, this),
      settingsTab(p)
{
  addAndMakeVisible(lcd);
  addAndMakeVisible(tabs);

  setSize(820, 900);

  const auto bgColor = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);

  tabs.addTab("Browse", bgColor, &patchBrowser, false);
  tabs.addTab("Common", bgColor, &editCommonTab, false);
  tabs.addTab("Tone 1", bgColor, &editTone1Tab, false);
  tabs.addTab("Tone 2", bgColor, &editTone2Tab, false);
  tabs.addTab("Tone 3", bgColor, &editTone3Tab, false);
  tabs.addTab("Tone 4", bgColor, &editTone4Tab, false);
  tabs.addTab("Rhythm", bgColor, &editRhythmTab, false);
  tabs.addTab("Settings", bgColor, &settingsTab, false);

  if (!processor.loaded) {
    juce::AlertWindow::showAsync(
        juce::MessageBoxOptions()
            .withIconType(juce::MessageBoxIconType::WarningIcon)
            .withTitle("Error")
            .withMessage("Cannot load ROMs. Please copy the ROM files to the "
                         "ROM folder and restart the plugin to continue.")
            .withButton("Open ROMs Folder")
            .withAssociatedComponent(this)
            .withParentComponent(this),
        [](int /* param */) {
          juce::File romsDir(juce::File::getSpecialLocation(
                                 juce::File::userApplicationDataDirectory)
                                 .getChildFile("JV880"));
          if (romsDir.exists()) {
            juce::Process::openDocument(romsDir.getFullPathName(), "");
          }
        });
  }
  else
  {
      updateEditTabs();
  }
}

VirtualJVEditor::~VirtualJVEditor() {}

void VirtualJVEditor::updateEditTabs()
{
    editCommonTab.updateValues();
    editTone1Tab.updateValues();
    editTone2Tab.updateValues();
    editTone3Tab.updateValues();
    editTone4Tab.updateValues();
    editRhythmTab.updateValues();
    settingsTab.updateValues();
}

uint8_t VirtualJVEditor::getSelectedRomIdx()
{
    auto idx = patchBrowser.categoriesListBox.getSelectedRow();

    if (idx <= 0)
    {
        return 2; // internal ROM 2 of the 880, contains multisample info table
    }
    else
    {
        return std::min(romCountRequired + idx, romCount - 1); // RD expansion ROM and other SR-JV ROMs henceforth
    }
}

void VirtualJVEditor::resized() {
  lcd.setBounds(0, 0, 820, 100);
  tabs.setBounds(0, 100, 820, 800);
}
