#ifndef STATUS_H
#define STATUS_H

typedef enum {
    ST_OK = 0,
    ST_ERR_ARGS = 1,
    ST_ERR_PARSE = 2,
    ST_ERR_RANGE = 3,
    ST_ERR_IO = 4,
    ST_ERR_OVERFLOW = 5
} status_t;

#endif /* STATUS_H */
