#include "perMessage.h"
#include "message.h"
#include "protoModel.h"
#include <unistd.h>

#define SLEEP_USEC 50

char* msg_text = "This message has one hundred characters in it. Trust me. I counted. Multiple times. With spaces. Yep\n";

void* dnsApp(void* arg)
{
    procPerMessage* perMessage = (procPerMessage*) arg;
    for (int i = 0; i < 100; i++)
    {
        Message* msg = new Message(msg_text, 100);
        perMessage->dnsSend(DNS_ID, msg);
        usleep(SLEEP_USEC);
    }
}

void* ftpApp(void* arg)
{
    procPerMessage* perMessage = (procPerMessage*) arg;
    for (int i = 0; i < 100; i++)
    {
        Message* msg = new Message(msg_text, 100);
        perMessage->ftpSend(FTP_ID, msg);
        usleep(SLEEP_USEC);
    }
}

void* rdpApp(void* arg)
{
    procPerMessage* perMessage = (procPerMessage*) arg;
    for (int i = 0; i < 100; i++)
    {
        Message* msg = new Message(msg_text, 100);
        perMessage->rdpSend(RDP_ID, msg);
        usleep(SLEEP_USEC);
    }
}

void* telnetApp(void* arg)
{
    procPerMessage* perMessage = (procPerMessage*) arg;
    for (int i = 0; i < 100; i++)
    {
        Message* msg = new Message(msg_text, 100);
        perMessage->telnetSend(TELNET_ID, msg);
        usleep(SLEEP_USEC);
    }
}

int main(int argc, char**argv)
{
    char* outPort;
    char* inPort;

    switch (argc)
    {
        //Empty case
    case 1:
        outPort = "8888";
        inPort = "8889";        
        break;

    case 3:
        outPort = argv[1];
        inPort = argv[2];
        break;

    default:
        printf("ERROR: Use the right syntax.\n");
        exit(0);
    }

    int err;
    pthread_t thread[4];
    procPerMessage* perMessage = new procPerMessage(outPort, inPort);
    sleep(5);

    err = pthread_create(&thread[0], NULL, ftpApp, (void*) perMessage);
    if (err != 0) {
        cout << "Creating the pthread failed: " << err << endl;
        exit(1);
    }

    err = pthread_create(&thread[1], NULL, telnetApp, (void*) perMessage);
    if (err != 0) {
        cout << "Creating the pthread failed: " << err << endl;
        exit(1);
    }

    err = pthread_create(&thread[2], NULL, rdpApp, (void*) perMessage);
    if (err != 0) {
        cout << "Creating the pthread failed: " << err << endl;
        exit(1);
    }

    err = pthread_create(&thread[3], NULL, dnsApp, (void*) perMessage);
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