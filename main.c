#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 6667
#define BUFFER_SIZE 1024

int main(void) {
  int sockfd;
  struct sockaddr_in serv_addr;

  // Create a socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
    return -1;
  }

  struct hostent *he = gethostbyname("irc.libera.chat");

  if (he != NULL) {
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    memcpy(&serv_addr.sin_addr.s_addr, he->h_addr_list[0], 4); // Assuming INET
  } else {
    // Error handling
  }

  // Connect to the server
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("connection failed");
    return -1;
  }

  // Send a message to the server
  char userBuffer[BUFFER_SIZE];
  sprintf(userBuffer, "USER codebambot localhost irc.libera.chat "
                      "codebambot\nNICK codebambot\n");
  send(sockfd, userBuffer, strlen(userBuffer), 0);

  // Receive a response from the server
  char response[BUFFER_SIZE];
  while (1) {
    int bytesReceived = recv(sockfd, response, BUFFER_SIZE, 0);
    if (bytesReceived < 0) {
      perror("recv failed");
      return -1;
    }
    printf("%s\n", response);
  }

  // Close the socket
  close(sockfd);
  return 0;
}
