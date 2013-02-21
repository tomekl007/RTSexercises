/*
 * my_hdr.h
 *
 *  Created on: Nov 13, 2012
 *      Author: Tomek
 */

#ifndef MY_HDR_H_
#define MY_HDR_H_

typedef struct my_msg
{
	short type;
	int rcvid;
	struct sigevent event;
}my_msg;

#define MY_PULSE_CODE _PULSE_CODE_MINAVAIL+5 //5
#define MSG_GIVE_PULSE _IO_MAX+4 //515
#define MY_SERV "my_server_name"


#endif /* MY_HDR_H_ */
