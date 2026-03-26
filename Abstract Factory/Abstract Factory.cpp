#include <iostream>
#include <string>
using namespace std;

/*
АНАЛИЗ:
Abstract Factory лучше отдельных фабрик для каждого продукта, так как она гарантирует
СОВМЕСТИМОСТЬ продуктов. Мы не сможем случайно создать Windows-кнопку вместе с Linux-скроллбаром.
Она группирует продукты в "семейства", упрощая замену всего интерфейса одной строчкой кода.
*/

class Button 
{ 
public: 
    virtual string render() = 0; 
    virtual void interact() = 0; 
    virtual ~Button() {} 
};
class ScrollBar 
{ 
public: 
    virtual string render() = 0; 
    virtual void interact() = 0; 
    virtual ~ScrollBar() {} 
};
class CheckBox 
{ 
public: 
    virtual string render() = 0; 
    virtual void interact() = 0; 
    virtual ~CheckBox() {} 
};

class WinButton : public Button 
{
public:
    string render() override { return "Windows Кнопка"; }
    void interact() override { cout << "[Клик по Windows кнопке!]\n"; }
};

class WinScrollBar : public ScrollBar 
{
public:
    string render() override { return "Windows Скроллбар"; }
    void interact() override { cout << "[Прокрутка Windows скроллбара!]\n"; }
};

class WinCheckBox : public CheckBox 
{
public:
    string render() override { return "Windows Чекбокс"; }
    void interact() override { cout << "[Галочка в Windows чекбоксе!]\n"; }
};

class LinuxButton : public Button 
{
public:
    string render() override { return "Linux Кнопка"; }
    void interact() override { cout << "[Клик по Linux кнопке!]\n"; }
};

class LinuxScrollBar : public ScrollBar 
{
public:
    string render() override { return "Linux Скроллбар"; }
    void interact() override { cout << "[Прокрутка Linux скроллбара!]\n"; }
};

class LinuxCheckBox : public CheckBox 
{
public:
    string render() override { return "Linux Чекбокс"; }
    void interact() override { cout << "[Галочка в Linux чекбоксе!]\n"; }
};

class UIFactory 
{
public:
    virtual Button* createButton() = 0;
    virtual ScrollBar* createScrollBar() = 0;
    virtual CheckBox* createCheckBox() = 0;
    virtual ~UIFactory() {}
};

class WinFactory : public UIFactory 
{
public:
    Button* createButton() override { return new WinButton(); }
    ScrollBar* createScrollBar() override { return new WinScrollBar(); }
    CheckBox* createCheckBox() override { return new WinCheckBox(); }
};

class LinuxFactory : public UIFactory 
{
public:
    Button* createButton() override { return new LinuxButton(); }
    ScrollBar* createScrollBar() override { return new LinuxScrollBar(); }
    CheckBox* createCheckBox() override { return new LinuxCheckBox(); }
};



int main() 
{
    string os_type = "linux";
    UIFactory* factory = nullptr;

    if (os_type == "win") 
    {
        factory = new WinFactory();
    } else if (os_type == "linux") 
    {
        factory = new LinuxFactory();
    }

    if (factory) {
        Button* btn = factory->createButton();
        ScrollBar* sb = factory->createScrollBar();
        CheckBox* cb = factory->createCheckBox();

        cout << "Отрисовка: " << btn->render() << ", " << sb->render() << ", " << cb->render() << endl;
        
        btn->interact();
        sb->interact();
        cb->interact();

        delete btn; delete sb; delete cb; delete factory;
    }

    return 0;
}