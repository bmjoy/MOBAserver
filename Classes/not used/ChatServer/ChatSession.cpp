#include <boost/bind.hpp>
#include "ChatSession.h"

ChatSession::ChatSession(boost::asio::io_service& io_service, ChatRoom& room)
	:
	// ������ �����.
	// ���Ŀ� �� socket�� accepet�� ���� ������ Ŭ���̾�Ʈ�� ����� ���� ��ġ�� �ȴ�.
	_socket(io_service),
	// ä�ù��� �����´�.
	_room(room)
{
	std::cout << "ChatSession : constructed" << std::endl;
}

ChatSession::~ChatSession()
{
	std::cout << "ChatSession : destructed" << std::endl;
}

boost::asio::ip::tcp::socket& ChatSession::socket()
{
	return _socket;
}

// ������ ����
// ä�ù濡 �����ϰ� socket���κ��� read�� �����Ѵ�.
void ChatSession::start()
{
	// ä�ù濡 ���ǿ� ���� this�����͸� �Ѱ��ش�.
	// �����͸� ����Ʈ�����ͷ� �ޱ� ������, shared_from_this�� �̿��Ѵ�.
	// shared_from_this�� ����Ʈ�������� this �����̴�.
	_room.join(shared_from_this());

	// �񵿱�� header�� read�� �Ѵ�.
	// ���� �����ʹ� _read_msg.data()�� �����Ѵ�.
	// read�� ���� ��� recursiveReadHeader�Լ��� ȣ���Ѵ�.
	boost::asio::async_read(
		_socket,
		boost::asio::buffer(
			_read_msg.data(),
			ChatMessage::HEADER_LENGTH),
		boost::bind(
			&ChatSession::handleReadHeader,
			shared_from_this(),
			boost::asio::placeholders::error));
}

// �������� deliver �Լ�
// Ŭ���̾�Ʈ���� �ٽ� write�� ���ִ� �Լ�
// �� �ٽ� �����ϴ� �κ��̴�.
void ChatSession::deliver(const ChatMessage& msg)
{
	std::cout << "deliver msg" << std::endl;

	// write_msg�� ��������� false ???????????
	bool write_in_progress = !_write_msgs.empty();

	_write_msgs.push_back(msg);

	// false�� �� ����
	if (!write_in_progress)
	{
		// client���� �񵿱�� _write_buffer�� ù �κ��� write�Ѵ�.
		// �� Ŭ���̾�Ʈ�� write�� �Ѵ�.
		// �񵿱� write�� ���� ��� handleDeliver�Լ��� ȣ���Ѵ�.
		boost::asio::async_write(
			_socket,
			boost::asio::buffer(
				_write_msgs.front().data(),
				_write_msgs.front().length()),
			boost::bind(
				&ChatSession::handleWrite,
				shared_from_this(),
				boost::asio::placeholders::error));
	}
}

// �񵿱� read �۾��� ���� ��� ȣ��Ǵ� �Լ�
void ChatSession::handleReadHeader(const boost::system::error_code& error)
{
	// error�� ����, decodeHeader�� �������� ���
	if (!error && _read_msg.decodeHeader())
	{
		// body�� �д´�.
		// ���������� �񵿱�� �д´�.
		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
				_read_msg.body(),
				_read_msg.bodyLength()),
			boost::bind(
				&ChatSession::handleReadBody,
				shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		_room.leave(shared_from_this());
	}
}

// �������� �����͸� �޾Ƶ��̴� �Լ��̴�.
void ChatSession::handleReadBody(const boost::system::error_code& error)
{
	if (!error)
	{
		// ä�ù����� msg�� �Ѹ���.
		_room.deliver(_read_msg);

		// �ٽ� header���� �д´�.
		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
				_read_msg.data(),
				ChatMessage::HEADER_LENGTH),
			boost::bind(
				&ChatSession::handleReadHeader,
				shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		_room.leave(shared_from_this());
	}
}


void ChatSession::handleWrite(const boost::system::error_code& error)
{
	if (!error)
	{
		_write_msgs.pop_front();
		if (!_write_msgs.empty())
		{
			boost::asio::async_write(
				_socket,
				boost::asio::buffer(
					_write_msgs.front().data(),
					_write_msgs.front().length()),
				boost::bind(
					&ChatSession::handleWrite,
					shared_from_this(),
					boost::asio::placeholders::error));
		}
	}
	else
	{
		_room.leave(shared_from_this());
	}
}
