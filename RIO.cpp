//
// Created by yang on 7/26/18.
//

#include <cerrno>
#include <cstring>
#include "RIO.h"
#include <stdio.h>

void RIO::rio_readinitb(int fd) {
    rp->rio_fd=fd;
    rp->rio_cnt=0;
    rp->rio_bufptr=rp->rio_buf;
}

ssize_t RIO::rio_readlineb(char *usrbuf, size_t maxlen) {
    ssize_t n,rc;
    char c,*bufp=usrbuf;
    for(n=1;n<maxlen;++n){
        if((rc=rio_read(&c,1))==1){
            *bufp++=c;
            if(c=='\n')
                break;
        }else if(rc==0){
            if(n==1)
                return 0;  //EOF,no data read
            else
                break;     //EOF some data was read
        }else
            return -1;   //Error

    }
    *bufp='\0';
    return n;
}

ssize_t RIO::rio_readnb( char *usrbuf, size_t n) {
    size_t nleft=n;
    ssize_t nread;
    char *bufp=usrbuf;
    while(nleft>0){
        if((nread=rio_read(bufp,nleft))<0){
            if(errno==EINTR)
                nread=0;
            else
                return -1;
        }else if(nread==0)
            break;
        nleft-=nread;
        bufp+=nread;
    }
    return (n-nleft);
}

ssize_t RIO::rio_readn( char *usrbuf, size_t n) {
    size_t  nleft=n;
    ssize_t nread;
    char *bufp=usrbuf;

    while(nleft>0){
        if((nread=read(this->rp->rio_fd,bufp,nleft))<0){
            if(errno==EINTR)
                nread=0;
            else return -1;
        }else if(nread==0)
            break;
        nleft-=nread;
        bufp+=nread;
    }

    return (n-nleft);
}

ssize_t RIO::rio_writen(char *usrbuf, size_t n) {
    size_t nleft=n;
    ssize_t nwriten;
    char *bufp=usrbuf;

    while(nleft>0){
        if((nwriten=write(this->rp->rio_fd,bufp,nleft))<=0){
            if(errno==EINTR)
                nwriten=0;
            else
                return -1;
        }
        nleft-=nwriten;
        bufp+=nwriten;
    }
    return n;
}

ssize_t RIO::rio_read(char *usrbuf, size_t n) {
    ssize_t cnt;
    while(rp->rio_cnt<=0) { /*Refill if buf is empty */
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));

        if (rp->rio_cnt < 0) { //The call was interrupted by a signal before any data was read.
            if (errno != EINTR)// Interrupted by sig handler return
                return -1;
        } else if (rp->rio_cnt == 0) { // read  to EOF
            return 0;
        } else {
            rp->rio_bufptr = rp->rio_buf;  //Reset buffer ptr
        }
    }
    cnt = n;
    if (rp->rio_cnt < n)
        cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr+=cnt;
    rp->rio_cnt -= cnt;

    return cnt;
}

RIO::RIO() {
    this->rp=new rio_t;
}

RIO::~RIO() {
    delete (this->rp);
}
