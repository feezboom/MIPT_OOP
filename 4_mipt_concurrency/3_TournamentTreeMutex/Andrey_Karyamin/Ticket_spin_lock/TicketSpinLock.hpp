//
// Created by avk on 25.02.16.
//

#ifndef TICKET_SPIN_LOCK_TICKET_SPIN_LOCK_HPP
#define TICKET_SPIN_LOCK_TICKET_SPIN_LOCK_HPP

#include <atomic>

// �����������
void ourThreadFunc(int id);

// �������� ������� "��������"
// ������ ����� ���������
class TicketSpinLock {
public:
	TicketSpinLock();

	// ��������� ������
	int lock();

	// ��������� ������
	void unlock();

private:
	std::atomic <int> currentTicket; //
	std::atomic <int> counter; // 
};

inline void TicketSpinLock::TicketSpinLock()
{
	currentTicket.store(0);
	counter.store(0);
}

#endif //TICKET_SPIN_LOCK_TICKET_SPIN_LOCK_HPP
