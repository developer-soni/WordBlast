/**************************************************************
* Project: Assignment 4 – Word Blast
*
* File: os4_main.c
*
* Description: This file reads a .txt file (WarandPeace.txt)
* and count and tally each of the words that are 6 or more
* characters long using threads to perform operations in parallel
* and prints top 10 most frequent words(>6 letters).
*
**************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define FREQ 2000  //number of words to hold in the array
#define MINCHARS 6 //minimum characters requirement
#define TOP 10     //top 10 words

// You may find this Useful
char *delim = "\"\'.“”‘’?:;-,—*($%)! \t\n\x0A\r";

int count = 0; //number of elements in array
int rem = 0;   //used for last pthread
int flag = 0;  //used to init words array
int fileDesc, chunkSize;

pthread_mutex_t lock;

typedef struct Pair //data structure to store each word and its frequency
{
    char *word;
    int count;
} Pair;

struct Pair words[FREQ]; //array to store all words from the file

void initWords() //initializes the array for words
{
    for (int i = 0; i < FREQ; i++)
    {
        words[i].word = malloc(TOP);
        words[i].count = 0;
    }
    flag = 1;
}

void *addWord(void *p) //called by each thread to process the chunks
{
    int compare;
    char *buf, *eachWord;

    buf = malloc(chunkSize + rem); //allocates the buffer
    if (buf == NULL)               //malloc error check
    {
        printf("ERROR allocating buffer");
        return NULL;
    }

    read(fileDesc, buf, chunkSize + rem); //reads file into buffer

    //strtok_r is thread safe and tokenizes words and loops through the buffer
    while ((eachWord = strtok_r(buf, delim, &buf)))
    {
        if (strlen(eachWord) >= MINCHARS) //only storing words > 6 letters
        {
            for (int i = 0; i < FREQ; i++) //checking if word is already in words(array)
            {
                compare = strcasecmp(words[i].word, eachWord);
                if (compare == 0) //increase count
                {
                    pthread_mutex_lock(&lock);
                    words[i].count++;
                    pthread_mutex_unlock(&lock);
                    break;
                }
            }

            if (compare != 0 && count < FREQ) //checking if word is not in words(array)
            {                                 //add word and increase count
                pthread_mutex_lock(&lock);
                strcpy(words[count].word, eachWord);
                words[count].count++;
                pthread_mutex_unlock(&lock);
                count++;
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    //***TO DO***  Look at arguments, open file, divide by threads
    //             Allocate and Initialize and storage structures

    int fileSize, threadCount;
    Pair temp;
    char *filename;
    filename = argv[1];

    if (argc > 2) //number of threads to process from args
    {
        threadCount = atoi(argv[2]);
    }

    if (flag == 0) //initializes words array
    {
        initWords();
    }

    if (pthread_mutex_init(&lock, NULL) != 0) //initializes mutex lock
    {
        printf("ERROR: Mutex init failed\n");
        return 1;
    }

    fileDesc = open(filename, O_RDONLY);     //opening the file
    fileSize = lseek(fileDesc, 0, SEEK_END); //reading from start point
    lseek(fileDesc, 0, SEEK_SET);            //setting file position to beginning for next read
    chunkSize = fileSize / threadCount;      //dividing file size by number of threads into chunks

    //**************************************************************
    // DO NOT CHANGE THIS BLOCK
    //Time stamp start
    struct timespec startTime;
    struct timespec endTime;

    clock_gettime(CLOCK_REALTIME, &startTime);
    //**************************************************************
    // *** TO DO ***  start your thread processing
    //                wait for the threads to finish

    pthread_t thread[threadCount]; //declaring pthread and array of threadCount

    for (int i = 0; i < threadCount; i++) //creating threads to run in parallel
    {
        if (i == threadCount - 1) //last thread adjustment
        {
            rem = fileSize % threadCount;
        }
        pthread_create(&thread[i], NULL, addWord, (void *)&i); //creates thread
    }

    for (int i = 0; i < threadCount; i++) //wait for thread to finish
    {
        pthread_join(thread[i], NULL);
    }

    // ***TO DO *** Process TOP 10 and display

    for (int i = 0; i < FREQ; i++) //sorting the words in decreasing order of frequency
    {
        for (int j = i + 1; j < FREQ; j++)
        {
            if (words[i].count < words[j].count)
            {
                temp = words[i];
                words[i] = words[j];
                words[j] = temp;
            }
        }
    }

    printf("\nWord Frequency Count on %s with %d threads\n", filename, threadCount);
    printf("Printing top %d words %d characters or more.\n", TOP, MINCHARS);

    for (int i = 0; i < TOP; i++) //printing top 10 words
    {
        printf("Number %d is %s with a count of %d\n", i + 1, words[i].word, words[i].count);
    }

    //**************************************************************
    // DO NOT CHANGE THIS BLOCK
    //Clock output
    clock_gettime(CLOCK_REALTIME, &endTime);
    time_t sec = endTime.tv_sec - startTime.tv_sec;
    long n_sec = endTime.tv_nsec - startTime.tv_nsec;
    if (endTime.tv_nsec < startTime.tv_nsec)
    {
        --sec;
        n_sec = n_sec + 1000000000L;
    }

    printf("Total Time was %ld.%09ld seconds\n", sec, n_sec);
    //**************************************************************

    // ***TO DO *** cleanup

    close(fileDesc);              //closing the file
    pthread_mutex_destroy(&lock); //destroying mutex lock
    for (int i = 0; i < FREQ; i++)
    {
        free(words[i].word);
    }
    return 0;
}
