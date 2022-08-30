import os.path
import requests
import re
from urllib.request import urlopen
from urllib.request import urlretrieve
import cgi
from bs4 import BeautifulSoup

head_url = 'https://jazzomat.hfm-weimar.de/dbformat/'  # this may be the convention for other websites to open sublinks
database_url = 'https://jazzomat.hfm-weimar.de/dbformat/dbcontent.html'
audio_labs_head_url = 'http://mir.audiolabs.uni-erlangen.de/'  # this is pre written, can write another way to find this

def parse_database():

    # get database page as text
    html_text = requests.get(database_url).text

    # creates a soup object containing the page
    database_soup = BeautifulSoup(html_text, 'html.parser')
    # download_csv_files(database_soup)
    downdown_song_information(database_soup) # also writes the chord sequence


    #download_scores(database_soup) # there is a certificate issue with this, not needed anyway at the moment
    #download_midi(database_soup) # need to check midifiles, again not needed


def download_csv_files(database_soup):
    for link in database_soup.find_all(href=re.compile("http://mir.audiolabs.uni-erlangen.de/jazztube/solos/solo")):
        html_text = requests.get(link['href']).text

        # creates a soup object containing the sub link
        song_info_soup = BeautifulSoup(html_text, 'html.parser')

        for solo_link in song_info_soup.find_all(href=re.compile("/jazztube/api/v1.0/export/solo")):
            url = (audio_labs_head_url + (solo_link['href']))
            write_file(url, get_file_name(url), 'notes')

        for solo_link in song_info_soup.find_all(href=re.compile("/jazztube/api/v1.0/export/beat")):
            url = (audio_labs_head_url + (solo_link['href']))
            write_file(url, get_file_name(url), 'beats')


def downdown_song_information(database_soup):
    for link in database_soup.find_all(href=re.compile("synopsis/solo")):
        html_text = requests.get('https://jazzomat.hfm-weimar.de/dbformat/' + link['href']).text
        # creates a soup object containing the sub link
        song_synopsis_soup = BeautifulSoup(html_text, 'html.parser')

        # figure out how to interpret the speech marks not as weird characters
        file_name = ''
        for song_name_line in song_synopsis_soup.find_all('iframe', src=re.compile("scores/")):
            file_name = song_name_line['src']
            break

        file_name = file_name.replace('scores/', '')
        chords_file_name = file_name.replace('_FINAL.pdf', '_chord_changes.txt')

        # this should be okay, may need to check some of the files if there is another occurrence of the highlight line
        for div in song_synopsis_soup.find_all('div', attrs={"class": "highlight-default notranslate"}):
            write_text_to_file(div.text, chords_file_name, "../chord_changes")

        table = song_synopsis_soup.find('table')
        rows = table.find_all('tr')
        song_info = []
        for row in rows:
            cols = row.find_all('td')
            song_info.append(cols[1].get_text())

        song_info_line = file_name.replace('_FINAL.pdf', '')

        instrument = "no_instrument"
        key = "no_key"
        time_signature = "no_time_signature"

        if len(song_info[0]) != 0:
            instrument = song_info[0]
        else:
            print(instrument)
        if len(song_info[8]) != 0:
            key = song_info[8]
        else:
            print(key)
        if len(song_info[9]) != 0:
            time_signature = song_info[9]
        else:
            print(time_signature)

        song_info_to_write = song_info_line + '\n' + instrument + '\n' + key + '\n' + time_signature + '\n'
        song_info_file_name = file_name.replace('_FINAL.pdf', '_song_information.txt')

        write_text_to_file(song_info_to_write, song_info_file_name, "../song_information")


# def download_midi(database_soup):
#     for link in database_soup.find_all(href=re.compile("synopsis/solo")):
#         html_text = requests.get('https://jazzomat.hfm-weimar.de/dbformat/' + link['href']).text
#         # creates a soup object containing the sub link
#         song_synopsis_soup = BeautifulSoup(html_text, 'html.parser')
#
#         # this nested structure is also used above, this can be refactored
#         for midi_file_link in song_synopsis_soup.find_all(href=re.compile("midi/")):
#             url = (audio_labs_head_url + (midi_file_link['href']))
#             file_name = url.rsplit('/', 1)[-1]  # figure out how to get the file name, why is this different?
#             write_file(url, file_name, 'midi_files')
#
# def download_scores(database_soup):
#     for link in database_soup.find_all(href=re.compile("synopsis/scores")):
#         url = (head_url + (link['href']))
#         write_file(url, get_file_name(url), 'scores')  # this isn't safe always, look into urllib when improving project


def get_file_name(url):
    remotefile = urlopen(url)
    file_info = remotefile.info()['Content-Disposition']
    value, params = cgi.parse_header(file_info)
    filename = params["filename"]
    # urlretrieve(url, filename)  #§ this might have fixed the issue...
    print(filename)
    return str(filename)


def write_text_to_file(content, file_name, sub_directory):
    try:
        os.mkdir(sub_directory)
    except Exception:
        pass
    file_path = os.path.join(sub_directory, file_name)
    with open(file_path, 'w') as f:
        print('writing file: ' + file_path)
        f.write(content)


# this will need to be re written to write different types of files
def write_file(url, file_name, sub_directory):
    try:
        os.mkdir(sub_directory)
    except Exception:
        pass

    # send a HTTP request to the server and save
    # the HTTP response in a response object called r
    file_path = os.path.join(sub_directory, file_name)
    if not os.path.isfile(file_path):
        r = requests.get(url)  # create HTTP response object
        with open(file_path, 'wb') as f:
            # Saving received content as a png file in
            # binary format

            # write the contents of the response (r.content)
            # to a new file in binary mode.
            print ('writing file: ' + file_path)
            f.write(r.content)


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    parse_database()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
