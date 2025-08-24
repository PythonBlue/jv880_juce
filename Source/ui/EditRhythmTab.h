/*
  ==============================================================================

    EditRhythmTab.h
    Created: 20 Aug 2024 2:34:06pm
    Author:  Giulio Zausa

  ==============================================================================
*/

#pragma once

#include "../PluginProcessor.h"
#include <JuceHeader.h>

class VirtualJVEditor;

//==============================================================================

class EditRhythmTab : public juce::Component,
                    public juce::Slider::Listener,
                    public juce::Button::Listener,
                    public juce::ComboBox::Listener
{
public:
    EditRhythmTab(VirtualJVProcessor &, VirtualJVEditor *);
    ~EditRhythmTab() override;

    void resized() override;
    void sliderValueChanged(juce::Slider *) override;
    void buttonClicked(juce::Button *) override;
    void buttonStateChanged(juce::Button*) override {}
    void comboBoxChanged(juce::ComboBox *) override;

    void updateValues();

    void sendSysexPatchRhythmChange1Byte(uint8_t address, uint8_t value);
    void sendSysexPatchRhythmChange2Byte(uint8_t address, uint8_t value);
    void sendSysexPatchRhythmChange();

private:
    void addMenuEntriesFromArray(juce::ComboBox &menu, const std::vector<std::string> &array);
    void updateWaveformComboBox(juce::ComboBox &wfMenu);

    VirtualJVProcessor &processor;
    VirtualJVEditor *editor;
    juce::Slider toneSlider;
    juce::Label toneLabel;
    uint8_t toneCount;

    juce::ComboBox waveGroupComboBox;
    juce::Label waveGroupLabel;
    juce::ComboBox waveformComboBox;
    juce::Label waveformLabel;
    juce::ToggleButton toneSwitchToggle;
    juce::Label toneSwitchLabel;

    juce::Slider muteSlider;
    juce::Label muteLabel;
    juce::ComboBox envModeComboBox;
    juce::Label envModeLabel;
    juce::Slider bendRangeSlider;
    juce::Label bendRangeLabel;

    juce::Slider pitchCoarseSlider;
    juce::Label pitchCoarseLabel;
    juce::Slider pitchFineSlider;
    juce::Label pitchFineLabel;
    juce::ComboBox pitchRandomComboBox;
    juce::Label pitchRandomLabel;
    juce::Slider penvLevSensSlider;
    juce::Label penvLevSensLabel;
    juce::ComboBox penvTimeSensComboBox;
    juce::Label penvTimeSensLabel;
    juce::Slider penvDepthSlider;
    juce::Label penvDepthLabel;
    juce::Slider penv1TimeSlider;
    juce::Label penv1TimeLabel;
    juce::Slider penv1LevelSlider;
    juce::Label penv1LevelLabel;
    juce::Slider penv2TimeSlider;
    juce::Label penv2TimeLabel;
    juce::Slider penv2LevelSlider;
    juce::Label penv2LevelLabel;
    juce::Slider penv3TimeSlider;
    juce::Label penv3TimeLabel;
    juce::Slider penv3LevelSlider;
    juce::Label penv3LevelLabel;
    juce::Slider penv4TimeSlider;
    juce::Label penv4TimeLabel;
    juce::Slider penv4LevelSlider;
    juce::Label penv4LevelLabel;

    juce::ComboBox filterModeComboBox;
    juce::Label filterModeLabel;
    juce::Slider filterCutoffSlider;
    juce::Label filterCutoffLabel;
    juce::Slider filterResoSlider;
    juce::Label filterResoLabel;
    juce::ComboBox filterResoModeComboBox;
    juce::Slider fenvLevSensSlider;
    juce::Label fenvLevSensLabel;
    juce::ComboBox fenvTimeSensComboBox;
    juce::Label fenvTimeSensLabel;
    juce::Slider fenvDepthSlider;
    juce::Label fenvDepthLabel;
    juce::Slider fenv1TimeSlider;
    juce::Label fenv1TimeLabel;
    juce::Slider fenv1LevelSlider;
    juce::Label fenv1LevelLabel;
    juce::Slider fenv2TimeSlider;
    juce::Label fenv2TimeLabel;
    juce::Slider fenv2LevelSlider;
    juce::Label fenv2LevelLabel;
    juce::Slider fenv3TimeSlider;
    juce::Label fenv3TimeLabel;
    juce::Slider fenv3LevelSlider;
    juce::Label fenv3LevelLabel;
    juce::Slider fenv4TimeSlider;
    juce::Label fenv4TimeLabel;
    juce::Slider fenv4LevelSlider;
    juce::Label fenv4LevelLabel;

    juce::Slider levelSlider;
    juce::Label levelLabel;
    juce::Slider panSlider;
    juce::Label panLabel;
    juce::Slider aenvLevSensSlider;
    juce::Label aenvLevSensLabel;
    juce::ComboBox aenvTimeSensComboBox;
    juce::Label aenvTimeSensLabel;
    juce::Slider aenv1TimeSlider;
    juce::Label aenv1TimeLabel;
    juce::Slider aenv1LevelSlider;
    juce::Label aenv1LevelLabel;
    juce::Slider aenv2TimeSlider;
    juce::Label aenv2TimeLabel;
    juce::Slider aenv2LevelSlider;
    juce::Label aenv2LevelLabel;
    juce::Slider aenv3TimeSlider;
    juce::Label aenv3TimeLabel;
    juce::Slider aenv3LevelSlider;
    juce::Label aenv3LevelLabel;
    juce::Slider aenv4TimeSlider;
    juce::Label aenv4TimeLabel;

    juce::Slider drySlider;
    juce::Label dryLabel;
    juce::Slider reverbSlider;
    juce::Label reverbLabel;
    juce::Slider chorusSlider;
    juce::Label chorusLabel;
    juce::ComboBox outputComboBox;
    juce::Label outputLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditRhythmTab)
};
