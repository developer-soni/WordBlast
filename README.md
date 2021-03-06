# WordBlast
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Ubuntu](https://img.shields.io/badge/Ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white)
[![Licence](https://img.shields.io/github/license/Ileriayo/markdown-badges?style=for-the-badge)](./LICENSE)

## Description
In this program we read a .txt file (WarandPeace.txt, in this case) and count and tally each of the words that are 6 or more characters long. Also made sure that only Linux file functions are used such as open, close, read, lseek and not library functions. We do this process by using threads and each thread takes a chunk of the file and processes it. It then returns the results to the main and main tallies and prints the top 10, 6 or more-character words with the highest frequencies and their tallies. In the main, I used lseek to get the file size and then divided the file size into equal chunks. I create threads and pass each of the chunks into their respective blocks and then call the pthread_create. The adding of the words is done inside a mutex lock. I made a data structure of arrays and named it Pair. This is implemented into a function addWord. This function checks if a match is found and increments the counter by 1 and if its not found then it adds it to the words array. After it is done, the top 10 words are printed each with their respective frequencies in descending order. The total time taken to perform the threading is also displayed at the end. After the process completes, cleanup is also performed to free all the memory for further usage.

# Please refer to WriteUp for more information.
