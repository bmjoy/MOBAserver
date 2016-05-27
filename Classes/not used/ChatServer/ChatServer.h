#pragma once
#include <iostream>
#include <boost\asio.hpp>
#include <boost\bind.hpp>

#include "ChatRoom.h"

class ChatSession;

class ChatServer
{
public:
	ChatServer(
		// � ������� ����� ������
		boost::asio::io_service& io_service,
		// ������ ����� endpoint
		const boost::asio::ip::tcp::endpoint& endpoint);

	~ChatServer();

	void startAccept();

	// �񵿱� accept�� ���� ���� ���� �۾�
	void handleAccept(
		boost::shared_ptr<ChatSession> session,
		const boost::system::error_code& error);

private:
	boost::asio::io_service& _io_service;
	boost::asio::ip::tcp::acceptor _acceptor;
	ChatRoom _room;
};