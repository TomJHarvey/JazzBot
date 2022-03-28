#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/xcode
  make -f /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/xcode
  make -f /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/xcode
  make -f /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/xcode
  make -f /Users/tom-harvey/dev/JazzBot/MidiFileGenerator/xcode/CMakeScripts/ReRunCMake.make
fi

