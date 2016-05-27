#include "ChatServer.h"
#include "ChatMessage.hpp"
#include "ChatSession.h"

ChatServer::ChatServer(
	// � ������� ����� ������
	boost::asio::io_service& io_service,
	// ������ ����� endpoint
	const boost::asio::ip::tcp::endpoint& endpoint)
	:
	_io_service(io_service),
	// endpoint�� ��Ź������ acceptor�� �����Ѵ�.
	_acceptor(io_service, endpoint)
{
	std::cout << "ChatServer : constructed" << std::endl;
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

ChatServer::~ChatServer()
{
	_acceptor.close();
	std::cout << "ChatServer : destructed" << std::endl;
}

void ChatServer::startAccept()
{
	//chat ������ �����Ѵ�.
	boost::shared_ptr<ChatSession> new_session(new ChatSession(_io_service, _room));

	// �񵿱� accept�� �����Ѵ�.
	// accept �۾��� ���� ��� handleAccept �Լ��� ȣ�����ش�.
	_acceptor.async_accept(
		new_session->socket(),
		boost::bind(
			&ChatServer::handleAccept,
			this,
			new_session,
			boost::asio::placeholders::error));
}

// �񵿱� accept�� ���� ���� ���� �۾�
void ChatServer::handleAccept(
	boost::shared_ptr<ChatSession> session,
	const boost::system::error_code& error)
{
	if (!error)
	{
		// ������ �۾��� �������ش�.
		session->start();
	}

	// �ٽ� accpet�� �����Ѵ�.
	startAccept();
}


