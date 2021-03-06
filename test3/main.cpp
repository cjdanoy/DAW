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
#include "SuperpoweredMixer.h"

using namespace std;

// enrique sends me a file
// encoded a certain way
// JSON text

struct Node{
    short int * iBuffer; // add the address of the pointer of the chunk of frames to this Node
    float * fBuffer;
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
    string success = "zero";
    unsigned int getFramesPerChunk;
    // needs to have pointer to head node
    // needs to have a pointer to a intBuffer, created within decodeAnalyzeBuffer
    };

    // adding argument to allow for file name
int decodeAnalyzeBuffer(string soundPath, AnalyzerClass * analyzerClass, string name){
    
    Node * tail = NULL;
    Node * head = NULL;
    const char *cSoundPath = soundPath.c_str();
    string woah = "/Users/Chriskringle/Desktop/" + name + ".wav";
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
    
    float * fBuffer = (float *)malloc(decoder->getFramesPerChunk() * 2 * sizeof(float) + 16384);
    
    while (true) {
           short int * iBuffer = (short int *)malloc(decoder->getFramesPerChunk() * 2 * sizeof(short int) + 16384);
           int framesDecoded = decoder->decodeAudio(iBuffer, decoder->getFramesPerChunk());
           if (framesDecoded < 1) break;
        
           Node * newNode = new Node;
           newNode->iBuffer=iBuffer;
           newNode->fBuffer=fBuffer;
           newNode->framesDecoded=framesDecoded;
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
    analyzerClass->getFramesPerChunk=decoder->getFramesPerChunk();
    // after making results we can put the analyzer structures / variables into AnalyzeClass Structure
    // can put writeWAV in different function.
    
    while(analyzerClass->head != NULL){
        // check value of head?
    Superpowered::writeWAV(destinationFile, analyzerClass->head->iBuffer, analyzerClass->head->framesDecoded * 4);
    free(analyzerClass->head->iBuffer);
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
    
    string soundPath2 = "/Users/Chriskringle/Desktop/Gracie 2.mp3";
    string soundPath = "/Users/Chriskringle/Desktop/Crazy Train.mp3";
    string name = "Crazy Train";
    string name2 = "Gracie";
    // ask for path
    AnalyzerClass * analyzerClass = new AnalyzerClass;
    AnalyzerClass * analyzerClass2 = new AnalyzerClass;
        
    // call function here
    decodeAnalyzeBuffer(soundPath, analyzerClass, name);
    // decodeAnalyzeBuffer(soundPath2, analyzerClass2, name2);
    
    analyzerClass->success;
    Superpowered::MonoMixer *monoMix = new Superpowered::MonoMixer();
    short int *intBufferOutput1 = (short int *)malloc(analyzerClass->getFramesPerChunk * 2 * sizeof(short int) + 16384);
    float * outputFloatBuffer = (float *)malloc(analyzerClass->getFramesPerChunk * 2 * sizeof(float) + 16384);
    
    //should my mixing processor be in its own function?
    //How to record audio? What
    // monoMix->process(analyzerClass->head->fBuffer, analyzerClass2->head->fBuffer, NULL, NULL, outputFloatBuffer, analyzerClass->getFramesPerChunk);
    printf("End of Program.");
    
    return 0;
    // have some fun with Superpowered
    }


// function to decode and analyze audio then put into a structure, return the structure
