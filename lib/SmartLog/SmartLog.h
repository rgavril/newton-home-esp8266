#ifndef __SMART_LOG__H__
#define __SMART_LOG__H__

#include <Arduino.h>

class SmartLog {
    private:
        struct StreamItem {
            Stream *stream;
            struct StreamItem *next;
        };
        StreamItem *logDestinations;

    public:
        void addDestination(Stream *stream);
        void removeDestination(Stream *stream);

        void printLevel(int level, const char *format, va_list *args);
};

extern SmartLog slog;

#endif