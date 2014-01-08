//
//  wrap.h
//  Chatty
//
//  Created by 徐瑞琦 on 13/10/4.
//  Copyright (c) 2013年 徐瑞琦. All rights reserved.
//

#ifndef Chatty_wrap_h
#define Chatty_wrap_h

extern char *wrap(char buf[], char head[]);

extern char *wrapHead1(char buf[], int *head_len);

extern char *wrapHead2(char buf[], int *head_len, int fd);

extern char *wrapHead3(char buf[], int *head_len);

extern char *getContent(char *dst, char *src, char delimiter);
#endif
