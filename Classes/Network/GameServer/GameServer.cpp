#include <iostream>
#include "GameServer.h"
#include "GameMessage.hpp"
#include "GameSession.h"

#include "../../IDGenerator.h"
#include <boost/make_shared.hpp>

GameServer::GameServer(
	// � ������� ����� ������
	boost::asio::io_service& io_service,
	// ������ ����� endpoint
	const boost::asio::ip::tcp::endpoint& endpoint)
	:
	_io_service(io_service),
	// endpoint�� ��Ź������ acceptor�� �����Ѵ�.
	_acceptor(io_service, endpoint)
{
	std::cout << "GameServer : constructed" << std::endl;
	startAccept();

	std::cout << std::endl << "[ip �ּ�]" << std::endl;

	using boost::asio::ip::tcp;
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(boost::asio::ip::host_name(), "");
	tcp::resolver::iterator iter = resolver.resolve(query);
	tcp::resolver::iterator end; // End marker.
	while (iter != end)
	{
		tcp::endpoint ep = *iter++;
		std::cout << ep << std::endl;
	}
	std::cout << std::endl;

	std::cout << "[port ��ȣ]" << std::endl;
	std::cout << endpoint.port() << std::endl;
}

void GameServer::startAccept()
{
	std::cout << "GameServer : ������ �����ϰ�, �񵿱� accept�� �����Ѵ�." << std::endl;

	unsigned int id = IDGen.genID();
	boost::shared_ptr<GameSession> new_session 
		= boost::make_shared<GameSession>(_io_service, _room, id);

	// �񵿱� accept�� �����Ѵ�.
	// accept �۾��� ���� ��� handleAccept �Լ��� ȣ�����ش�.
	_acceptor.async_accept(
		new_session->socket(),
		boost::bind(
			&GameServer::handleAccept,
			this,
			new_session,
			boost::asio::placeholders::error));
}

// �񵿱� accept�� ���� ���� ���� �۾�
void GameServer::handleAccept(
	boost::shared_ptr<GameSession> session,
	const boost::system::error_code& error)
{
	std::cout << "handleAccept : �񵿱� accept�� ���� ���� ���� �۾� ����" << std::endl;
	if (!error)
	{
		// ������ �۾��� �������ش�.
		session->start();
	}

	// �ٽ� accpet�� �����Ѵ�.
	startAccept();
}

GameRoom& GameServer::getGameRoom()
{
	return _room;
}

