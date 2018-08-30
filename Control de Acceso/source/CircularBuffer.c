#include "CircularBuffer.h"

#define __CIRCULAR_BUFFER_NEW(name,size) \
        static char name##_memory[size]; \
        CircularBuffer name =            \
        {                                \
            .buffer = name##_memory,     \
            .head = 0,                   \
            .tail = 0,                   \
            .maxlen = size               \
        }                                \

bool push(CircularBuffer *this,void* data)
{
   /* int next;
    // Next is head position after push operation
    next = this->head + 1; 
    if (next >= this->maxlen)
        next = 0;
    // Full buffer condition: head+1==tail
    if (next == this->tail) 
        return false;
    // Save data and update head position
    this->buffer[this->head] = data; 
    this->head = next;*/
    return true;
}

bool pop(CircularBuffer *this,void * data)
{
	/*int next;
	// Empty buffer condition head==tail
    if (this->head == this->tail)
        return false;
    // Next is tail position after the pop operation
    next = this->tail + 1; 
    if(next >= this->maxlen)
        next = 0;
    // Read data and update tail position
    (*data) = this->buffer[this->tail]; 
    this->tail = next;*/
    return true;
}

void flush(CircularBuffer * this)
{
	this->head = this->tail;
}

int numel(CircularBuffer *this)
{
	return 0; // IMPLEMENTAR
}

bool isEmpty(CircularBuffer *this)
{
	return this->head == this->tail;
}

bool isFull(CircularBuffer *this)
{
	return this->head + 1 == this->tail;
}
