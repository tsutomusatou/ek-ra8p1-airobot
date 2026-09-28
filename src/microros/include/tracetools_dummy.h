#ifndef TRACETOOLS_DUMMY_H
#define TRACETOOLS_DUMMY_H

/* Disable all tracing macros for embedded targets */

#define TRACEPOINT(...)
#define TRACEPOINT_DURATION_BEGIN(...)
#define TRACEPOINT_DURATION_END(...)
#define TRACEPOINT_DURATION(...)
#define TRACEPOINT_COUNTER_ADD(...)
#define TRACEPOINT_COUNTER_SUB(...)
#define TRACEPOINT_COUNTER_SET(...)

#endif /* TRACETOOLS_DUMMY_H */
