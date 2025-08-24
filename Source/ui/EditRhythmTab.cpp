/*
  ==============================================================================

    EditRhythmTab.cpp
    Created: 20 Aug 2024 2:34:06pm
    Author:  Giulio Zausa

  ==============================================================================
*/

#include <JuceHeader.h>
#include <algorithm>
#include "EditRhythmTab.h"
#include "../PluginEditor.h"
#include "../dataStructures.h"

//==============================================================================
EditRhythmTab::EditRhythmTab
    ( VirtualJVProcessor &p, VirtualJVEditor *e)
    : processor(p), editor(e)
{
    toneCount = 0;
    
    addAndMakeVisible(toneSlider);
    toneSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    toneSlider.setRange(36, 96, 1);
    toneSlider.setValue(36);
    toneSlider.addListener(this);
    addAndMakeVisible(toneLabel);
    toneLabel.setText("Tone Key", juce::dontSendNotification);
    toneLabel.attachToComponent(&toneSlider, true);
    
    
    addAndMakeVisible(waveGroupLabel);
    waveGroupLabel.setText("Wave Group", juce::dontSendNotification);
    waveGroupLabel.attachToComponent(&waveGroupComboBox, true);
    addAndMakeVisible(waveGroupComboBox);
    waveGroupComboBox.setScrollWheelEnabled(true);
    waveGroupComboBox.addListener(this);
    waveGroupComboBox.addItem("Internal", 1);
    waveGroupComboBox.addItem("Expansion", 2);
    waveGroupComboBox.setSelectedItemIndex(0);

    addAndMakeVisible(waveformComboBox);
    waveformComboBox.setScrollWheelEnabled(true);
    waveformComboBox.addListener(this);

    //updateWaveformComboBox(waveformComboBox);

    addAndMakeVisible(waveformLabel);
    waveformLabel.setText("Waveform", juce::dontSendNotification);
    waveformLabel.attachToComponent(&waveformComboBox, true);

    addAndMakeVisible(toneSwitchToggle);
    toneSwitchToggle.addListener(this);
    toneSwitchToggle.setButtonText("Enable");
    
    addAndMakeVisible(envModeComboBox);
    envModeComboBox.setScrollWheelEnabled(true);
    envModeComboBox.addListener(this);
    envModeComboBox.addItem("No Sus", 1);
    envModeComboBox.addItem("Sustain", 2);
    
    addAndMakeVisible(muteSlider);
    muteSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    muteSlider.setRange(0, 31, 1);
    muteSlider.addListener(this);
    addAndMakeVisible(muteLabel);
    muteLabel.setText("Mute/Env", juce::dontSendNotification);
    muteLabel.attachToComponent(&muteSlider, true);
    
    addAndMakeVisible(pitchCoarseSlider);
    pitchCoarseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    pitchCoarseSlider.setRange(-48, 48, 1);
    pitchCoarseSlider.addListener(this);
    addAndMakeVisible(pitchCoarseLabel);
    pitchCoarseLabel.setText("Coarse", juce::dontSendNotification);
    pitchCoarseLabel.attachToComponent(&pitchCoarseSlider, true);

    addAndMakeVisible(pitchFineSlider);
    pitchFineSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    pitchFineSlider.setRange(-50, 50, 1);
    pitchFineSlider.addListener(this);
    addAndMakeVisible(pitchFineLabel);
    pitchFineLabel.setText("Fine", juce::dontSendNotification);
    pitchFineLabel.attachToComponent(&pitchFineSlider, true);

    const std::vector<std::string> pitchRandoms
    {
          "0",   "5",  "10",   "20",
         "30",  "40",  "50",   "70",
        "100", "200", "300",  "400",
        "500", "600", "800", "1200",
    };

    const std::vector<std::string> np100
    {
        "-100",  "-70",  "-50",  "-40",
         "-30",  "-20",  "-10",    "0",
         "+10",  "+20",  "+30",  "+40",
         "+50",  "+70", "+100"
    };

    addAndMakeVisible(pitchRandomComboBox);
    pitchRandomComboBox.addListener(this);
    pitchRandomComboBox.setScrollWheelEnabled(true);
    addMenuEntriesFromArray(pitchRandomComboBox, pitchRandoms);
    addAndMakeVisible(pitchRandomLabel);
    pitchRandomLabel.setText("Random | KF", juce::dontSendNotification);
    pitchRandomLabel.attachToComponent(&pitchRandomComboBox, true);
    
    addAndMakeVisible(bendRangeSlider);
    bendRangeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    bendRangeSlider.setRange(0, 12, 1);
    bendRangeSlider.addListener(this);
    addAndMakeVisible(bendRangeLabel);
    bendRangeLabel.setText("Bend Range", juce::dontSendNotification);
    bendRangeLabel.attachToComponent(&bendRangeSlider, true);

    addAndMakeVisible(penvLevSensSlider);
    penvLevSensSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    penvLevSensSlider.setRange(-12, 12, 1);
    penvLevSensSlider.addListener(this);
    addAndMakeVisible(penvLevSensLabel);
    penvLevSensLabel.setText("Pitch Sens", juce::dontSendNotification);
    penvLevSensLabel.attachToComponent(&penvLevSensSlider, true);

    addAndMakeVisible(penvTimeSensComboBox);
    penvTimeSensComboBox.addListener(this);
    penvTimeSensComboBox.setScrollWheelEnabled(true);
    addMenuEntriesFromArray(penvTimeSensComboBox, np100);
    addAndMakeVisible(penvTimeSensLabel);
    penvTimeSensLabel.setText("Time Sens", juce::dontSendNotification);
    penvTimeSensLabel.attachToComponent(&penvTimeSensComboBox, true);

    addAndMakeVisible(penvDepthSlider);
    penvDepthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    penvDepthSlider.setRange(-12, 12, 1);
    penvDepthSlider.addListener(this);
    addAndMakeVisible(penvDepthLabel);
    penvDepthLabel.setText("Pitch Depth", juce::dontSendNotification);
    penvDepthLabel.attachToComponent(&penvDepthSlider, true);

    addAndMakeVisible(penv1TimeSlider);
    penv1TimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    penv1TimeSlider.setRange(0, 127, 1);
    penv1TimeSlider.addListener(this);
    addAndMakeVisible(penv1TimeLabel);
    penv1TimeLabel.setText("Time | Level 1", juce::dontSendNotification);
    penv1TimeLabel.attachToComponent(&penv1TimeSlider, true);
    addAndMakeVisible(penv1LevelSlider);
    penv1LevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    penv1LevelSlider.setRange(-63, 63, 1);
    penv1LevelSlider.addListener(this);

    addAndMakeVisible(penv2TimeSlider);
    penv2TimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    penv2TimeSlider.setRange(0, 127, 1);
    penv2TimeSlider.addListener(this);
    addAndMakeVisible(penv2TimeLabel);
    penv2TimeLabel.setText("2", juce::dontSendNotification);
    penv2TimeLabel.attachToComponent(&penv2TimeSlider, true);
    addAndMakeVisible(penv2LevelSlider);
    penv2LevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    penv2LevelSlider.setRange(-63, 63, 1);
    penv2LevelSlider.addListener(this);

    addAndMakeVisible(penv3TimeSlider);
    penv3TimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    penv3TimeSlider.setRange(0, 127, 1);
    penv3TimeSlider.addListener(this);
    addAndMakeVisible(penv3TimeLabel);
    penv3TimeLabel.setText("3", juce::dontSendNotification);
    penv3TimeLabel.attachToComponent(&penv3TimeSlider, true);
    addAndMakeVisible(penv3LevelSlider);
    penv3LevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    penv3LevelSlider.setRange(-63, 63, 1);
    penv3LevelSlider.addListener(this);

    addAndMakeVisible(penv4TimeSlider);
    penv4TimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    penv4TimeSlider.setRange(0, 127, 1);
    penv4TimeSlider.addListener(this);
    addAndMakeVisible(penv4TimeLabel);
    penv4TimeLabel.setText("4", juce::dontSendNotification);
    penv4TimeLabel.attachToComponent(&penv4TimeSlider, true);
    addAndMakeVisible(penv4LevelSlider);
    penv4LevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    penv4LevelSlider.setRange(-63, 63, 1);
    penv4LevelSlider.addListener(this);

    addAndMakeVisible(filterModeComboBox);
    filterModeComboBox.addListener(this);
    filterModeComboBox.setScrollWheelEnabled(true);
    filterModeComboBox.addItem("Off", 1);
    filterModeComboBox.addItem("Lowpass", 2);
    filterModeComboBox.addItem("Highpass", 3);
    addAndMakeVisible(filterModeLabel);
    filterModeLabel.setText("Filter Mode", juce::dontSendNotification);
    filterModeLabel.attachToComponent(&filterModeComboBox, true);

    addAndMakeVisible(filterCutoffSlider);
    filterCutoffSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    filterCutoffSlider.setRange(0, 127, 1);
    filterCutoffSlider.addListener(this);
    addAndMakeVisible(filterCutoffLabel);
    filterCutoffLabel.setText("Cutoff", juce::dontSendNotification);
    filterCutoffLabel.attachToComponent(&filterCutoffSlider, true);

    addAndMakeVisible(filterResoSlider);
    filterResoSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    filterResoSlider.setRange(0, 127, 1);
    filterResoSlider.addListener(this);
    addAndMakeVisible(filterResoLabel);
    filterResoLabel.setText("Reso | Mode", juce::dontSendNotification);
    filterResoLabel.attachToComponent(&filterResoSlider, true);

    addAndMakeVisible(filterResoModeComboBox);
    filterResoModeComboBox.addListener(this);
    filterResoModeComboBox.setScrollWheelEnabled(true);
    filterResoModeComboBox.addItem("Soft", 1);
    filterResoModeComboBox.addItem("Hard", 2);

    addAndMakeVisible(fenvLevSensSlider);
    fenvLevSensSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    fenvLevSensSlider.setRange(-63, 63, 1);
    fenvLevSensSlider.addListener(this);
    addAndMakeVisible(fenvLevSensLabel);
    fenvLevSensLabel.setText("TVF Sens", juce::dontSendNotification);
    fenvLevSensLabel.attachToComponent(&fenvLevSensSlider, true);

    addAndMakeVisible(fenvTimeSensComboBox);
    fenvTimeSensComboBox.addListener(this);
    fenvTimeSensComboBox.setScrollWheelEnabled(true);
    addMenuEntriesFromArray(fenvTimeSensComboBox, np100);
    addAndMakeVisible(fenvTimeSensLabel);
    fenvTimeSensLabel.setText("Time Sens", juce::dontSendNotification);
    fenvTimeSensLabel.attachToComponent(&fenvTimeSensComboBox, true);

    addAndMakeVisible(fenvDepthSlider);
    fenvDepthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    fenvDepthSlider.setRange(-63, 63, 1);
    fenvDepthSlider.addListener(this);
    addAndMakeVisible(fenvDepthLabel);
    fenvDepthLabel.setText("TVF Depth", juce::dontSendNotification);
    fenvDepthLabel.attachToComponent(&fenvDepthSlider, true);

    addAndMakeVisible(fenv1TimeSlider);
    fenv1TimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    fenv1TimeSlider.setRange(0, 127, 1);
    fenv1TimeSlider.addListener(this);
    addAndMakeVisible(fenv1TimeLabel);
    fenv1TimeLabel.setText("Time | Level 1", juce::dontSendNotification);
    fenv1TimeLabel.attachToComponent(&fenv1TimeSlider, true);
    addAndMakeVisible(fenv1LevelSlider);
    fenv1LevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    fenv1LevelSlider.setRange(0, 127, 1);
    fenv1LevelSlider.addListener(this);

    addAndMakeVisible(fenv2TimeSlider);
    fenv2TimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    fenv2TimeSlider.setRange(0, 127, 1);
    fenv2TimeSlider.addListener(this);
    addAndMakeVisible(fenv2TimeLabel);
    fenv2TimeLabel.setText("2", juce::dontSendNotification);
    fenv2TimeLabel.attachToComponent(&fenv2TimeSlider, true);
    addAndMakeVisible(fenv2LevelSlider);
    fenv2LevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    fenv2LevelSlider.setRange(0, 127, 1);
    fenv2LevelSlider.addListener(this);

    addAndMakeVisible(fenv3TimeSlider);
    fenv3TimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    fenv3TimeSlider.setRange(0, 127, 1);
    fenv3TimeSlider.addListener(this);
    addAndMakeVisible(fenv3TimeLabel);
    fenv3TimeLabel.setText("3", juce::dontSendNotification);
    fenv3TimeLabel.attachToComponent(&fenv3TimeSlider, true);
    addAndMakeVisible(fenv3LevelSlider);
    fenv3LevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    fenv3LevelSlider.setRange(0, 127, 1);
    fenv3LevelSlider.addListener(this);

    addAndMakeVisible(fenv4TimeSlider);
    fenv4TimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    fenv4TimeSlider.setRange(0, 127, 1);
    fenv4TimeSlider.addListener(this);
    addAndMakeVisible(fenv4TimeLabel);
    fenv4TimeLabel.setText("4", juce::dontSendNotification);
    fenv4TimeLabel.attachToComponent(&fenv4TimeSlider, true);
    addAndMakeVisible(fenv4LevelSlider);
    fenv4LevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    fenv4LevelSlider.setRange(0, 127, 1);
    fenv4LevelSlider.addListener(this);

    addAndMakeVisible(levelSlider);
    levelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    levelSlider.setRange(0, 127, 1);
    levelSlider.addListener(this);
    addAndMakeVisible(levelLabel);
    levelLabel.setText("Level", juce::dontSendNotification);
    levelLabel.attachToComponent(&levelSlider, true);

    addAndMakeVisible(panSlider);
    panSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    panSlider.setRange(-64, 64, 1);
    panSlider.addListener(this);
    addAndMakeVisible(panLabel);
    panLabel.setText("Pan", juce::dontSendNotification);
    panLabel.attachToComponent(&panSlider, true);

    addAndMakeVisible(aenvLevSensSlider);
    aenvLevSensSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    aenvLevSensSlider.setRange(-63, 63, 1);
    aenvLevSensSlider.addListener(this);
    addAndMakeVisible(aenvLevSensLabel);
    aenvLevSensLabel.setText("TVF Sens", juce::dontSendNotification);
    aenvLevSensLabel.attachToComponent(&aenvLevSensSlider, true);

    addAndMakeVisible(aenvTimeSensComboBox);
    aenvTimeSensComboBox.addListener(this);
    aenvTimeSensComboBox.setScrollWheelEnabled(true);
    addMenuEntriesFromArray(aenvTimeSensComboBox, np100);
    addAndMakeVisible(aenvTimeSensLabel);
    aenvTimeSensLabel.setText("Time Sens", juce::dontSendNotification);
    aenvTimeSensLabel.attachToComponent(&aenvTimeSensComboBox, true);

    addAndMakeVisible(aenv1TimeSlider);
    aenv1TimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    aenv1TimeSlider.setRange(0, 127, 1);
    aenv1TimeSlider.addListener(this);
    addAndMakeVisible(aenv1TimeLabel);
    aenv1TimeLabel.setText("Time | Level 1", juce::dontSendNotification);
    aenv1TimeLabel.attachToComponent(&aenv1TimeSlider, true);
    addAndMakeVisible(aenv1LevelSlider);
    aenv1LevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    aenv1LevelSlider.setRange(0, 127, 1);
    aenv1LevelSlider.addListener(this);

    addAndMakeVisible(aenv2TimeSlider);
    aenv2TimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    aenv2TimeSlider.setRange(0, 127, 1);
    aenv2TimeSlider.addListener(this);
    addAndMakeVisible(aenv2TimeLabel);
    aenv2TimeLabel.setText("2", juce::dontSendNotification);
    aenv2TimeLabel.attachToComponent(&aenv2TimeSlider, true);
    addAndMakeVisible(aenv2LevelSlider);
    aenv2LevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    aenv2LevelSlider.setRange(0, 127, 1);
    aenv2LevelSlider.addListener(this);

    addAndMakeVisible(aenv3TimeSlider);
    aenv3TimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    aenv3TimeSlider.setRange(0, 127, 1);
    aenv3TimeSlider.addListener(this);
    addAndMakeVisible(aenv3TimeLabel);
    aenv3TimeLabel.setText("3", juce::dontSendNotification);
    aenv3TimeLabel.attachToComponent(&aenv3TimeSlider, true);
    addAndMakeVisible(aenv3LevelSlider);
    aenv3LevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    aenv3LevelSlider.setRange(0, 127, 1);
    aenv3LevelSlider.addListener(this);

    addAndMakeVisible(aenv4TimeSlider);
    aenv4TimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    aenv4TimeSlider.setRange(0, 127, 1);
    aenv4TimeSlider.addListener(this);
    addAndMakeVisible(aenv4TimeLabel);
    aenv4TimeLabel.setText("4", juce::dontSendNotification);
    aenv4TimeLabel.attachToComponent(&aenv4TimeSlider, true);

    addAndMakeVisible(drySlider);
    drySlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    drySlider.setRange(0, 127, 1);
    drySlider.addListener(this);
    addAndMakeVisible(dryLabel);
    dryLabel.setText("Dry Send", juce::dontSendNotification);
    dryLabel.attachToComponent(&drySlider, true);

    addAndMakeVisible(reverbSlider);
    reverbSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    reverbSlider.setRange(0, 127, 1);
    reverbSlider.addListener(this);
    addAndMakeVisible(reverbLabel);
    reverbLabel.setText("Reverb", juce::dontSendNotification);
    reverbLabel.attachToComponent(&reverbSlider, true);

    addAndMakeVisible(chorusSlider);
    chorusSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    chorusSlider.setRange(0, 127, 1);
    chorusSlider.addListener(this);
    addAndMakeVisible(chorusLabel);
    chorusLabel.setText("Chorus", juce::dontSendNotification);
    chorusLabel.attachToComponent(&chorusSlider, true);

    addAndMakeVisible(outputComboBox);
    outputComboBox.addListener(this);
    outputComboBox.setScrollWheelEnabled(true);
    outputComboBox.addItem("Main", 1);
    outputComboBox.addItem("Sub", 2);
    addAndMakeVisible(outputLabel);
    outputLabel.setText("Output", juce::dontSendNotification);
    outputLabel.attachToComponent(&outputComboBox, true);

}

EditRhythmTab::~EditRhythmTab()
{
}

void EditRhythmTab::addMenuEntriesFromArray(juce::ComboBox &m, const std::vector<std::string> &array)
{
    uint32_t idx = 1;

    for (auto e : array)
    {
        m.addItem(e, idx);
        idx++;
    }
}

void EditRhythmTab::updateWaveformComboBox(juce::ComboBox &wfMenu)
{
    const int priorSelection = wfMenu.getSelectedItemIndex();

    wfMenu.clear(juce::dontSendNotification);

    if (!editor)
        return;

    const auto romIdx = waveGroupComboBox.getSelectedItemIndex() == 0 ? 2U : editor->getSelectedRomIdx();
    auto names = processor.readMultisampleNames(romIdx);

    for (int i = 0; i < names.size(); i++)
    {
        if (i % 32 == 0 && i > 0)
        {
            wfMenu.getRootMenu()->addColumnBreak();
        }

        wfMenu.addItem(std::string{std::to_string(i + 1) + ": " + names[i]}, i + 1);
    }

    if (priorSelection > -1)
    {
        const int newSelection = std::clamp(priorSelection, 0, wfMenu.getNumItems() - 1);

        wfMenu.setSelectedItemIndex(newSelection);
    }
}

void EditRhythmTab::updateValues()
{
    Rhythm* rhythm = (Rhythm*)processor.status.drums;
    RhythmTone tone = rhythm->tones[toneCount];
    waveGroupComboBox.setSelectedItemIndex((tone.flags & 0x3), juce::dontSendNotification);
    waveformComboBox.setSelectedItemIndex((tone.waveNumber & 0xff), juce::dontSendNotification);
    toneSwitchToggle.setToggleState(((tone.flags >> 7) & 0x01) ? 1 : 0, juce::dontSendNotification);

    updateWaveformComboBox(waveformComboBox);
    
    muteSlider.setValue(int8_t((tone.muteGroup) & 0x1f), juce::dontSendNotification);
    envModeComboBox.setSelectedItemIndex((((tone.muteGroup) >> 5) & 0x01), juce::dontSendNotification);
    
    pitchCoarseSlider.setValue(int8_t((tone.pitchCoarse) - 64), juce::dontSendNotification);
    pitchFineSlider.setValue(int8_t((tone.pitchFine)), juce::dontSendNotification);
    pitchRandomComboBox.setSelectedItemIndex((tone.pitchRandom), juce::dontSendNotification);
    bendRangeSlider.setValue((tone.bendRange >> 4), juce::dontSendNotification);
    penvLevSensSlider.setValue(int8_t((tone.tvpVelocity)), juce::dontSendNotification);
    penvTimeSensComboBox.setSelectedItemIndex((tone.bendRange & 0x0f), juce::dontSendNotification);
    penvDepthSlider.setValue(int8_t((tone.tvpEnvDepth)), juce::dontSendNotification);
    penv1TimeSlider.setValue(int8_t((tone.tvpEnvTime1)), juce::dontSendNotification);
    penv1LevelSlider.setValue(int8_t((tone.tvpEnvLevel1)), juce::dontSendNotification);
    penv2TimeSlider.setValue(int8_t((tone.tvpEnvTime2)), juce::dontSendNotification);
    penv2LevelSlider.setValue(int8_t((tone.tvpEnvLevel2)), juce::dontSendNotification);
    penv3TimeSlider.setValue(int8_t((tone.tvpEnvTime3)), juce::dontSendNotification);
    penv3LevelSlider.setValue(int8_t((tone.tvpEnvLevel3)), juce::dontSendNotification);
    penv4TimeSlider.setValue(int8_t((tone.tvpEnvTime4)), juce::dontSendNotification);
    penv4LevelSlider.setValue(int8_t((tone.tvpEnvLevel4)), juce::dontSendNotification);

    filterModeComboBox.setSelectedItemIndex((tone.tvfTimeVelLpfHpf & 0x30) >> 4, juce::dontSendNotification);
    filterCutoffSlider.setValue(int8_t((tone.tvfCutoff)), juce::dontSendNotification);
    filterResoSlider.setValue(int8_t((tone.tvfResonance)), juce::dontSendNotification);
    filterResoModeComboBox.setSelectedItemIndex((tone.tvfResonance & 0x80) >> 7, juce::dontSendNotification);
    fenvLevSensSlider.setValue(int8_t((tone.tvfVelocity)), juce::dontSendNotification);
    fenvTimeSensComboBox.setSelectedItemIndex((tone.tvfTimeVelLpfHpf & 0xf), juce::dontSendNotification);
    fenvDepthSlider.setValue(int8_t((tone.tvfEnvDepth)), juce::dontSendNotification);
    fenv1TimeSlider.setValue(int8_t((tone.tvfEnvTime1)), juce::dontSendNotification);
    fenv1LevelSlider.setValue(int8_t((tone.tvfEnvLevel1)), juce::dontSendNotification);
    fenv2TimeSlider.setValue(int8_t((tone.tvfEnvTime2)), juce::dontSendNotification);
    fenv2LevelSlider.setValue(int8_t((tone.tvfEnvLevel2)), juce::dontSendNotification);
    fenv3TimeSlider.setValue(int8_t((tone.tvfEnvTime3)), juce::dontSendNotification);
    fenv3LevelSlider.setValue(int8_t((tone.tvfEnvLevel3)), juce::dontSendNotification);
    fenv4TimeSlider.setValue(int8_t((tone.tvfEnvTime4)), juce::dontSendNotification);
    fenv4LevelSlider.setValue(int8_t((tone.tvfEnvLevel4)), juce::dontSendNotification);

    levelSlider.setValue(int8_t((tone.tvaLevel)), juce::dontSendNotification);
    panSlider.setValue(int8_t(tone.tvaPan - 64), juce::dontSendNotification);
    
    aenvLevSensSlider.setValue(int8_t((tone.tvaVelocity)), juce::dontSendNotification);
    aenvTimeSensComboBox.setSelectedItemIndex((tone.tvaTimeVelocity & 0x0f), juce::dontSendNotification);
    aenv1TimeSlider.setValue(int8_t((tone.tvaEnvTime1 & 0x7f)), juce::dontSendNotification);
    aenv1LevelSlider.setValue(int8_t((tone.tvaEnvLevel1 & 0x7f)), juce::dontSendNotification);
    aenv2TimeSlider.setValue(int8_t((tone.tvaEnvTime2)), juce::dontSendNotification);
    aenv2LevelSlider.setValue(int8_t((tone.tvaEnvLevel2)), juce::dontSendNotification);
    aenv3TimeSlider.setValue(int8_t((tone.tvaEnvTime3)), juce::dontSendNotification);
    aenv3LevelSlider.setValue(int8_t((tone.tvaEnvLevel3)), juce::dontSendNotification);
    aenv4TimeSlider.setValue(int8_t((tone.tvaEnvTime4)), juce::dontSendNotification);

    drySlider.setValue(int8_t((tone.drySend)), juce::dontSendNotification);
    reverbSlider.setValue(int8_t((tone.reverbSend)), juce::dontSendNotification);
    chorusSlider.setValue(int8_t((tone.chorusSend)), juce::dontSendNotification);

}

void EditRhythmTab::resized()
{
    const auto top = 10;
    const auto sliderLeft1 = 100;
    const auto width = getWidth() / 3 - sliderLeft1 - 10;
    const auto halfWidth = width / 2;
    const auto sliderLeft2 = sliderLeft1 + getWidth() / 3 + 10;
    const auto sliderLeft3 = sliderLeft2 + getWidth() / 3 - 10;
    const auto height = 24;
    const auto vMargin = 24;

    toneSwitchToggle      .setBounds(sliderLeft1 - 90, top + height * 0 + vMargin * 0, width, height);

    waveGroupComboBox     .setBounds(sliderLeft1, top + height * 1 + vMargin * 1, width, height);
    waveformComboBox      .setBounds(sliderLeft1, top + height * 2 + vMargin * 1, width, height);

    muteSlider     .setBounds(sliderLeft1, top + height * 3 + vMargin * 2, halfWidth, height);
    envModeComboBox    .setBounds(sliderLeft1 + halfWidth, top + height * 3 + vMargin * 2, halfWidth, height);
    bendRangeSlider    .setBounds(sliderLeft1, top + height * 4 + vMargin * 2, width, height);
    
    
    toneSlider    .setBounds(sliderLeft1, top + height * 21 + vMargin * 6, width, height);

    pitchCoarseSlider     .setBounds(sliderLeft2, top + height * 1 + vMargin * 0, width, height);
    pitchFineSlider       .setBounds(sliderLeft2, top + height * 2 + vMargin * 0, width, height);
    pitchRandomComboBox   .setBounds(sliderLeft2, top + height * 3 + vMargin * 0, halfWidth, height);
    penvLevSensSlider     .setBounds(sliderLeft2, top + height * 4 + vMargin * 0, width, height);
    penvTimeSensComboBox .setBounds(sliderLeft2, top + height * 5 + vMargin * 0, halfWidth, height);
    penvDepthSlider       .setBounds(sliderLeft2, top + height * 7 + vMargin * 0, width, height);
    penv1TimeSlider       .setBounds(sliderLeft2, top + height * 8 + vMargin * 0, halfWidth, height);
    penv1LevelSlider      .setBounds(sliderLeft2 + halfWidth, top + height * 8 + vMargin * 0, halfWidth, height);
    penv2TimeSlider       .setBounds(sliderLeft2, top + height * 9 + vMargin * 0, halfWidth, height);
    penv2LevelSlider      .setBounds(sliderLeft2 + halfWidth, top + height * 9 + vMargin * 0, halfWidth, height);
    penv3TimeSlider       .setBounds(sliderLeft2, top + height * 10 + vMargin * 0, halfWidth, height);
    penv3LevelSlider      .setBounds(sliderLeft2 + halfWidth, top + height * 10 + vMargin * 0, halfWidth, height);
    penv4TimeSlider       .setBounds(sliderLeft2, top + height * 11 + vMargin * 0, halfWidth, height);
    penv4LevelSlider      .setBounds(sliderLeft2 + halfWidth, top + height * 11 + vMargin * 0, halfWidth, height);

    levelSlider           .setBounds(sliderLeft2, top + height * 12 + vMargin * 1, width, height);
    panSlider             .setBounds(sliderLeft2, top + height * 13 + vMargin * 1, width, height);
    aenvLevSensSlider     .setBounds(sliderLeft2, top + height * 18 + vMargin * 1, width, height);
    aenvTimeSensComboBox  .setBounds(sliderLeft2, top + height * 19 + vMargin * 1, halfWidth, height);
    aenv1TimeSlider       .setBounds(sliderLeft2, top + height * 21 + vMargin * 1, halfWidth, height);
    aenv1LevelSlider      .setBounds(sliderLeft2 + halfWidth, top + height * 21 + vMargin * 1, halfWidth, height);
    aenv2TimeSlider       .setBounds(sliderLeft2, top + height * 22 + vMargin * 1, halfWidth, height);
    aenv2LevelSlider      .setBounds(sliderLeft2 + halfWidth, top + height * 22 + vMargin * 1, halfWidth, height);
    aenv3TimeSlider       .setBounds(sliderLeft2, top + height * 23 + vMargin * 1, halfWidth, height);
    aenv3LevelSlider      .setBounds(sliderLeft2 + halfWidth, top + height * 23 + vMargin * 1, halfWidth, height);
    aenv4TimeSlider       .setBounds(sliderLeft2, top + height * 24 + vMargin * 1, halfWidth, height);

    filterModeComboBox    .setBounds(sliderLeft3, top + height * 0 + vMargin * 0, width, height);
    filterCutoffSlider    .setBounds(sliderLeft3, top + height * 1 + vMargin * 0, width, height);
    filterResoSlider      .setBounds(sliderLeft3, top + height * 2 + vMargin * 0, halfWidth, height);
    filterResoModeComboBox.setBounds(sliderLeft3 + halfWidth, top + height * 2 + vMargin * 0, halfWidth, height);
    fenvLevSensSlider     .setBounds(sliderLeft3, top + height * 4 + vMargin * 0, width, height);
    fenvTimeSensComboBox .setBounds(sliderLeft3, top + height * 5 + vMargin * 0, halfWidth, height);
    fenvDepthSlider       .setBounds(sliderLeft3, top + height * 7 + vMargin * 0, width, height);
    fenv1TimeSlider       .setBounds(sliderLeft3, top + height * 8 + vMargin * 0, halfWidth, height);
    fenv1LevelSlider      .setBounds(sliderLeft3 + halfWidth, top + height * 8 + vMargin * 0, halfWidth, height);
    fenv2TimeSlider       .setBounds(sliderLeft3, top + height * 9 + vMargin * 0, halfWidth, height);
    fenv2LevelSlider      .setBounds(sliderLeft3 + halfWidth, top + height * 9 + vMargin * 0, halfWidth, height);
    fenv3TimeSlider       .setBounds(sliderLeft3, top + height * 10 + vMargin * 0, halfWidth, height);
    fenv3LevelSlider      .setBounds(sliderLeft3 + halfWidth, top + height * 10 + vMargin * 0, halfWidth, height);
    fenv4TimeSlider       .setBounds(sliderLeft3, top + height * 11 + vMargin * 0, halfWidth, height);
    fenv4LevelSlider      .setBounds(sliderLeft3 + halfWidth, top + height * 11 + vMargin * 0, halfWidth, height);

    drySlider             .setBounds(sliderLeft3, top + height * 12 + vMargin * 1, width, height);
    reverbSlider          .setBounds(sliderLeft3, top + height * 13 + vMargin * 1, width, height);
    chorusSlider          .setBounds(sliderLeft3, top + height * 14 + vMargin * 1, width, height);
    outputComboBox        .setBounds(sliderLeft3, top + height * 15 + vMargin * 1, width, height);
}

void EditRhythmTab::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &toneSlider)
    {
        toneCount = uint8_t(slider->getValue()) - 36;
        updateValues();
    }
    else
    {
        sendSysexPatchRhythmChange();
    }
}

void EditRhythmTab::buttonClicked(juce::Button* /* button */)
{
    sendSysexPatchRhythmChange();
}

void EditRhythmTab::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &waveGroupComboBox)
    {
        updateWaveformComboBox(waveformComboBox);
    }

    sendSysexPatchRhythmChange();
}

void EditRhythmTab::sendSysexPatchRhythmChange1Byte(uint8_t address, uint8_t value)
{
    uint8_t data[5];
    data[0] = 0x00;              // address MSB
    data[1] = 0x07;              // address
    data[2] = 0x40 + toneCount;  // address
    data[3] = address & 127;     // address LSB
    data[4] = value;             // data

    uint32_t checksum = 0;

    for (size_t i = 0; i < 5; i++) {
        checksum += data[i];

        if (checksum >= 128) {
            checksum -= 128;
        }
    }

    uint8_t buf[12];
    buf[0] = 0xf0;
    buf[1] = 0x41;
    buf[2] = 0x10; // unit number
    buf[3] = 0x46;
    buf[4] = 0x12; // command

    checksum = 128 - checksum;

    for (size_t i = 0; i < 5; i++) {
        buf[i + 5] = data[i];
    }

    buf[10] = (uint8_t)checksum;
    buf[11] = 0xf7;

    processor.mcuLock.enter();
    processor.mcu->postMidiSC55(buf, 12);
    processor.mcuLock.exit();
}

void EditRhythmTab::sendSysexPatchRhythmChange2Byte(uint8_t address, uint8_t value)
{
    uint8_t data[6];
    data[0] = 0x00;                 // address MSB
    data[1] = 0x07;                 // address
    data[2] = 0x40 + toneCount;     // address
    data[3] = address & 127;        // address LSB
    data[4] = (value & 0xf0) >> 4;  // data
    data[5] = value & 0x0f;         // data

    uint32_t checksum = 0;

    for (size_t i = 0; i < 6; i++) {
        checksum += data[i];

        if (checksum >= 128) {
            checksum -= 128;
        }
    }

    uint8_t buf[13];
    buf[0] = 0xf0;
    buf[1] = 0x41;
    buf[2] = 0x10; // unit number
    buf[3] = 0x46;
    buf[4] = 0x12; // command

    checksum = 128 - checksum;

    for (size_t i = 0; i < 6; i++) {
        buf[i + 5] = data[i];
    }

    buf[11] = (uint8_t)checksum;
    buf[12] = 0xf7;

    processor.mcuLock.enter();
    processor.mcu->postMidiSC55(buf, 13);
    processor.mcuLock.exit();
}


void EditRhythmTab::sendSysexPatchRhythmChange()
{
    sendSysexPatchRhythmChange1Byte(0x00, uint8_t(waveGroupComboBox.getSelectedItemIndex()));
    sendSysexPatchRhythmChange2Byte(0x01, uint8_t(waveformComboBox.getSelectedItemIndex()));
    sendSysexPatchRhythmChange1Byte(0x03, toneSwitchToggle.getToggleStateValue() == 1 ? 0x01 : 0x00);
    sendSysexPatchRhythmChange1Byte(0x04, uint8_t(pitchCoarseSlider.getValue() + 64));
    sendSysexPatchRhythmChange1Byte(0x05, uint8_t(muteSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x06, uint8_t(envModeComboBox.getSelectedItemIndex()));
    
    sendSysexPatchRhythmChange1Byte(0x07, uint8_t(pitchFineSlider.getValue() + 64));
    sendSysexPatchRhythmChange1Byte(0x08, uint8_t(pitchRandomComboBox.getSelectedItemIndex()));
    sendSysexPatchRhythmChange1Byte(0x09, uint8_t(bendRangeSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x0a, uint8_t(penvLevSensSlider.getValue() + 64));
    sendSysexPatchRhythmChange1Byte(0x0b, uint8_t(penvTimeSensComboBox.getSelectedItemIndex()));
    sendSysexPatchRhythmChange1Byte(0x0c, uint8_t(penvDepthSlider.getValue() + 64));
    sendSysexPatchRhythmChange1Byte(0x0d, uint8_t(penv1TimeSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x0e, uint8_t(penv1LevelSlider.getValue() + 64));
    sendSysexPatchRhythmChange1Byte(0x0f, uint8_t(penv2TimeSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x10, uint8_t(penv2LevelSlider.getValue() + 64));
    sendSysexPatchRhythmChange1Byte(0x11, uint8_t(penv3TimeSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x12, uint8_t(penv3LevelSlider.getValue() + 64));
    sendSysexPatchRhythmChange1Byte(0x13, uint8_t(penv4TimeSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x14, uint8_t(penv4LevelSlider.getValue() + 64));
    sendSysexPatchRhythmChange1Byte(0x15, uint8_t(filterModeComboBox.getSelectedItemIndex()));
    sendSysexPatchRhythmChange1Byte(0x16, uint8_t(filterCutoffSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x17, uint8_t(filterResoSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x18, uint8_t(filterResoModeComboBox.getSelectedItemIndex()));
    sendSysexPatchRhythmChange1Byte(0x19, uint8_t(fenvLevSensSlider.getValue() + 64));
    sendSysexPatchRhythmChange1Byte(0x1a, uint8_t(fenvTimeSensComboBox.getSelectedItemIndex()));
    sendSysexPatchRhythmChange1Byte(0x1b, uint8_t(fenvDepthSlider.getValue() + 64));
    sendSysexPatchRhythmChange1Byte(0x1c, uint8_t(fenv1TimeSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x1d, uint8_t(fenv1LevelSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x1e, uint8_t(fenv2TimeSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x1f, uint8_t(fenv2LevelSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x20, uint8_t(fenv3TimeSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x21, uint8_t(fenv3LevelSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x22, uint8_t(fenv4TimeSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x23, uint8_t(fenv4LevelSlider.getValue()));
    
    sendSysexPatchRhythmChange1Byte(0x24, uint8_t(levelSlider.getValue()));
    sendSysexPatchRhythmChange2Byte(0x25, uint8_t(panSlider.getValue() + 64));
    sendSysexPatchRhythmChange1Byte(0x27, uint8_t(aenvLevSensSlider.getValue() + 64));
    sendSysexPatchRhythmChange1Byte(0x28, uint8_t(aenvTimeSensComboBox.getSelectedItemIndex()));
    sendSysexPatchRhythmChange1Byte(0x29, uint8_t(aenv1TimeSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x2a, uint8_t(aenv1LevelSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x2b, uint8_t(aenv2TimeSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x2c, uint8_t(aenv2LevelSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x2d, uint8_t(aenv3TimeSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x2e, uint8_t(aenv3LevelSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x2f, uint8_t(aenv4TimeSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x30, uint8_t(drySlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x31, uint8_t(reverbSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x32, uint8_t(chorusSlider.getValue()));
    sendSysexPatchRhythmChange1Byte(0x33, uint8_t(outputComboBox.getSelectedItemIndex()));

    Rhythm* rhythm = (Rhythm*)processor.status.drums;
    RhythmTone* tone = &rhythm->tones[toneCount];

    tone->flags = uint8_t(waveGroupComboBox.getSelectedItemIndex() + (toneSwitchToggle.getToggleState() << 7));
    tone->waveNumber = uint8_t(waveformComboBox.getSelectedItemIndex());
    tone->muteGroup = uint8_t((uint8_t(muteSlider.getValue()) << 0) + (envModeComboBox.getSelectedItemIndex() << 5));
    tone->pitchCoarse = uint8_t(pitchCoarseSlider.getValue() + 64);
    tone->pitchFine = uint8_t(pitchFineSlider.getValue());
    tone->bendRange = uint8_t(penvTimeSensComboBox.getSelectedItemIndex() + (uint8_t(bendRangeSlider.getValue()) << 4));
    tone->pitchRandom = uint8_t(pitchRandomComboBox.getSelectedItemIndex());
    tone->tvpVelocity = uint8_t(penvDepthSlider.getValue());
    tone->tvpEnvDepth = uint8_t(penvDepthSlider.getValue());
    tone->tvpEnvTime1 = uint8_t(penv1TimeSlider.getValue());
    tone->tvpEnvLevel1 = uint8_t(penv1LevelSlider.getValue());
    tone->tvpEnvTime2 = uint8_t(penv2TimeSlider.getValue());
    tone->tvpEnvLevel2 = uint8_t(penv2LevelSlider.getValue());
    tone->tvpEnvTime3 = uint8_t(penv3TimeSlider.getValue());
    tone->tvpEnvLevel3 = uint8_t(penv3LevelSlider.getValue());
    tone->tvpEnvTime4 = uint8_t(penv4TimeSlider.getValue());
    tone->tvpEnvLevel4 = uint8_t(penv4LevelSlider.getValue());
    tone->tvfTimeVelLpfHpf = uint8_t(fenvTimeSensComboBox.getSelectedItemIndex() + (filterModeComboBox.getSelectedItemIndex() << 4));
    tone->tvfCutoff = uint8_t(filterCutoffSlider.getValue());
    tone->tvfResonance = uint8_t(filterResoSlider.getValue() + (filterResoModeComboBox.getSelectedItemIndex() << 7));
    tone->tvfVelocity = uint8_t(fenvLevSensSlider.getValue());
    tone->tvfEnvDepth = uint8_t(fenvDepthSlider.getValue());
    tone->tvfEnvTime1 = uint8_t(fenv1TimeSlider.getValue());
    tone->tvfEnvLevel1 = uint8_t(fenv1LevelSlider.getValue());
    tone->tvfEnvTime2 = uint8_t(fenv2TimeSlider.getValue());
    tone->tvfEnvLevel2 = uint8_t(fenv2LevelSlider.getValue());
    tone->tvfEnvTime3 = uint8_t(fenv3TimeSlider.getValue());
    tone->tvfEnvLevel3 = uint8_t(fenv3LevelSlider.getValue());
    tone->tvfEnvTime4 = uint8_t(fenv4TimeSlider.getValue());
    tone->tvfEnvLevel4 = uint8_t(fenv4LevelSlider.getValue());
    tone->tvaLevel = uint8_t(levelSlider.getValue());
    tone->tvaPan = uint8_t(panSlider.getValue() + 64);
    tone->tvaVelocity = uint8_t(aenvLevSensSlider.getValue());
    tone->tvaTimeVelocity = uint8_t(aenvTimeSensComboBox.getSelectedItemIndex());
    tone->tvaEnvTime1 = uint8_t(aenv1TimeSlider.getValue());
    tone->tvaEnvLevel1 = uint8_t(aenv1LevelSlider.getValue());
    tone->tvaEnvTime2 = uint8_t(aenv2TimeSlider.getValue());
    tone->tvaEnvLevel2 = uint8_t(aenv2LevelSlider.getValue());
    tone->tvaEnvTime3 = uint8_t(aenv3TimeSlider.getValue());
    tone->tvaEnvLevel3 = uint8_t(aenv3LevelSlider.getValue());
    tone->tvaEnvTime4 = uint8_t(aenv4TimeSlider.getValue());
    tone->drySend = uint8_t(drySlider.getValue());
    tone->reverbSend = uint8_t(reverbSlider.getValue());
    tone->chorusSend = uint8_t(chorusSlider.getValue());
}
