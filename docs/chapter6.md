### Глобальная корректировка

Глобальная корректировка (ГК) позволяет применить изменения сразу к нескольким записям в базе данных (даже ко всей базе в целом). Это очень мощный инструмент, который следует использовать с осторожностью.

Документация по глобальной корректировке доступна на сайте http://sntnarciss.ru/irbis/spravka/pril00701000000.htm

#### Операторы ГК

Глобальная корректировка состоит из отдельных операторов. Каждый оператор выполняет одно действие, например, добавляет в запись поле или подполе, заменяет одну строку на другое и т. д. Также существуют составные операторы, которые включают в себя несколько операторов (например, условный оператор, который в зависимости от выполнения определённых условий исполняет или нет группу операторов).
 
Оператор ГК в `PlusIrbis` воплощён в классе `GblStatement`, включающем в себя следующие поля:

| Поле       | Тип    | Назначение
|------------|--------|-----------
| command    | string | Команда, выполняемая оператором. Например, `ADD` или `DEL`.
| parameter1 | string | Первый параметр, как правило, спецификация поля/подполя.
| parameter2 | string | Второй параметр, как правило, спецификация повторения.
| format1    | string | Первый формат, например, выражение для замены.
| format2    | string | Второй формат, например, заменяющее выражение.

Лишние (не нужные для данной команды) параметры можно не задавать, они автоматически будут заполнены символом `X`.

Пример инициализации операторов ГК (формат немного сокращен для читаемости):

```c++
std::vector<irbis::GblStatement> statements;
statements.push_back 
    (
        irbis::GblStatement { L"ADD", L"920", L"XXX", L"ATHRA" }
    );
statements.push_back 
    (
        irbis::GblStatement { L"DEL", L"1007", L"1" }
    );
statements.push_back
    (
        irbis::GblStatement { L"REP", L"330^f", L"F", L"(if p(v330) then if p(v330^f) then ... fi/)" }
    );
);
```

#### Настройки ГК

Настроек для ГК довольно много, поэтому они собраны в класс `GblSettings`, предоставляющий следующий поля:

| Поле                 | Тип    | Назначение
|----------------------|--------|-----------
| actualize            | bool   | Актуализировать записи?
| autoin               | bool   | Запускать autoin.gbl?
| database             | string | Имя базы данных, на которой будет выполняться ГК.
| filename             | string | Имя файла на сервере, хранящего сценарий ГК.
| formalControl        | string | Применять формальный контроль?
| lowerBound           | int    | Нижняя граница MFN для поиска обрабатываемых записей.
| maxMfn               | int    | Максимальный MFN.
| mfnList              | array  | Массив явно заданных MFN, на которых будет выполняться ГК.
| minMfn               | int    | Минимальный MFN. 0 означает "все записи в базе".
| parameters           | array  | (Опциональные) параметры ГК.
| searchExpression     | string | Поисковое выражение отбора записей по словарю.
| sequentialExpression | string | Выражение для отбора записей последовательным поиском (опциональное).
| statements           | array  | Массив операторов.
| upperBound           | int    | Верхняя граница MFN для поиска обрабатываемых записей.

Если какая-либо из настроек не задана, она принимает (разумное) значение по умолчанию. Абсолютно необходимым является задание массива операторов, т. к. иначе ГК не имеет смысла. 

#### Простой пример выполнения ГК

```c++
irbis::GblSettings settings;
settings.database = L"IBIS";
settings.mfnList = std::vector<int> { 1, 2, 3 };
settings.statements.push_back
    (
        irbis::GblStatement { L"ADD", L"3000", L"XXX", L"'Hello'" }
    );
auto result = connection.globalCorrection(settings);
for (const auto &line : result) {
    std::wcout << line << std::endl;
}

```

#### Построитель ГК

Для более удобного формирования массива операторов ГК можно применять класс `Gbl`. В частности, с его помощью довольно наглядно (за счёт отступов) формируются вложенные группы операторов.

Предоставляются следующие методы:

| Метод          | Назначение 
|----------------|-----------
| add            | Добавление нового повторения поля в заданное (существующее или нет) поле.
| all            | Дополняет записи всеми полями текущей записи.
| build          | Выдача настроек ГК по заданным значениям.
| change         | Замена данных в поле или в подполе.
| changeWithCase | Замена данных в поле или в подполе с учётом регистра символов.
| comment        | Комментарий. Может находиться между другими операторами и содержать любой текст.
| correct        | Из текущей записи вызывает на корректировку другие записи, отобранные по поисковым терминам  из текущей или другой базы данных.
| delete         | Удаляет поле или подполе.
| deleteRecord   | Удаляет записи, поданные на корректировку. Не требует никаких дополнительных данных.
| empty_         | Очищает (опустошает) текущую запись.
| if_            | Определяет условие выполнения операторов, следующих за ним.
| newMfn         | Создаёт новую запись в текущей или другой базе данных.
| parameter      | Задание параметра ГК.
| putlog         | Формирование пользовательского протокола.
| repeat         | Организует цикл выполнения группы операторов.
| replace        | Замена целиком поля или подполя на новое значение.
| statement      | Добавление произвольного оператора к ГК.
| undelete       | Восстанавливает удалённые записи.
| undo           | Переход к одной из предыдущих копий записи.

Пример применения построителя.

```c++
irbis::Gbl gbl;
gbl.parameter(L"mhr.mnu", L"Укажите место хранения")
    .comment(L"Это комментарий в начале")
    .newMfn(L"'TEST'",
        (
            irbis::Gbl::Gbl()
                .add(700, L"'^AἈριστοτέλης'")
                .add(200, L"'^Aアリストテレス'")
                .add(300, L"'Пробная запись'")
                .add(920, L"'PAZK'")
        )
    .comment(L"Это комментарий в конце");
    
auto settings = gbl.build();
settings.database = L"IBIS";
auto result = connection.globalCorrection(settings);    
```

Сформированные настройки выдаются методом `build` (см. пример выше).

[Предыдущая глава](chapter5.md)