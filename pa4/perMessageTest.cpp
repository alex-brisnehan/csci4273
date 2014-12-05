#include <iostream>
#include <stdio.h>
#include "perMessage.h"
#include "message.h"
#include "protoModel.h"
#include <unistd.h>

using namespace std;

int main(){
    procPerMessage hostOne("32000", "32001");
    procPerMessage hostTwo("32001", "32000");

    sleep(1);
    char* text = "This is a test";
    
    Message* msg1 = new Message(text, 14);
    Message* msg2 = new Message(text, 14);
    Message* msg3 = new Message(text, 14);
    Message* msg4 = new Message(text, 14);

    hostOne.ftpSend(FTP_ID, msg1);
    hostOne.telnetSend(TELNET_ID, msg2);
    hostOne.rdpSend(RDP_ID, msg3);
    hostOne.dnsSend(DNS_ID, msg4);

    Message* msg5 = new Message(text, 14);
    Message* msg6 = new Message(text, 14);
    Message* msg7 = new Message(text, 14);
    Message* msg8 = new Message(text, 14);

    hostTwo.ftpSend(FTP_ID, msg5);
    hostTwo.telnetSend(TELNET_ID, msg6);
    hostTwo.rdpSend(RDP_ID, msg7);
    hostTwo.dnsSend(DNS_ID, msg8);

    while(1){
        continue;
    }
}