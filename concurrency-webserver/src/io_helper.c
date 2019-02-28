#include "io_helper.h"

ssize_t readline(int fd, void *buf, size_t maxlen) {
    char c;
    char *bufp = buf;
    int n;
    for (n = 0; n < maxlen - 1; n++) { // leave room at end for '\0'
	int rc;
        if ((rc = read_or_die(fd, &c, 1)) == 1) {
            *bufp++ = c;
            if (c == '\n')
                break;
        } else if (rc == 0) {
            if (n == 1)
                return 0; /* EOF, no data read */
            else
                break;    /* EOF, some data was read */
        } else
            return -1;    /* error */
    }
    *bufp = '\0';
    return n;
}


int open_client_fd(char *hostname, int port) {
    int client_fd;
    struct hostent *hp;
    struct sockaddr_in server_addr;
    
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1; 
    
    // Fill in the server's IP address and port 
    if ((hp = gethostbyname(hostname)) == NULL)
        return -2; // check h_errno for cause of error 
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *) hp->h_addr, 
          (char *) &server_addr.sin_addr.s_addr, hp->h_length);
    server_addr.sin_port = htons(port);
    
    // Establish a connection with the server 
    if (connect(client_fd, (sockaddr_t *) &server_addr, sizeof(server_addr)) < 0)
        return -1;
    return client_fd;
}

int open_listen_fd(int port) {
    // Create a socket descriptor 
    int listen_fd;
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	fprintf(stderr, "socket() failed\n");
	return -1;
    }
    
    // Eliminates "Address already in use" error from bind
    int optval = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval, sizeof(int)) < 0) {
	fprintf(stderr, "setsockopt() failed\n");
	return -1;
    }
    
    // Listen_fd will be an endpoint for all requests to port on any IP address for this host
    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    server_addr.sin_port = htons((unsigned short) port); 
    if (bind(listen_fd, (sockaddr_t *) &server_addr, sizeof(server_addr)) < 0) {
	fprintf(stderr, "bind() failed\n");
	return -1;
    }
    
    // Make it a listening socket ready to accept connection requests 
    if (listen(listen_fd, 1024) < 0) {
	fprintf(stderr, "listen() failed\n");
	return -1;
    }
    return listen_fd;
}


