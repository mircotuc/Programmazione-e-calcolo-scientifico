#include <iostream>
#include <thread>
#include <chrono>

#include "timecounter.h"

int main(void)
{
    timecounter tc; /* instantiate the timecounter */
    tc.tic();       /* start it */

    /* sleep for some time */
    std::this_thread::sleep_for( std::chrono::milliseconds(1234) );
    
    double secs = tc.toc(); /* stop the timecounter, get time and print it */
    std::cout << "Elapsed time: " << secs << " seconds\n";
    
    return 0;
}

