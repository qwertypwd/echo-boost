# Тестовое задание для C++ разработчиков
Типо клиент-сервер. 
## Сборка
Windows, CMake
Для сборки требуются установленный boost (asio, system, unit_test_framework)
```
cd build
cmake .. -G "MinGW Makefiles"
cmake . --build 
```
## Описание

Сервер\
\-Поддерживает подключения нескольких клиентов одновременно;\
\-Принимает заявки на покупку или продажу долларов за рубли от разных клиентов;\
\-Даёт возможность просмотра баланса клиента\


Клиент\
\-Подключается к серверу и реализует все его возможности\

Торговая логика\

\-Торговая заявка содержит объём, цену и сторону (покупка/продажа);\
\-Если две заявки пересекаются по цене — для сделки выбирается цена более ранней заявки;\
\-Если заявка пересекается с несколькими заявками по другой стороне — приоритет в исполнении отдаётся заявкам с максимальной ценой для заявок на покупку и с минимальной ценой для заявок на продажу;\
\-Возможно частичное исполнение заявки;\
\-Торговая заявка активна до тех пор, пока по ней есть активный объём;\
\-Баланс клиента не ограничен — он может торговать в минус\


## Работоспособность
Есть юнит тесты на очередность заявок и на корректность их исполнения `src/test.cpp`

## Пример запуска
Client (input)
```
aboba
2
10 80
```

Client1
```
amogus
3
2 60
3
2 100
3
12 100

```

Server (output)
```
Server started! Listen 5555 port
Register request
New application
Register request
New application
consumer call
1 get userId
0 get userId
New application
consumer call
1 get userId
0 get userId
start exchanging
0       0       aboba (seller)
0       0       amogus (buyer)
exchanged 2 usd 100 rur
-2      200     aboba (seller)
2       -200    amogus (buyer)

New application
consumer call
1 get userId
0 get userId
start exchanging
-2      200     aboba (seller)
2       -200    amogus (buyer)
exchanged 8 usd 100 rur
-10     1000    aboba (seller)
10      -1000   amogus (buyer)
...
```