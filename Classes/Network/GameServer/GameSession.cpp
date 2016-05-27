#include <cassert>
#include <boost/bind.hpp>
#include "GameSession.h"
#include "../Packet/PacketFactory.h"

// ### shared_ptr�� use_count�� ��û���� �����ϴ� ����
// �񵿱�� callback�� ��ϵǴ� �ӵ��� ó���Ǵ� �ӵ����� ������ �����̶�� ����

GameSession::GameSession(
	boost::asio::io_service& io_service,
	GameRoom& room,
	unsigned int id)
	:
	GameParticipant(id),
	// ������ �����. ���Ŀ� �� socket�� accepet�� ���� ������ Ŭ���̾�Ʈ�� ����� ���� ��ġ�� �ȴ�.
	_socket(io_service),
	_room(room),
	_client_name("default"),
	_character_name("Andariel"),
	_team(1)
{
	std::cout << "GameSession : constructed" << std::endl;
}


GameSession::~GameSession()
{
	std::cout << "~GameSession()" << std::endl;
	
	_socket.close();
}

boost::asio::ip::tcp::socket& GameSession::socket()
{
	return _socket;
}

// ������ ����
// ä�ù濡 �����ϰ� socket���κ��� read�� �����Ѵ�.
void GameSession::start()
{
	std::cout << "GameSession : start" << std::endl;

	// ä�ù濡 ���ǿ� ���� this�����͸� �Ѱ��ش�.
	// �����͸� ����Ʈ�����ͷ� �ޱ� ������, shared_from_this�� �̿��Ѵ�.
	// shared_from_this�� ����Ʈ�������� this �����̴�.
	_room.join(shared_from_this());

	// �񵿱�� header�� read�� �Ѵ�.
	// ���� �����ʹ� _read_msg.data()�� �����Ѵ�.
	// read�� ���� ��� handleReadHeader�Լ��� ȣ���Ѵ�.
	boost::asio::async_read(
		_socket,
		boost::asio::buffer(
			_read_msg.getData(),
			GameMessage::HEADER_LENGTH),
		boost::bind(
			&GameSession::handleReadHeader,
			shared_from_this(),
			boost::asio::placeholders::error));
}

void GameSession::send(const GameMessage& msg)
{
	//std::cout << "GameSession : update" << std::endl;

	boost::asio::async_write(
		_socket,
		boost::asio::buffer(
			msg.getData(),
			msg.getLength()),
		boost::bind(
			&GameSession::handleWrite,
			shared_from_this(),
			boost::asio::placeholders::error));

	/*bool write_in_progress = !_write_msgs.empty();

	_write_msgs.push(msg);
	
	// false�� �� ����
	if (!write_in_progress)
	{
		GameMessage temp_msg;
		
		while (_write_msgs.pop(temp_msg))
		{
			boost::asio::async_write(
				_socket,
				boost::asio::buffer(
					temp_msg.getData(),
					temp_msg.getLength()),
				boost::bind(
					&GameSession::handleWrite,
					shared_from_this(),
					boost::asio::placeholders::error));
		}
	}*/
}

// �񵿱� read �۾��� ���� ��� ȣ��Ǵ� �Լ�
void GameSession::handleReadHeader(const boost::system::error_code& error)
{
	// error�� ����, decodeHeader�� �������� ���
	if (!error && _read_msg.decodeHeader())
	{
		// body�� �д´�.
		// ���������� �񵿱�� �д´�.
		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
				_read_msg.getBody(),
				_read_msg.getBodyLength()),
			boost::bind(
				&GameSession::handleReadBody,
				shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		_room.leave(shared_from_this());
	}
}

// �������� �����͸� �޾Ƶ��̴� �Լ��̴�.
void GameSession::handleReadBody(const boost::system::error_code& error)
{
	//std::cout << "GameSession : handleReadBody" << std::endl;
	if (!error)
	{
		// type�� �´� ó��
		//PacketFactory::getInstance().decodeByType(_read_msg);
		PacketFactory::getInstance().getQ().push(_read_msg);

		// �ٽ� header���� �д´�.
		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
				_read_msg.getData(),
				GameMessage::HEADER_LENGTH),
			boost::bind(
				&GameSession::handleReadHeader,
				shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		_room.leave(shared_from_this());
	}
}

// �񵿱� write�� ������ ���� ���¿��� buffer�� �ǵ帮�� ������ ����� ������
// queue�� �̿��Ѵ�.
void GameSession::handleWrite(const boost::system::error_code& error)
{
	//std::cout << "GameSession : handleWrite" << std::endl;
	if (!error)
	{
		/*if (!_write_msgs.empty())
		{
			GameMessage temp_msg;
			if (_write_msgs.pop(temp_msg))
			{
				boost::asio::async_write(
					_socket,
					boost::asio::buffer(
						temp_msg.getData(),
						temp_msg.getLength()),
					boost::bind(
						&GameSession::handleWrite,
						shared_from_this(),
						boost::asio::placeholders::error));
			}
			else
			{
				std::cout << "failed to pop" << std::endl;
			}
		}*/
	}
	else
	{
		_room.leave(shared_from_this());
	}
}


std::string GameSession::getClientName() const
{
	return _client_name;
}

std::string GameSession::getCharacterName() const
{
	return _character_name;
}

int GameSession::getTeam() const
{
	return _team;
}

void GameSession::setClientName(const std::string& name)
{
	_client_name = name;
}

void GameSession::setCharacterName(const std::string& name)
{
	_character_name = name;
}

void GameSession::setTeam(int team)
{
	_team = team;
}