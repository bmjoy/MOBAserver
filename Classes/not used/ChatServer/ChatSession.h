#pragma once

#include <boost\asio.hpp>
#include <boost\enable_shared_from_this.hpp>

#include "ChatMessage.hpp"
#include "ChatParticipant.h"
#include "ChatRoom.h"

// chating session
// session�� ������ participant(������)�̶� �� �� �ִ�.
class ChatSession
	:
	public ChatParticipant,
	public boost::enable_shared_from_this<ChatSession>
{
public:
	ChatSession(boost::asio::io_service& io_service, ChatRoom& room);

	~ChatSession();

	boost::asio::ip::tcp::socket& socket();
	
	// ������ ����
	// ä�ù濡 �����ϰ� socket���κ��� read�� �����Ѵ�.
	void start();

	// �������� deliver �Լ�
	// Ŭ���̾�Ʈ���� �ٽ� write�� ���ִ� �Լ�
	// �� �ٽ� �����ϴ� �κ��̴�.
	void deliver(const ChatMessage& msg);

	// �񵿱� read �۾��� ���� ��� ȣ��Ǵ� �Լ�
	void handleReadHeader(const boost::system::error_code& error);

	// �������� �����͸� �޾Ƶ��̴� �Լ��̴�.
	void handleReadBody(const boost::system::error_code& error);

	void handleWrite(const boost::system::error_code& error);

private:
	// client�� ����� socket
	boost::asio::ip::tcp::socket _socket;

	// �� ����(������)�� �������� ä�ù�
	ChatRoom& _room;

	// �� �����ڰ� ���� �޽���
	ChatMessage _read_msg;


	std::deque<ChatMessage> _write_msgs;
};