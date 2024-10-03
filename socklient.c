/* sockets.c */

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

//#define IP "127.0.0.1" // google.com
#define port 80 // http (Note: For HTTPS, use a library like OpenSSL)

int main(int argc, char *argv[])
{
    int s;
    struct sockaddr_in sock;
    char buf[4096];
    char data[512];

    if (argc < 3)
    {
        printf("Usage: ./socket IP METHOD\n");
        return -1;
    }
    
    char *ip = argv[1];      // IP address from the argument
    char *method = argv[2];   // HTTP method (GET, HEAD, POST)

    // Create the HTTP request based on user input
    snprintf(data, sizeof(data), "%s / HTTP/1.0\r\nHost: %s\r\n\r\n", method, ip);

    // Create a socket
    s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1)
    {
        perror("socket() error");
        return -1;
    }
    printf("Socket established.\n");
    sleep(1);

    // Setup sockaddr_in struct for connecting to the server
    sock.sin_addr.s_addr = inet_addr(ip); 
    sock.sin_port = htons(port);
    sock.sin_family = AF_INET;

    // Connect to the server
    if (connect(s, (struct sockaddr *)&sock, sizeof(struct sockaddr_in)) != 0)
    {
        perror("connect() error");
        close(s);
        return -1;
    }
    printf("Connection established.\n");
    sleep(1);

    // Send the HTTP request
    if (write(s, data, strlen(data)) == -1)
    {
        perror("write() error");
        close(s);
        return -1;
    }
    printf("HTTP request sent.\n");

    // Read the response from the server
    int bytes_read = read(s, buf, sizeof(buf) - 1);
    if (bytes_read == -1)
    {
        perror("read() error");
        close(s);
        return -1;
    }
    buf[bytes_read] = '\0';  // Null-terminate the response

    printf("Response received:\n---------------------------------------\n%s\n---------------------------------------\n", buf);

    // Close the socket
    close(s);
    printf("Socket closed.\n");
    
    return 0;
}

