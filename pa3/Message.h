/*********************************************
 * Author: Alex Brisnehan                    *
 * Date: 10/27/2014                          *
 * File: Message.h                           *
 * Use: The header file for the Message class*
 *********************************************/

#ifndef _MESSAGE_
#define _MESSAGE_

#include <list>
#include <string.h>

using namespace std;

class Message {
    public:
	Message();
	Message(char *msg, size_t len);
	~Message();
	void msgAddHdr(char *hdr, size_t length);
	char *msgStripHdr(int len);
	int msgSplit(Message &secondMsg, size_t len);
	void msgJoin(Message &secondMsg);
	size_t msgLen();
	void msgFlat(char *buffer);
    private:
	size_t msgLen;
	list<char *> msgContent;
};

#endif
