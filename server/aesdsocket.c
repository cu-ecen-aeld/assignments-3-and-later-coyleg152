// File: aesdsocket.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 9000
#define BACKLOG 5
#define CHUNK_SIZE 20
#define OUTPUT_FILE "/var/tmp/aesdsocketdata"

int is_running = 1;

void exit_program() {is_running = 0;}

int find_char(char * str, char c) {
  for (int i = 0; *(str + i) != '\0'; i++) {
    if (*(str + i) == c) return i;
  }
  return -1;
}

int main(int argc, char ** argv) {
  struct sockaddr_in tcp_server, client_addr;
  socklen_t addrlen = sizeof(client_addr);

  int listen_sd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_sd == -1) {
    fprintf(stderr, "ERROR: socket() failed\n");
    return -1;
  }

  fcntl(listen_sd, F_SETFL, O_NONBLOCK);
  printf("TCP socket created on descriptor %d\n", listen_sd);

  memset(&tcp_server, 0, sizeof(tcp_server));
  tcp_server.sin_family = AF_INET;
  tcp_server.sin_addr.s_addr = htonl(INADDR_ANY);
  tcp_server.sin_port = htons(PORT);

  if (bind(listen_sd, (struct sockaddr *) &tcp_server,
  sizeof(tcp_server)) == -1) {
    fprintf(stderr, "ERROR: bind() failed\n");
    close(listen_sd);
    return -1;
  }

  printf("TCP server bound to port %d\n", PORT);

  if (argc > 1 && strcmp(*(argv + 1), "-d") == 0) {
    int pid = fork();
    if (pid == -1) {
      fprintf(stderr, "ERROR: fork() failed\n");
      close(listen_sd);
      return -1;
    }
    else if (pid > 0) {
      printf("Parent exiting\n");
      return 0;
    }
  }

  if (listen(listen_sd, BACKLOG) == -1) {
    fprintf(stderr, "ERROR: listen() failed\n");
    close(listen_sd);
    return -1;
  }

  int of = open(OUTPUT_FILE, O_RDWR | O_CREAT | O_TRUNC, 0660);
  if (of == -1) {
    fprintf(stderr, "ERROR: open() failed\n");
    close(listen_sd);
    return -1;
  }

  printf("Waiting for incoming connections...\n");
  char * buffer = malloc((CHUNK_SIZE + 1) * sizeof(char));
  int conn_sd, n;
  openlog(NULL, 0, LOG_USER);
  signal(SIGINT, exit_program);
  signal(SIGTERM, exit_program);

  while (is_running) {
    conn_sd = -1;
    while (is_running && conn_sd == -1) {
      conn_sd = accept(listen_sd, (struct sockaddr *) &client_addr, &addrlen);
    }

    if (!is_running) {
      printf("Caught signal, exiting\n");
      syslog(LOG_DEBUG, "Caught signal, exiting\n");
      break;
    }

    fcntl(conn_sd, F_SETFL, O_NONBLOCK);
    printf("Accepted connection from %d\n", client_addr.sin_addr.s_addr);
    syslog(LOG_DEBUG, "Accepted connection from %d\n",
    client_addr.sin_addr.s_addr);

    while (is_running) {
      n = -1;
      while (is_running && n < 0) {
        n = recv(conn_sd, buffer, CHUNK_SIZE, 0);
      }

      if (n == 0) break;
      if (!is_running) {
        printf("Caught signal, exiting\n");
        syslog(LOG_DEBUG, "Caught signal, exiting\n");
        break;
      }
      
      *(buffer + n) = '\0';
      printf("%s", buffer);
      write(of, buffer, n);

      while (find_char(buffer, '\n') == -1) {
        n = recv(conn_sd, buffer, CHUNK_SIZE, 0);
        *(buffer + n) = '\0';
        printf("%s", buffer);
        write(of, buffer, n);
      }

      n = read(of, buffer, CHUNK_SIZE);
      while (n > 0) {
        *(buffer + n) = '\0';
        send(conn_sd, buffer, n, 0);
        n = read(of, buffer, CHUNK_SIZE);
      }
    }

    printf("Closed connection from %d\n", client_addr.sin_addr.s_addr);
    syslog(LOG_DEBUG, "Closed connection from %d\n",
    client_addr.sin_addr.s_addr);
    close(conn_sd);
  }

  close(listen_sd);
  close(of);
  closelog();
  free(buffer);
  unlink(OUTPUT_FILE);
  return 0;
}
