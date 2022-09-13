# JazzBot

This project consists of 3 sub projects :

MidiFileBpmCorrector
A C++ application which takes time information in seconds for the onsets and durations for notes in an improvised jazz solo. These solos weren't played to a metronome so there are fluctuations in tempo. This project works out the midi tick onsets and durations for the notes by looking at each beat separately. This enables the midi tick information to be quantised so the whole solo can be played in a consistent tempo. As players don't often play exactly in time as the tempo fluctates, this conversion isn't entirely accurate. It is just used as a starting point to make the data more useable and quantifiable.

DatabaseParser:
A python application built specificly to go through a jazz solo database and save the relevant information into files to be used for this project.

JazzBot:
A c++ application used to generate MIDI jazz melodies. For more information see project_summary.pdf. 


 
