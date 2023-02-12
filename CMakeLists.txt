cmake_minimum_required(VERSION 3.16)
project(AudioPlugins VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(TFLITE_C_BUILD_SHARED_LIBS OFF)
set(TFLITE_ENABLE_MMAP OFF)
set(TFLITE_ENABLE_EXTERNAL_DELEGATE OFF)

add_subdirectory("repos/JUCE")
add_subdirectory("repos/tensorflow/tensorflow/lite" "${CMAKE_CURRENT_BINARY_DIR}/tensorflow-lite" EXCLUDE_FROM_ALL)

# TODO
# juce_set_vst2_sdk_path(...)
# juce_set_aax_sdk_path(...)
 
juce_add_plugin(AudioPlugin
    ICON_BIG "src/Assets/icon.png"
    ICON_SMALL "src/Assets/icon.png"
    COMPANY_NAME ${PROJECT_NAME}
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT TRUE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Carl
    PLUGIN_CODE Dem0
    FORMATS Standalone
    PRODUCT_NAME "The Audio Plugin"
)
target_compile_features(AudioPlugin
    PRIVATE
    cxx_std_20
)

target_compile_definitions(AudioPlugin
    PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0)

file(GLOB SRC_FILES "src/AudioPlugin/*")
target_sources(AudioPlugin PRIVATE ${SRC_FILES})

file(GLOB MODEL_FILES "src/Assets/models/*")
juce_add_binary_data(Models SOURCES ${MODEL_FILES})
set_target_properties(Models PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

target_link_libraries(AudioPlugin
    PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_gui_extra
    tensorflow-lite
    Models
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

# TODO Enable tests.
#add_executable(Tests ${TestFiles})
#target_compile_features(Tests PRIVATE cxx_std_20)
