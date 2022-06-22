cmake_minimum_required(VERSION 3.15)

project(TubeMoped VERSION 1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin/Mac)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/bin/Mac)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/bin/Mac)
else()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin/Win)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/bin/Win)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/bin/Win)
endif()

add_subdirectory(dependencies)

juce_add_plugin(TubeMopedPlugin
        PLUGIN_MANUFACTURER_CODE kdmp
        PLUGIN_CODE kdtm
        FORMATS VST3 AU
        BUNDLE_ID com.kdsmoped.tubemoped
        PRODUCT_NAME "Tube Moped"
        COMPANY_NAME "KDsMoped"
        )

target_compile_features(TubeMopedPlugin PUBLIC cxx_std_17)

target_include_directories(TubeMopedPlugin PRIVATE include)

target_sources(TubeMopedPlugin
        PRIVATE
            source/Editor.cpp
            source/Processor.cpp
            source/MainComponent.cpp
        )

target_compile_definitions(TubeMopedPlugin PUBLIC
            JUCE_WEB_BROWSER=0
            JUCE_USE_CURL=0
            JUCE_DISPLAY_SPLASH_SCREEN=0
            JUCE_VST3_CAN_REPLACE_VST2=0
        )

target_link_libraries(TubeMopedPlugin
        PRIVATE
            juce::juce_audio_utils
            juce::juce_recommended_config_flags
            juce::juce_recommended_lto_flags
            juce::juce_recommended_warning_flags
        )
