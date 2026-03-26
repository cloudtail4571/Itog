#include <iostream>
#include <string>
using namespace std;

/*
ПОЧЕМУ STATE ЛУЧШЕ IF-ELSE:
1. Инкапсуляция: Логика каждого состояния лежит в своем классе. Если нужно изменить
   поведение "Нет монет", мы правим только NoCoinState, не боясь сломать остальное.
2. Принцип открытости/закрытости (OCP): Мы добавили WinnerState, практически не меняя
   основной код VendingMachine. При if-else пришлось бы переписывать каждый метод автомата.
3. Отсутствие "спагетти-кода": При if-else методы превращаются в каскады проверок
   типа "if (hasCoin) { if (inventory > 0) ... } else if (soldOut) ...", что трудно читать и дебажить.
*/

class VendingMachine;

class State {
public:
    virtual ~State() {}
    virtual void insertCoin(VendingMachine* vm) = 0;
    virtual void dispense(VendingMachine* vm) = 0;
    virtual void ejectCoin(VendingMachine* vm) = 0;
};

class NoCoinState : public State {
public:
    void insertCoin(VendingMachine* vm) override;
    void dispense(VendingMachine* vm) override { cout << "Вставьте монету сначала!" << endl; }
    void ejectCoin(VendingMachine* vm) override { cout << "Вы не вставили монету." << endl; }
};

class SoldOutState : public State {
public:
    void insertCoin(VendingMachine* vm) override { cout << "Извините, напитки кончились." << endl; }
    void dispense(VendingMachine* vm) override { cout << "Напитки кончились!" << endl; }
    void ejectCoin(VendingMachine* vm) override { cout << "Автомат пуст." << endl; }
};

// Состояние "Победитель" — выдает два напитка по цене одного
class WinnerState : public State {
public:
    void insertCoin(VendingMachine* vm) override { cout << "Подождите, идет выдача приза!" << endl; }
    void ejectCoin(VendingMachine* vm) override { cout << "Нельзя вернуть монету, вы выиграли!" << endl; }
    void dispense(VendingMachine* vm) override;
};

class HasCoinState : public State {
public:
    void insertCoin(VendingMachine* vm) override { cout << "Монета уже вставлена!" << endl; }
    void ejectCoin(VendingMachine* vm) override;
    void dispense(VendingMachine* vm) override;
};

class VendingMachine {
private:
    State* currentState;
    int inventory;
    static int luckyCounter;

public:
    VendingMachine(int count) : inventory(count) {
        if (inventory > 0) currentState = new NoCoinState();
        else currentState = new SoldOutState();
    }
    ~VendingMachine() { delete currentState; }

    void setState(State* s) {
        delete currentState;
        currentState = s;
    }

    void releaseDrink() {
        if (inventory > 0) {
            inventory--;
            cout << "Напиток выдан! (Осталось: " << inventory << ")" << endl;
        }
    }

    int getInventory() { return inventory; }
    bool isLucky() { return ++luckyCounter % 3 == 0; }

    void insertCoin() { currentState->insertCoin(this); }
    void dispense() { currentState->dispense(this); }
};

int VendingMachine::luckyCounter = 0;

// Реализация методов
void NoCoinState::insertCoin(VendingMachine* vm) {
    cout << "Монета принята." << endl;
    vm->setState(new HasCoinState());
}

void HasCoinState::ejectCoin(VendingMachine* vm) {
    cout << "Возврат монеты..." << endl;
    vm->setState(new NoCoinState());
}

void HasCoinState::dispense(VendingMachine* vm) {
    if (vm->isLucky() && vm->getInventory() > 1) {
        vm->setState(new WinnerState());
        vm->dispense();
    }
    else {
        vm->releaseDrink();
        if (vm->getInventory() > 0) vm->setState(new NoCoinState());
        else vm->setState(new SoldOutState());
    }
}

void WinnerState::dispense(VendingMachine* vm) {
    cout << "АКЦИЯ! ВЫ ВЫИГРАЛИ ДОПОЛНИТЕЛЬНЫЙ НАПИТОК!" << endl;
    vm->releaseDrink();
    if (vm->getInventory() > 0) {
        vm->releaseDrink();
        if (vm->getInventory() > 0) vm->setState(new NoCoinState());
        else vm->setState(new SoldOutState());
    }
    else {
        vm->setState(new SoldOutState());
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    VendingMachine vm(5);

    for (int i = 0; i < 4; i++) {
        cout << "Покупка #" << i + 1 << endl;
        vm.insertCoin();
        vm.dispense();
    }
    return 0;
}