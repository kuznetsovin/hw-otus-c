#include <sys/socket.h>
#include "stdio.h"
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

static const char TELNET_ADDR[] = "telehack.com";
static const size_t TELNET_PORT = 23;

const size_t END_TELEHACK_BLOCK = 0x2e;
const size_t NET_BUFF_SIZE = 2048;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Telehack font is require!");
        exit(EXIT_FAILURE);
    };
    char *font = argv[1];
    printf("Font: %s\n", font);

    if (argc < 3) {
        fprintf(stderr, "Message is require!");
        exit(EXIT_FAILURE);
    };
    char *msg = argv[2];
    printf("Message: %s\n", msg);

    struct hostent *hostnm;
    if ((hostnm = gethostbyname(TELNET_ADDR)) == NULL) {
        fprintf(stderr, "Gethostbyname failed\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server = {
            .sin_family = AF_INET,
            .sin_port  = htons(TELNET_PORT),
            .sin_addr.s_addr = *((unsigned long *) hostnm->h_addr_list[0]),
    };

    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("int socket error");
        exit(EXIT_FAILURE);
    }

    if (connect(s, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("connect error");
        exit(EXIT_FAILURE);
    }

    printf("Connecting to server %s...\n", inet_ntoa(server.sin_addr));

    unsigned char buf[NET_BUFF_SIZE];

    /*
     * Received hello packets
     */
    ssize_t recv_bytes = 0;
    while (1) {
        if ((recv_bytes = recv(s, buf, sizeof(buf), 0)) < 0) {
            perror("received data error");
            exit(EXIT_FAILURE);
        }

        // telehack message block ends to "."
        if (buf[recv_bytes - 1] == END_TELEHACK_BLOCK) {
            break;
        }
    };
    memset(buf, 0, NET_BUFF_SIZE);

    size_t command_len = strlen(font) + strlen(msg) + 11;
    char *command = calloc(command_len, sizeof(char));

    snprintf(command, command_len, "figlet /%s %s\r\n", font, msg);

    if (send(s, command, command_len * sizeof(char), 0) < 0) {
        perror("send command error");
        exit(EXIT_FAILURE);
    }
    free(command);

    /*
     * Received format message
     */
    while (1) {
        if ((recv_bytes = recv(s, buf, sizeof(buf), 0)) < 0) {
            perror("received data error");
            exit(EXIT_FAILURE);
        }

        printf("%s", buf);

        // telehack message block ends to "."
        if (buf[recv_bytes - 1] == END_TELEHACK_BLOCK) {
            memset(buf, 0, NET_BUFF_SIZE);
            break;
        }
        memset(buf, 0, NET_BUFF_SIZE);
    };

    close(s);
    printf("\nConnecting was closed\n");
    return 0;
}
