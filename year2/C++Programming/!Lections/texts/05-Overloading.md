## Перегрузка операций

Одной из важнейших особенностей языка С++ явяется возможность **перегрузки операторов (операций)**.

Вспомним сначала, что такое **перегрузка функций**:

**Перегрузка** - это создание функции с тем же именем, но с отличающимся списком параметров. Иными словами, перегруженные функции называются одинаково, но отличаются количеством или типами своих параметров. Благодаря этому, компилятор их отличает друг от друга в точке вызова.

Перегрузка операций позволяет разработать аналог операции для пользовательского типа в виде функции с определенным набором параметров.

Перегрузка может быть осуществлена

- в виде функции-члена класса (методом)
- в виде глобальной функции (обычной или дружественной классу)

Хороший пример задачи, в которой используется перегрузка: матрицы

```cpp
Matrix a,b,c;
...
c=a+b;
```

Для успешного использования перегрузки, надо знать и соблюдать ряд правил:

- перегружать можно все имеющиеся операции, кроме 
   - ?: - тернарный оператор;
   - :: - доступ к вложенным именам;
   - .  - доступ к полям;
   - .* - доступ к полям по указателю;
   - sizeof, typeid и операторы cast.

- нельзя придумывать свои операции, например @,+++,)( и др.
- при перегрузке нельзя изменять арность операции, ассоциативность, приоритет
- способы перегрузки унарных и бинарных операций отличаются
- хотя бы один из аргументов перегружаемых оператором должен быть пользовательского типа

- следующие операторы можно перегрузить только в качестве методов:
   - = - присваивание;
   - -> - доступ к полям по указателю;
   - () - вызов функции;
   - [] - доступ по индексу;
   - ->* - доступ к указателю-на-поле по указателю;
   - операторы конверсии и управления памятью.

- следующие операторы можно перегрузить только в виде внешних функций:
   - чтение из потока
   - запись в поток
   
### Перегрузка методом класса

#### Бинарные операции

Для перегрузки бинарных операций нужно создать метод, принимающий один аргумент - ссылку на второй аргумент операции.

```cpp
class Complex
{
   private:
     double Re,Im;
   public:
     Complex(double re,double im):Re(re),Im(im) {}
     Complex operator+(const Complex& c) {
        return Complex(Re+c.Re,Im+c.Im);
     }
};
```
Метод, перегружающий операцию сложения, называется **operator+**. Для других операций используются их обозначения.
В результате операции сложения возникает новый объект типа **Complex**.

Перегрузим присваивание:

```cpp
class Complex 
{
  ...
  Complex& operator=(const Complex& c) {
     Re=c.Re;
     Im=c.Im;
     return *this;
  }
  ...
};
```

Теперь можно воспользоваться двумя перегруженными операциями:

```cpp
Complex a(1.1,2.2),b(3.3,4.4),c(0.0,0.0);
c=a+b;
```

#### Унарные операции

Поскольку в операции участвует только один операнд, то никаких внешних ссылок методу, реализующему операцию, передавать не нужно.

```cpp
Complex operator- ()
{
   Complex temp;
   temp.Re=-Re;
   temp.Im=-Im;
   return temp;
}
```

Для операций ++ и -- таже существуют две формы: **префиксная** и **постфиксная**, которые реализуются при перегрузке особым образом.

Рассмотрим пример (класс Coord)

```cpp
class Coord {
   int x,y,z;
   public:
     Coord(int _x,int _y, int _z): x(_x),y(_y),z(_z) {}
     ...
     // Перегрузка префиксной формы:
     Coord& operator++() {
        ++x; ++y; ++z;
        return *this;
     }
     // Перегрузка постфиксной формы:
     Coord operator++(int) {
        Coord temp=*this;
        ++x; ++y; ++z;
        return temp;
     }
     ...
};
```

### Перегрузка внешней функцией

В этом случае у нас есть также две возможности: создать обычную функцию, или функцию, **дружественную** классу. В случае **унарной** операции функция должна принимать ссылку на объект класса, к которому она применяется. Если операция **бинарная**, то таких ссылок должно быть две: на первый аргумент и на второй.

Вот пример дружественной функции:

```cpp
Coord operator+(const Coord& c1, const Coord& c2) {
  Coord temp;
  temp.x=c1.x+c2.x;
  temp.y=c1.y+c2.y;
  temp.z=c1.z+c2.z;
  return temp;
}
```

Для образования дружественности нужно поместить прототип этой функции в класс **Coord** со словом **friend**.

Для обычной функции прямой доступ к закрытым полям невозможен, поэтому она использует публичные методы (геттеры).


### Особые случаи

Операция **присваивания** определена в любом классе по умолчанию как поэлементное копирование. Если класс содержит поля, память под которые выделяется динамически, необходимо определить собственную операцию присваивания. Чтобы сохранить семантику присваивания, функция должна возвращать ссылку на объект, для которого она вызвана и принимать в качестве параметра ссылку на присваиваемый объект.

Оператор = можно перегружать только методом класса.

Замечательной способностью потоков является то, что операции ввода и вывода могут быть перегружены для пользовательских типов. Функции **operator«** и **operator»** должны быть друзьями класса, описывающего пользовательский тип. Функции принимают в качестве аргументов ссылку на входной (выходной) поток и ссылку на объект класса, данные из кото- рого необходимо обработать. В качестве возвращаемых значений необходимо также указывать ссылки на поток ввода или вывода (в этом случае можно конструировать сложные потоковые конструкции).

```cpp
class Complex
{
     ...
     friend ostream& operator<<(ostream& os, Complex& c);
     friend istream& operator>>(istream& is, Complex& c);
     ...
};
ostream& operator<<(ostream& os, Complex& c) {
    return os<<'('<<c.re<<','<<c.im<<')';
}
istream& operator>>(istream& is, Complex& c) {
    return is>>c.re>>c.im;
}
int main() {
Complex c(0,0); cin>>c; // 23,45 cout<<c; // (23,45)
}
```

Существует возможность перегрузить операторы **new**, **delete** для более эффективного распределения динамической памяти. Всего существует 4 формы операторов выделения и освобождения памяти

1 new - работа с одиночными объектами
2 new[] - работа с массивами объектов
3 delete - работа с одиночными объектами
4 delete[] работа с массивами объектов

При перегрузке операторов для работы с динамической памятью нужно соблюдать следующие правила:

- операторам не требуется передавать параметр типа класс
- первым параметром для **new** должен передаваться размер объекта **size_t**
- возвращаемые значения **new** должны иметь тип __void*__ 
- возвращаемые значения **delete, delete[]** должны иметь тип **void** 
- первый аргумент **delete, delete[]** должен иметь тип __void*__ 
- данные операции являются статическими членами класса

```cpp
class Foo {
   ...
   static void* operator new(size_t size);
   static void  operator delete(void * obj, size_t
size);
```

...