//
// Created by avk on 20.03.16.
//

#include <iostream>

// Конструкторы.
template<class T, class H>
ThreadSafeHashSet<T, H>::ThreadSafeHashSet()
        :   ThreadSafeHashSet(DEFAULT_MUTEX_NUMBER,
                              DEFAULT_EXTREME_LOAD_FACTOR, DEFAULT_GROWTH_FACTOR)
{ }


template<class T, class H>
ThreadSafeHashSet<T, H>::ThreadSafeHashSet(unsigned int mutexNumber)
        :   ThreadSafeHashSet(mutexNumber, DEFAULT_EXTREME_LOAD_FACTOR, DEFAULT_GROWTH_FACTOR)
{ }

template<class T, class H>
ThreadSafeHashSet<T, H>::ThreadSafeHashSet(unsigned int mutexNumber, double extremeLoadFactor)
        :   ThreadSafeHashSet(mutexNumber, extremeLoadFactor, DEFAULT_GROWTH_FACTOR)
{ }


template<class T, class H>
ThreadSafeHashSet<T, H>::ThreadSafeHashSet(unsigned int mutexNumber, double extremeLoadFactor,
                                           unsigned int growthFactor)
        :   extremeLoadFactor_      (extremeLoadFactor),
            growthFactor_           (growthFactor),
            mutexNumber_            (mutexNumber),

            currentElementsNumber_  (0),

            stripesLocks_           (mutexNumber),
            buckets_                (mutexNumber)
{ }


// Вставка элемента в хэш-таблицу.
template<class T, class H>
void ThreadSafeHashSet<T, H>::insert(const T& element) {
    // Проверка нужна поскольку это множество. Иначе уже multiset.
    if (contains(element)) {
        return;
    }
    // Расширяет таблицу, если нужно. Описание в заголовке.
    expandIfNeeded();
    // Нужная корзина - в которую попадёт элемент.
    unsigned int neededBucket = getHash(element) % getCurrentBucketsNumber();
    // Мьютекс, который придется заблокировать.
    unsigned int neededMutex = neededBucket % mutexNumber_;
    // Блокируем этот мьютекс.
    stripesLocks_[neededMutex].writeLock();
    buckets_[neededBucket].push_front(element);
    stripesLocks_[neededMutex].writeUnlock();
    // Это блок на атомарную инкрементацию / декрементацию счётчика элементов в таблице.
    std::unique_lock<std::mutex> incrementingLock(elementsCounterLock_);
    // Мы добавили элемент, проинкрементируем счётчик.
    currentElementsNumber_++;
    // Insertion done.
}


// Возвращает хэш
template<class T, class H>
unsigned int ThreadSafeHashSet<T, H>::getHash(T element) const {
    return H()(element);
}


template<class T, class H>
void ThreadSafeHashSet<T, H>::expandIfNeeded() {
    // Проверим, нужно ли вообще расширяться
    // с помощью loadFactor.
    if (getCurrentLoadFactor() < extremeLoadFactor_) {
        return;
    }
    // Если получилось захватить первый мьютекс
    // так, что никто не успел расширить
    // контейнер до нас, то расширять придётся
    // как раз нам.
    bool toBeContinued = smartlyGrabAllMutexes();
    if (!toBeContinued) {
        return;
    }
    // Расширимся...
    unsigned int newBucketsNumber = growthFactor_ * getCurrentBucketsNumber();
    buckets_.resize(newBucketsNumber);
    // Затем нужно перехешироваться.
    reHash(newBucketsNumber);
    // Отпустим все мьютексы.
    releaseAllMutexes();
}

template<class T, class H>
bool ThreadSafeHashSet<T, H>::smartlyGrabAllMutexes() {
    // Запомним перед количество корзин,
    // захватим один мьютекс, и, если окажется,
    // что количество корзин изменилось, то значит кто-то успел расширить/сжать
    // наш контейнер до нас. Бросим это занятие.
    //
    // Достаточно захватить один мьютекс, чтобы
    // предотвратить двукратное расширение/сужение контейнера.

    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    // |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

    // Запомнили количество корзин.
    unsigned int oldBucketsNumber = getCurrentBucketsNumber();
    // Захватили первый мьютекс.
    stripesLocks_[0].writeLock();
    // Проверили.
    if (oldBucketsNumber!= getCurrentBucketsNumber()) {
        // Расширяться/сжиматься не нужно(кто-то уже расширил/сжал до нас)!
        stripesLocks_[0].writeUnlock();
        return false;
    }
    // Если размер остался тем, же то "дозахватим" остальные.
    for (int i = 1; i < stripesLocks_.size(); ++i)
        stripesLocks_[i].writeLock();
    // Расширяться/сжиматься нужно(никто не расширил до нас)!
    return true;
}


// Возвращает текущее количество корзин
template<class T, class H>
unsigned int ThreadSafeHashSet<T, H>::getCurrentBucketsNumber() const {
    return static_cast<unsigned int>(buckets_.size());
}


// Делает рехэш для всех элементов.
template<class T, class H>
void ThreadSafeHashSet<T, H>::reHash(unsigned int bucketsNumber) {
    // Пробежимся по всем корзинам и тупо по всем спискам,
    // перехешировывая всё что только можно.

    // По всем корзинам
    for(int i = 0; i < buckets_.size(); ++i) {
        // По всем элементам корзины
        for(auto it = buckets_[i].before_begin(); it._M_next() != buckets_[i].end();) {
            // Поскольку список односвязный, и удалять элементы мы можем
            // только перед собой, то нужно проверить, не последние ли мы
            // и вообще, есть ли кто-то перед нами.

            //Если нет - пересчитываем хэш, нашу новую корзину
            unsigned int newHash = getHash(*(it._M_next()));
            unsigned int newBucket = newHash % bucketsNumber;
            // Если корзина изменилась, то вставляем туда, где ему место
            // и удаляем отсюда
            if (newBucket != i) {
                // Если хэш у элемента после нас изменился, то положим его туда,
                // где ему место, а здесь его удалим.
                buckets_[newBucket].push_front(*(it._M_next()));
                buckets_[i].erase_after(it);
            } else {
                // Так как ничего в списке не перед нами не удалилось,
                // нужно продвинуться вперед.
                it++;
            }
        };
    };
}


// Отпускает все захваченные мьютексы в контейнере
// с мьютексами защищающими страйпы. По-тупому.s
template<class T, class H>
void ThreadSafeHashSet<T, H>::releaseAllMutexes() {
    for (int i = mutexNumber_ - 1; i >= 0; --i) {
        stripesLocks_[i].writeUnlock();
    }
}


// Возвращает текущий фактор загруженности таблицы.
template<class T, class H>
double ThreadSafeHashSet<T, H>::getCurrentLoadFactor() const {
    double loadFactor = static_cast<double>(currentElementsNumber_) / buckets_.size();
    return loadFactor;
}


// Удаление элемента из хэш таблицы.
template<class T, class H>
void ThreadSafeHashSet<T, H>::remove(const T& element) {
    // Если нужно сжать таблицу - то сожмём.
    // Условие сжатия - в заголовке.
    condenseIfNeeded();
    // Вычислим где он может находиться в таблице
    unsigned int neededBucket = getHash(element) % getCurrentBucketsNumber();
    unsigned int neededMutex = neededBucket % mutexNumber_;
    // Возьмём мьютекс на время удаления.
    stripesLocks_[neededMutex].writeLock();
    // Удалим элемент из таблицы.
    for (auto it = buckets_[neededBucket].before_begin();
         it._M_next() != buckets_[neededBucket].end(); it++)
    {
        if (*(it._M_next()) == element) {
            // Если следующий за нами элемент подлежит удалению, то удалим его.
            buckets_[neededBucket].erase_after(it);
            // Покажем, что в таблице элементов стало на 1 меньше.
            // Предварительно захватим мьютекс на атомарную декрементацию.
            std::unique_lock<std::mutex> decrementingLock(elementsCounterLock_);
            currentElementsNumber_--;

            stripesLocks_[neededMutex].writeUnlock();
            return;
        }
    }
    stripesLocks_[neededMutex].writeUnlock();
}



// Сужение таблицы, при условии, что это необходимо, т.е.
// новый load factor будет меньше чем предельный.
template<class T, class H>
void ThreadSafeHashSet<T, H>::condenseIfNeeded() {
    // Посчитаем новое количество элементов и новый load factor.
    unsigned int oldBucketsNumber = getCurrentBucketsNumber();
    unsigned int newBucketsNumber = oldBucketsNumber / growthFactor_;
    // Число корзин должно быть всегда не меньше числа мьютексов(страйпов).
    if (newBucketsNumber < mutexNumber_) {
        return;
    }
    // Посчитаем новый, будущий load factor.
    double newLoadFactor = currentElementsNumber_ / newBucketsNumber;
    // Проверим, нужно ли нам сокращать таблицу в соответствии с этим newLoadFactor.
    if (newLoadFactor > extremeLoadFactor_) {
        return;
    }
    // Если мы сюда попали, значит нужно уменьшить количество корзин
    // Сделаем сначала reHash от нового количества корзин, чтобы
    // чтобы при resize ничего не потерять

    // Захватим все мьютексы
    bool toBeContinued = smartlyGrabAllMutexes();
    if (!toBeContinued) {
        return;
    }
    // Если успешно захватили, то рехэш и ресайз.
    reHash(newBucketsNumber);
    // И, теперь уменьшим количество корзин
    buckets_.resize(newBucketsNumber);
    // Ну и отпустим все мьютексы.
    releaseAllMutexes();
}


// Функция проверки принадлежности элемента таблице.
template<class T, class H>
bool ThreadSafeHashSet<T, H>::contains(const T& element) {
    // Вычислим хэш, а следовательно и корзину в которой нужно искать.
    unsigned int bucketToCheck = getHash(element) % getCurrentBucketsNumber();
    unsigned int neededMutex = bucketToCheck % mutexNumber_;
    stripesLocks_[neededMutex].readLock();
    // Пройдемся по вычисленной корзине. Элемент если и есть, то только там.
    for (const auto& it : buckets_[bucketToCheck]) {
        if (it == element) {
            // Нашли
            stripesLocks_[neededMutex].readUnlock();
            return true;
        }
    }
    // Не нашли.
    stripesLocks_[neededMutex].readUnlock();
    return false;
}


template<class T, class H>
void ThreadSafeHashSet<T, H>::printTable() const {
    std::cout << "Current table:" << std::endl;
    std::cout << "Elements number = " << currentElementsNumber_<< std::endl;
    std::cout << "Buckets number = " << getCurrentBucketsNumber() << std::endl;
    std::cout << "Load factor = " << getCurrentLoadFactor() << std::endl;
    std::cout << "OUR ELEMENTS : " << std::endl;
    for (int i = 0; i < buckets_.size(); ++i) {
        std::cout << "Bucket[" << i <<"] : start --> ";
        for (auto it = buckets_[i].begin(); it != buckets_[i].end(); it++) {
            std::cout << *it << " --> ";
        }
        std::cout << "finish" << std::endl;
    }
}

// Возвращает количество элементов в хэш-таблице.
template<class T, class H>
unsigned int ThreadSafeHashSet<T, H>::size() {
    return currentElementsNumber_;
}


template<class T, class H>
class ThreadSafeHashSet<T, H>:: HonestRwMutex {
public:
    HonestRwMutex();
    // Захват-отпущение мьютекса писателем.
    void writeLock();
    void writeUnlock();

    // Захват-отпущение мьютекса читателем.
    void readLock();
    void readUnlock();

private:
    // Турникет. Если писатель заблокировался, то через турникет никто не пройдёт.
    std::mutex turnstile;
    // Мьютекс для атомарного инкрементирования/декрементирования счётчика читателей.
    std::mutex lightsSwitch;
    // Кольцо всевластия. У кого кольцо - тот и имеет доступ к данным.
    std::mutex ring;
    // Текущее количество активных читателей.
    int readersNumber;
};

template<class T, class H>
ThreadSafeHashSet<T, H>::HonestRwMutex::HonestRwMutex()
        :   readersNumber(0)
{ }


template<class T, class H>
void ThreadSafeHashSet<T, H>::HonestRwMutex::writeLock() {
    // Блокируем турникет и пробуем захватить кольцо, чтобы начать писать.
    turnstile.lock();
    ring.lock();
    turnstile.unlock();
}


template<class T, class H>
void ThreadSafeHashSet<T, H>::HonestRwMutex::writeUnlock() {
    // Написали всё что нужно, отпускаем кольцо.
    ring.unlock();
}

template<class T, class H>
void ThreadSafeHashSet<T, H>::HonestRwMutex::readLock() {
    // Нужно пройти через турникет, чтобы у писателей был приоритет.
    // Если писатель ждёт, то пройти не получится. Только после того, как он сможет захватить кольцо.
    turnstile.lock();
    turnstile.unlock();

    // Дальше - если кто-то уже читает, то значит кольцо уже у читателей. Его хватать не нужно.
    // Но если это первый читатель, то придётся захватить кольцо, и тогда оно будет у читателей.
    lightsSwitch.lock();
    readersNumber++;
    if (readersNumber == 1) {
        ring.lock();
    }
    lightsSwitch.unlock();
}

template<class T, class H>
void ThreadSafeHashSet<T, H>::HonestRwMutex::readUnlock() {
    // Если это последний читатель - то нужно отпустить кольцо. Доступ к данным будет открыт.
    lightsSwitch.lock();
    readersNumber--;
    if (readersNumber == 0) {
        ring.unlock();
    }
    lightsSwitch.unlock();
}


