#pragma once

#include <boost\shared_ptr.hpp>
#include <boost\atomic.hpp>

#include <map>
#include <set>
#include <iostream>
#include <queue>
#include <array>

class GameParticipant;
class GameMessage;

class GameRoom
{
public:
	enum { SLOT_SIZE = 6, SLOT_EMPTY = 0, SLOT_FULL = -1};

	GameRoom();

	std::map<unsigned int, boost::shared_ptr<GameParticipant> >& getParticipants();

	void incClientReadyCnt();

	void decClientReadyCnt();

	void clearClientReadyCnt();

	int getClientReadyCnt() const;

	bool isReadyToStart() const;

	void setRoomState(int state);

	int getRoomState() const;

	int getSlotIndex(unsigned int id) const;

	void setParticipantName(unsigned int id, const std::string& client_name);

	void setParticipantCharacter(unsigned int id, const std::string& cname);

	void setParticipantTeam(unsigned int id, int team);

	void setParticipantClickReady(unsigned int id, int ready_flag);


	// ����(������)�� ä�ù濡 ������Ű�� ���� command_que�� ���� ����� �ִ´�.
	void join(boost::shared_ptr<GameParticipant> participant);

	// ����(������)�� ä�ù濡�� �����ϱ� ���� command_que�� ���� ����� �ִ´�.
	void leave(boost::shared_ptr<GameParticipant> participant);

	void update();


	void send(unsigned int id, const GameMessage& msg);

	void sendAll(const GameMessage& msg);

	void sendExceptOne(unsigned int id, const GameMessage& msg);

private:

	unsigned int findSlotIndex() const;

	bool updateSlotAtJoin(const boost::shared_ptr<GameParticipant>& participant);

	void updateSlotAtLeave(unsigned int leave_id);


	// ������ ����Ʈ
	std::map<unsigned int, boost::shared_ptr<GameParticipant> > _participants;
	
	enum { JOIN, LEAVE };

	// ��ȸ ���� erase�� insert�� �Ͼ�� �ȵǱ� ������ command_que�� �ΰ�,
	// ���� loop�� ������ ��ȸ ���� ���� ó���ǵ��� �Ѵ�.
	// lock free queue�� �ؾ� �� �� ������, ������ ������ ���� ����
	// ���� shared_ptr�� �Ǿ��ֱ� ������ �߰��� raw pointer�� ����ϸ� �޸𸮰� �����ȴ�.
	std::queue<std::pair<int, boost::shared_ptr<GameParticipant> > > _command_que;

	boost::atomic<int> _client_ready_cnt;

	int _room_state;

	std::array<unsigned int, SLOT_SIZE> _slots;
};