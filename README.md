# JazzBot

The aim of this project is to create jazz melodies over fixed chord sequences by exploring the use of AI.

The project currently consists of two sub projects :

MidiFileGenerator:
A C++ application which takes time information in seconds for the bar onsets, and the note onsets and durations from an improvised jazz solo. These solos weren't played to a metronome so there are fluctuations in tempo. This project works out the midi tick onsets and durations for the notes by looking at each beat separately. This enables the midi tick information to be quantised so the whole solo can be played in a consistent tempo. As players don't often play exactly in time as the tempo fluctates, this conversion isn't entirely accurate. It is just used as a starting point to make the data more useable for the AI.

DatabaseParser:
A python application built specificly to go through a jazz solo database and save the relevant information into files to be used for this project.



 
