# Generated by CMake, DO NOT EDIT

TARGETS:= 
empty:= 
space:= $(empty) $(empty)
spaceplus:= $(empty)\ $(empty)

TARGETS += $(subst $(space),$(spaceplus),$(wildcard /Applications/CMake.app/Contents/share/cmake-3.20/Modules/BasicConfigVersion-ExactVersion.cmake.in))
TARGETS += $(subst $(space),$(spaceplus),$(wildcard /Applications/CMake.app/Contents/share/cmake-3.20/Modules/CMakePackageConfigHelpers.cmake))
TARGETS += $(subst $(space),$(spaceplus),$(wildcard /Applications/CMake.app/Contents/share/cmake-3.20/Modules/WriteBasicConfigVersionFile.cmake))
TARGETS += $(subst $(space),$(spaceplus),$(wildcard /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/JUCE/CMakeLists.txt))
TARGETS += $(subst $(space),$(spaceplus),$(wildcard /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/JUCE/extras/Build/CMake/JUCECheckAtomic.cmake))
TARGETS += $(subst $(space),$(spaceplus),$(wildcard /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/JUCE/extras/Build/CMake/JUCEConfig.cmake.in))
TARGETS += $(subst $(space),$(spaceplus),$(wildcard /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/JUCE/extras/Build/CMake/JUCEHelperTargets.cmake))
TARGETS += $(subst $(space),$(spaceplus),$(wildcard /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/JUCE/extras/Build/CMake/JUCEUtils.cmake))
TARGETS += $(subst $(space),$(spaceplus),$(wildcard /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/JUCE/extras/Build/CMakeLists.txt))
TARGETS += $(subst $(space),$(spaceplus),$(wildcard /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/JUCE/extras/Build/juceaide/CMakeLists.txt))
TARGETS += $(subst $(space),$(spaceplus),$(wildcard /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/JUCE/modules/CMakeLists.txt))
TARGETS += $(subst $(space),$(spaceplus),$(wildcard /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/xcode/JUCE/tools/JUCEToolsExport.cmake))
TARGETS += $(subst $(space),$(spaceplus),$(wildcard /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/xcode/CMakeFiles/cmake.verify_globs))

.NOTPARALLEL:

.PHONY: all VERIFY_GLOBS

all: VERIFY_GLOBS /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/xcode/CMakeFiles/cmake.check_cache

VERIFY_GLOBS:
	/Applications/CMake.app/Contents/bin/cmake -P /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/xcode/CMakeFiles/VerifyGlobs.cmake

/Users/tom-harvey/dev/JazzBot/MidiFileGenerator/xcode/CMakeFiles/cmake.check_cache: $(TARGETS)
	/Applications/CMake.app/Contents/bin/cmake -H/Users/tom-harvey/dev/JazzBot/MidiFileGenerator -B/Users/tom-harvey/dev/JazzBot/MidiFileGenerator/xcode
