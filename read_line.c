/*
 * An example on how to read a file descriptor, line by line, using a circular 
 * buffer and not to call read on every byte read, because
 * it's not OK to read one byte at a time, because you are making too many system calls - better is to use a buffer, read a chunk and check if you got \n. 
 *  After getting a line, the rest of the bytes read remains in the buffer, 
 *  so you cannot mix read/recv with read_line. 
 *  SeeAlso: https://stackoverflow.com/questions/9828752/read-line-by-line-from-a-socket-buffer
 */ 
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

#define CBSIZE 128

typedef struct cbuf {
    char buf[CBSIZE];
    int fd;
    unsigned int rpos, wpos;
} cbuf_t;


int read_line(cbuf_t *cbuf, char *dst, unsigned int size)
{
    unsigned int i = 0;
    ssize_t n;
    while (i < size) {
        if (cbuf->rpos == cbuf->wpos) {
            size_t wpos = cbuf->wpos % CBSIZE;
            //if ((n = read(cbuf->fd, cbuf->buf + wpos, (CBSIZE - wpos))) < 0) {
            if((n = recv(cbuf->fd, cbuf->buf + wpos, (CBSIZE - wpos), 0)) < 0) {
                if (errno == EINTR) {
                    continue;
                }
                return -1;
            } else if (n == 0) {
                return 0;
            }
            cbuf->wpos += n;
        }
        dst[i++] = cbuf->buf[cbuf->rpos++ % CBSIZE];
        if (dst[i - 1] == '\n') {
            break;
        }
    }

    if(i == size) {
        fprintf(stderr, "line too large: %d %d\n", i, size);
        return -1;
    }

    dst[i] = 0;
    return i;
}

int main()
{
    cbuf_t *cbuf;
    char buf[512];
    struct sockaddr_in saddr;
    struct hostent *h;
    char *ip;
    char host[] = "www.google.com";

    if(!(h = gethostbyname(host))) {
        perror("gethostbyname");
        return 1;
    }
    ip = inet_ntoa(*(struct in_addr*)h->h_addr);

    cbuf = calloc(1, sizeof(*cbuf));

    fprintf(stdout, "Connecting to ip: %s\n", ip);
    if((cbuf->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(80);
    inet_aton(ip, &saddr.sin_addr);
    if(connect(cbuf->fd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
        perror("connect");
        return 1;
    }

    snprintf(buf, sizeof(buf), "GET / HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", host);
    write(cbuf->fd, buf, strlen(buf));
    while(read_line(cbuf, buf, sizeof(buf)) > 0) {
        // if it's an empty \r\n on a line, header ends //
        if(buf[0]=='\r' && buf[1] == '\n') {
            printf("------------------------\n");
        }
        printf("[%s]", buf);
    }
    close(cbuf->fd);
    free(cbuf);
    return 0;
}
