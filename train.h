#ifndef TRAIN_H                  // Header guard start → prevents this file from being included multiple times
#define TRAIN_H                  // Defines TRAIN_H so compiler knows this file is already included

#include "track.h"               // Includes custom track header file so this file can use TOTAL_TRACKS and track-related definitions
#include <time.h>                // Includes time library for using time_t datatype and time-related functions
#include <stdio.h>               // Includes standard input/output library for FILE datatype and logging functionality

extern int track_status[TOTAL_TRACKS];  
// extern → variable memory is created in another .c file, this file only accesses it
// int → array stores integer values
// track_status → shared array storing occupancy status of each track
// TOTAL_TRACKS → size of array comes from track.h
// Example:
// 0 = track free
// 1-5 = occupied by train with that ID
// Shared among multiple train threads → requires synchronization to avoid race conditions

typedef enum {                   
// typedef → creates custom datatype alias
// enum → creates named integer constants
// Train states help track current lifecycle stage of train thread

    WAITING_SIGNAL,              
    // Train selected route but waiting for signal permission
    // Similar to process waiting state in Operating Systems

    WAITING_TRACK,               
    // Train got signal but track is still occupied
    // Waiting for shared resource availability

    MOVING,                      
    // Train is actively moving
    // Similar to RUNNING process state in OS

    ARRIVING,                    
    // Train is reaching destination
    // Used for cleanup or GUI updates

    FINISHED                     
    // Train completed all loops/routes
    // Similar to TERMINATED process state

} TrainState;                    
// Creates datatype named TrainState
// Variables of this type can now store train states

typedef struct {                 
// typedef struct → creates custom structure datatype
// Structure groups related train data together

    int id;                      
    // Unique train identifier
    // Example: Train 1, Train 2

    int track1;                  
    // Starting/source track number

    int track2;                  
    // Destination/end track number

    int color_index;             
    // GUI color identifier for displaying train visually

    TrainState state;            
    // Stores current train state using TrainState enum

    int departure_time;          
    // Initial delay before train starts moving
    // Helps stagger train execution

    int loop_count;              
    // Stores how many route loops train has completed

    int max_loops;               
    // Maximum number of loops train is allowed to complete

    time_t arrival_time;         
    // time_t → datatype storing time value
    // Stores time when train entered waiting/request queue

    time_t finish_time;          
    // Stores time when train completed execution

    char current_route_str[64];  
    // char → character datatype
    // current_route_str → stores route as readable string
    // [64] → character array can store up to 64 characters
    // Example: "Track 1 -> Track 3"

    volatile int reached_bottom; 
    // volatile → compiler must always read latest value from memory
    // Prevents compiler optimization caching old value
    // Shared between GUI thread and train thread
    // Indicates whether train reached bottom of GUI path

    double total_wait_time;      
    // double → floating-point datatype for decimal values
    // Stores total accumulated waiting time

    int wait_count;              
    // Counts number of times train entered waiting state

    time_t current_wait_start;   
    // Stores timestamp when current waiting period started

    int is_currently_waiting;    
    // Boolean-style flag
    // 0 = not waiting
    // 1 = currently waiting

} Train;                         
// Creates datatype named Train
// Can now declare variables like:
// Train t1;

void* train(void* arg);          
// void* → generic pointer type
// train → thread function name
// (void* arg) → accepts generic argument pointer
// Required pthread function format
// Each train thread begins execution from this function

extern Train* global_trains;     
// Shared pointer to array of all trains
// Train* → pointer to Train structure
// Accessible across multiple files

extern int global_train_count;   
// Stores total number of trains in simulation

extern FILE* log_file;           
// FILE* → pointer to file object
// Shared log file used for writing simulation activity

extern volatile int simulation_running;  
// Shared simulation control flag
// volatile ensures latest value always read from memory
// Used for safely stopping all threads

#endif                           
// Ends header guard conditional block
