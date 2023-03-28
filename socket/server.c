#include "message.h"

#define MAX_CLIENTS 5

#define MAX_CARS 5

#define NO_SOCKET -1

#define MODEL_LEN 16

struct car
{
	int have;
	char model[MODEL_LEN];
} ;

int listen_sock;//сокет сервера
peer_t connection_list[MAX_CLIENTS];//массив подключенных клиентов
char read_buffer[MAX_MESSAGES_SIZE]; // для сообщения которое ввели

void shutdown_properly();//функция закрытия сокетов

void sig_handler(){
	 //printf("Server shutdown\n");
	 int i;
	 for (i = 0; i < MAX_CLIENTS; i++){
		 write(connection_list[i].socket, exstr, strlen(exstr));
	 }
	 shutdown_properly();
	 exit(0);
}

void print_car(struct car *table){
	printf("\nCAR LIST\n");
	int i,fl = 0;
	for (i = 0; i < MAX_CARS; i++){
		if (table[i].have == 1){
			fl = 1;
			printf("car: %s", table[i].model);
		}
	}
	if (fl == 0){
		printf("Empty list\n");
	}
	return;
}


int start_listen_socket(int *listen_sock)//открытие сокетов
{
	
	//printf("start_listen_socket\n");
  *listen_sock = socket(AF_INET, SOCK_STREAM, 0);// указатель на слушающий сокет
  if (*listen_sock < 0) {
    perror("socket");
    return -1;
  }
 
  int reuse = 1;
  if (setsockopt(*listen_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
    perror("setsockopt");
    return -1;
  }
  
  struct sockaddr_in my_addr;//объявление структуры
  memset(&my_addr, 0, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = INADDR_ANY;
  my_addr.sin_port = htons(PORT);
  
 
  if (bind(*listen_sock, (struct sockaddr*) &my_addr, sizeof(struct sockaddr)) != 0) {
    perror("bind");
    return -1;
  }
  
 
  if (listen(*listen_sock, MAX_CLIENTS) != 0) {
    perror("listen");
    return -1;
  }
  printf("Accepting connections on port %d.\n", (int)PORT);
 
  return 0;
}

void shutdown_properly()//завершение
{
	//printf("shutdown_properly\n");
	shutdown(listen_sock, 2);
	int i;
	close(listen_sock);
  
	for (i = 0; i < MAX_CLIENTS; ++i)
		if (connection_list[i].socket != NO_SOCKET){
			shutdown(connection_list[i].socket, 2);
			close(connection_list[i].socket);
		}
    
	printf("Shutdown server properly.\n");
	exit(0);
}

int build_fd_sets(fd_set *read_fds)// строим список дескрипторов
{
	//printf("build_fd_sets\n");
  int i;
  
  FD_ZERO(read_fds);
  FD_SET(STDIN_FILENO, read_fds);
  FD_SET(listen_sock, read_fds);
  for (i = 0; i < MAX_CLIENTS; ++i)
    if (connection_list[i].socket != NO_SOCKET)
      FD_SET(connection_list[i].socket, read_fds);
 
  //printf("build_fd_sets done\n");
  return 0;
}  

int handle_new_connection()// обработка новых подключений
{
  struct sockaddr_in client_addr;
  memset(&client_addr, 0, sizeof(client_addr));// обнуление
  socklen_t client_len = sizeof(client_addr);
  int new_client_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &client_len);
  if (new_client_sock < 0) {
    perror("accept()");
    return -1;
  }
  
  char client_ipv4_str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &client_addr.sin_addr, client_ipv4_str, INET_ADDRSTRLEN);// преобразование адресов
  
  printf("Incoming connection from %s:%d.\n", client_ipv4_str, client_addr.sin_port);
  
  int i;
  for (i = 0; i < MAX_CLIENTS; ++i) {
    if (connection_list[i].socket == NO_SOCKET) {
      connection_list[i].socket = new_client_sock;
      connection_list[i].addres = client_addr;
      return 0;
    }
  }
  
  printf("There is too much connections. Close new connection %s:%d.\n", client_ipv4_str, client_addr.sin_port);
  close(new_client_sock);
  return -1;
}

int close_client_connection(peer_t *client)// отсоединяем клиента
{
  close(client->socket);
  client->socket = NO_SOCKET;
  return 0;
}
 
int main(int argc, char **argv)
{
	//printf("begin server\n");
	signal(SIGINT, sig_handler);
	
	struct car table[MAX_CARS];
	for (int i = 0; i < MAX_CARS; i++){
		table[i].have = 0;
	}
  
  if (start_listen_socket(&listen_sock) != 0){// вводная часть
    exit(EXIT_FAILURE);
  }
    
    //printf("listen server\n");
  
  int i;
  for (i = 0; i < MAX_CLIENTS; ++i) {
    connection_list[i].socket = NO_SOCKET;// обнуление
  }
  //printf("NO SOKET done\n");
  
  fd_set read_fds;
  
  int high_sock = listen_sock;
  
  printf("Waiting for incoming connections.\n");
  
  while (1) {
	  
	  //printf("while\n");
	  
    build_fd_sets(&read_fds);
    
   // printf("max socket %d\n",high_sock);
    
    high_sock = listen_sock;
    for (i = 0; i < MAX_CLIENTS; ++i) {
      if (connection_list[i].socket > high_sock)
        high_sock = connection_list[i].socket;
    }
    
    //printf("max socket %d\n",high_sock);
    
    int activity = select(high_sock + 1, &read_fds, NULL, NULL, NULL);
    
  //printf("activity = %d\n",activity);
  
    switch (activity) {
      case -1:
        perror("select()");
        shutdown_properly();
 
      case 0:
        printf("select() returns 0.\n");
        shutdown_properly();
      
      default:

        if (FD_ISSET(listen_sock, &read_fds)) {//пришел новый запрос на соединение
          handle_new_connection();
        }
        
        int j,count;
        for (j=0 ; j < MAX_CLIENTS; ++j){
			if (FD_ISSET(connection_list[j].socket, &read_fds)) {// пришло сообщение от клиента
				//printf("get message from client and j = %d\n", j);
				count = read(connection_list[j].socket, read_buffer, sizeof(read_buffer)-1);
				//printf ("count = %d\n", count);
				if (count == 0){
					//exit(0);
					close_client_connection(&connection_list[j]);
					continue;
				}
				read_buffer[count] = '\0';
				//printf ("message is:\n");
				printf ("~message is: %s\n", read_buffer);
				//printf ("strlen = %ld\n", strlen(read_buffer));
				//printf ("end of message\n");
				
						
				if (strncmp(read_buffer, "buy", BUY) == 0){
					printf("*Client wants to buy car\n");
					print_car(table);
					int fl = 0;
					for (int i = 0; i < MAX_CARS; i++){
						if (strcmp(read_buffer + (BUY + 1), table[i].model) == 0 && table[i].have == 1){
							printf("*model of car: %s\n", table[i].model);
							table[i].have = 0;
							//printf("Car was bought\n");
							fl = 1;
							break;
						}
					}
					if (fl == 1){
							printf("MESSAGE FOR EVERYONE: Car was bought\n");
							int k;
							for (k = 0 ; k < MAX_CLIENTS; ++k){
								if (connection_list[k].socket != NO_SOCKET){
									write(connection_list[k].socket, buystr, strlen(buystr));
								}
							}
					} else {
						printf("We haven't got this car\n");			
						//отправить что ошибка
						write(connection_list[j].socket, nobuystr, strlen(nobuystr));
					}
				} else if (strncmp(read_buffer, "sell", SELL) == 0 ){
					printf("*Client wants to sell car\n");
					print_car(table);
					int fl = 0;
					int i;
					for (i = 0; i < MAX_CARS; i++){
						if (table[i].have == 0){
							table[i].have = 1;
							strncpy(table[i].model, read_buffer + (SELL + 1), MODEL_LEN);
							printf("*model of car: %s\n",table[i].model );
							//printf("Car sold\n");
							fl = 1;
							break;
						}
					}
					if (fl == 1){
							printf("MESSAGE FOR EVERYONE: Car sold\n");
							int k;
							for (k = 0 ; k < MAX_CLIENTS; ++k){
								if (connection_list[k].socket != NO_SOCKET){
									write(connection_list[k].socket, sellstr, strlen(sellstr));
								}
							}
					} else {
						printf("No free space\n");			
						//отправить что ошибка
						write(connection_list[j].socket, nosellstr, strlen(nosellstr));
					}
				} else {
						printf("Wrong message\n");
				}
		    }
		}
	}
    //printf("end of while\n");
    //printf("And we are still waiting for clients' or stdin activity. You can type something to send:\n");
  }
 
  return 0;
}

