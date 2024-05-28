#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <semaphore.h>
#include <unistd.h> // Для использования функции sleep()

using namespace std;

// Глобальные переменные для хранения информации о состоянии билетной системы
const int NUM_SEATS = 100; // Общее количество мест
int availableSeats = NUM_SEATS; // Количество доступных мест

// Семафоры для синхронизации доступа к общим ресурсам
sem_t ticketAccess;
sem_t inquiryAccess;

// Функция для бронирования места
void bookTicket(int id) {
    sem_wait(&ticketAccess); // Захват семафора доступа к билетам

    // "Критическая секция" - доступ к информации о местах
    if (availableSeats > 0) {
        cout << "Покупатель " << id << " забронировал место. Осталось мест: " << --availableSeats << endl;
        // Здесь может быть код для фактического бронирования места
    } else {
        cout << "Покупатель " << id << " не смог забронировать место. Мест больше нет." << endl;
    }

    sem_post(&ticketAccess); // Освобождение семафора доступа к билетам
}

// Функция для запроса информации о свободных местах
void inquireAvailability(int id) {
    sem_wait(&inquiryAccess); // Захват семафора доступа к запросам

    // "Критическая секция" - запрос информации о местах
    cout << "Покупатель " << id << " запрашивает информацию о доступных местах. Осталось мест: " << availableSeats << endl;

    sem_post(&inquiryAccess); // Освобождение семафора доступа к запросам
}

int main() {
    // Инициализация семафоров
    sem_init(&ticketAccess, 0, 1); // Изначально доступ к билетам разрешен
    sem_init(&inquiryAccess, 0, 1); // Изначально доступ к запросам разрешен

    // Создание потоков для покупателей
    vector<thread> threads;
    for (int i = 1; i <= 10; ++i) {
        threads.push_back(thread(bookTicket, i));
        threads.push_back(thread(inquireAvailability, i));
        // Каждый покупатель делает запрос и пытается забронировать место
        // В реальности, эти операции будут выполняться одновременно разными пользователями
        // Здесь мы просто создаем потоки для иллюстрации работы с семафорами
        // В реальной программе потоки будут создаваться по мере необходимости
        sleep(1); // Пауза между созданием потоков для наглядности
    }

    // Дождемся завершения всех потоков
    for (auto& t : threads) {
        t.join();
    }

    // Уничтожение семафоров
    sem_destroy(&ticketAccess);
    sem_destroy(&inquiryAccess);

    return 0;
}
