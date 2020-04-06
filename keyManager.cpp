#include "stdafx.h"
#include "keyManager.h"


keyManager::keyManager()
{
}


keyManager::~keyManager()
{
}

HRESULT keyManager::init()
{
	for (int i = 0; i < KEYMAX; i++)
	{
		this->getKeyUp().set(i, false);
	}

	return S_OK;
}

void keyManager::release()
{
}

bool keyManager::isOnceKeyDown(int key)
{
	//��� Ű�� ��������
	if (GetAsyncKeyState(key) & 0x8000)
	{
		//���࿡ �� Ű�� ���������� �ʴٸ�
		if (!this->getKeyDown()[key])
		{
			//�������ٰ� ��ȣ�� ������
			this->setKeyDown(key, true);

			return true;
		}
	}
	else this->setKeyDown(key, false);

	return false;
}

bool keyManager::isOnceKeyUp(int key)
{
	// ��� Ű�� ��������, �������ٰ� ��ȣ�� ������.
	if (GetAsyncKeyState(key) & 0x8000) this->setKeyUp(key, true);
	else
	{
		if (this->getKeyUp()[key])
		{
			this->setKeyUp(key, false);

			return true;
		}
	}
	return false;
}

bool keyManager::isStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;

	return false;
}

bool keyManager::isToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;

	return false;
}

bool keyManager::isOncekeytwoDown(int one, int two)
{
	if ((GetAsyncKeyState(one) & 0x8000) && (GetAsyncKeyState(two) & 0x8000))
	{
		if ((!(this->getKeyDown()[one]) && !(this->getKeyDown()[two])))
		{
			this->setKeyTwoDown(one, two, true);

			return true;
		}
	}
	// ����ó�� ����

	return false;
}