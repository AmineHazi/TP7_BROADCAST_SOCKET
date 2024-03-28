#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define BUF_SIZE 512
#define PORT 6336
#define MAX_CLIENTS 1024

char* client_ips[MAX_CLIENTS];
int client_count = 0;

char* my_strdup(const char* s) {
    size_t len = strlen(s) + 1; 
    char* new = malloc(len);
    if (new == NULL) return NULL;
    memcpy(new, s, len);
    return new;
}

// Vérifie si l'IP est déjà dans le tableau
int ip_exists(char* ip) {
    for (int i = 0; i < client_count; i++) {
        if (strcmp(client_ips[i], ip) == 0) {
            return 1;
        }
    }
    return 0;
}

// Ajoute l'IP au tableau si elle n'y est pas déjà
void add_ip(char* ip) {
    if (!ip_exists(ip)) {
        if (client_count < MAX_CLIENTS) {
            client_ips[client_count++] = my_strdup(ip); 
        } else {
            printf("Nombre maximal de clients atteint.\n");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        return EXIT_FAILURE;
    }

    memset(client_ips, 0, sizeof(client_ips));

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int yes = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        perror("setsockopt(SO_REUSEADDR)");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    switch(fork()) {
        case -1: 
            perror("fork");
            exit(EXIT_FAILURE);

        case 0: // Envoi des messages
            addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
            if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes)) < 0) {
                perror("setsockopt(SO_BROADCAST)");
                exit(EXIT_FAILURE);
            }
            while(1) {
                if (sendto(sock, argv[1], strlen(argv[1]), 0, (struct sockaddr*)&addr, addr_len) < 0) {
                    perror("sendto");
                    exit(EXIT_FAILURE);
                }
                sleep(1);
            }
            break;

        default: // Réception et traitement des messages
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
            if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
                perror("bind");
                exit(EXIT_FAILURE);
            }
            char buffer[BUF_SIZE] = {0};
            struct sockaddr_in client_addr;
            time_t start_time = time(NULL);
            while(1) {
                if (recvfrom(sock, buffer, BUF_SIZE - 1, 0, (struct sockaddr*)&client_addr, &addr_len) > 0) {
                    buffer[BUF_SIZE - 1] = '\0'; 
                    char* client_ip = inet_ntoa(client_addr.sin_addr);
                    if (strcmp(buffer, argv[1]) == 0) {
                        add_ip(client_ip);
                    } else {
                        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
                    }
                }

                if (difftime(time(NULL), start_time) >= 30) {
                    printf("Adresses IP ayant répondu avec le même message :\n");
                    for (int i = 0; i < client_count; i++) {
                        printf("%s\n", client_ips[i]);
                    }
                    start_time = time(NULL); // Réinitialise le compteur
                }
            }
            break;
    }
    return 0;
}
