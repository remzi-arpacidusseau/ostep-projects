#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define MAXBUF (8192)

//
// This program is intended to help you test your web server.
// You can use it to test that you are correctly having multiple threads
// handling http requests.
// 

double get_seconds() {
    struct timeval t;
    int rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (double) ((double)t.tv_sec + (double)t.tv_usec / 1e6);
}


int main(int argc, char *argv[]) {
    // Extract arguments
    double spin_for = 0.0;
    char *buf;
    if ((buf = getenv("QUERY_STRING")) != NULL) {
	// just expecting a single number
	spin_for = (double) atoi(buf);
    }

    double t1 = get_seconds();
    while ((get_seconds() - t1) < spin_for)
	sleep(1);
    double t2 = get_seconds();
    
    /* Make the response body */
    char content[MAXBUF];
    sprintf(content, "<p>Welcome to the CGI program (%s)</p>\r\n", buf);
    sprintf(content, "%s<p>My only purpose is to waste time on the server!</p>\r\n", content);
    sprintf(content, "%s<p>I spun for %.2f seconds</p>\r\n", content, t2 - t1);
    
    /* Generate the HTTP response */
    printf("Content-length: %lu\r\n", strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);
    
    exit(0);
}

