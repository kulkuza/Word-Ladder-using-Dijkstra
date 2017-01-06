/* mytimer.h */

//
// Functions for getting the time 
// difference between two points in
// a program. A common use is to time
// how long a function or algorithm takes.
//
// Michael Mei
// Linux Mint 18(Sarah) with gcc
// U. of Illinois, Chicago
// CS251, Fall 2016
// HW #14
//

void timer_start();
void timer_stop();
double timer_value();
void timer_stats(char* message);
