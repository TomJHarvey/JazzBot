//
//  ChordUtility.cpp
//  JazzBotGui
//
//  Created by Tom Harvey on 20/05/2022.
//

#include "ChordUtility.hpp"

#include <iostream>


static const std::string minor_key = "-min";
static const std::string major_key = "-maj";

static const std::size_t key_size = 4;
static const int number_of_notes = 12;

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

ChordRoot
ChordUtility::getKey(const std::string& key)
{
    // Set a bool to test if the key is minor or major.
    ChordRoot chord_type = ChordRoot::Invalid;
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
    return convertStringToChordRoot(key_str, is_minor_key); // could do the minor key conver here and not in the function, as another function now uses it. It is better to minimalise functionality for each function.
}

ChordRoot
ChordUtility::convertStringToChordRoot(const std::string& key, const bool& is_minor_key)
{
    ChordRoot chord_type = ChordRoot::Invalid;
    // Strip the key to remove minor or major so its just left with the notes.
    if (key == "A")
    {
        chord_type = ChordRoot::A;
    }
    else if (key == "Bb" ||
             key == "A#")
    {
        chord_type = ChordRoot::Bb;
    }
    else if (key == "B" ||
             key == "Cb")
    {
        chord_type = ChordRoot::B;
    }
    else if (key == "C")
    {
        chord_type = ChordRoot::C;
    }
    else if (key == "Db" ||
             key == "C#")
    {
        chord_type = ChordRoot::Db;
    }
    else if (key == "D")
    {
        chord_type = ChordRoot::D;
    }
    else if (key == "Eb" ||
             key == "D#")
    {
        chord_type = ChordRoot::Eb;
    }
    else if (key == "E" ||
             key == "Fb")
    {
        chord_type = ChordRoot::E;
    }
    else if (key == "F")
    {
        chord_type = ChordRoot::F;
    }
    else if (key == "Gb" ||
             key == "F#")
    {
        chord_type = ChordRoot::Gb;
    }
    else if (key == "G")
    {
        chord_type = ChordRoot::G;
    }
    else if (key == "Ab" ||
             key == "G#")
    {
        chord_type = ChordRoot::Ab;
    }
    else
    {
        std::cout << "key not handled: " << key << std::endl;
        chord_type = ChordRoot::Invalid;
    }
    
    if (is_minor_key)
    {
        int chord = static_cast<int>(chord_type) + 3;
        if (chord > 11)
        {
           chord -= 12;
        }
        chord_type = static_cast<ChordRoot>(chord);
    }
    return chord_type;
}

ChordsInKey
ChordUtility::getChordsInKey(const ChordRoot& key)
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
        chords.insert({static_cast<ChordRoot>(chord), chord_degrees[counter]});
        counter ++;
        chord ++;
    }
    return chords;
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
    
    
    // What about Cb how is that dealt with.
    
    std::string chord_root_str; // this bit could maybe be further split into functions, lets see.
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

    // sharp_to_flat
    ChordRoot chord_root = convertStringToChordRoot(chord_root_str, false);
    
    std::string simplified_chord_type;
    if (chord_root != ChordRoot::Invalid)
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
