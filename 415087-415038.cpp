#include <iostream>
using namespace std;

/* ==================================================
   CLASS TANIMLARI 
   ================================================== */

class DynamicArray {
private:
    int* data;
    int size;
    int capacity;

    void resize();

public:
    DynamicArray();
    DynamicArray(int initialCapacity);
    DynamicArray(const DynamicArray& other);
    virtual ~DynamicArray();

    virtual void push(int value);
    void pop();
    int get(int index) const;
    void set(int index, int value);
    int getSize() const;
    int getCapacity() const;
    bool isEmpty() const;
    void clear();
    void print() const;

    int& operator[](int index);
    DynamicArray operator+(const DynamicArray& other) const;
    DynamicArray& operator=(const DynamicArray& other);
    bool operator==(const DynamicArray& other) const;
    bool operator!=(const DynamicArray& other) const;

    friend ostream& operator<<(ostream& os, const DynamicArray& arr);
};

class SortedArray : public DynamicArray {
public:
    SortedArray() : DynamicArray() {}
    SortedArray(int capacity) : DynamicArray(capacity) {}

    void push(int value) override;
    int binarySearch(int value) const;
};

class UniqueArray : public DynamicArray {
public:
    UniqueArray() : DynamicArray() {}
    UniqueArray(int capacity) : DynamicArray(capacity) {}

    void push(int value) override;
    bool push(int value, bool returnStatus); 
    bool contains(int value) const;
};

/* ==================================================
   MAIN (Test Programı)
   ================================================== */

int main() {
    // Nesne oluşturma ve temel ekleme işlemleri
    DynamicArray arr1;
    arr1.push(10);
    arr1.push(20);
    arr1.push(30);
    cout << "arr1: ";
    arr1.print();

    // İndeks operatörü ile doğrudan erişim testi
    arr1[1] = 25;
    cout << "arr1[1] degisti: " << arr1[1] << endl;

    // İki farklı diziyi operatör kullanarak birleştirme
    DynamicArray arr2;
    arr2.push(40);
    arr2.push(50);
    DynamicArray arr3 = arr1 + arr2;
    cout << "Birlesmis dizi: " << arr3 << endl;

    // Elemanları otomatik sıralayan dizi testi
    SortedArray sorted;
    sorted.push(50);
    sorted.push(10);
    sorted.push(30);
    sorted.push(20);
    cout << "Sorted: ";
    sorted.print();
    cout << "30 bulundu, indeks: " << sorted.binarySearch(30) << endl;

    // Tekrar eden elemanları engelleyen dizi testi
    UniqueArray unique;
    unique.push(10);
    unique.push(20);
    unique.push(10); // Bu değer dizi içinde olduğu için eklenmemeli
    unique.push(30);
    cout << "Unique: ";
    unique.print();

    cout << "20 var mi? " << (unique.contains(20) ? "Evet" : "Hayir") << endl;

    return 0;
}

/* ==================================================
   METHOD IMPLEMENTASYONLARI
   ================================================== */

// --- DynamicArray ---
DynamicArray::DynamicArray() : size(0), capacity(2) {
    data = new int[capacity];
}

DynamicArray::DynamicArray(int initialCapacity) {
    capacity = (initialCapacity > 0) ? initialCapacity : 2;
    size = 0;
    data = new int[capacity];
}

// Derin kopyalama (Deep Copy) yaparak bellek çakışmalarını önlüyoruz
DynamicArray::DynamicArray(const DynamicArray& other) : size(other.size), capacity(other.capacity) {
    data = new int[capacity];
    for (int i = 0; i < size; i++) data[i] = other.data[i];
}

DynamicArray::~DynamicArray() {
    delete[] data; 
}

// Dizi dolduğunda kapasiteyi iki katına çıkaran yardımcı fonksiyon
void DynamicArray::resize() {
    capacity *= 2;
    int* newData = new int[capacity];
    for (int i = 0; i < size; i++) newData[i] = data[i];
    delete[] data; // Eski küçük alanı temizle
    data = newData; // Yeni büyük alana geç
}

void DynamicArray::push(int value) {
    if (size == capacity) resize();
    data[size++] = value;
}

void DynamicArray::pop() {
    if (size > 0) size--;
    else cout << "Hata: Dizi zaten bos, pop yapilamaz!" << endl;
}

int DynamicArray::get(int index) const {
    if (index < 0 || index >= size) {
        cout << "Hata: Gecersiz indeks!" << endl;
        return -1;
    }
    return data[index];
}

void DynamicArray::set(int index, int value) {
    if (index >= 0 && index < size) data[index] = value;
    else cout << "Hata: Gecersiz indeks!" << endl;
}

int DynamicArray::getSize() const { return size; }
int DynamicArray::getCapacity() const { return capacity; }
bool DynamicArray::isEmpty() const { return size == 0; }
void DynamicArray::clear() { size = 0; }

void DynamicArray::print() const {
    cout << *this << endl;
}

// İndeks operatörü (Hem okuma hem yazma için referans döndürür)
int& DynamicArray::operator[](int index) {
    if (index < 0 || index >= size) {
        static int dummy = -1; // Geçersiz erişimde hata yönetimi için dummy değer
        cout << "Hata: Gecersiz indeks erişimi!" << endl;
        return dummy;
    }
    return data[index];
}

// + Operatörü: Yeni bir dizi nesnesi oluşturup iki dizinin elemanlarını içine kopyalar
DynamicArray DynamicArray::operator+(const DynamicArray& other) const {
    DynamicArray result(size + other.size);
    for (int i = 0; i < size; i++) result.push(data[i]);
    for (int i = 0; i < other.size; i++) result.push(other.data[i]);
    return result;
}


DynamicArray& DynamicArray::operator=(const DynamicArray& other) {
    if (this != &other) { // Kendi kendine atama kontrolü
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new int[capacity];
        for (int i = 0; i < size; i++) data[i] = other.data[i];
    }
    return *this;
}

bool DynamicArray::operator==(const DynamicArray& other) const {
    if (size != other.size) return false;
    for (int i = 0; i < size; i++) if (data[i] != other.data[i]) return false;
    return true;
}

bool DynamicArray::operator!=(const DynamicArray& other) const { return !(*this == other); }

ostream& operator<<(ostream& os, const DynamicArray& arr) {
    os << "[";
    for (int i = 0; i < arr.size; i++) {
        os << arr.data[i] << (i == arr.size - 1 ? "" : ", ");
    }
    os << "]";
    return os;
}

// --- SortedArray ---
// Eleman eklendiğinde dizinin her zaman sıralı kalmasını sağlar 
void SortedArray::push(int value) {
    DynamicArray::push(value); 
    int i = getSize() - 1;
    while (i > 0 && get(i - 1) > value) {
        set(i, get(i - 1));
        i--;
    }
    set(i, value);
}

// Sıralı dizide hızlı arama yapan Binary Search algoritması
int SortedArray::binarySearch(int value) const {
    int low = 0, high = getSize() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (get(mid) == value) return mid;
        if (get(mid) < value) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

// --- UniqueArray ---
bool UniqueArray::contains(int value) const {
    for (int i = 0; i < getSize(); i++) if (get(i) == value) return true;
    return false;
}

// Mevcut elemanı kontrol eder, yoksa temel sınıftaki push'u çağırır
void UniqueArray::push(int value) {
    if (!contains(value)) DynamicArray::push(value);
}

bool UniqueArray::push(int value, bool returnStatus) {
    if (contains(value)) return false;
    DynamicArray::push(value);
    return true;
}