#pragma once

#include <set>
#include <deque>
#include <iostream>
#include "ChatMessage.hpp"

class ChatParticipant;

class ChatRoom
{
public:
	// ����(������)�� ä�ù濡 ������Ų��.
	void join(boost::shared_ptr<ChatParticipant> participant);

	// ����(������)�� ä�ù濡�� �����Ѵ�.
	void leave(boost::shared_ptr<ChatParticipant> participant);

	// ä�ù����� msg�� �Ѹ���.
	void deliver(const ChatMessage& msg);

private:

	// ä�ù� ������ ����Ʈ
	std::set< boost::shared_ptr<ChatParticipant> > _participants;
	
	// �ִ� ��� msg
	enum { MAX_RECENT_MSG = 100 };

	// msg ����Ʈ
	std::deque<ChatMessage> _recent_msgs;
};