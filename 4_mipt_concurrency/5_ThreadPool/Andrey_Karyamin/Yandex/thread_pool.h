#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <deque>
#include <exception>


template <class T, class Container = std::deque<T>>
class thread_safe_queue {
public:
    // Обычные конструкторы.
    explicit thread_safe_queue();
    explicit thread_safe_queue(size_t capacity);
    // Запрещённый конструктор копирования.
    thread_safe_queue(const thread_safe_queue<T>&) = delete;
    // Засунуть элемент item в очередь. Засыпает пока очередь полна.
    void enqueue(const T& item);
    void enqueue(T&& item);
    // Вынуть элемент из очереди. Засыпает пока очередь пуста.
    void pop(T &item);

    void shutdown();

    ~thread_safe_queue();
private:
    void enqueuePrepare();
    // Размер нашей очереди. Обусловимся, что если capacity == -1, то размер очереди неограничен.
    // Ну почти.
    size_t capacity;
    // Собственно, очередь, для помещения в неё данных для обмена между потоками.
    Container ourQueue;
    // Мютекс для безопасных операций с очередью.
    std::mutex ourMutex;
    // Условная переменная нужная для ожидания момента, когда очередь перестанет быть полной.
    std::condition_variable someonePopped;
    // Условная переменная нужная для ожидания момента, когда очередь перестанет быть пустой.
    std::condition_variable someonePushed;
    // Флаг завершения работы
    bool isShutdown;
};

template <class T, class C>
void thread_safe_queue<T, C>::shutdown() {
    isShutdown = true;
    someonePopped.notify_all();
}


// Обычный конструктор. Задаём вместимость очереди.
template <class T, class C>
thread_safe_queue<T, C>::thread_safe_queue(size_t capacity): capacity(capacity), isShutdown(false)
{}

template<class T, class C>
thread_safe_queue<T, C>::thread_safe_queue() : thread_safe_queue(static_cast<size_t>(-1))
{}


// Помещение элемента в очередь.
template<class T, class C>
void thread_safe_queue<T, C>::enqueue(const T& item) {
    T temp = item;
    enqueue(std::move(temp));
}
template <class T, class C>
void thread_safe_queue<T, C>::enqueue(T&& item) {
    if (isShutdown) {
        throw std::exception();
    }

    // Захватим мютекс, чтобы попробовать положить элемент в очередь.
    std::unique_lock<std::mutex> ourLock(ourMutex);

    // Стоит подождать, пока какой-нибудь другой поток
    // не вытащит по крайней мере один элемент из очереди, чтобы освободилось место.
    while(ourQueue.size() == capacity && !isShutdown) {
        someonePopped.wait(ourLock);
    }
    if (isShutdown) {
        throw std::exception();
    }
    // И теперь уже кладём в очередь.
    ourQueue.push_back(std::move(item));
    // Скажем  кому-нибудь, что мы засунули элемент в очередь.
    someonePushed.notify_one();

    // Отпускаем мютекс
}
template <class T, class C>
void thread_safe_queue<T, C>::enqueuePrepare() {
    if (isShutdown) {
        throw std::exception();
    }

    // Захватим мютекс, чтобы попробовать положить элемент в очередь.
    std::unique_lock<std::mutex> ourLock(ourMutex);

    // Стоит подождать, пока какой-нибудь другой поток
    // не вытащит по крайней мере один элемент из очереди, чтобы освободилось место.
    while(ourQueue.size() == capacity && !isShutdown) {
        someonePopped.wait(ourLock);
    }
    if (isShutdown) {
        throw std::exception();
    }
}

// Извлечение элемента из очереди.
template<class T, class C> void thread_safe_queue<T, C>::pop(T &item) {
    if (isShutdown && (ourQueue.size() == 0)) {
        throw std::exception();
    }
    // Блокируем мютекс, чтобы безопасно извлечь элемент из очереди.
    std::unique_lock<std::mutex> ourLock(ourMutex);
    // Если вдруг очередь пуста, то придётся подождать.
    while(ourQueue.size() == 0) {
        someonePushed.wait(ourLock);
    }
    // Всё, теперь очередь не пуста, извлечём долгожданный элемент.
    item = std::move(ourQueue.front());
    ourQueue.pop_front();
    // И уведомим кого-нибудь о том, что из очереди был извлечён элемент.
    someonePopped.notify_one();
}


template<class T, class C>
thread_safe_queue<T, C>::~thread_safe_queue() {
    std::unique_lock<std::mutex> ourLock(ourMutex);
    someonePopped.notify_all();
    someonePushed.notify_all();
}




#include <thread>
#include <future>
#include <memory>

// Класс пула потоков для параллельного
// выполнения заданий
template<class T>
class ThreadPool {
public:
    typedef std::shared_ptr<std::promise<T>> SmartPointerToPromise;
    typedef std::shared_ptr<std::future<T>> SmartPointerToFuture;

    // Конструкторы
    ThreadPool();
    explicit ThreadPool(unsigned threadsNumber);
    // Конструктор копирования и '=' запрещены в параллельном мире
    ThreadPool(const ThreadPool<T>& threadPool) = delete;
    ThreadPool<T>& operator=(const ThreadPool<T>& threadPool) = delete;
    // Отправка задания на исполнение потребителем.
    SmartPointerToFuture submitTask(std::function<T()> functionToPerform);
    // Деструктор. Кидает всем потокам отраву и ждёт пока все умрут.
    // Таким образом невыполненных заданий оставаться не будет.
    ~ThreadPool();
private:
    // Количество потоков-исполнителей в пуле.
    unsigned threadsNumber_;
    // Вектор потоков-работников-исполнителей.
    std::vector<std::thread> ourWorkers_;

    // Вспомогательный класс упакованного задания для потоков-исполнителей.
    // Используется как шаблонный параметр блокирующей очереди.
    // Имеет всего лишь два поля:   job - функтор для исполнения
    //                              promise - умный указатель на promise,
    //                              для возвращения результата работы.
    // Описание в ThreadPool.hpp
    class ThreadTask;
    // Блокирующая очередь для передачи заданий исполнителям. Потокобезопасна.
    // Состоит из запакованных заданий ThreadTasks.
    thread_safe_queue<ThreadPool<T>::ThreadTask> blockingQueue_;

// В паре с конструктором.
    // Возвращает количество ядер.
    // Число в кандидаты количества воркеров. Если что-то идёт не так - то четыре.
    unsigned getCoresNumber() const;
    // Возвращает рабочую функцию потоков-исполнителей.
    auto getWorkerFunction();

// Упаковка.
    // Вспомогательная функция для упаковки задания
    // в класс ThreadTask. Заголовок пару строчками выше.
    ThreadPool<T>::ThreadTask packThreadTask(std::function<T()> job,
                               SmartPointerToPromise promise);

// В паре с деструктором.
    // Отправляет в очередь ядовитые задания для воркеров, чтобы их убить.
    void sendPoisonousPills();
    // Работает в паре с деструктором. Ждёт завершения всех потоков.
    void joinAllWorkers();
};



// Конструктор по умолчанию.
// Нужен, если не указан параметр.
template<class T>
ThreadPool<T>::ThreadPool() : ThreadPool(getCoresNumber())
{}
// Конструктор. Запускает потоки-воркеры.
// Ровно столько, сколько указано в параметрах.
template<class T>
ThreadPool<T>::ThreadPool(unsigned threadsNumber) :
        threadsNumber_(threadsNumber)
{
    // Генерируем функцию запуска.
    auto workerFunction = getWorkerFunction();
    // Стартуем наших workers.
    for (int i = 0; i < threadsNumber_; ++i)
        ourWorkers_.push_back(std::thread(workerFunction));
}



// Это метод для потребителя, который хочет
// получить его задание выполненным.
// На входе - функтор.
template<class T>
std::shared_ptr<std::future<T>>
ThreadPool<T>::submitTask(std::function<T()> taskFunction) {
    // Умный указатель на PROMISE, который будет передан воркеру-исполнителю(какому-то).
    auto pointerToPromise = std::make_shared<std::promise<T>>(std::promise<T>());
    // Умный указатель на FUTURE, для возврата тому, кто попросил выполнить задание.
    // Вернется заказчику.
    auto pointerToFuture = std::make_shared<std::future<T>>(pointerToPromise->get_future());
    // Запаковывам задание.
    // Собираем пакет для отправки. Он состоит из указателя на promise и функтора для выполнения.
    ThreadTask taskToSend = packThreadTask(taskFunction, pointerToPromise);
    // Отправляем этот пакет.
    blockingQueue_.enqueue(taskToSend);
    // Возвращаем псевдорезультат заказчику.
    return pointerToFuture;
}




// Деструктор. Убивает все потоки и ждёт их смерти.
template<class T>
ThreadPool<T>::~ThreadPool() {
    // Here we must kill all threads we've run.
    // We'll send them poisonous pills.
    sendPoisonousPills();
    // And then we must join all threads.
    joinAllWorkers();
}


// Вспомогательный класс упакованного задания для потоков-исполнителей.
// Используется как шаблонный параметр блокирующей очереди.
template<class T>
class ThreadPool<T>::ThreadTask {
public:
    // Умный указатель на promise для передачи ответа/сигнала потребителю.
    SmartPointerToPromise promise;
    // Функция для исполнения. Предоставленная работа.
    std::function<T()> job;
};
// Получает количество ядер.
// Если не получается, то возвращает 4;
template<class T>
unsigned ThreadPool<T>::getCoresNumber() const {
    unsigned temp = std::thread::hardware_concurrency();
    return (temp == 0) ? temp : 4;
}



// Здесь мы конструируем функцию, с помощью которой будут
// запускаться наши воркеры и возвращаем её.
template<class T>
auto ThreadPool<T>::getWorkerFunction()
{
    // Лямбда-функция.
    auto ourWorkerFunction = [this] () -> void {
        // Воркер вытаскивает пакет с заданием.
        ThreadPool<T>::ThreadTask myPackage;
        blockingQueue_.pop(myPackage);
        // Воркер смотрит, не яд ли там.
        // Если яд, то он вскоре завершится.
        while (myPackage.job != nullptr) {
            // Если воркер сюда попал, то задание нормальное.
            // Вычислим результат и отправим его потребителю.
            myPackage.promise->set_value(myPackage.job());
            // Воркер вытаскивает пакет с новым заданием.
            blockingQueue_.pop(myPackage);
        };
    };

    // Вернём рабочую функцию.
    return ourWorkerFunction;
}



// Вспомогательный метод для сбора пакета для отправки исполнителям.
template<class T>
typename ThreadPool<T>::ThreadTask
ThreadPool<T>::packThreadTask(std::function<T()> job,
                              SmartPointerToPromise promise) {
    // Класс ThreadTask объявлен в private полях класса.
    // Имеет всего лишь два поля - job, promise.
    ThreadPool<T>::ThreadTask taskToReturn;
    taskToReturn.job = job;
    taskToReturn.promise = promise;
    return taskToReturn;
}



// Шлёт ядовитые задания.
template<class T>
void ThreadPool<T>::sendPoisonousPills() {
    // Ядовитая пилюля будет состоять из:
    // - функтор, при сравнении с nullptr дающий true;
    // - указатель на promise равный также nullptr.
    for (int i = 0; i < threadsNumber_; ++i) {
        // Функтор.
        std::function<T()> nullFunction = nullptr;
        // Promise
        SmartPointerToPromise nullPromise = nullptr;
        // Упаковка яда.
        ThreadPool<T>::ThreadTask poisonousTask = packThreadTask(nullFunction, nullPromise);
        // Отправка яда.
        blockingQueue_.enqueue(poisonousTask);
    }
}



// Ждёт, пока завершатся все потоки.
template<class T>
void ThreadPool<T>::joinAllWorkers() {
    // Пробегается по вектору потоков-исполнителей.
    for (auto& i : ourWorkers_)
        if (i.joinable())
            i.join();
}
