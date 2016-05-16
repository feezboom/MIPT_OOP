//
// Created by avk on 25.02.16.
//

#ifndef TICKET_SPIN_LOCK_TICKET_SPIN_LOCK_HPP
#define TICKET_SPIN_LOCK_TICKET_SPIN_LOCK_HPP

#include <atomic>

// Определение
void ourThreadFunc(int id);

// Алгоритм мютекса "Булочная"
// Сильно греет процессор
class TicketSpinLock {
public:
	TicketSpinLock();

	// Захватить мютекс
	int lock();

	// Отпустить мютекс
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
