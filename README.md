Chatty
======  

一个非常简单的聊天小程序 —— A very simple chatting software
You are free to download/use/modify these codes for __non-commercial__ uses     

===  
  
本程序主要用来熟悉多线程、网络编程，程序的界面很简单——其实就是控制台，没有界面……  
![Frame](https://raw.github.com/fanfank/Chatty/master/appendix/screenshots/Chatty.png)  

###程序功能
1. 聊天功能1—— *回显* ，选择模式1，即 *1 echo back* 即可  
2. 聊天功能2—— *自定义发送对象* ，选择模式2，即 *2 Specify a receiver* ，然后会列出当前在线的用户，显示的格式是： *选择号 IP地址 端口地址 用户名称* ，输入选择号后，输入信息回车即可  
3. 聊天功能3—— *广播* ，选择模式3，即 *3 Broadcast to all online clients* ，发送消息即可广播，包括自己在内会收到广播内容  
     
###安装方法   
+ Release版：可以单独到`./client`或者`./server`下输入`make`，也可以在主目录下输入`make`   
+ Debug版：此版本可以使用GDB调试，方法同上，不过`make`要变成`make debug`   
完成后直接运行对应的可执行程序——客户端是`client`或者`client_debug`，服务器是`server`或者`server_debug`   

###测试环境  
  在Ubuntu64位下测试成功，本地测试下服务器IP地址是 *127.0.0.1* ，端口是 *4001* ，可以在宏里面修改，客户端也可以在运行后重新输入。  
  
###TODO
> 1. 或者放到网上试试连接？  
> 2. ~~在客户端A选择模式的时候，服务器认为客户端A在线，这样使得其它用户B可能会指定客户端A作为发送对象，这个时候如果客户端A选择模式2可能会出错~~    
> 3. 目前写套接字的工作由Worker完成，尝试写Sender进行进一步测试
> 4. 让压力测试更加实际，加入模式2和模式3的测试等    

===
###版本记录 Version Logs  
####Vesrion 0.2
1. 加入了链表，使得在广播的时候无需遍历整个描述符数组   
2. 增加了客户端是否就绪的判定，如果客户端A选择了模式2，在没有选择好指定发送对象的时候，其它客户端无法发送信息给客户端A    
  
####Version 0.1  
1. 第一次发布  

===  
+ Git: fanfank
+ Code author: Reetsee.Xu  
+ Contact email: reetsee.xu@gmail.com  

<div class = "footer">
&copy; 2013 Reetsee.Xu
</div>

*README.md Last update: 20130908*
