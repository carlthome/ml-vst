cmake_minimum_required(VERSION 3.16)
project(AudioPlugin VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(
    repos/tensorflow/tensorflow/lite
    "${CMAKE_CURRENT_BINARY_DIR}/tensorflow-lite"
    EXCLUDE_FROM_ALL
)
add_subdirectory(repos/JUCE)

juce_add_gui_app(AudioPlugin_Derp PRODUCT_NAME "AudioPlugin")

target_compile_definitions(AudioPlugin_Derp
    PRIVATE
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0
    JUCE_BUILD_EXAMPLES=OFF
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:GuiAppExample,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:GuiAppExample,JUCE_VERSION>")

target_sources(AudioPlugin_Derp
    PRIVATE
    AudioPlugin/Main.cpp
    AudioPlugin/MainComponent.cpp)

target_include_directories(AudioPlugin_Derp
    PRIVATE
    ${GTK3_INCLUDEDIR})

target_link_libraries(AudioPlugin_Derp
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

    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)