#include "message.h"

peer_t server;
char read_buffer[128]; // для сообщения которое ввели

void shutdown_properly();

int connect_server(peer_t *server)//создаем сокет  
{
  //printf("connect_server\n");
  
  server->socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server->socket < 0) {
    perror("socket()");
    return -1;
  }
  
  // настройка адреса
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);
  
  server->addres = server_addr;
  
  if (connect(server->socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) != 0) {
    perror("connect()");
    return -1;
  }
  
  printf("Connected to %s:%d.\n", SERVER_IPV4_ADDR, PORT);
  
  return 0;
}

int build_fd_sets(peer_t *server, fd_set *read_fds)//организация дескрипторов
{
	
	//printf("build_fd_sets\n");
	FD_ZERO(read_fds);
	FD_SET(STDIN_FILENO, read_fds);
	FD_SET(server->socket, read_fds);
	return 0;
}

void shutdown_properly()// завершение
{
  printf("Client finished work\n");
  close(server.socket);
  exit(0);
}


int main(int argc, char **argv)
{
	
	//printf("begin client\n");
  
  if (connect_server(&server) != 0)
    shutdown_properly();

  fd_set read_fds;
  
  printf("Please, type text to send:\n");
  
  int maxfd = server.socket;
  
  while (1) {
    
    build_fd_sets(&server, &read_fds);
        
    int activity = select(maxfd + 1, &read_fds, NULL, NULL, NULL);
    
    switch (activity) {
      case -1:
        perror("select()");
        shutdown_properly();

      case 0:
        // you should never get here
        printf("select() returns 0.\n");
        shutdown_properly();

      default:
      //printf("default\n");
      
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {// если вводится с клавиатуры
            
			int count = read(STDIN_FILENO, read_buffer, sizeof(read_buffer)-1);
			read_buffer[count]= '\0';
			printf("~message is %s\n",read_buffer);
			if (strncmp(read_buffer, "exit", EXIT) == 0 ){
				shutdown_properly();
			}
			
			if (strncmp(read_buffer, "buy", BUY) == 0 || strncmp(read_buffer, "sell", SELL) == 0){
				write(server.socket, read_buffer, strlen(read_buffer)); // теперь нужно его отправить
			} else {
				printf("wrong message\n");
			}
			//printf("message was send\n");
		}

        if (FD_ISSET(server.socket, &read_fds)) {// если сервер присылает сообщение
			//printf("server send a message\n");
			int count = read(server.socket, read_buffer, sizeof(read_buffer)-1);
			if (count == 0){
				exit(0);
			}
			read_buffer[count] ='\0';
			if (strcmp(read_buffer, buystr) == 0){
				printf("This car was bought\n");
			} else if (strcmp(read_buffer, sellstr) == 0){
				printf("We have new car\n");
			} else if (strcmp(read_buffer, nobuystr) == 0){
				printf("We have not got this car\n");
			} else if (strcmp(read_buffer, nosellstr) == 0){
				printf("No empty space\n");
			} else {
				printf("~message is : %s\n",read_buffer);
			}
			//printf("%s\n",read_buffer);
			if (strcmp(read_buffer,exstr) == 0){
				shutdown_properly();
			}
        }

    }
    //printf("message: \n");
    //printf("And we are still waiting for server or stdin activity. You can type something to send:\n");
  }
  
  return 0;
}
