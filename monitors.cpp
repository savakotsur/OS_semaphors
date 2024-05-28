#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <unistd.h> // Для использования функции sleep()

using namespace std;

// Класс монитора для управления билетной системой
class TicketSystem {
public:
    TicketSystem(int numSeats) : availableSeats(numSeats) {}

    // Метод для бронирования места
    void bookTicket(int id) {
        lock_guard<mutex> lock(mtx); // Блокировка мьютекса для синхронизации доступа к состоянию билетной системы

        if (availableSeats > 0) {
            cout << "Покупатель " << id << " забронировал место. Осталось мест: " << --availableSeats << endl;
            // Здесь может быть код для фактического бронирования места
        } else {
            cout << "Покупатель " << id << " не смог забронировать место. Мест больше нет." << endl;
        }
    }

    // Метод для запроса информации о свободных местах
    void inquireAvailability(int id) {
        lock_guard<mutex> lock(mtx); // Блокировка мьютекса для синхронизации доступа к состоянию билетной системы

        cout << "Покупатель " << id << " запрашивает информацию о доступных местах. Осталось мест: " << availableSeats << endl;
    }

private:
    int availableSeats; // Количество доступных мест
    mutex mtx; // Мьютекс для синхронизации доступа к состоянию билетной системы
};

int main() {
    // Создание объекта монитора для билетной системы
    TicketSystem ticketSystem(100); // Предполагаем, что у нас 100 мест

    // Создание потоков для покупателей
    vector<thread> threads;
    for (int i = 1; i <= 10; ++i) {
        threads.push_back(thread(&TicketSystem::bookTicket, &ticketSystem, i));
        threads.push_back(thread(&TicketSystem::inquireAvailability, &ticketSystem, i));
        // Каждый покупатель делает запрос и пытается забронировать место
        // В реальности, эти операции будут выполняться одновременно разными пользователями
        // Здесь мы просто создаем потоки для иллюстрации работы с мониторами
        // В реальной программе потоки будут создаваться по мере необходимости
        sleep(1); // Пауза между созданием потоков для наглядности
    }

    // Дождемся завершения всех потоков
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
