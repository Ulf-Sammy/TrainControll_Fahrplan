#pragma once

#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 

constexpr const char* SERVER = "192.168.178.10";   // "127.0.0.1"  // or "localhost" - ip address of UDP server
constexpr const int  LENGTHANSWER = 512;  // max length of answer
constexpr const int  PORT = 8888;  // the port on which to listen for incoming data


class C_UDP_Client
{
private:
	sockaddr_in server = {};
	SOCKET client_socket = 0;
	CString Error_Info;
	CString WifiStatus;
	int slen = 0;
	byte Get_Data[LENGTHANSWER] = { 0 };
public:
	C_UDP_Client();
	~C_UDP_Client();
	bool isClientSocket();
	bool Begin_Winsock();
	void Send_Data(byte* message);
	bool Listen_to_Data();
	byte get_Kennung();
	void get_Wifi_RSSI();
	void connect_Server();
	void disconnect_Server();
	void make_Message(byte Kennung, char* Text, byte* Data);
	void make_Message(byte Kennung, byte Data1, byte* Data);
	void make_Message(byte Kennung, byte Data1, byte Data2, byte* Data);
	void make_Message_LZV_FE(byte Kennung, byte Data1, byte Data2, byte* Data);
	CString Get_Error_Info() const { return Error_Info; }
	CString Get_WifiStatus() const { return WifiStatus; }
	CString Get_Data_String() const;
	void Trace_Data() const;
	byte Get_Data_Byte1() const { return Get_Data[2]; }
	byte Get_Data_Byte(int index) const;
};

