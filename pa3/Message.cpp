/*********************************************
 * Author: Alex Brisnehan                    *
 * Date: 10/27/2014                          *
 * File: Message.cpp                         *
 * Use: The source file for the Message class*
 *********************************************/

#include "Message.h"
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

Message::Message()
{
    msgLen = 0;
    msgContent.clear();
}
Message::Message(char *msg, size_t len)
{
    msgLen = len;
    msgContent.push_front(msg);
}
Message::~Message()
{
    msgContent.clear();
}
Message::msgAddHdr(char *hdr, size_t length)
{
    msgContent.push_front(hdr);
    msgLen = msgLen + length;
}
char* Message::msgStripHdr(int len)
{
    int i,j = 0;
    int k = len
    char *stripped;
    char *rem;

    if((len == 0) || (msgLen < len)){
	return NULL;
    }
    if((int)sizeof(msgContent.front()) == len){
	msgLen = msgLen - len;
	stripped = msgContent.front();
	msgContent.pop_front();
	return stripped;
    }
    else{
	i = (int)sizeof(msgContent.front());
	if( i > len){
	    stripped = msgContent.front();
	    rem = msgContent.front();
	    msgContent.pop_front();
	    rem = rem + len;
	    msgContent.push_front(rem);
	}
	else if( i < len ){
	    stripped = new char[len];
	    for(j = 0; j < len;){
		i = (int)sizeof(msgContent.front());
		k = len - j;
		if( i <= k){
		    rem = msgContent.front();
		    memcpy(stripped + j, rem, i);
		    msgContent.pop_front();
		    rem = rem + k;
		    msgContent.push_front(rem);
		    j = j+k;
		}
	    }
	    msgLen = msgLen - len;
	    return stripped;
	}
    }
}
int Message::msgSplit(Message &secondMsg; size_t len)
{
    list <char *> content;
    int i = 0;
    if((len > msgLen) || (len < 0)){
	return 0;
    }
    for(int j = 0; j < len;){
	i = len - 1;
	if(sizeof(msgContent.front()) <= i){
	    content.push_front(msgContent.front());
	    msgContent.pop_front();
	    j = j + i;
	}
	if(sizeof(msgContent.front()) > i){
	    char * firstNode = new char[i];
	    char * secondNode = msgContent.front();
	    memcpy(firstNode, secondNode, i);
	    content.push_front(firstNode);
	    secondNode = secondNode + i;
	    msgContent.pop_front();
	    msgContent.push_front(secondNode);
	    j = j + i;
	}
    }
    secondMsg.msgLen = mgLen - len;
    secondMsg.msgContent = msgContent;
    msgContent = content;
    msgLen = len;
    return 1;
}
void Message::msgJoin(Message &secondMsg)
{
    list <char *> content = secondMsg.msgContent;
    while(!content.empty()){
	msgContent.push_back(content.front());
	content.pop_front();
    }
    msgLen = secondMsg.msgLen + msgLen;
    secondMsg.msgContent.clear();
    secondMsg.msgLen = 0;
}
size_t Message::msgLen()
{
    return msgLen;
}
void Message::msgFlat(char *buffer)
{
    list <char *> endBuf = msgContent;
    char * node;
    int nodeLen
    for(int i = 0; i < msgLen;){
	node = endBuf.front();
	nodeLen = sizeof(node);
	memcpy(buffer + 1, node, nodeLen);
    }
}
