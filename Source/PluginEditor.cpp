/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Oscilloscope.h"
#include "PresetPanel.h"

//==============================================================================
DistAdvAudioProcessorEditor::DistAdvAudioProcessorEditor
(DistAdvAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : juce::AudioProcessorEditor(&p), valueTreeState(vts), 
    presetPanel(p.getPresetManager()), 
    audioProcessor(p),
    scopeComponent(audioProcessor.getAudioBufferQueue())
{
    customLookAndFeel = std::make_unique<BoostKnobLookAndFeel>();
    //setLookAndFeel(&boostKnobLookAndFeel);
    setSize(700, 720);
    addAndMakeVisible(scopeComponent);


    //Slider styles
    ngPreThreshSlider.setSliderStyle(juce::Slider::Rotary);
    ngPreRatioSlider.setSliderStyle(juce::Slider::Rotary);
    ngPreRelSlider.setSliderStyle(juce::Slider::Rotary);
    ngPreAtkSlider.setSliderStyle(juce::Slider::Rotary);
    gainKnob.setSliderStyle(juce::Slider::Rotary);
    //gainKnob.setLookAndFeel(customLookAndFeel.get());

    ngPostThreshSlider.setSliderStyle(juce::Slider::Rotary);
    ngPostRatioSlider.setSliderStyle(juce::Slider::Rotary);
    ngPostAtkSlider.setSliderStyle(juce::Slider::Rotary);
    ngPostRelSlider.setSliderStyle(juce::Slider::Rotary);
    lowpassInSlider.setSliderStyle(juce::Slider::Rotary);

    //ranges
    ngPreRatioSlider.setRange(1.0f, 10.0f, 1.0f);
    ngPreThreshSlider.setRange(-60.0f, 10.0f, 1.0f);
    ngPreAtkSlider.setRange(1.0f, 1000.0f, 5.0f);
    ngPreRelSlider.setRange(1.0f, 1000.0f, 5.0f);

    ngPostThreshSlider.setRange(-30.0f, 30.0f, 1.0f);
    ngPostRatioSlider.setRange(0.0f, 10.0f, 1.0f);
    ngPostAtkSlider.setRange(5.0f, 2000.0f, 5.0f);
    ngPostRelSlider.setRange(5.0f, 2000.0f, 5.0f);
    lowpassInSlider.setRange(20.0f, 666.0f);
    gainKnob.setRange(1.0f, 11.0f);
    driveSlider.setRange(0.0f, 80.0f);
    ceilingSlider.setRange(0.0f, 1.0f);
    mixSlider.setRange(0.0f, 1.0f);
    threshSlider.setRange(0.0f, 1.0f);
    tubeMix.setRange(0.0f, 1.0f, .1f);
    tubeOG.setRange(0.0f, 1.0f, .1f);
    tubeIG.setRange(0.0f, 1.0f, .1f);
    tubeBias.setRange(0.0f, 1.0f, .1f);
    tubeDrive.setRange(0.0f, 1.0f, .1f);
    hiEqFreqKnob.setRange(3000.0f, 5000.0f, 10.0f);
    hiEqQKnob.setRange(0.01f, 1.5f, 0.01f);
    hiEqGainKnob.setRange(-12.0f, 12.0f, 1.0f);
    midEqFreqKnob.setRange(500.0f, 3000.0, 10.0f);
    midEqQKnob.setRange(0.01f, 1.5f, 0.01f);
    midEqGainKnob.setRange(-12.0f, 12.0f, 0.5f);

    //add and make visible
    //sliders
    addAndMakeVisible(ngPreRatioSlider);
    addAndMakeVisible(ngPreThreshSlider);
    addAndMakeVisible(ngPreAtkSlider);
    addAndMakeVisible(ngPreRelSlider);
    addAndMakeVisible(ngPostThreshSlider);
    addAndMakeVisible(ngPostRatioSlider);
    addAndMakeVisible(ngPostAtkSlider);
    addAndMakeVisible(ngPostRelSlider);
    addAndMakeVisible(lowpassInSlider);
    addAndMakeVisible(gainKnob);
    addAndMakeVisible(distSelect);
    addAndMakeVisible(driveSlider);
    addAndMakeVisible(ceilingSlider);
    addAndMakeVisible(mixSlider);
    addAndMakeVisible(threshSlider);
    addAndMakeVisible(tubeMix);//tube
    addAndMakeVisible(tubeIG);
    addAndMakeVisible(tubeOG);
    addAndMakeVisible(tubeBias);
    addAndMakeVisible(tubeDrive);
    addAndMakeVisible(hiEqFreqKnob);//eqs
    addAndMakeVisible(hiEqQKnob);
    addAndMakeVisible(hiEqGainKnob);
    addAndMakeVisible(midEqFreqKnob);
    addAndMakeVisible(midEqQKnob);
    addAndMakeVisible(midEqGainKnob);

    addAndMakeVisible(bypassCabSim);
    addAndMakeVisible(bypassDistBtn);
    addAndMakeVisible(bypassNgPre);
    addAndMakeVisible(bypassNgPost);
    addAndMakeVisible(bypassTube);
    addAndMakeVisible(loadIrBtn);
    addAndMakeVisible(tunerBtn);
    //textbox styles
    ngPreAtkSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    ngPreRelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    ngPostThreshSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    ngPostRatioSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    ngPostAtkSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    ngPostRelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lowpassInSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    gainKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    ngPreThreshSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    ngPreRatioSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);



    distSelect.addItem("Hard Clip", 1); //hc1
    distSelect.addItem("Harder Clip", 4); //hc2
    distSelect.addItem("Soft Clip", 2);
    distSelect.addItem("Saturate", 3);
    distSelect.addItem("Fuzz", 5);
    distSelect.addItem("Diode", 6);
    distSelect.addItem("Lofi", 7);
    distSelect.setSelectedId(1);


    gainKnob.addListener(this);
    distSelect.addListener(this);
    ngPostRelSlider.addListener(this);
    ngPreAtkSlider.addListener(this);
    ngPostAtkSlider.addListener(this);
    driveSlider.addListener(this);
    ceilingSlider.addListener(this);
    mixSlider.addListener(this);
    threshSlider.addListener(this);
    tubeMix.addListener(this);
    tubeIG.addListener(this);
    tubeOG.addListener(this);
    tubeBias.addListener(this);
    tubeDrive.addListener(this);
    hiEqFreqKnob.addListener(this);
    hiEqQKnob.addListener(this);
    hiEqGainKnob.addListener(this);
    midEqFreqKnob.addListener(this);
    midEqQKnob.addListener(this);
    midEqGainKnob.addListener(this);
    bypassCabSim.addListener(this);
    bypassDistBtn.addListener(this);
    bypassNgPre.addListener(this);
    bypassNgPost.addListener(this);
    bypassTube.addListener(this);
    loadIrBtn.addListener(this);
    tunerBtn.addListener(this);
    ngPostRatioSlider.addListener(this);
    lowpassInSlider.addListener(this);
    ngPreRelSlider.addListener(this);
    ngPostThreshSlider.addListener(this);
    ngPreRatioSlider.addListener(this);
    ngPreThreshSlider.addListener(this);

    //button text
    bypassCabSim.setButtonText("CAB");
    bypassDistBtn.setButtonText("DIST");
    bypassNgPre.setButtonText("NOISE GATE PRE DIST");
    bypassNgPost.setButtonText("NOISE GATE POST DIST");
    bypassTube.setButtonText("TUBE");
    loadIrBtn.setButtonText("LOAD IR");
    tunerBtn.setButtonText("TUNER");

    //Colors
    bypassTube.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::mediumvioletred);
    bypassNgPost.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::mediumvioletred);
    bypassNgPre.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::mediumvioletred);
    bypassDistBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::mediumvioletred);
    bypassCabSim.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::mediumvioletred);



    //lbl
    lblNgPreThreshold   .setText("Thresh pre", juce::dontSendNotification);
    lblNgPreRatio       .setText("Ratio", juce::dontSendNotification);
    lblNgPreAtk         .setText("Attack", juce::dontSendNotification);
    lblNgPreRel         .setText("Release", juce::dontSendNotification);
    lblNgPostThreshold  .setText("Thresh post", juce::dontSendNotification);
    lblNgPostRatio      .setText("Ratio", juce::dontSendNotification);
    lblNgPostAtk        .setText("Attack", juce::dontSendNotification);
    lblNgPostRel        .setText("Release", juce::dontSendNotification);
    lblMainGain         .setText("Boost", juce::dontSendNotification);
    lblInputLp          .setText("Tighten LP", juce::dontSendNotification);
    lblDistSel          .setText("Dist Type", juce::dontSendNotification);
    lblDistDrive        .setText("Distortion Level", juce::dontSendNotification);
    lblDistCeiling      .setText("Squash", juce::dontSendNotification);
    lblDistMix          .setText("Distortion Mix", juce::dontSendNotification);
    lblDistThresh       .setText("Slam", juce::dontSendNotification);
    lbltubeMix          .setText("Tube Mix",juce::dontSendNotification);
    lbltubeIG           .setText("Tube Input Gain",juce::dontSendNotification);
    lbltubeOG           .setText("Tube Output Gain",juce::dontSendNotification);
    lbltubeBias         .setText("Tube Bias",juce::dontSendNotification);
    lbltubeDrive        .setText("Tube Drive",juce::dontSendNotification);


    //font
    juce::Font parameterLabel("Arial", 12.0f, juce::Font::bold);

    lblMainGain         .setFont(parameterLabel);
    lblInputLp          .setFont(parameterLabel);
    lblNgPreThreshold   .setFont(parameterLabel);
    lblNgPostThreshold  .setFont(parameterLabel);
    lblNgPreRatio       .setFont(parameterLabel);
    lblNgPostRatio      .setFont(parameterLabel);
    lblNgPreAtk         .setFont(parameterLabel);
    lblNgPostAtk        .setFont(parameterLabel);
    lblNgPreRel         .setFont(parameterLabel);
    lblNgPostRel        .setFont(parameterLabel);
    lblDistSel          .setFont(parameterLabel);
    lblDistDrive        .setFont(parameterLabel);
    lblDistCeiling      .setFont(parameterLabel);
    lblDistMix          .setFont(parameterLabel);
    lblDistThresh       .setFont(parameterLabel);
    lbltubeMix          .setFont(parameterLabel);
    lbltubeIG           .setFont(parameterLabel);
    lbltubeOG           .setFont(parameterLabel);
    lbltubeBias         .setFont(parameterLabel);
    lbltubeDrive        .setFont(parameterLabel);

    //colors

    lblMainGain         .setColour(juce::Label::textColourId, juce::Colours::white);
    lblInputLp          .setColour(juce::Label::textColourId, juce::Colours::white);
    lblNgPreThreshold   .setColour(juce::Label::textColourId, juce::Colours::white);
    lblNgPostThreshold  .setColour(juce::Label::textColourId, juce::Colours::white);
    lblNgPreRatio       .setColour(juce::Label::textColourId, juce::Colours::white);
    lblNgPostRatio      .setColour(juce::Label::textColourId, juce::Colours::white);
    lblNgPreAtk         .setColour(juce::Label::textColourId, juce::Colours::white);
    lblNgPostAtk        .setColour(juce::Label::textColourId, juce::Colours::white);
    lblNgPreRel         .setColour(juce::Label::textColourId, juce::Colours::white);
    lblNgPostRel        .setColour(juce::Label::textColourId, juce::Colours::white);
    lblDistSel          .setColour(juce::Label::textColourId, juce::Colours::white);
    lblDistDrive        .setColour(juce::Label::textColourId, juce::Colours::white);
    lblDistCeiling      .setColour(juce::Label::textColourId, juce::Colours::white);
    lblDistMix          .setColour(juce::Label::textColourId, juce::Colours::white);
    lblDistThresh       .setColour(juce::Label::textColourId, juce::Colours::white);
    lbltubeMix          .setColour(juce::Label::textColourId, juce::Colours::white);
    lbltubeIG           .setColour(juce::Label::textColourId, juce::Colours::white);
    lbltubeOG           .setColour(juce::Label::textColourId, juce::Colours::white);
    lbltubeBias         .setColour(juce::Label::textColourId, juce::Colours::white);
    lbltubeDrive        .setColour(juce::Label::textColourId, juce::Colours::white);


    addAndMakeVisible(lblTunerNote);
    addAndMakeVisible(lblTunerOffsetCents);
    addAndMakeVisible(lblNgPreThreshold);
    addAndMakeVisible(lblNgPostThreshold);
    addAndMakeVisible(lblNgPreRatio);
    addAndMakeVisible(lblNgPostRatio);
    addAndMakeVisible(lblNgPreRel);
    addAndMakeVisible(lblNgPostRel);
    addAndMakeVisible(lblNgPreAtk);
    addAndMakeVisible(lblNgPostAtk);
    addAndMakeVisible(lblDistSel);
    addAndMakeVisible(lblDistDrive);
    addAndMakeVisible(lblDistCeiling);
    addAndMakeVisible(lblDistMix);
    addAndMakeVisible(lblDistThresh);
    addAndMakeVisible(lbltubeMix);
    addAndMakeVisible(lbltubeIG);
    addAndMakeVisible(lbltubeOG);
    addAndMakeVisible(lbltubeBias);
    addAndMakeVisible(lbltubeDrive);
    addAndMakeVisible(lblMainGain);
    addAndMakeVisible(lblInputLp);
    addAndMakeVisible(presetPanel);

   
    //attachments
    gainAtt              = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "gain",                     gainKnob              );
    tubeMixAtt           = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "tubeMix",                  tubeMix               );
    tubeBiasAtt          = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "tubeBias",                 tubeBias              );
    tubeInAtt            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "tubeIn",                   tubeIG                );
    tubeOutAtt           = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "tubeOut",                  tubeOG                );
    tubeDriveAtt         = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "tubeDrive",                tubeDrive             );

    midEqFreqAtt         = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "midEqFreq",                midEqFreqKnob         );
    midEqQAtt            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "midEqQ",                   midEqQKnob            );
    midEqGainAtt         = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "midEqGain",                midEqGainKnob         );
    hiEqFreqAtt          = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "hiEqFreq",                 hiEqFreqKnob          );
    hiEqQAtt             = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "hiEqQ",                    hiEqQKnob             );
    hiEqGainAtt          = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "hiEqGain",                 hiEqGainKnob          );

    driveAtt             = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "drive",                    driveSlider           );
    driveThreshAtt       = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "driveThresh",              threshSlider          );
    driveCeilAtt         = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "driveCeil",                ceilingSlider         );
    driveMixAtt          = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "driveMix",                 mixSlider             );
    ngPreThreshAtt       = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "ngPreThresh",              ngPreThreshSlider     );
    ngPreRatioAtt        = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "ngPreRatio",               ngPreRatioSlider      );
    ngPreAtkAtt          = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "ngPreAtk",                 ngPreAtkSlider        );
    ngPreRelAtt          = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "ngPreRel",                 ngPreRelSlider        );
    ngPostThreshAtt      = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "ngPostThresh",             ngPostThreshSlider    );
    ngPostRatioAtt       = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "ngPostRatio",              ngPostRatioSlider     );
    ngPostAtkAtt         = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "ngPostAtk",                ngPostAtkSlider       );
    ngPostRelAtt         = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "ngPostRel",                ngPostRelSlider       );
    tightenLpAtt         = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState,             "tightenLp",                lowpassInSlider       );
    cabOnAtt             = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(valueTreeState,             "cabOn",                  bypassCabSim            );
    distOnAtt            = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(valueTreeState,             "distOn",                bypassDistBtn            );
    tubeOnAtt            = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(valueTreeState,             "tubeOn",                bypassTube               );
    ng1OnAtt             = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(valueTreeState,             "ng1On",                  bypassNgPre             );
    ng2OnAtt             = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(valueTreeState,             "ng2On",                  bypassNgPost            );
    tunerOnAtt           = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(valueTreeState,             "tunerOn",              tunerBtn                  );
    distSelectAtt        = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(valueTreeState,           "distType",                distSelect             );


}

DistAdvAudioProcessorEditor::~DistAdvAudioProcessorEditor()
{

		gainAtt                    = nullptr;
        tubeMixAtt                 = nullptr;
        tubeBiasAtt                = nullptr;
        tubeInAtt                  = nullptr;
        tubeOutAtt                 = nullptr;
        tubeDriveAtt               = nullptr;
        midEqFreqAtt               = nullptr;
        midEqQAtt                  = nullptr;
        midEqGainAtt               = nullptr;
        hiEqFreqAtt                = nullptr;
        hiEqQAtt                   = nullptr;
        hiEqGainAtt                = nullptr;
        driveAtt                   = nullptr;
        driveThreshAtt             = nullptr;
        driveCeilAtt               = nullptr;
        driveMixAtt                = nullptr;
        ngPreThreshAtt             = nullptr;
        ngPreRatioAtt              = nullptr;
        ngPreAtkAtt                = nullptr;
        ngPreRelAtt                = nullptr;
        ngPostThreshAtt            = nullptr;
        ngPostRatioAtt             = nullptr;
        ngPostAtkAtt               = nullptr;
        ngPostRelAtt               = nullptr;
        tightenLpAtt               = nullptr;
        cabOnAtt                   = nullptr;
        distOnAtt                  = nullptr;
        tubeOnAtt                  = nullptr;
        ng1OnAtt                   = nullptr;
        ng2OnAtt                   = nullptr;
        tunerOnAtt                 = nullptr;
        distSelectAtt              = nullptr;
    ngPreThreshSlider       .removeListener(this);
    ngPreRatioSlider        .removeListener(this);
    ngPreAtkSlider          .removeListener(this);
    ngPreRelSlider          .removeListener(this);
    ngPostThreshSlider      .removeListener(this);
    ngPostRatioSlider       .removeListener(this);
    ngPostAtkSlider         .removeListener(this);
    ngPostRelSlider         .removeListener(this);
    hiEqFreqKnob            .removeListener(this);
    hiEqQKnob               .removeListener(this);
    hiEqGainKnob            .removeListener(this);
    midEqFreqKnob           .removeListener(this);
    midEqQKnob              .removeListener(this);
    midEqGainKnob           .removeListener(this);
    lowpassInSlider         .removeListener(this);
    ceilingSlider           .removeListener(this);
    mixSlider               .removeListener(this);
    threshSlider            .removeListener(this);
    gainKnob                .removeListener(this);
    tubeBias                .removeListener(this);
    tubeIG                  .removeListener(this);
    tubeOG                  .removeListener(this);
    tubeMix                 .removeListener(this);
    tubeDrive               .removeListener(this);
    bypassDistBtn           .removeListener(this);
    bypassTube              .removeListener(this);
    bypassCabSim            .removeListener(this);
    bypassNgPost            .removeListener(this);
    bypassNgPre             .removeListener(this);
    tunerBtn                .removeListener(this);
    loadIrBtn               .removeListener(this);
    threshSlider            .removeListener(this);
    distSelect              .removeListener(this);
}


//==============================================================================
void DistAdvAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(20.0f));

    bypassCabSim.setColour(juce::ToggleButton::ColourIds::textColourId,     bypassCabSim.getToggleState() ? juce::Colours::palegreen : juce::Colours::palevioletred);
    bypassDistBtn.setColour(juce::ToggleButton::ColourIds::textColourId,    bypassDistBtn.getToggleState() ? juce::Colours::palegreen : juce::Colours::palevioletred);
    bypassTube.setColour(juce::ToggleButton::ColourIds::textColourId,       bypassTube.getToggleState() ? juce::Colours::palegreen : juce::Colours::palevioletred);
    bypassNgPre.setColour(juce::ToggleButton::ColourIds::textColourId,      bypassNgPre.getToggleState() ? juce::Colours::palegreen : juce::Colours::palevioletred);
    bypassNgPost.setColour(juce::ToggleButton::ColourIds::textColourId,     bypassNgPost.getToggleState() ? juce::Colours::palegreen : juce::Colours::palevioletred);

    //tuner
    if (audioProcessor.tunerOn) {
        const float note = round(log(audioProcessor.tunerfrequency / 440.0) / log(2) * 12 + 69);
        noteName = juce::MidiMessage::getMidiNoteName(note, true, true, 4);
        const auto currentAbs = juce::MidiMessage::getMidiNoteInHertz(note);
        const float offset = 1200 * log(audioProcessor.tunerfrequency / currentAbs) / log(2);
        offsetTune = juce::String(offset).formatted("%.2f", offset);

        juce::Font Tuner("Arial", 25.0f, juce::Font::bold);
        lblTunerNote.setVisible(true);
        lblTunerNote.setText(noteName, juce::dontSendNotification);
        lblTunerNote.setFont(Tuner);
        lblTunerOffsetCents.setVisible(true);
        lblTunerOffsetCents.setText(offsetTune, juce::dontSendNotification);
        lblTunerOffsetCents.setFont(Tuner);


        if (offset < 10.0f && offset >= -10.0f) {
            lblTunerNote.setColour(juce::Label::textColourId, juce::Colours::green);
        }

        lblTunerNote.setColour(juce::Label::textColourId, juce::Colours::orange);
        lblTunerOffsetCents.setColour(juce::Label::textColourId, juce::Colours::orange);
    }
    else {
        lblTunerNote.setVisible(false);
        lblTunerOffsetCents.setVisible(false);

    }

}


void DistAdvAudioProcessorEditor::resized()
{
    const auto area = getLocalBounds();
    presetPanel.setBounds(20, 670, 300,                     40);
    distSelect.setBounds(20, 600, 100,                      30);
    gainKnob.setBounds(240, 600, 80,                        80);
    lowpassInSlider.setBounds(320, 600, 80,                 80);
    scopeComponent.setBounds(0, 0, area.getWidth(),         100);
    driveSlider.setBounds(20,               120, 300,       20);                                     
    threshSlider.setBounds(20,              140, 300,       20);
    ceilingSlider.setBounds(20,             160, 300,       20);
    mixSlider.setBounds(20,                 180, 300,       20);                                       
    tubeMix.setBounds(20,                   200, 300,       20);
    tubeIG.setBounds(20,                    220, 300,       20);
    tubeOG.setBounds(20,                    240, 300,       20);
    tubeBias.setBounds(20,                  260, 300,       20);
    tubeDrive.setBounds(20,                 280, 300,       20);

    midEqFreqKnob.setBounds(20, 460, 300, 20);
    midEqGainKnob.setBounds(20, 480, 300, 20);
    midEqQKnob.setBounds(20, 500, 300, 20);
    hiEqFreqKnob.setBounds(20, 520, 300, 20);
    hiEqGainKnob.setBounds(20, 540, 300, 20);
    hiEqQKnob.setBounds(20, 560, 300, 20);
    
    ngPreThreshSlider  .setBounds(500, 140, 80, 80);
    ngPreRatioSlider   .setBounds(580, 140, 80, 80);
    ngPreAtkSlider     .setBounds(500, 220, 80, 80);
    ngPreRelSlider     .setBounds(580, 220, 80, 80);
    ngPostThreshSlider .setBounds(500, 300, 80, 80);
    ngPostRatioSlider  .setBounds(580, 300, 80, 80);
    ngPostAtkSlider    .setBounds(500, 380, 80, 80);
    ngPostRelSlider    .setBounds(580, 380, 80, 80);

    lblNgPreThreshold.setBounds(500,  130, 100, 20);
    lblNgPreRatio.setBounds(580,      130, 100, 20);
    lblNgPreAtk.setBounds(500,        210, 100, 20);
    lblNgPreRel.setBounds(580,        210, 100, 20);


    lblNgPostThreshold.setBounds(500, 290, 100, 20);
    lblNgPostRatio.setBounds(580,     290, 100, 20);
    lblNgPostAtk.setBounds(500,       370, 100, 20);
    lblNgPostRel.setBounds(580,       370, 100, 20);

    bypassCabSim.setBounds(20, 70, 80, 50);
    bypassDistBtn.setBounds(180, 70, 80, 50);
    loadIrBtn.setBounds(260, 70, 70, 50);
    bypassTube.setBounds(340, 70, 80, 50);
    bypassNgPre.setBounds(500, 70, 80, 50);
    bypassNgPost.setBounds(580, 70, 80, 50);
    tunerBtn.setBounds(580, 460, 80, 50);

    //labels etc
    lblDistSel          .setBounds(150, 590, 50,    30);
    lblMainGain         .setBounds(260, 590, 100,   30);
    lblInputLp          .setBounds(320, 590, 100,   30);                    
    lblDistDrive        .setBounds(350, 120, 90,    30);
    lblDistCeiling      .setBounds(350, 140, 90,    30);
    lblDistMix          .setBounds(350, 160, 90,    30);
    lblDistThresh       .setBounds(350, 180, 90,    30);     
    lbltubeMix          .setBounds(350, 200, 90,    30);
    lbltubeIG           .setBounds(350, 220, 90,    30);
    lbltubeOG           .setBounds(350, 240, 90,    30);
    lbltubeBias         .setBounds(350, 260, 90,    30);
    lbltubeDrive        .setBounds(350, 280, 90,    30);    
    lblTunerNote        .setBounds(600, 500, 50,    50);
    lblTunerOffsetCents .setBounds(600, 525, 100,   50);

}
void DistAdvAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{

}

void DistAdvAudioProcessorEditor::buttonClicked(juce::Button* btn) {
    if (btn == &loadIrBtn) {

        filechooser = std::make_unique<juce::FileChooser>("Choose File", audioProcessor.root, "*");
        const auto fileChooserFlags = juce::FileBrowserComponent::openMode |
            juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::canSelectDirectories;

        filechooser->launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                juce::File result(chooser.getResult());
                if (result.getFileExtension() == ".wav") {

                    audioProcessor.saveFile = result;

                    audioProcessor.root = result.getParentDirectory().getFullPathName();
                    audioProcessor.setCab(audioProcessor.saveFile);

                }
            });
    }
    if (btn == &tunerBtn)
    {
        audioProcessor.bypassTuner(!tunerBtn.getToggleState());
    }
}
void DistAdvAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &distSelect)
    {
        int selectedId = distSelect.getSelectedId();
        audioProcessor.setDistType(selectedId-1);
    }
}