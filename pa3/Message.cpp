/*********************************************
 * Author: Alex Brisnehan                    *
 * Date: 10/27/2014                          *
 * File: Message.cpp                         *
 * Use: The source file for the Message class*
 *********************************************/

#include "Message.h"
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

Message::Message()
{
    msgLength = 0;
    msgContent.clear();
}
Message::Message(char *msg, size_t len)
{
    msgLength = len;
    pair<char *, size_t> mess(msg, len);
    msgContent.push_front(mess);
}
Message::~Message()
{
    msgContent.clear();
}
void Message::msgAddHdr(char *hdr, size_t length)
{
    pair<char *, size_t>head(hdr,length);
    msgContent.push_front(head);
    msgLength = msgLength + length;
}
char* Message::msgStripHdr(int len)
{
    int i = 0;
    int j;
    int k = len;
    char *stripped;

    if((len == 0) || (msgLength < len)){
	return NULL;
    }
    if(((int)msgContent.front().second) == len){
	msgLength = msgLength - len;
	stripped = msgContent.front().first;
	msgContent.pop_front();
	return stripped;
    }
    else{
        stripped = new char[len];
        for(j = 0; j < len;){
	    i = (int)msgContent.front().second;
	    k = len - j;
	    if( i <= k){
		memcpy(stripped + j, msgContent.front().first, i);
		msgContent.pop_front();
		j = j+i;
	    }
	    else{
		memcpy(stripped + j, msgContent.front().first, k);
		msgContent.front().first += k;
		msgContent.front().second -= k;
		j = j + k;
	    }
	}
	msgLength = msgLength - len;
	return stripped;

    }
}
int Message::msgSplit(Message &secondMsg, size_t len)
{
    list <pair<char *, size_t>> content;
    int i = 0;
    int j;
    if((len > msgLength) || (len < 0)){
	return 0;
    }
    for(j = 0; j < (int)len;){
	i = len - j;
	if(((int)msgContent.front().second) <= i){
	    pair<char *, size_t>temp(msgContent.front().first, msgContent.front().second);
	    content.push_back(temp);
	    msgContent.pop_front();
	    j = j + (int)content.back().second;
	}
	else{
	    char * firstNode = new char[i];
	    memcpy(firstNode, msgContent.front().first, i);
	    pair<char *, size_t> secondNode(firstNode, i);
	    content.push_back(secondNode);
	    msgContent.front().first += i;
	    msgContent.front().second -= i;
	    j += i;
	}
    }
    secondMsg.msgLength = msgLength - len;
    secondMsg.msgContent = msgContent;
    msgContent = content;
    msgLength = len;
    return 1;
}
void Message::msgJoin(Message &secondMsg)
{
    list <pair<char *, size_t> > content = secondMsg.msgContent;
    while(!content.empty()){
	msgContent.push_back(content.front());
	content.pop_front();
    }
    msgLength = secondMsg.msgLength + msgLength;
    secondMsg.msgContent.clear();
    secondMsg.msgLength = 0;
}
void Message::msgFlat(char *buffer)
{
    //cout << msgLength << endl;
    list <pair<char *, size_t> > endBuf = msgContent;
    char * node;
    int nodeLen = 0;
    int i;
    for(i = 0; i < msgLength;){
	node = endBuf.front().first;
	nodeLen = (int)endBuf.front().second;
	memcpy(buffer + i, node, nodeLen);
	i += nodeLen;
	endBuf.pop_front();
    }
}
size_t Message::msgLen()
{
    return msgLength;
}
