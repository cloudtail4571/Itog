#include <iostream>
#include <string>
using namespace std;

/*
АНАЛИЗ: Почему Adapter лучше изменения старого класса?
1. Принцип открытости/закрытости (OCP): Мы расширяем систему, не меняя протестированный годами код (Legacy).
2. Принцип единственной ответственности (SRP): Старый класс отвечает за железо, адаптер — за совместимость.
3. Отсутствие исходников: Часто старый код доступен только в виде скомпилированных библиотек.
*/

class USBDevice 
{
public:
    virtual ~USBDevice() {}
    virtual string processUSB(const string& data, int quality) = 0;
};

class OldLPTPrinter 
{
public:
    string printLPT(const string& data) 
    {
        return "LPT Печать: " + data;
    }
};

class OldLPTScanner {
public:
    string scanLPT() {
        return "Данные со сканера LPT";
    }
};

class PrinterAdapter : public USBDevice 
{
private:
    OldLPTPrinter* printer;
public:
    PrinterAdapter(OldLPTPrinter* p) : printer(p) {}
    string processUSB(const string& data, int quality) override 
    {
        string enhancedData = data + " [Качество: " + to_string(quality) + "%]";
        return printer->printLPT(enhancedData + " (через USB-переходник)");
    }
};

class ScannerAdapter : public USBDevice 
{
private:
    OldLPTScanner* scanner;
public:
    ScannerAdapter(OldLPTScanner* s) : scanner(s) {}
    string processUSB(const string& data, int quality) override {
        return scanner->scanLPT() + " [Оцифровано в " + to_string(quality) + " DPI]";
    }
};

class PrinterClassAdapter : public USBDevice, private OldLPTPrinter {
public:
    string processUSB(const string& data, int quality) override {
        return printLPT(data + " (Class Adapter, q:" + to_string(quality) + ")");
    }
};

int main() 
{
    setlocale(LC_ALL, "RUS");

    OldLPTPrinter oldP;
    USBDevice* printer = new PrinterAdapter(&oldP);
    cout << printer->processUSB("Отчет", 300) << endl;

    OldLPTScanner oldS;
    USBDevice* scanner = new ScannerAdapter(&oldS);
    cout << scanner->processUSB("", 600) << endl;

    USBDevice* classAdp = new PrinterClassAdapter();
    cout << classAdp->processUSB("Документ", 150) << endl;

    delete printer; delete scanner; delete classAdp;
    return 0;
}