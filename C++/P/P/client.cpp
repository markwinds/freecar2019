#include <WINSOCK2.H>

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <string>
using std::string;
#include <conio.h>

#pragma comment(lib,"ws2_32.lib")

#include "Ws2tcpip.h"

void getDataAsClient()
{
	int err;
	WORD versionRequired;
	WSADATA wsaData;                            //����ϵͳ��֧�ֵ�WinStock�汾��Ϣ
	versionRequired = MAKEWORD(1, 1);           //��ʼ���汾1.1
	//ע��WinStock������״̬
	err = WSAStartup(versionRequired, &wsaData);//Э���İ汾��Ϣ
	if (!err)                                   //���ؽ��Ϊ0��ʾ��ʼ��ʧ��
	{
		cout << LPSTR("�ͻ����׽����Ѿ���!\n");
	}
	else
	{
		//����WSAGetLastError()�鿴������Ϣ
		cout << ("�ͻ����׽��ִ�ʧ�ܣ�") << WSAGetLastError() << endl;
		return;//����
	}
	/*
	�����׽��֣�
	��ʽ�׽��֣�   SOCK_STREAM , IPPROTO_TCP
	���ݱ��׽���: SOCK_DGRAM  , IPPROTO_UDP
	*/
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    //������ʽ�׽���
	SOCKADDR_IN clientsock_in;                                          //ר�����Internet ͨ�����Winsock��ַ�ṹ
	//clientsock_in.sin_addr.S_un.S_addr = inet_addr("192.168.4.1");        //ͨ��inet_addr�ṹָ���׽��ֵ�����IP��ַ 
	PCWSTR IPadd = TEXT("192.168.4.1");
	InetPton(AF_INET, IPadd, &clientsock_in.sin_addr.s_addr);
	clientsock_in.sin_family = AF_INET;                                 //ָ��Э�����:AF_INET
	clientsock_in.sin_port = htons(8686);                               //ָ����Ҫ������׽��ֵĴ����˿ںţ�6000

	int fail = connect(clientSocket, (SOCKADDR*)& clientsock_in, sizeof(SOCKADDR));//��ʼ����
	if (fail) {
		cout << "����������ʧ�ܣ������˳�..." << endl;
		_getch();
		return;
	}
	string s;
	while (cin >> s) {
		char receiveBuf[1024];
		//��������
		recv(clientSocket, receiveBuf, 1024, 0);
		cout << receiveBuf << endl;
		//��������
		send(clientSocket, s.c_str(), (int)s.length() + 1, 0);
		if (s == "quit") {
			break;
		}
	}
	closesocket(clientSocket);
	//�ر��׽���
	if (WSACleanup() == SOCKET_ERROR) {
		cout << "�׽��ֹر�ʧ�ܣ�" << WSAGetLastError() << endl;
	}
	else {
		cout << "�׽��ֳɹ��ر�." << endl;
	}
	_getch();
	return;
}
//inet_addr�ṹ��
/*
Struct in_addr {
Union{
Struct{ u_char s_b1, s_b2, s_b3, s_b4; } S_un_b;
Struct{ u_short s_w1, s_w2; } S_un_w;
U_long  S_addr;
}
}
*/
