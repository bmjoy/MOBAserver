#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <sstream>
#include <iomanip>

class ChatMessage
{
public:
	// header�� ��� ����
	// HEADER_LENGTH �ڸ������� ���
	enum { HEADER_LENGTH = 4 };

	// body�� ��� ����
	// message�� ��� ���ڼ�
	enum { MAX_BODY_LENGTH = 1000 };

	ChatMessage()
		:
		_body_length(0)
	{
	}

	const char* data() const
	{
		return _data;
	}

	char* data()
	{
		return _data;
	}

	size_t length() const
	{
		return HEADER_LENGTH + _body_length;
	}

	const char* body() const
	{
		return _data + HEADER_LENGTH;
	}

	char* body()
	{
		return _data + HEADER_LENGTH;
	}

	size_t bodyLength() const
	{
		return _body_length;
	}

	void bodyLength(size_t new_length)
	{
		_body_length = new_length;
		if (_body_length > MAX_BODY_LENGTH)
			_body_length = MAX_BODY_LENGTH;
	}

	// buffer���� �����Ͱ� ���������, �󸶸�ŭ�� ���̰� ����Ǿ����� �� ����� ����.
	// �׷��Ƿ� header�� body�� �и��ϰ�, header �κп� ���̸� ������ �ش�.
	// �� �Լ��� header�� �����͸� ���̷� ��ȯ�� �ִ� �Լ��̴�.
	// ���� �������� header�� body�� ���̰� �ٸ� ���� �̻��� �ִ� ����̰�
	// �̸� return�� ���� �˷��ش�.
	bool decodeHeader()
	{
		char header[HEADER_LENGTH + 1] = "";
		memcpy(header, _data, HEADER_LENGTH);
		_body_length = atoi(header);
		if (_body_length > MAX_BODY_LENGTH)
		{
			_body_length = 0;
			return false;
		}
		return true;
	}

	// �ݴ�� body�� ���̸�ŭ�� ���ۿ� �ִ� �۾�
	// ����� ���̰� HEADER_LENGTH �̱� ������, 
	// HEADER_LENGTH��� ������ �ξ�� �ϰ�,
	// �׷��� �ϱ� ���� setw(HEADER_LENGTH)�� �̿��Ѵ�.
	void encodeHeader()
	{
		std::stringstream ss;
		ss << std::setw(HEADER_LENGTH) << _body_length;
		memcpy(_data, ss.str().c_str(), HEADER_LENGTH);
	}

private:
	// �������� ��ü�� �����ϴ� ����
	char _data[HEADER_LENGTH + MAX_BODY_LENGTH];

	// �������� ���̸� �����ϴ� ����
	size_t _body_length;
};