#include <boost\asio.hpp>
#include <boost/bind.hpp>
#include "ChatRoom.h"
#include "ChatParticipant.h"

void ChatRoom::join(boost::shared_ptr<ChatParticipant> participant)
{
	std::cout << "ChatRoom : join" << std::endl;

	// ������ ����Ʈ�� �߰�
	_participants.insert(participant);

	// ��� ���� msg�� �����ڿ��� deliver �Լ��� �ѷ��ش�.
	std::for_each(_recent_msgs.begin(), _recent_msgs.end(),
		boost::bind(&ChatParticipant::deliver, participant, _1));
}

// ����(������)�� ä�ù濡�� �����Ѵ�.
void ChatRoom::leave(boost::shared_ptr<ChatParticipant> participant)
{
	std::cout << "ChatRoom : leave" << std::endl;
	_participants.erase(participant);
}

// ä�ù����� msg�� �Ѹ���.
void ChatRoom::deliver(const ChatMessage& msg)
{
	std::cout << "ChatRoom : deliver" << std::endl;

	// ��ȭ ��� ����Ʈ�� msg�� �߰�
	_recent_msgs.push_back(msg);

	// �ִ� ��� msg �������� msg�� ������ msg�� �����Ѵ�.
	while (_recent_msgs.size() > MAX_RECENT_MSG)
		_recent_msgs.pop_front();

	// ��� �������� deliver�� ȣ�����ش�.
	std::for_each(_participants.begin(), _participants.end(),
		boost::bind(&ChatParticipant::deliver, _1, boost::ref(msg)));
}
