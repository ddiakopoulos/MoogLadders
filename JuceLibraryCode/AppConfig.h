/*

    IMPORTANT! This file is auto-generated each time you save your
    project - if you alter its contents, your changes may be overwritten!

    If you want to change any of these values, use the Introjucer to do so,
    rather than editing this file directly!

    Any commented-out settings will assume their default values.

*/

#ifndef __JUCE_APPCONFIG_QYBE1O__
#define __JUCE_APPCONFIG_QYBE1O__

//==============================================================================
#define JUCE_MODULE_AVAILABLE_juce_audio_basics             1
#define JUCE_MODULE_AVAILABLE_juce_audio_devices            1
#define JUCE_MODULE_AVAILABLE_juce_audio_formats            1
#define JUCE_MODULE_AVAILABLE_juce_audio_plugin_client      1
#define JUCE_MODULE_AVAILABLE_juce_audio_processors         1
#define JUCE_MODULE_AVAILABLE_juce_audio_utils              1
#define JUCE_MODULE_AVAILABLE_juce_core                     1
#define JUCE_MODULE_AVAILABLE_juce_cryptography             1
#define JUCE_MODULE_AVAILABLE_juce_data_structures          1
#define JUCE_MODULE_AVAILABLE_juce_events                   1
#define JUCE_MODULE_AVAILABLE_juce_graphics                 1
#define JUCE_MODULE_AVAILABLE_juce_gui_basics               1
#define JUCE_MODULE_AVAILABLE_juce_gui_extra                1
#define JUCE_MODULE_AVAILABLE_juce_opengl                   1
#define JUCE_MODULE_AVAILABLE_juce_video                    1

//==============================================================================
// juce_audio_devices flags:

//#define  JUCE_ASIO
//#define  JUCE_WASAPI
//#define  JUCE_DIRECTSOUND
//#define  JUCE_ALSA
//#define  JUCE_JACK
//#define  JUCE_USE_ANDROID_OPENSLES
//#define  JUCE_USE_CDREADER
//#define  JUCE_USE_CDBURNER

//==============================================================================
// juce_audio_formats flags:

//#define  JUCE_USE_FLAC
//#define  JUCE_USE_OGGVORBIS
//#define  JUCE_USE_MP3AUDIOFORMAT
//#define  JUCE_USE_WINDOWS_MEDIA_FORMAT

//==============================================================================
// juce_audio_processors flags:

//#define  JUCE_PLUGINHOST_VST
//#define  JUCE_PLUGINHOST_AU

//==============================================================================
// juce_core flags:

//#define  JUCE_FORCE_DEBUG
//#define  JUCE_LOG_ASSERTIONS
//#define  JUCE_CHECK_MEMORY_LEAKS
//#define  JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES

//==============================================================================
// juce_graphics flags:

//#define  JUCE_USE_COREIMAGE_LOADER
//#define  JUCE_USE_DIRECTWRITE

//==============================================================================
// juce_gui_basics flags:

//#define  JUCE_ENABLE_REPAINT_DEBUGGING
//#define  JUCE_USE_XSHM
//#define  JUCE_USE_XRENDER
//#define  JUCE_USE_XCURSOR

//==============================================================================
// juce_gui_extra flags:

//#define  JUCE_WEB_BROWSER

//==============================================================================
// juce_video flags:

//#define  JUCE_DIRECTSHOW
//#define  JUCE_MEDIAFOUNDATION
#define    JUCE_QUICKTIME 0
//#define  JUCE_USE_CAMERA

//==============================================================================
// Audio plugin settings..

#define JucePlugin_Build_VST    0
#define JucePlugin_Build_AU     1
#define JucePlugin_Build_RTAS   0

#define JucePlugin_Name                 "MoogLadders"
#define JucePlugin_Desc                 "MoogLadders"
#define JucePlugin_Manufacturer         "dimitri diakopoulos"
#define JucePlugin_ManufacturerCode     'nqst'
#define JucePlugin_PluginCode           'Plug'
#define JucePlugin_MaxNumInputChannels  2
#define JucePlugin_MaxNumOutputChannels 2
#define JucePlugin_PreferredChannelConfigurations   {1, 1}, {2, 2}
#define JucePlugin_IsSynth              0
#define JucePlugin_WantsMidiInput       0
#define JucePlugin_ProducesMidiOutput   0
#define JucePlugin_SilenceInProducesSilenceOut  0
#define JucePlugin_TailLengthSeconds    0
#define JucePlugin_EditorRequiresKeyboardFocus  0
#define JucePlugin_VersionCode          0x1
#define JucePlugin_VersionString        "0.0.1"
#define JucePlugin_VSTUniqueID          JucePlugin_PluginCode
#define JucePlugin_VSTCategory          kPlugCategEffect
#define JucePlugin_AUMainType           kAudioUnitType_Effect
#define JucePlugin_AUSubType            JucePlugin_PluginCode
#define JucePlugin_AUExportPrefix       MoogLaddersAU
#define JucePlugin_AUExportPrefixQuoted "MoogLaddersAU"
#define JucePlugin_AUManufacturerCode   JucePlugin_ManufacturerCode
#define JucePlugin_CFBundleIdentifier   com.dd.ml
#define JucePlugin_AUCocoaViewClassName MoogLaddersAU_V1
#define JucePlugin_RTASCategory         ePlugInCategory_None
#define JucePlugin_RTASManufacturerCode JucePlugin_ManufacturerCode
#define JucePlugin_RTASProductId        JucePlugin_PluginCode

#endif  // __JUCE_APPCONFIG_QYBE1O__
