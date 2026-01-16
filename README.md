[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/QfAIDTGi)
# Making Music Great Again
 
### Lone Star

Amrit Das
       
### Project Description:

This is a music streaming project where a client can connect to a server to browse and play songs by different artists. The server stores a library of songs, and the client can choose songs based on the first letter of the artist or the artist’s name. Once a song is selected, it streams directly to the client, and the user can pause, unpause, or quit the song. Multiple clients can connect at the same time, and each client plays independently without affecting others. The project uses TCP sockets for communication, process forking to handle multiple clients, and simple command-line controls for music playback.


### Instructions:

Installation: The user will need the mpg123 program, which can be installed with **sudo apt install mpg123**. Once it's installed, you can use the make compile command to compile the program's clients and server. There is only one server, which contains the music library and should have the music files. You will also need the music files downloaded in the same repository, in mp3 format. Upon compilation you will need to run the server through ./server and, on a different terminal/client, you can run the ./client.

Once the client runs you need to input the capital letter for the first name of the artist, which will then give you the list of songs in that library. Then, input the full name of the artist as it's mentioned in the list. Next, song name as it's mentioned in the list. The music will then play and you can use p to pause(slight lag), u to unpause, and q to quit the client. You might have to hit enter after q to fully exit the client and prompt a new terminal command.

### Video link:

Video as a youtube link: https://youtu.be/TWpCAjn28Tk
Slight pause ~3:00 for slight techincal difficulty.
### Resources/ References:

mpg123 program -- **sudo apt install mpg123**
