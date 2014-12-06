#include "perProto.h"
#include "message.h"
#include <unistd.h>

#define SLEEP_USEC 50

char* msg_text = "This message has one hundred characters in it. Trust me. I counted. Multiple times. With spaces. Yep\n";

// Struct for sending to pipes
struct pipeStuff {
    int protocol_id;
    Message *msg;
};

void* dnsApp(void* arg)
{
    procPerProto* perProto = (procPerProto*) arg;
    for (int i = 0; i < 100; i++)
    {
        Message* msg = new Message(msg_text, 100);
        pipeStuff dns;
        dns.protocol_id = 5;
        dns.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(perProto->dnsPipeSend.pipeMutex);

        // Write to ftp send pipe
        write(perProto->dnsPipeSend.pipe_d[1], (char*) &dns, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(perProto->dnsPipeSend.pipeMutex);
        usleep(SLEEP_USEC);
    }
}

void* ftpApp(void* arg)
{
    procPerProto* perProto = (procPerProto*) arg;
    for (int i = 0; i < 100; i++)
    {
        Message* msg = new Message(msg_text, 100);
        pipeStuff ftp;
        ftp.protocol_id = 5;
        ftp.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(perProto->ftpPipeSend.pipeMutex);

        // Write to ftp send pipe
        write(perProto->ftpPipeSend.pipe_d[1], (char*) &ftp, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(perProto->ftpPipeSend.pipeMutex);
        usleep(SLEEP_USEC);
    }
}

void* rdpApp(void* arg)
{
    procPerProto* perProto = (procPerProto*) arg;
    for (int i = 0; i < 100; i++)
    {
        Message* msg = new Message(msg_text, 100);
        pipeStuff rdp;
        rdp.protocol_id = 5;
        rdp.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(perProto->rdpPipeSend.pipeMutex);

        // Write to ftp send pipe
        write(perProto->rdpPipeSend.pipe_d[1], (char*) &rdp, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(perProto->rdpPipeSend.pipeMutex);
        usleep(SLEEP_USEC);
    }
}

void* telnetApp(void* arg)
{
    procPerProto* perProto = (procPerProto*) arg;
    for (int i = 0; i < 100; i++)
    {
        Message* msg = new Message(msg_text, 100);
        pipeStuff tel;
        tel.protocol_id = 6;
        tel.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(perProto->telnetPipeSend.pipeMutex);

        // Write to ftp send pipe
        write(perProto->telnetPipeSend.pipe_d[1], (char*) &tel, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(perProto->telnetPipeSend.pipeMutex);
        usleep(SLEEP_USEC);
    }
}

int main(int argc, char**argv)
{
    char* inPort;
    char* outPort;

    switch (argc)
    {
    case 1:
        outPort = "8889";
        inPort = "8888";        
        break;

    case 3:
        outPort = argv[1];
        inPort = argv[2];
        break;

    default:
        printf("ERROR: USe valid syntax\n");
        exit(0);
    }

    int err;
    pthread_t thread[4];
    procPerProto* perProto = new procPerProto(outPort, inPort);
    sleep(5);

    err = pthread_create(&thread[0], NULL, ftpApp, (void*) perProto);
    if (err != 0) {
        cout << "Creating the pthread failed: " << err << endl;
        exit(1);
    }

    err = pthread_create(&thread[1], NULL, telnetApp, (void*) perProto);
    if (err != 0) {
        cout << "Creating the pthread failed: " << err << endl;
        exit(1);
    }

    err = pthread_create(&thread[2], NULL, rdpApp, (void*) perProto);
    if (err != 0) {
        cout << "Creating the pthread failed: " << err << endl;
        exit(1);
    }

    err = pthread_create(&thread[3], NULL, dnsApp, (void*) perProto);
    if (err != 0) {
        cout << "Creating the pthread failed: " << err << endl;
        exit(1);
    }

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);
    pthread_join(thread[3], NULL);

    while(1){
        sleep(1);
    }

    return 0;
}