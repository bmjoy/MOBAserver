#pragma once
#include <iostream>
#include <boost\asio.hpp>
#include <boost\bind.hpp>

#include "GameRoom.h"

class GameSession;

class GameServer
{
public:
	GameServer(
		// � ������� ����� ������
		boost::asio::io_service& io_service,
		// ������ ����� endpoint
		const boost::asio::ip::tcp::endpoint& endpoint);

	void startAccept();

	// �񵿱� accept�� ���� ���� ���� �۾�
	void handleAccept(
		boost::shared_ptr<GameSession> session,
		const boost::system::error_code& error);

	GameRoom& getGameRoom();

private:
	boost::asio::io_service& _io_service;
	boost::asio::ip::tcp::acceptor _acceptor;
	GameRoom _room;
};