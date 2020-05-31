#include "average_buffer.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

/**
 * An init function for the ring buffer
 * @param capacity - the maximum size of buffer
 * @param above_threshold_func- ptr to function to use when after adding we reach the limit
 * @param threshold given average limit
 * @return  ptr to the new buffer , or NULL if there was an Error during the allocation
 */
AverageBuffer* allocAverageBuffer(int capacity, void(*above_threshold_func)(), int threshold)
{
    AverageBuffer* ab = (AverageBuffer*) malloc(sizeof(AverageBuffer));
    if(ab != NULL)
    {
        int len = (int) (capacity*sizeof(int));
        ab->samples = (int*) malloc(capacity * sizeof(int));
        if(ab->samples != NULL)
        {
            for(int i =0; i<len; i++)
            {
                ab->samples[i] = 0;
            }
            ab->count=ab->head=ab->tail=0;
            ab->forever_sum = 0;
            ab->forever_count= 0;
            ab->capacity = capacity;
            ab->threshold = threshold;
            ab->above_threshold_func = above_threshold_func;
        }
        else
        {
            free(ab);
            return NULL;
        }
        return ab;
    }
    else
    {
        return NULL;
    }
}

/**
 * A function to make all the samples zeros("clean the buffer")
 * @param buff
 */
void clearAverageBuffer(AverageBuffer* buff){
    for(int i=0; i<buff->capacity; i++)
    {
        buff->samples[i] = 0;
    }
}

/**
 * A function to free the dynamic allocated memory of the buffer
 * @param buff
 */
void freeAverageBuffer(AverageBuffer* buff){
    free(buff->samples);
    free(buff);
}


// A function to traverse the ring buffer forward
static  int nextIndex(AverageBuffer* buff){
    int next = buff->head + 1;
    if (next >= buff->capacity)
    {
        next = 0;
    }
    return next;
}

// A function to traverse the ring buffer in a reverse manner
static  int prevIndex(AverageBuffer* buff){
    int prev = buff->head - 1;
    if (prev <= 0)
    {
        prev = buff->capacity;
    }
    return prev;
}

/**
 * A function to add a new item(sample) to the buffer
 * @param buff
 * @param num_to_add the sample to add
 */
void addSample(AverageBuffer* buff,int num_to_add)
{
    int next = buff->head + 1;
    if (next >= buff->capacity)
    {
        buff->forever_count = next-1;
        next = 0;
    }
    buff->forever_sum+=num_to_add;
    buff->forever_count += 1;

    if (buff->count < buff->capacity)
        buff->count++;
    buff->samples[buff->head] = num_to_add;
    buff->head = next;
    double average = getAverage(buff);
    if (average > buff->threshold)
    {
        buff->above_threshold_func((int)average, buff->threshold);
    }
}


/**
 * A function to get the regular average of the current buffer samples
 * @param buff
 * @return the regular average
 */
double getAverage(AverageBuffer* buff){
    int sum = 0;
    for(int i=0; i<buff->capacity; i++)
    {
        sum+=buff->samples[i];
    }
    return (double)(sum)/(double)(buff->capacity);
}

/**
 * A function to get the forever average
 * @param buff
 * @return forever average
 */
double getAverageForever(AverageBuffer* buff){
    return (double) buff->forever_sum/(double)buff->forever_count;
}

/**
 * A function to get the upper quarter(by time) average
 * @param buff
 * @return the upper quarter(by time) average
 */
double  getLowerQuarterAverage(AverageBuffer* buff){
    int sum = 0;
    int quarter = floor((double )(buff->capacity)/4);;
    int curr_index = buff->head;
    for(int i =0; i<quarter; i++)
    {
        curr_index = nextIndex(buff);
        sum+= buff->samples[curr_index];
    }
    return (double)sum/(double)quarter;

}

/**
 * A function to get the lower quarter(by time) average
 * @param buff
 * @return the lower quarter(by time) average
 */
double  getUpperQuarterAverage(AverageBuffer* buff){
    int sum = 0;
    int quarter = floor((double )(buff->capacity)/4);
    int curr_index = buff->head;
    for(int i =0; i<quarter; i++)
    {
        curr_index = prevIndex(buff);
        sum+= buff->samples[curr_index];
    }
    return (double)sum/(double)quarter;

}
