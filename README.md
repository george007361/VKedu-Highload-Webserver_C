# Highload

## Задание
Разработка веб-сервера для раздачи статики
> Язык: **Си (С99)**
> Модель асинхронности: **thread-per-request**
Автор:
> **Георгий Илларионов, Web-31**

## Использование
### 1. Настройка конфигурации:
   > Папка `conf/` содержит примеры файлов конфигурации. 

   Струкрута конфика example.conf
   ```
   thread_limit <num> # Ограничение количества воркеров, обрабатывающий соединения
   connection_limit <num> # Ограничение на количество соединений в очереди
   port <num> # Порт сервера
   server_name <string> # Название сервера. Заголовок (http) Server
   document_root <absolute path> # Абсолютный путь до папки, из которой раздаём статику
   dir_index <string> # Имя файла, отдаваемого для директории по умолчанию
   ```
### 2. Команды
   #### Сборка
   ```
   make build 
   ```
   #### Запуск
   ```
   ./build/hl-web-server  <path/to/config.conf>
   ```
   #### Очистка
   ```
   make clean 
   ```
   #### Сборка в докере
   > Конфигурацию писать в `conf/httpd_docker.conf`, в контейнере сервер лежит в `/server/`

   ##### Сборка и запуск
   ```
   make docker-build
   make docker-run
   ```
   > ##### Запуск с выводом логов
   > ```
   > make docker-run-log
   > ```

   ##### Остановка и очистка:
   ```
   make docker-stop
   make docker-clean
   ```

## Тестирование
### 1. Функциональное
   1. Собрать 
   2. Запустить с конфигом `conf/httpd_func.conf`
   3. Запустить тесты 
      ```
      make func-test
      ```
### 2. Нагрузочное в докере
   1. Собрать в докере, запустить в докере
   2. Запустить тесты 
      ```
      make stress-test
      ```
### 3. Нагрузочное локально
   1. Собрать
   2. Запустить с конфигом `conf/httpd_stress.conf`
   3. Запустить тесты 
      ```
      make stress-test
      ```
### 4. Сравнение с nginx на стресс тесте
   1. Запустить стресс тест на сервер
   2. Запуск стресс теста на nginx
      ```
      make nginx-stress
      ```
### 5. Линтеры
   1. clang-format
      ```
      make style
      ```
   2. cppcheck
      ```
      make cppcheck
      ```
   3. clang-tidy
      ```
      make clang-tidy
      ```
## Результаты стресс тестов
> Тесты выполнялись в докер-контейнере \
> Команда: ```ab -n 60000 -c 700 http://localhost:80/uri/to/test/file```
 
### Веб-сервер
> **Резултаты** \
> Ось абсцисс - количество одновременно работающих тредов \
> Ось ординат - RPS

1. Проверка достаточно лёгкого сайта: ```/tests/nginx/site_example```
   ![server_site_example](tests/results/server_site_example.jpg)
2. Проверка достаточно тяжёлого сайта: ```/tests/func/httptest/wikipedia_russia.html```
   ![server_wikipedia](tests/results/server_wikipedia.jpg)
### Nginx
> **Резултаты** \
> Ось абсцисс - количество одновременно работающих воркеров \
> Ось ординат - RPS

1. Проверка достаточно лёгкого сайта: ```/tests/nginx/site_example```
   ![nginx_site_example](tests/results/nginx_site_example.jpg)
2. Проверка достаточно тяжёлого сайта: ```/tests/func/httptest/wikipedia_russia.html```
   ![nginx_wikipedia](tests/results/nginx_wikipedia.jpg)
