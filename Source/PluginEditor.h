#ifndef __PLUGINEDITOR_H_AFF08764__
#define __PLUGINEDITOR_H_AFF08764__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class DafxAudioProcessorEditor  : public AudioProcessorEditor, 
                                  public SliderListener, 
                                  public ButtonListener,
                                  public ComboBoxListener
{
public:
    DafxAudioProcessorEditor (DafxAudioProcessor* ownerFilter);
    ~DafxAudioProcessorEditor();
    
    void sliderValueChanged (Slider*);
    void buttonClicked(Button*);
    void comboBoxChanged(ComboBox*); 

    void paint (Graphics& g);
    
private:
    
    ScopedPointer<ComboBox> filterTypeSelector;
    
    Label *cutoffSliderLabel, *resonanceSliderLabel, *helloLabel; 
    Slider *cutoffSlider, *resSlider;
    
    DrawableButton *myButton; 
    
     DafxAudioProcessor* getProcessor() const { return static_cast <DafxAudioProcessor*> (getAudioProcessor()); }
    
};


#endif  // __PLUGINEDITOR_H_AFF08764__
