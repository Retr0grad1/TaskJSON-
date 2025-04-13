#include <iostream>
#include <string>
#include <sstream>
#include <future>
#include <vector>
#include "json.hpp"
#include "httplib.h"

using json = nlohmann::json;
using namespace std;

// Рекурсивно собирает все id комментариев
void collect_ids(const json& node, vector<int>& ids) {
    if (!node.contains("id")) return;
    ids.push_back(node["id"]);
    for (const auto& reply : node["replies"]) {
        collect_ids(reply, ids);
    }
}

// Получение body по id
json fetch_comment(int id, httplib::Client& cli) {
    json result;
    auto res = cli.Get(("/posts/" + to_string(id)).c_str());
    if (res && res->status == 200) {
        result = json::parse(res->body);
    }
    else {
        result["body"] = "Not found";
    }
    result["id"] = id;
    return result;
}

// Заполнение дерева комментариев body по id
void insert_bodies(json& node, const map<int, string>& id_to_body) {
    int id = node["id"];
    node["body"] = id_to_body.at(id);
    for (auto& reply : node["replies"]) {
        insert_bodies(reply, id_to_body);
    }
}

int main() {
    // Чтение JSON из stdin
    stringstream buffer;
    buffer << cin.rdbuf();
    json input_tree = json::parse(buffer.str());

    // Сбор id
    vector<int> ids;
    collect_ids(input_tree, ids);

    // Запросы по сети
    httplib::Client cli("http://winry.khashaev.ru");
    vector<future<json>> futures;
    for (int id : ids) {
        futures.push_back(async(launch::async, fetch_comment, id, ref(cli)));
    }

    map<int, string> id_to_body;
    for (auto& fut : futures) {
        auto comment = fut.get();
        id_to_body[comment["id"]] = comment["body"];
    }

    // Добавление тела к каждому узлу
    insert_bodies(input_tree, id_to_body);

    // Вывод результата
    cout << input_tree.dump(2) << endl;
    return 0;
}
