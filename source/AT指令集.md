
[AT指令集](https://www.espressif.com/sites/default/files/documentation/4a-esp8266_at_instruction_set_cn.pdf)





指令    作用
AT+CWLIF    查看已经连接的设备

进⼊透传模式发送数据，每包最⼤ 2048 字
节，或者每包数据以 20 ms 间隔区分。

发送 +++ 退出透传时，请⾄少间隔 1
秒再发下⼀条 AT 指令。

AT+CIPMUX=0
AT+CIPMODE=1

AT+SAVETRANSLINK—保存透传到 Flash  支持tcp
AT+SAVETRANSLINK=1,"192.168.6.110",1002,"UDP",1005   udp


- AP要透传
    - UDP传输
    - TCP传输


AT+CWSAP="esp","123456789",5,4


AT+CIPSTART="TCP","192.168.4.2",1234


AT+SAVETRANSLINK=1,"192.168.4.2",1234,"TCP"













