#pragma once

#include <boost\asio.hpp>
#include <boost\enable_shared_from_this.hpp>

#include "GameMessage.hpp"
#include "GameParticipant.h"
#include "GameRoom.h"

#include <boost\lockfree\queue.hpp>

// session�� ���� �ܿ��� Ŭ���̾�Ʈ�� ����ϴ� �븮���̴�.
// �� client�� 1:1 �����ȴ�.
class GameSession
	:
	public GameParticipant,
	public boost::enable_shared_from_this<GameSession>
{
public:
	GameSession(
		boost::asio::io_service& io_service,
		GameRoom& room,
		unsigned int id);

	virtual ~GameSession();

	boost::asio::ip::tcp::socket& socket();
	

	std::string		getClientName() const;
	std::string		getCharacterName() const;
	int				getTeam() const;
	
	void			setClientName(const std::string& name);
	void			setCharacterName(const std::string& name);
	void			setTeam(int team);


	// ������ ����
	// ä�ù濡 �����ϰ� socket���κ��� read�� �����Ѵ�.
	void start();

	// ��� client���� �񵿱� write
	virtual void send(const GameMessage& msg) override;

	// �񵿱� read �۾��� ���� ��� ȣ��Ǵ� �Լ�
	void handleReadHeader(const boost::system::error_code& error);

	// �������� �����͸� �޾Ƶ��̴� �Լ��̴�.
	void handleReadBody(const boost::system::error_code& error);

	void handleWrite(const boost::system::error_code& error);

private:

	// client�� ����� socket
	boost::asio::ip::tcp::socket _socket;

	// �� ����(������)�� �������� ä�ù�
	GameRoom& _room;

	// �� �����ڰ� ���� �޽���
	GameMessage _read_msg;

	// �񵿱� �۾��� �Ϲ� �۾��� critical section�� ����� ���α׷��� ���� �� �ִ�.
	// �׷��Ƿ� lockfree queue�� �̿��Ѵ�.
	boost::lockfree::queue<GameMessage, boost::lockfree::capacity<1000> > _write_msgs;
	

	// ������ ����
	std::string		_client_name;
	std::string		_character_name;
	int				_team;
};