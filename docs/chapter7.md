### Внутренние классы и функции

#### Логирование

* `LOG_TRACE(__message)` - вывод трассировочного сообщения.

* `LOG_ERROR(__message)` - вывод сообщения об ошибке. 

#### NotNull&lt;T&gt;

Простая (невладеющая) обертка над указателем [по мотивам Guidelines Support Library](https://github.com/microsoft/GSL).

```c++
int value1 = 123, value2 = 321;
NotNull<int> pointer1 (&value1);
NotNull<int> pointer2 (&value2);
int value3 = *pointer1 + *pointer2;
```

При попытке присвоить ему значение `nullptr` бросает исключение.

Запрещены операторы `++`, `--` и аналогичные. Поддерживаются операторы `==`, `!=` и аналогичные.

#### Optional&lt;T&gt;

Опциональное значение, аналог `std::optional<T>` из C++17 с той же семантикой.

```c++
Optional<int> opt (5);
if (opt) {
    total += opt.value;
}
```

Поддерживается присваивание:

```c++
Optional<int> opt;
opt = 5;
if (opt) {
    total += opt.value;
}
```

#### Maybe&lt;T&gt;

Монада maybe:

```c++
struct Address
{
    std::string *city;
};

struct Person
{
    Address *address;
};

auto *address = new Address;
address->city = new std::string ("Irkutsk");
auto *person  = new Person;
person->address = address;
std::size_t counter { 0 };

auto m = maybe (person)
         & [] (Person *p)  { return p->address; }
         & [] (Address *a) { return a->city; }
         | [&counter] (std::string *s) { counter = s->size (); };

std::cout << counter << std::endl; // выведет 7
```

#### Span&lt;T&gt;

Невладеющий указатель на непрерывную область памяти, содержащую значения определенного типа:

```c++
const int array[] { 1, 2, 3, 4, 5, 6, 7 };
Span<int> span (array, irbis::size(array));
auto result = std::find_if (std::begin (span), std::end (span),
        [] (int x) { return x % 2 == 0; });

std::cout << *result << std::endl; // выведет 2

result = std::find_if (std::begin (span), std::end (span),
        [] (int x) { return x < 0; });

bool flag = result == std::end (span); // true
```

#### Frugal&lt;T, N&gt;

Контейнер, предназначенный для уменьшения использования динамической памяти (кучи). Первые N элементов сохраняются на стеке.

#### PointerGuard&lt;T&gt;

#### File

Обертка над файлом.

#### ChunkedBuffer

Буфер, состоящий из отрезков равной длины (`MemoryChink`). 

#### ClientSocket

Клиентский сокет. Абстрактный класс.

Конкретная реализация: `Tcp4Socket`.

#### ClientQuery

Клиентский запрос.

#### IO

Операции ввода-вывода.

#### Text

Операции над текстом.

#### ServerResponse

Ответ сервера.

#### ByteNavigator

Инструмент для простого разбора массива байт.

#### TextNavigator

Инструмент для простого разбора массива символов.

#### Методы

* choose

* oneOf

* safeAt

* sameChar

* sameString

* firstChar
