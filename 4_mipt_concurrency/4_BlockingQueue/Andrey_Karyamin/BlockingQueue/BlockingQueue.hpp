//
// Created by avk on 11.03.16.
//


// Обычный конструктор. Задаём вместимость очереди.
template <class T>
BlockingQueue<T>::BlockingQueue(unsigned capacity): capacity(capacity)
{}

template<class T>
BlockingQueue<T>::BlockingQueue() : BlockingQueue(static_cast<unsigned>(-1))
{}


// Помещение элемента в очередь.
template <class T>
void BlockingQueue<T>::enqueue(const T& item) {
    // Захватим мютекс, чтобы попробовать положить элемент в очередь.
    std::unique_lock<std::mutex> ourLock(ourMutex);

    // Стоит подождать, пока какой-нибудь другой поток
    // не вытащит по крайней мере один элемент из очереди, чтобы освободилось место.
    while(ourQueue.size() == capacity) {
        someonePopped.wait(ourLock);
    }
    // И теперь уже кладём в очередь.
    ourQueue.push(item);
    // Скажем  кому-нибудь, что мы засунули элемент в очередь.
    someonePushed.notify_one();

    // Отпускаем мютекс
}


template<class T>
T BlockingQueue<T>::dequeue() {
    // Блокируем мютекс, чтобы безопасно извлечь элемент из очереди.
    std::unique_lock<std::mutex> ourLock(ourMutex);
    // Если вдруг очередь пуста, то придётся подождать.
    while(ourQueue.size() == 0) {
        someonePushed.wait(ourLock);
    }
    // Всё, теперь очередь не пуста, извлечём долгожданный элемент.
    T item = ourQueue.front();
    ourQueue.pop();
    // И уведомим кого-нибудь о том, что из очереди был извлечён элемент.
    someonePopped.notify_one();
    return item;
}


// Извлечение элемента из очереди.
template <class T> void BlockingQueue<T>::dequeue(T& item) {
    item = dequeue();
}


template<class T>
BlockingQueue<T>::~BlockingQueue() {
    std::unique_lock<std::mutex> ourLock(ourMutex);
    someonePopped.notify_all();
    someonePushed.notify_all();
}
