#include <stdint.h>
// Author: Doron Bruder  Brief: Home-Test  Date: 30/5/20
// ** Note: I was not sure what considered a "clear buffer" so I make an assumption in the following implementation
// that a clear buffer is a buffer which full of zeros in all its indices, and this is also the condition in which a new
// buffer is allocated **
/**
 * A ring buffer that can track after the average of its elements ("forever" ,regular & upper /lower quarter (by time))
 */
typedef struct ring_buf_s
{
    int* samples;
    int head;         // new data is written at this position in the buffer
    int tail;         // data is read from this position in the buffer
    int count;        // total number of elements in the queue <= RBUF_SIZE
    int capacity;      // total maximum size of the buffer chosen by the user
    void(*above_threshold_func)(int avg_value, int threshold); // function to use when after adding we reach the limit
    int threshold; // given average limit
    int forever_sum; // the forever sum
    int forever_count; // the forever counter
} AverageBuffer;

/**
 * An init function for the ring buffer
 * @param capacity - the maximum size of buffer
 * @param above_threshold_func- ptr to function to use when after adding we reach the limit
 * @param threshold given average limit
 * @return  ptr to the new buffer , or NULL if there was an Error during the allocation
 */
AverageBuffer* allocAverageBuffer(int capacity ,void(*above_threshold_func)(int avg_value, int threshold),int threshold);

/**
 * A function to add a new item(sample) to the buffer
 * @param buff
 * @param num_to_add the sample to add
 */
void addSample(AverageBuffer* buff,int num_to_add);

/**
 * A function to get the regular average of the current buffer samples
 * @param buff
 * @return the regular average
 */
double getAverage(AverageBuffer* buff);

/**
 * A function to get the forever average
 * @param buff
 * @return forever average
 */
double getAverageForever(AverageBuffer* buff);

/**
 * A function to get the lower quarter(by time) average
 * @param buff
 * @return the lower quarter(by time) average
 */
double getUpperQuarterAverage(AverageBuffer* buff);

/**
 * A function to get the upper quarter(by time) average
 * @param buff
 * @return the upper quarter(by time) average
 */
double getLowerQuarterAverage(AverageBuffer* buff);

/**
 * A function to make all the samples zeros("clean the buffer")
 * @param buff
 */
void clearAverageBuffer(AverageBuffer* buff);

/**
 * A function to free the dynamic allocated memory of the buffer
 * @param buff
 */
void freeAverageBuffer(AverageBuffer* buff);


