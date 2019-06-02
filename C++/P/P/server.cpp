#include <WINSOCK2.H>

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;

#pragma comment(lib,"ws2_32.lib")

void getDataAsServer()
{
	//�����׽���
	WORD myVersionRequest;
	WSADATA wsaData;                    //����ϵͳ��֧�ֵ�WinStock�汾��Ϣ
	myVersionRequest = MAKEWORD(1, 1);  //��ʼ���汾1.1
	int err;
	err = WSAStartup(myVersionRequest, &wsaData);
	if (!err) {
		printf("�Ѵ��׽���\n");
	}
	else {
		//��һ�����׽���
		printf("�׽���δ��!");
		return;
	}
	SOCKET serSocket = socket(AF_INET, SOCK_STREAM, 0);//�����˿�ʶ���׽���
	//��Ҫ�󶨵Ĳ���
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//ip��ַ
	addr.sin_port = htons(2019);//�󶨶˿�
	//���׽��ְ󶨵�ָ���������ַ
	bind(serSocket, (SOCKADDR*)& addr, sizeof(SOCKADDR));//�����
	listen(serSocket, 1);                              //�ڶ������������ܹ����յ�����������
	SOCKADDR_IN clientsocket;
	int len = sizeof(SOCKADDR);
	SOCKET serConn;
	//�ȴ��ͻ��˵�����
	serConn = accept(serSocket, (SOCKADDR*)& clientsocket, &len);
	cout << "�ͻ���" << "������" << endl;             //�ͻ���������
	while (1) {
		char sendBuf[100];
		printf(sendBuf, "server : welcome to server.");
		//�ڶ�Ӧ��IP�����ҽ������ִ�ӡ������
		send(serConn, sendBuf, (int)strlen(sendBuf) + 1, 0);
		char receiveBuf[100];
		//���տͻ��˴�������Ϣ
		recv(serConn, receiveBuf, (int)strlen(receiveBuf) + 1, 0);
		char quit[] = "quit";
		//����ͻ��˴�����quit�źţ������˹رգ��ͻ���Ҳ�ر�
		if (!strcmp(receiveBuf, quit)) {
			break;
		}
		printf("%s\n", receiveBuf);
	}
	closesocket(serConn);   //�ر�
	WSACleanup();           //�ͷ���Դ�Ĳ���
}