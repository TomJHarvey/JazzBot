//
//  ChordUtility.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 20/05/2022.
//

#include "ChordUtility.hpp"
#include "SequenceUtility.hpp"

#include <iostream>

static const std::string minor_key = "-min";
static const std::string major_key = "-maj";

static const std::size_t key_size = 4;
static const int number_of_notes = 12;
static const int beat_length = 480; // repeated definition

std::string chord_degrees[12] = {"I",
                                 "bII",
                                 "II",
                                 "bIII",
                                 "III",
                                 "IV",
                                 "bV",
                                 "V",
                                 "bVI",
                                 "VI",
                                 "bVII",
                                 "VII"};

RootNote
ChordUtility::getKey(const std::string& key)
{
    RootNote chord_type = RootNote::Invalid;
    bool is_minor_key = false;
    std::string key_str;
    std::size_t found = key.find(minor_key);
    if (found != std::string::npos)
    {
        is_minor_key = true;
    }
    else
    {
        found = key.find(major_key);
        if (found == std::string::npos)
        {
            std::cout << "Key not recognised: " << key << std::endl;
            return chord_type;
        }
    }
    key_str = key.substr(0, key.size() - key_size);
    return convertStringToRootNote(key_str, is_minor_key); // could do the minor key conver here and not in the function, as another function now uses it. It is better to minimalise functionality for each function.
}

RootNote
ChordUtility::convertStringToRootNote(const std::string& key, const bool& is_minor_key)
{
    RootNote chord_type = RootNote::Invalid;
    if (key == "A")
    {
        chord_type = RootNote::A;
    }
    else if (key == "Bb" ||
             key == "A#")
    {
        chord_type = RootNote::Bb;
    }
    else if (key == "B" ||
             key == "Cb")
    {
        chord_type = RootNote::B;
    }
    else if (key == "C")
    {
        chord_type = RootNote::C;
    }
    else if (key == "Db" ||
             key == "C#")
    {
        chord_type = RootNote::Db;
    }
    else if (key == "D")
    {
        chord_type = RootNote::D;
    }
    else if (key == "Eb" ||
             key == "D#")
    {
        chord_type = RootNote::Eb;
    }
    else if (key == "E" ||
             key == "Fb")
    {
        chord_type = RootNote::E;
    }
    else if (key == "F")
    {
        chord_type = RootNote::F;
    }
    else if (key == "Gb" ||
             key == "F#")
    {
        chord_type = RootNote::Gb;
    }
    else if (key == "G")
    {
        chord_type = RootNote::G;
    }
    else if (key == "Ab" ||
             key == "G#")
    {
        chord_type = RootNote::Ab;
    }
    else
    {
        std::cout << "key not handled: " << key << std::endl;
        chord_type = RootNote::Invalid;
    }
    
    if (is_minor_key) // add three semi tones so the relative major key is now used
    {
        int chord = static_cast<int>(chord_type) + 3;
        if (chord > 11)
        {
           chord -= 12;
        }
        chord_type = static_cast<RootNote>(chord);
    }
    return chord_type;
}

ChordsInKey
ChordUtility::getChordsInKey(const RootNote& key)
{
    ChordsInKey chords;
    int counter = 0;
    int chord = static_cast<int>(key);
    while (counter < number_of_notes)
    {
        if (chord > 11)
        {
            chord = 0;
        }
        chords.insert({static_cast<RootNote>(chord), chord_degrees[counter]});
        counter ++;
        chord ++;
    }
    return chords;
}

RootNote
ChordUtility::getChordLetter(const std::string& chord)
{
    std::string chord_str = chord;
    std::size_t slash_index = chord.find('/');
    if (slash_index != std::string::npos)
    {
        chord_str = chord_str.substr(0, slash_index);
    } // else exit?
    
    std::string chord_root_str;
    std::string chord_type;
    
    bool sharp_to_flat = false;
    if (chord_str.size() > 1 && chord_str[1] == 'b')
    {
        chord_root_str = chord_str.substr(0, 2);
        chord_type = chord_str.substr(2, chord_str.size()-2);
    }
    else if (chord_str.size() > 1 && chord_str[1] == '#')
    {
        chord_root_str = chord_str.substr(0, 2);
        chord_type = chord_str.substr(2, chord_str.size()-2);
        sharp_to_flat = true;
    }
    else
    {
        chord_root_str = chord_str.substr(0, 1);
        chord_type = chord_str.substr(1, chord_str.size()-1);
    }
    RootNote chord_root = convertStringToRootNote(chord_root_str, false);
    return chord_root;
}

std::string
ChordUtility::convertChordNameToDegree(const ChordsInKey& chords_in_key, const std::string& chord)
{
    // For each chord it will need to set its degree (e.g I VI, VII, bII, bVII) + its type (e.g min7, maj7, dom7 etc)
    // To do this it will take the current chord and split it into Note name and chord

    std::string chord_str = chord;
    std::size_t slash_index = chord.find('/');
    if (slash_index != std::string::npos)
    {
        chord_str = chord_str.substr(0, slash_index);
    } // else exit?
    
    std::string chord_root_str;
    std::string chord_type;
    
    bool sharp_to_flat = false;
    if (chord_str.size() > 1 && chord_str[1] == 'b')
    {
        chord_root_str = chord_str.substr(0, 2);
        chord_type = chord_str.substr(2, chord_str.size()-2);
    }
    else if (chord_str.size() > 1 && chord_str[1] == '#')
    {
        chord_root_str = chord_str.substr(0, 2);
        chord_type = chord_str.substr(2, chord_str.size()-2);
        sharp_to_flat = true;
    }
    else
    {
        chord_root_str = chord_str.substr(0, 1);
        chord_type = chord_str.substr(1, chord_str.size()-1);
    }
    RootNote chord_root = convertStringToRootNote(chord_root_str, false);
    
    
    std::string simplified_chord_type;
   
    if (chord_root != RootNote::Invalid)
    {
        auto it = chords_in_key.find(chord_root);
        if (it != chords_in_key.end())
        {
            if (!chord_type.empty())
            {
                simplified_chord_type = getSimplifiedChordType(chord_type);
            }
            else
            {
                simplified_chord_type = "maj";
            }

            if (!simplified_chord_type.empty())
            {
                simplified_chord_type = it->second + simplified_chord_type;
            }
        }
    } // else exit?
    return simplified_chord_type;
}

std::string
ChordUtility::getSimplifiedChordType(const std::string& chord_type)
{
    std::string chord;
    if (chord_type[0] == '-')
    {
        if (chord_type ==  "-" ||
            chord_type ==  "-7" ||
            chord_type ==  "-7911" ||
            chord_type ==  "-79" ||
            chord_type ==  "-7913" ||
            chord_type ==  "-79b")
        {
            chord = "-7";
        }
        else if (chord_type ==  "-6" ||
                 chord_type ==  "-69")
        {
            chord = "-6";
        }
                 
        else if (chord_type == "-j7" ||
                 chord_type == "-j7913" ||
                 chord_type == "-j7911#")
        {
            chord = "-j7";
        }
        else
        {
            std::cout << "Chord type not handled " << chord_type << std::endl;
        }
    }
    else if (chord_type[0] == '7')
    {
        if (chord_type ==  "7" ||
            chord_type ==  "79" ||
            chord_type ==  "7913" ||
            chord_type ==  "7911" ||
            chord_type ==  "" ||
            chord_type ==  "" ||
            chord_type ==  "" ||
            chord_type ==  "")
        {
            chord = "7";
        }
        else if (chord_type == "79b" ||
                 chord_type == "79#" ||
                 chord_type == "79b13" ||
                 chord_type == "7913b" ||
                 chord_type == "7911#" ||
                 chord_type == "79#11#" ||
                 chord_type == "79#13" ||
                 chord_type == "79b13b" ||
                 chord_type == "7alt")
        {
            chord = "7alt";
        }
        else
        {
            std::cout << "Chord type not handled " << chord_type << std::endl;
        }
    }
    else if (chord_type[0] == 'j')
    {
        if (chord_type ==  "j7" ||
            chord_type ==  "j79")
        {
            chord = "j7";
        }
        else if (chord_type ==  "j79#11#" ||
                 chord_type ==  "j7911#" ||
                 chord_type ==  "j79#")
        {
            chord = "j7#11";
        }
        else
        {
            std::cout << "Chord type not handled " << chord_type << std::endl;
        }
    }
    else if (chord_type[0] == '+')
    {
        if (chord_type ==  "+" ||
            chord_type ==  "+7" ||
            chord_type ==  "+79#" ||
            chord_type ==  "+7911#" ||
            chord_type ==  "+79b" ||
            chord_type ==  "+j7" ||
            chord_type ==  "+79")
        {
            chord = "+";
        }
        else
        {
            std::cout << "Chord type not handled " << chord_type << std::endl;
        }
    }
    else if (chord_type[0] == '6')
    {
        if (chord_type ==  "6" ||
            chord_type ==  "69")
        {
            chord = "6"; // could try maj7 too lets see with time
        }
        else if (chord_type ==  "6911#")
        {
            chord = "j7#11";
        }
        else
        {
            std::cout << "Chord type not handled " << chord_type << std::endl;
        }
    }
    else if (chord_type[0] == 'o')
    {
        if (chord_type ==  "o" ||
            chord_type ==  "o7")
        {
            chord = "dim";
        }
        else
        {
            std::cout << "Chord type not handled " << chord_type << std::endl;
        }
    }
    else if (chord_type == "m7b5")
    {
        chord = chord_type;
    }
    
    else if (chord_type.find("sus") == 0)
    {
        if (chord_type ==  "sus" ||
            chord_type ==  "sus7" ||
            chord_type ==  "sus79" ||
            chord_type ==  "sus7913")
        {
            chord = "sus";
        }
        else
        {
            std::cout << "Chord type not handled " << chord_type << std::endl;
        }
    }
    else // Why are there empty chords?
    {
        //std::cout << chord_type << std::endl;
    }
    return chord;
}

bool
ChordUtility::parseChordSequence(std::string& chord_sequence_str,
                                 ChordSequence& chord_sequence,
                                 const TimeSignature& time_signature,
                                 const RootNote& key,
                                 const juce::String& file_name)
{
    ChordsInKey chords_in_key = ChordUtility::getChordsInKey(key);
    std::string current_bar;
    std::string delimiter = "|";
    std::size_t pos, bar_number = 0;
    const int bar_length = static_cast<int>(time_signature) * beat_length;
    while ((pos = chord_sequence_str.find(delimiter)) != std::string::npos)
    {
        // Set the current bar from the beggining of the string to the next delimiter '|'
        // As the string is erased each time then the starting position will always be 0
        current_bar = chord_sequence_str.substr(0, pos);
        if (!current_bar.empty() &&
            std::find(current_bar.begin(), current_bar.end(), ':') == current_bar.end())
        {
            bool found_chord = false;
            if (isupper(current_bar[0])) // Check first character of the bar is a chord
            {
                found_chord = true;
            }
            else if (current_bar[0] == 'N') // Check that the chord is not N.C
            {
                // std::cout << file.getFileName() << ": Does not handle N.C chord at bar number " << bar_number << std::endl;
                return false;
            }
            else
            {
                std::cout <<  file_name << ": Rest before chord not Handled" << std::endl;
                return false;
            }
            int total_beats = 0;
            if (!setChordsInCurrentBar(current_bar,
                                       chord_sequence,
                                       found_chord,
                                       chords_in_key,
                                       bar_length,
                                       bar_number,
                                       total_beats))
            {
                return false;
            }
            if (total_beats != static_cast<int>(time_signature))
            {
                std::cout << file_name << ": Has incorrect number of beats at bar number " << bar_number << std::endl;
                chord_sequence.clear();
                return false;
            }
            bar_number ++;
        }
        chord_sequence_str.erase(0, pos + delimiter.length());
    }
    bar_number --; // it is increased at the end of the above loop, so needs to be set back down
    return (checkLastBarHasCorrectMidiTickValue(chord_sequence, bar_number, bar_length));
}

bool
ChordUtility::setChordsInCurrentBar(const std::string& current_bar,
                                       ChordSequence& chord_sequence,
                                       const bool& found_chord,
                                       const ChordsInKey& chords_in_key,
                                       const int& bar_length,
                                       std::size_t& bar_number,
                                       int& total_beats)
{
    int previous_number_of_beats = 0;
    std::size_t current_chord_position = 0; // maybe not 0.
    for (std::size_t i = 0; i < current_bar.length(); i++)
    {
        if (current_bar[i] == 'N')
        {
            //std::cout << file.getFileName() << ": Does not handle N.C chord at bar number " << bar_number << std::endl;
            chord_sequence.clear();
            return false;
        }
        else if (isupper(current_bar[i]))
        {
            if (found_chord)
            {
                std::size_t bar_position = i+1;
                std::size_t number_of_beats = findChord(current_bar, bar_position);
                std::string chord;
                if (number_of_beats != 0)
                {
                    chord = current_bar.substr(current_chord_position, bar_position - current_chord_position - 1);
                    chord.erase(remove(chord.begin(), chord.end(), ' '), chord.end());
                }
                else
                {
                    chord = current_bar.substr(current_chord_position, bar_position - current_chord_position);
                }
                std::string chord_degree = ChordUtility::convertChordNameToDegree(chords_in_key, chord);
                int chord_position = (bar_number * bar_length) + (previous_number_of_beats * beat_length);
                //std::cout << "Bar number = " << bar_number << " - Position = " << chord_position << " chord = " << chord << " degree = " << chord_degree << std::endl;
                number_of_beats += 1;
                previous_number_of_beats += number_of_beats;
                total_beats += number_of_beats;
                chord_sequence.push_back({chord, chord_position, bar_number, chord_degree});
                current_chord_position = bar_position;
                i = bar_position - 1;
            }
            // Else would be reached if a chord is found and the bar did not start with a chord as the first character.
        }
    }
    return true;
}

bool
ChordUtility::checkLastBarHasCorrectMidiTickValue(ChordSequence& chord_sequence,
                                                     const std::size_t& bar_number,
                                                     const int& bar_length)
{
    // Checks to see if the midi tick values line up with the amount of bars.
    // There is still a chance that they're misaallihned and realligned in the middle,
    // however that is a very small chance, so this test will suffice.
    
    if (chord_sequence.size() > 1 &&
        chord_sequence[0].m_bar_number != chord_sequence[chord_sequence.size()-1].m_bar_number)
    {
        std::size_t last_bar_position = findLastBar(chord_sequence, bar_number);
        if ((chord_sequence[last_bar_position].m_chord_position != 0) &&
            (chord_sequence[last_bar_position].m_chord_position /
             chord_sequence[last_bar_position].m_bar_number) != bar_length)
        {
            chord_sequence.clear();
            std::cout << "Midi tick values for chords mis alligned" << std::endl;
            return false;
        }
    }
    return true;
}

std::size_t
ChordUtility::findLastBar(const ChordSequence& chord_sequence, const std::size_t& bar_number)
{
    for (std::size_t index = chord_sequence.size() -1; index >= 0; index --)
    {
        if (chord_sequence[index].m_bar_number != bar_number) // we don't want - 1, we will do that before entering function in this case
        {
            return (index +1);
        }
    }
    return 0;
}

std::size_t
ChordUtility::findChord(const std::string& current_bar, std::size_t& bar_position)
{
    std::size_t number_of_beats = 0;
    while (bar_position < current_bar.length())
    {
        if (current_bar[bar_position] == ' ')
        {
            // count the number of beats
            number_of_beats ++;
        }
        else if (isupper(current_bar[bar_position]))
        {
            return number_of_beats;
        }
        else if(current_bar[bar_position] == '/')
        {
            if (isupper(current_bar[bar_position+1]))
            {
                bar_position +=2;
                return findChord(current_bar, bar_position); // ignore slash enter function again
            }
            else
            {
                std::cout << "Invalid Slash chord format " << std::endl;
            }
        }
        bar_position ++;
    }
    return number_of_beats;
}

std::string
ChordUtility::findChordForNote(double& note_on, const ChordSequence& chord_sequence, const bool& next_chord)
{
    std::size_t last_bar =
        findLastBar(chord_sequence, chord_sequence[chord_sequence.size()-1].m_bar_number);
    
    // if the current note is not in the first time round of the chord sequence
    if (note_on > chord_sequence[last_bar].m_chord_position) // changed type
    {
        // find the amount of times the sequence has gone through, then minus that from the note on so it can index the chord sequence
        // if round up -
        double sequence_positioner = sequence_positioner= chord_sequence[last_bar].m_chord_position * std::floor(note_on/chord_sequence[last_bar].m_chord_position);
        note_on -= sequence_positioner;
    }
    
    auto it = std::find_if(chord_sequence.begin(),
                           chord_sequence.end(),
                           [&note_on](const Chord& chord){return chord.m_chord_position > note_on;});
    // what happens if it reaches the end?
    
    
    
    if (!next_chord)
    {
        --it;
    }
    if (it != chord_sequence.end())
    {
        return it->m_chord_degree;
    }
    else
    {
        return "NOCHORDDEGREE";
    }
}

RootNote
ChordUtility::findRootNoteForChord(double& note_on, const ChordSequence& chord_sequence, const bool& next_chord)
{
    std::size_t last_bar =
        findLastBar(chord_sequence, chord_sequence[chord_sequence.size()-1].m_bar_number);
    
    // if the current note is not in the first time round of the chord sequence
    if (note_on > chord_sequence[last_bar].m_chord_position) // changed type
    {
        // find the amount of times the sequence has gone through, then minus that from the note on so it can index the chord sequence
        // if round up -
        double sequence_positioner = sequence_positioner= chord_sequence[last_bar].m_chord_position * std::floor(note_on/chord_sequence[last_bar].m_chord_position);
        note_on -= sequence_positioner;
    }
    
    auto it = std::find_if(chord_sequence.begin(),
                           chord_sequence.end(),
                           [&note_on](const Chord& chord){return chord.m_chord_position > note_on;});
    // what happens if it reaches the end?
    it --;
    
    return ChordUtility::getChordLetter(it->m_chord);
}

RootNote
ChordUtility::convertNoteValueToRootNote(const int& note_value)
{
    float octave = (note_value / 12);
    int rounded_octave =  static_cast<int>(std::floor(octave));
    int note_degree =  note_value - (rounded_octave * 12);
    
    RootNote chord_type = RootNote::Invalid;
    switch (note_degree) {
        case 0:
            chord_type = RootNote::C;
            break;
        case 1:
            chord_type = RootNote::Db;
            break;
        case 2:
            chord_type = RootNote::D;
            break;
        case 3:
            chord_type = RootNote::Eb;
            break;
        case 4:
            chord_type = RootNote::E;
            break;
        case 5:
            chord_type = RootNote::F;
            break;
        case 6:
            chord_type = RootNote::Gb;
            break;
        case 7:
            chord_type = RootNote::G;
            break;
        case 8:
            chord_type = RootNote::Ab;
            break;
        case 9:
            chord_type = RootNote::A;
            break;
        case 10:
            chord_type = RootNote::Bb;
            break;
        case 11:
            chord_type = RootNote::B;
            break;
        default:
            break;
    }
    return chord_type;
}


int
ChordUtility::calculateRootNoteDifference(const RootNote& note_1, const RootNote& note_2)
{
    int search_note = static_cast<int>(note_1); // root for chord
    int target_note = static_cast<int>(note_2);
    int counter = 0;
    while (search_note != target_note)
    {
        search_note ++;
        counter ++;
        if (search_note > 11)
        {
            search_note -=12;
        }
    }
    return counter;
}
