//
//Created by avk on 13.03.16.
//

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
        ThreadPool<T>::ThreadTask myPackage = blockingQueue_.dequeue();
        // Воркер смотрит, не яд ли там.
        // Если яд, то он вскоре завершится.
        while (myPackage.job != nullptr) {
            // Если воркер сюда попал, то задание нормальное.
            // Вычислим результат и отправим его потребителю.
            myPackage.promise->set_value(myPackage.job());
            // Воркер вытаскивает пакет с новым заданием.
            blockingQueue_.dequeue(myPackage);
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
