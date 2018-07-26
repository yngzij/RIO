//
// Created by yang on 7/26/18.
//

#pragma once

#include <zconf.h>
#include <boost/noncopyable.hpp>
#define RIO_BUFSIZE 8192

typedef struct {
    int rio_fd;                /* Descriptor for this internal buf */
    ssize_t rio_cnt;               /* Unread bytes in internal buf */
    char *rio_bufptr;          /* Next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
} rio_t;


class RIO :boost::noncopyable{
public:
    RIO();
    ~RIO();
    void rio_readinitb(int fd);
    ssize_t rio_readlineb(char *usrbuf,size_t maxlen);
    ssize_t rio_readnb(char *usrbuf,size_t n);
    ssize_t rio_readn(char *usrbuf,size_t n);
    ssize_t rio_writen(char *usrbuf,size_t n);
    ssize_t rio_read(char *usrbuf,size_t n);


private:
    rio_t *rp;
};



