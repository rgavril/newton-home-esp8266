#include "SmartLog.h"

void SmartLog::addDestination(Stream *stream) {
    StreamItem* newNode = (StreamItem *) malloc(sizeof(StreamItem));
    newNode->stream = stream;
    newNode->next = this->logDestinations;

    this->logDestinations = newNode;
}

void SmartLog::removeDestination(Stream *stream) {
    StreamItem* temp = this->logDestinations;
    StreamItem* prev;
    
    if (temp != NULL && temp->stream == stream) {
        this->logDestinations = temp->next;
        free(temp);
        return;
    }

    while(temp != NULL && temp->stream != stream) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        return;
    }

    prev->next = temp->next;
    free(temp);
}

void SmartLog::printLevel(int level, const char *format, va_list *args) {
    
}