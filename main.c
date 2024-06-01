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

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
    return -1;
  }

  struct hostent *he = gethostbyname("irc.libera.chat");

  if (he != NULL) {
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    memcpy(&serv_addr.sin_addr.s_addr, he->h_addr_list[0], 4);
  } else {
    // Error handling
  }

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    perror("connection failed");
    return -1;
  }

  {
    char userBuffer[BUFFER_SIZE];
    sprintf(userBuffer, "USER codebambot localhost irc.libera.chat "
                        "codebambot\nNICK codebambot\n");
    send(sockfd, userBuffer, strlen(userBuffer), 0);
  }

  char response[BUFFER_SIZE];
  int motd_recieved = 0;
  while (motd_recieved == 0) {
    int bytesReceived = recv(sockfd, response, BUFFER_SIZE, 0);
    if (bytesReceived < 0) {
      perror("recv failed");
      return -1;
    }
    if (bytesReceived > 1000) {
      motd_recieved = 1;
    }
    printf("%s\n", response);
  }

  {
    char joinBuffer[BUFFER_SIZE];
    sprintf(joinBuffer, "JOIN #codebambot\n");
    send(sockfd, joinBuffer, strlen(joinBuffer), 0);
  }

  while (1) {
    int bytesReceived = recv(sockfd, response, BUFFER_SIZE, 0);
    if (bytesReceived < 0) {
      perror("recv failed");
      return -1;
    }
    printf("%s\n", response);
    char *saveptr = NULL;
    int private = 1;
    int i = 0;
    for (char *s = strtok_r(response, " ", &saveptr); s;
         s = strtok_r(NULL, " ", &saveptr)) {
      if (i == 1 && strcmp(s, "PRIVMSG")) {
        private = 0;
      }
      if (i < 3) {
        i++;
      } else {
        if (private) {
          {
            char privBuffer[BUFFER_SIZE];
            sprintf(privBuffer, "PRIVMSG #codebambot :hello world\n");
            send(sockfd, privBuffer, strlen(privBuffer), 0);
          }
          int i = 0;
          char *saveptr = NULL;
          for (char *s = strtok_r(response, " ", &saveptr); s;
               s = strtok_r(NULL, " ", &saveptr)) {
            printf("%d", i);
            i++;
            if (i == 4) {
              printf("%d %s", i, s);
              break;
            }
          }
        }
        break;
      }
    }
  }

  close(sockfd);
  return 0;
}
