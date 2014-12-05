#include "perProto.h"
#include <iostream>

using namespace std;

int main(){
    procPerProto hostOne("32002", "32001");
    procPerProto hostTwo("32001", "32002");

    char* text = "This is a test";
    Message* msg = new Message(text, 14);

    Message* msg2 = new Message(text, 14);

    Message* msg3 = new Message(text, 14);

    Message* msg4 = new Message(text, 14);

    char test[1024];
    msg->msgFlat(test);
    cout << "Testing created message: " << test << endl;

    hostOne.messageSend(msg, 8);
    hostTwo.messageSend(msg2, 7);
    hostOne.messageSend(msg3, 6);
    hostTwo.messageSend(msg4, 5);

    while(1){
        continue;
    }
}