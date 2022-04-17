#include <inttypes.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <linux/limits.h>

#define BUF_LEN        (1024 * (sizeof(struct inotify_event) + 16))
#define SOCKET_BUF     1024
#define MSG_SIZE       100

const char unknown_cmd[] = "unknown command";

void err_exit(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void file_monitoring(char *path, int client_sock) {
    int inotifyFd = inotify_init();
    if (inotifyFd == -1) {
        err_exit("Inotify init error");
    }

    int wd = inotify_add_watch(inotifyFd, ".", IN_CREATE | IN_MODIFY | IN_DELETE);

    if (wd == -1) {
        err_exit("add inotify watch error");
    }

    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        err_exit("open watching file failed");
    }

    printf("start watch to file: %s\n", path);

    char buf[BUF_LEN];
    int numRead;
    struct inotify_event *event;
    char *p;

    struct stat file_stat;
    char *msg = calloc(MSG_SIZE, sizeof(char));
    while (1) {
        numRead = read(inotifyFd, buf, BUF_LEN);

        if (numRead == 0) {
            break;
        }

        if (numRead == -1) {
            err_exit("read error");
        }

        for (p = buf; p < buf + numRead;) {
            event = (struct inotify_event *) p;
            if (strcmp(event->name, path) == 0) {
                if (event->mask & IN_CREATE) {
                    snprintf(msg, MSG_SIZE, "The file %s was create.\n", event->name);
                    fp = fopen(path, "rb");
                } else if (event->mask & IN_DELETE) {
                    snprintf(msg, MSG_SIZE, "The file %s was deleted.\n", event->name);
                    fclose(fp);
                } else if (event->mask & IN_MODIFY) {
                    fstat(fileno(fp), &file_stat);
                    snprintf(msg, MSG_SIZE, "The actual file size: %" PRId64 "\n", file_stat.st_size);
                }

                printf("%s", msg);

                if (send(client_sock, msg, strlen(msg), 0) == -1) {
                    printf("Error data sent!\n");
                } else {
                    printf("Data sent!\n");
                }

                memset(msg, 0, MSG_SIZE * sizeof(char));
            }

            p += sizeof(struct inotify_event) + event->len;
        }
    }

    free(msg);
    fclose(fp);
}

void run_server(char *socket_path, char *monitor_file) {
    int server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock == -1) {
        err_exit("socket init error");
    }

    struct sockaddr_un server_sockaddr;
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));

    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, socket_path);
    uint32_t len = sizeof(server_sockaddr);

    unlink(socket_path);
    int rc = bind(server_sock, (struct sockaddr *) &server_sockaddr, len);
    if (rc == -1) {
        close(server_sock);
        err_exit("socket bind error");
    }

    rc = listen(server_sock, 1);
    if (rc == -1) {
        close(server_sock);
        err_exit("socket bind error");
    }

    printf("listen %s...\n", socket_path);

    struct sockaddr_un client_sockaddr;
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    int client_sock = accept(server_sock, (struct sockaddr *) &client_sockaddr, &len);
    if (client_sock == -1) {
        close(server_sock);
        close(client_sock);
        err_exit("socket accept error");
    }

    printf("waiting to read...\n");


    char *buf = calloc(SOCKET_BUF, sizeof(char));
    while (1) {
        int bytes_rec = recv(client_sock, buf, SOCKET_BUF * sizeof(char), 0);
        if (bytes_rec == -1) {
            close(server_sock);
            close(client_sock);
            err_exit("received error");
        }

        printf("DATA RECEIVED = %s\n", buf);

        if (strcmp(buf, "SUBSCRIBE") == 0) {
            printf("subscribe success...\n");
            break;
        }

        if (send(client_sock, unknown_cmd, strlen(unknown_cmd), 0) == -1) {
            close(server_sock);
            close(client_sock);
            err_exit("received error");
        }

        memset(buf, 0, SOCKET_BUF * sizeof(char));
    }
    free(buf);

    file_monitoring(monitor_file, client_sock);
}

void daemonize(const char *cmd) {
    int fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;

    openlog(cmd, LOG_CONS, LOG_DAEMON);

    umask(0);

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        err_exit("get limit error");

    if ((pid = fork()) < 0)
        err_exit("fork error");
    else if (pid != 0)
        exit(0);
    setsid();

    signal(SIGHUP, SIG_IGN);

    if ((pid = fork()) < 0)
        syslog(LOG_CRIT, "fork call error");
    else if (pid != 0)
        exit(0);

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        err_exit("getcwd() error");
    }

    if (chdir(cwd) < 0)
        syslog(LOG_CRIT, "can't set root catalog");

    int x;
    for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }

    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2)
        syslog(LOG_CRIT, "error files descriptors %d %d %d",
               fd0, fd1, fd2);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        err_exit("Invalid watching file path");
    };

    if (argc < 3) {
        err_exit("Invalid socket path");
    };

    int sock_idx = 2;
    int f_mon_idx = 1;
    if (strcmp(argv[f_mon_idx], "-d") == 0) {
        daemonize(argv[0]);

        sock_idx += 1;
        f_mon_idx += 1;
    }

    run_server(argv[sock_idx], argv[f_mon_idx]);

    exit(EXIT_SUCCESS);
}
