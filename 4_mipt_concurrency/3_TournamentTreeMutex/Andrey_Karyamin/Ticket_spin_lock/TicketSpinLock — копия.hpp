//
// Created by avk on 25.02.16.
//

#ifndef TICKET_SPIN_LOCK_TICKET_SPIN_LOCK_HPP
#define TICKET_SPIN_LOCK_TICKET_SPIN_LOCK_HPP


#include <atomic>

class TicketSpinLock {
private:
    std::atomic <int> currentTicket; // 123
    std::atomic <int> counter; // 123
public:
	// 123
    TicketSpinLock() {
        currentTicket.store(0);
        counter.store(0);
    }

    int lock() {
        int ticket = counter.fetch_add(1);
        while (currentTicket.load() != ticket);

    }
    void unlock() {
        currentTicket.fetch_add(1);
    }

};


#endif //TICKET_SPIN_LOCK_TICKET_SPIN_LOCK_HPP
