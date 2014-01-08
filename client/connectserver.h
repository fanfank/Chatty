//
//  connectserver.h
//  Chatty
//
//  Created by 徐瑞琦 on 13/10/4.
//  Copyright (c) 2013年 徐瑞琦. All rights reserved.
//

#ifndef Chatty_connectserver_h
#define Chatty_connectserver_h

extern int connectServer(char addr[], char port[]);

extern int connectWait(int fd, struct sockaddr *addr, size_t size);

#endif
