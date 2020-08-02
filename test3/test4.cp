//
//  main.cpp
//  reverser
//
//  Created by Christian Danoy on 6/13/20.
//  Copyright © 2020 Christian Danoy. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Superpowered.h"
#include "SuperpoweredDecoder.h"
#include "SuperpoweredSimple.h"
#include "SuperpoweredRecorder.h"
#include "SuperpoweredResampler.h"
#include "SuperpoweredAnalyzer.h"
#include "SuperpoweredTimeStretching.h"

using namespace std;

// enrique sends me a file
// encoded a certain way
// JSON text

struct Node{
    short int * buffer; // add the address of the pointer of the chunk of frames to this Node
    int framesDecoded;
    Node *next;
};

// structure needs a header to start then another tail node to follow the tails.
// node is working, is the buffer working?

struct AnalyzerClass{
    float bpm;
    float peakDb;
    float loudpartsAverageDb;
    Node *head;
    short int * intBuffer;
    float * floatBuffer;
    FILE *df;
    string success = "swag!";
    // needs to have pointer to head node
    // needs to have a pointer to a intBuffer, created within decodeAnalyzeBuffer
};

// adding argument to allow for file name
int decodeAnalyzeBuffer(string soundPath, AnalyzerClass * analyzerClass, string name){
    
    Node * tail = NULL;
    Node * head = NULL;
    const char *cSoundPath = soundPath.c_str();
    string woah = "/Users/Chriskringle/Desktop/" + name + ".mp3";
    const char *variable = woah.c_str();
    Superpowered::Decoder *decoder = new Superpowered::Decoder();
    decoder->open(cSoundPath); // this will be in the function
    Superpowered::Analyzer *analyzer = new Superpowered::Analyzer(decoder->getSamplerate(), decoder->getDurationSeconds());
    FILE *destinationFile = Superpowered::createWAV(variable, decoder->getSamplerate(), 2);
         if (!destinationFile) {
             printf("\rFile creation error.\n");
             delete decoder;
             return 0;
         };
    
    Superpowered::TimeStretching *timeStretch = new Superpowered::TimeStretching(decoder->getSamplerate());
    
    short int * iBuffer = (short int *)malloc(decoder->getFramesPerChunk() * 2 * sizeof(short int) + 16384);
    
    float * fBuffer = (float *)malloc(decoder->getFramesPerChunk() * 2 * sizeof(float) + 16384);
    
    while (true) {
           int framesDecoded = decoder->decodeAudio(iBuffer, decoder->getFramesPerChunk());
           if (framesDecoded < 1) break;
        
           Node * newNode = new Node;
           newNode->buffer=iBuffer;
           newNode->framesDecoded=framesDecoded;
        /*
           tail=newNode;
           head=newNode;
           n = new Node;
           tail->next = n;
           tail = tail->next;
           newNode->next=NULL;
         */
        if(head==NULL){
            head = newNode;
            tail = newNode;
        }else{
            tail->next = newNode;
            tail = tail->next;
        }
           Superpowered::ShortIntToFloat(iBuffer, fBuffer, framesDecoded);
           analyzer->process(fBuffer, framesDecoded);
       } // end decode process
    
    analyzer->makeResults(60, 200, 0, 0, false, false, false, false, false);
    analyzerClass->bpm = analyzer->bpm;
    analyzerClass->peakDb = analyzer->peakDb;
    analyzerClass->loudpartsAverageDb = analyzer->loudpartsAverageDb;
    analyzerClass->head=head;
    // after making results we can put the analyzer structures / variables into AnalyzeClass Structure
    
    while(analyzerClass->head != NULL){
    Superpowered::writeWAV(destinationFile, analyzerClass->head->buffer, analyzerClass->head->framesDecoded * 4);
    // free(head->buffer);
    analyzerClass->head = analyzerClass->head->next;
    }
    analyzerClass->df=destinationFile;
    printf("we did it!");
    // take integer buffer in and make it into a float buffer
    // analyze the buffers
    // insert nodes
    // add info to AnalyzerClass structure
    Superpowered::closeWAV(destinationFile);
    delete timeStretch;
    delete decoder;
    delete analyzer;
    free(iBuffer);
    free(fBuffer);
    
    return 0;
    
};
// end decodeAnalyzeBuffer

int main(int argc, const char * argv[]) {
    
    Superpowered::Initialize(
                             "ExampleLicenseKey-WillExpire-OnNextUpdate",
                             true, // enableAudioAnalysis (using SuperpoweredAnalyzer, SuperpoweredLiveAnalyzer, SuperpoweredWaveform or SuperpoweredBandpassFilterbank)
                             false, // enableFFTAndFrequencyDomain (using SuperpoweredFrequencyDomain, SuperpoweredFFTComplex, SuperpoweredFFTReal or SuperpoweredPolarFFT)
                             true, // enableAudioTimeStretching (using SuperpoweredTimeStretching)
                             true, // enableAudioEffects (using any SuperpoweredFX class)
                             true, // enableAudioPlayerAndDecoder (using SuperpoweredAdvancedAudioPlayer or SuperpoweredDecoder)
                             false, // enableCryptographics (using Superpowered::RSAPublicKey, Superpowered::RSAPrivateKey, Superpowered::hasher or Superpowered::AES)
                             false  // enableNetworking (using Superpowered::httpRequest)
                             );
    
    // must instance the class to create object.
    
    string soundPath = "/Users/Chriskringle/Desktop/Gracie 2.mp3";
    string soundPath2 = "/Users/Chriskringle/Desktop/Crazy Train.mp3";
    string name = "Gracie";
    string name2 = "Crazy boy";
    // ask for path
    AnalyzerClass * analyzerClass = new AnalyzerClass;
    AnalyzerClass * analyzerClass2 = new AnalyzerClass;
        
    // call function here
    decodeAnalyzeBuffer(soundPath, analyzerClass, name);
    decodeAnalyzeBuffer(soundPath2, analyzerClass2, name2);
    
    analyzerClass->success;
    
    printf("End of Program.");
    
    return 0;
    // have some fun with Superpowered
    }


// function to decode and analyze audio then put into a structure, return the structure
