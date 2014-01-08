//
//  communicate.h
//  Chatty
//
//  Created by 徐瑞琦 on 13/10/4.
//  Copyright (c) 2013年 徐瑞琦. All rights reserved.
//

#ifndef Chatty_communicate_h
#define Chatty_communicate_h


extern void communicate(int server_fd);
extern void setMode(int *);
extern int setBufHead(char buf[], const int mode, int *head_len, int fd);
extern int setChatmate(char buf[], int fd);
extern void startChatting(char buf[], int mode, int head_len, int fd);
extern void *threadRead(void *arg);
extern void printSentMsg(char buf[]);
extern void printReceivedMsg(char buf[]);

#endif
