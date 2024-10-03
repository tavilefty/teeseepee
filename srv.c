/* a simple server made using sockets in C */

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <string.h>

#define PORT 80

int main()
{

	char *note = "Use nc for an 'interactive' mode.\n";
	
	int s, c;
	socklen_t addrlen = 0;
	struct sockaddr_in srv, cli;

	char buf[512];
	char *data = "Hello from Server\n";

	memset(&srv, 0, sizeof(srv));
	memset(&cli, 0, sizeof(cli));

	printf("%s", note);
	
	s = socket(PF_INET, SOCK_STREAM, 0);
	
	if (s == -1)
	{
		printf("Socket error.\n");
		return -1;
	}
	printf("Socket created.\n");
	sleep(1);

	srv.sin_family = PF_INET;
	srv.sin_addr.s_addr = 0;
	srv.sin_port = htons(PORT);

	if (bind(s, (struct sockaddr *)&srv, sizeof(srv)) == -1)
	{
		printf("Bind error.\n");
		close(s);
		return -1;
	}
	printf("Did the binding.\n");
	sleep(1);

	if (listen(s, 5) == -1)
	{
		printf("Listen error.\n");
		close(s);
		return -1;
	}
	printf("Listening on 0.0.0.0:%d\n", PORT);
	sleep(1);

	c = accept(s, (struct sockaddr *)&srv, &addrlen);
	if (c  == -1)
	{
		printf("accept error.\n");
		close(s);
		return -1;
	}
	printf("Client connected.\n");

	write(c, "Say something???\n", strlen("Say something???\n"));
	
	read(c, buf, 511);
	//printf("client said: %s", *buf);
	
	write(c, data, strlen(data));
	printf("client said: %s", buf);
	printf("Told the mf: %s", data);
	close(c);
	close(s);
	printf("Closed server.\n");

	return 0;
}
