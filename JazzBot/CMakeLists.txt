# Example GUI App CMakeLists.txt

cmake_minimum_required(VERSION 3.12)
project(JAZZBOT VERSION 0.0.1)
add_subdirectory(JUCE) # If you've put JUCE in a subdirectory called JUCE

# `juce_add_gui_app` adds an executable target with the name passed as the first argument
# (JazzBotGui here). This target is a normal CMake target, but has a lot of extra properties set
# up by default. This function accepts many optional arguments. Check the readme at
# `docs/CMake API.md` in the JUCE repo for the full list.

juce_add_gui_app(JazzBotGui
    # VERSION ...                       # Set this if the app version is different to the project version
    # ICON_BIG ...                      # ICON_* arguments specify a path to an image file to use as an icon
    # ICON_SMALL ...
    # DOCUMENT_EXTENSIONS ...           # Specify file extensions that should be associated with this app
    # COMPANY_NAME ...                  # Specify the name of the app's author
    PRODUCT_NAME "jazzbot gui")     # The name of the final executable, which can differ from the target name
    
juce_add_console_app(JazzBotCommandLine
    PRODUCT_NAME "Console App Example")     # The name of the final executable, which can differ from the target name

set(GUI_SOURCES SourceCode/GuiApp/CustomViewport.cpp
                SourceCode/GuiApp/GuiNote.cpp
                SourceCode/GuiApp/MainComponent.cpp
                SourceCode/GuiApp/MainMenu.cpp
                SourceCode/GuiApp/MidiSequenceView.cpp
                SourceCode/GuiApp/PianoRoll.cpp
                SourceCode/GuiApp/TrainingDataView.cpp)
set(CMD_LINE_SOURCES SourceCode/CommandLineApp/CommandLineApplication.cpp)
set(COMMON_SOURCES SourceCode/main.cpp
                   SourceCode/NoteGrouping/EighthNotes.cpp
                   SourceCode/NoteGrouping/NoteGrouping.cpp
                   SourceCode/Utility/ChordParsingUtility.cpp
                   SourceCode/Utility/SequenceUtility.cpp)
# set(COMMON_DIRECTORIES source_code/midi) 

#Useful to keep for future on how to include common directories
#target_include_directories(JazzBotGui PUBLIC source_code/gui_app ${COMMON_DIRECTORIES})
#target_include_directories(JazzBotCommandLine PUBLIC ${COMMON_DIRECTORIES})


target_include_directories(JazzBotGui PUBLIC SourceCode/GuiApp)

target_sources(JazzBotGui
    PRIVATE
        ${GUI_SOURCES}
        ${COMMON_SOURCES})

target_sources(JazzBotCommandLine
    PRIVATE
            ${CMD_LINE_SOURCES}
            ${COMMON_SOURCES})

set(PROJECT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(MIDI_FILES_DIRECTORY ${CMAKE_SOURCE_DIR}../midi_files)
set(CHORD_CHANGES_DIRECTORY ${CMAKE_SOURCE_DIR}../DatabaseParser/chord_changes)
set(SONG_INFORMATION_DIRECTORY ${CMAKE_SOURCE_DIR}../DatabaseParser/song_information)

target_compile_definitions(JazzBotGui
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:JazzBotGui,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:JazzBotGui,JUCE_VERSION>"
        GUI_APP=1
        "PROJECT_DIRECTORY=\"${PROJECT_DIRECTORY}\""
        "MIDI_FILES_DIRECTORY=\"${MIDI_FILES_DIRECTORY}\""
        "CHORD_CHANGES_DIRECTORY=\"${CHORD_CHANGES_DIRECTORY}\""
        "SONG_INFORMATION_DIRECTORY=\"${SONG_INFORMATION_DIRECTORY}\"")
        
target_compile_definitions(JazzBotCommandLine
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_console_app` call
        JUCE_USE_CURL=0    # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_console_app` call
        "PROJECT_DIRECTORY=\"${PROJECT_DIRECTORY}\"")
        
        
        
set (LIB_DIR_LOCATION ${NAME}/lib)
find_library(SQLite3 NAMES sqlite3 PATHS ${LIB_DIR_LOCATION})
set(INCLUDE_DIRS ${NAME}/include)
 
target_include_directories(JazzBotGui PRIVATE ${INCLUDE_DIRS})
        

target_link_libraries(JazzBotGui
    PRIVATE
        # GuiApp            # If we'd created a binary data target, we'd link to it here
        juce::juce_gui_extra
        juce::juce_audio_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        SQLite3)
        
target_link_libraries(JazzBotCommandLine
    PRIVATE
        # CommandLineApp            # If you'd created a binary data target, you'd link to it here
        juce::juce_core
        juce::juce_audio_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)
    
        
find_package(Catch2 3 REQUIRED)
add_executable(tests tests/ChordSequenceParsingTests.cpp
                     tests/EighthNoteGroupingDetectionTests.cpp
                     tests/EighthNoteGroupingGenerationTests.cpp
                     SourceCode/NoteGrouping/EighthNotes.cpp
                     SourceCode/NoteGrouping/NoteGrouping.cpp
                     SourceCode/Utility/ChordParsingUtility.cpp
                     SourceCode/Utility/SequenceUtility.cpp)

target_link_libraries(tests
    PRIVATE
        Catch2::Catch2WithMain
        juce::juce_core
        juce::juce_audio_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)
