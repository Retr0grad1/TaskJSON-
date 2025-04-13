TaskJSON - Утилита для обработки комментариев через HTTP API

Описание
    TaskJSON — это утилита командной строки (CLI), которая загружает комментарии для заданного дерева идентификаторов, используя HTTP API, и выводит его в том же формате JSON с телами комментариев.
    Основное преимущество утилиты — использование параллельных запросов, что значительно ускоряет процесс загрузки комментариев и снижает задержку, независимо от того, сколько комментариев нужно получить. Это позволяет программе работать с высоким коэффициентом                параллелизма и минимизировать общее время выполнения.
    Если хотим подключаться по https, то нужно скачать OpenSSL.
    Утилита подходит для работы с большими деревьями идентификаторов, где каждый узел — это комментарий, а его дочерние узлы — это ответы на этот комментарий.

Основной функционал
    Программа загружает тело комментариев для каждого идентификатора, используя GET-запросы к предоставленному API. Каждый комментарий возвращается в формате JSON и добавляется в выходное дерево.

Требования
    Компилятор C++ (например, MSVC).

Библиотеки:
    json.hpp: Для работы с JSON. Используется https://github.com/nlohmann/json.
    httplib.h: Для отправки HTTP-запросов. Используется https://github.com/yhirose/cpp-httplib.
        
Сборка проекта
    Скачайте репозиторий или клонируйте его.
    Разместите библиотеки json.hpp и httplib.h в папке include проекта.
    Откройте командную строку в директории проекта и выполните команду для компиляции: cl /EHsc /Iinclude TaskJSON.cpp
    После успешной компиляции будет создан исполнимый файл TaskJSON.exe.

Использование
    Программа принимает на стандартный поток JSON-структуру, в которой каждый объект имеет два поля:
        id — идентификатор комментария.
        replies — список объектов с полем id для дочерних комментариев.

Пример входного файла:

{
  "id": 1,
  "replies": [
    {
      "id": 2,
      "replies": []
    },
    {
      "id": 3,
      "replies": [
        {
          "id": 4,
          "replies": []
        },
        {
          "id": 5,
          "replies": []
        }
      ]
    }
  ]
}

Пример вывода:
{
  "body": "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto",
  "id": 1,
  "replies": [
    {
      "body": "est rerum tempore vitae\nsequi sint nihil reprehenderit dolor beatae ea dolores neque\nfugiat blanditiis voluptate porro vel nihil molestiae ut reiciendis\nqui aperiam non debitis possimus qui neque nisi nulla",
      "id": 2,
      "replies": []
    },
    {
      "body": "et iusto sed quo iure\nvoluptatem occaecati omnis eligendi aut ad\nvoluptatem doloribus vel accusantium quis pariatur\nmolestiae porro eius odio et labore et velit aut",
      "id": 3,
      "replies": [
        {
          "body": "ullam et saepe reiciendis voluptatem adipisci\nsit amet autem assumenda provident rerum culpa\nquis hic commodi nesciunt rem tenetur doloremque ipsam iure\nquis sunt voluptatem rerum illo velit",
          "id": 4,
          "replies": []
        },
        {
          "body": "repudiandae veniam quaerat sunt sed\nalias aut fugiat sit autem sed est\nvoluptatem omnis possimus esse voluptatibus quis\nest aut tenetur dolor neque",
          "id": 5,
          "replies": []
        }
      ]
    }
  ]
}

Обработка ошибок
    Если API возвращает ошибку 404, программа выведет следующее сообщение в поле body:
    "body": "Error: comment not found"
    Если возникает ошибка сети, программа продолжит обработку других запросов и выведет соответствующее сообщение об ошибке.

Параллельная обработка
    Для ускорения процесса загрузки комментариев программа выполняет параллельные HTTP-запросы, что позволяет уменьшить общее время работы программы. Вместо того чтобы загружать комментарии по одному, все запросы выполняются одновременно, что снижает время ожидания и          улучшает производительность.
