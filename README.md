# Cybernetic Implant Server

Это простой сервер для управления состояниями кибернетических имплантов.  
Сервер поддерживает базовые команды для обновления и получения состояния имплантов по уникальному `agent_id`.

## Структура проекта 

```
├── build/ # Скомпилированные объекты
├── include/ # Заголовочные файлы
│ ├── client_session.h
│ ├── implant_manager.h
│ ├── protocol.h
│ ├── server.h
│ └── utils.h
├── src/ # Исходный код
│ ├── client_session.cpp
│ ├── implant_manager.cpp
│ ├── main.cpp
│ ├── protocol.cpp
│ ├── server.cpp
│ └── utils.cpp
├── Makefile # Файл сборки
└── README.md # Этот файл
```