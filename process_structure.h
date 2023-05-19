#include <sys/types.h>
#define MAX_NAME_LEN 256

typedef struct {
    char name[MAX_NAME_LEN];
    int initial_time;
    // recebemos a duração mas salvaramos como momento final
    int end_time;
    int is_running;
    int is_ready;
    int is_IO_bounded;
    // flag para verificar se já escalonamos o programa
    int already_scheduled; 
    pid_t pid;
    pid_t father;
} Process;