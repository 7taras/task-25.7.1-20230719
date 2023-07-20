// task-25.7.1-20230719.cpp : 
// 
// Чат имеет клиент-серверную архитектуру.
// Серверное приложение имеет связь с БД MQSQL 7tChat, в которой две таблицы (users и messasges, 
// описание ниже), которые хранят всю информацию чата.
// Клиентское приложение отправляет команды (например отправка сообщения или прием), на которые 
// отвечает серверное (например, команда успешно выполнено или ошибка).
// 
// Таблица users
//		user_id 
//		username	- уникальное имя пользователя чата
//		name		- его имя (от этого имени будут видеть сообщения другие пользователи)
//		email		- опционально
//		hash_password	- хэш пароля
// Таблица messages
//		message_id
//		sender_id	- id отправителя
//		receiver_id	- id получателя
//		text
//		time		- время в формате Unix
//		isReceived	- флаг о получении
//		isRead		- флаг о прочтении
//
// Команды от клиента:
// 1 Создание нового пользователя (внесение в БД)
// 2 Редактирование данных пользователя (изменение строки в БД)
// 3 Удаление пользователя (удаление строки в БД)
// 4 Создание нового сообщения (внесение в БД)
// 5 Чтение сообщений (запрос в БД)


#include <iostream>
#include "mysql.h"

using namespace std;

int main() {
	MYSQL mysql; // Дескриптор соединения c MySql
	MYSQL_RES* res;
	MYSQL_ROW row;


	mysql_init(&mysql);

	if (&mysql == NULL) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		cout << "Error: can't create MySQL-descriptor" << endl;
	}

	// Подключаемся к серверу
	if (!mysql_real_connect(&mysql, "localhost", "root", "Pass4root$", "7tChat", 0, NULL, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	else {
		// Если соединение успешно установлено выводим фразу — "Success!"
		cout << "Connection to database - Success!" << endl;
	}


	// выводим в консоль список таблиц в БД
	mysql_query(&mysql, "SHOW TABLES");		
	if (res = mysql_store_result(&mysql)) 
	{
		// если БД не содержит ни одной таблицы, создаем таблицы users и messages
		if (res->row_count == 0)
		{
			mysql_query(&mysql, "CREATE TABLE users(user_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, username VARCHAR(32) NOT NULL UNIQUE, name VARCHAR(32) NOT NULL, email VARCHAR(64) UNIQUE, hash_password BINARY(20) NOT NULL)");
			mysql_query(&mysql, "CREATE TABLE messages(message_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, sender_id INT NOT NULL REFERENCES users (user_id), receiver_id INT, text TEXT NOT NULL, time INT NOT NULL, isReceived BIT, isRead BIT)");		
			mysql_query(&mysql, "SHOW TABLES");
			res = mysql_store_result(&mysql);
		}

		cout << res->fields->name << endl;
		while (row = mysql_fetch_row(res)) 
		{
			for (int i = 0; i < mysql_num_fields(res); i++) 
			{
				cout << row[i] << "  ";
			}
			cout << endl;
		}
	}
	else
	{
		cout << "Ошибка MySql номер " << mysql_error(&mysql);
	}


	// добавляем пользователей
	mysql_query(&mysql, "INSERT INTO users(username, name, email, hash_password) VALUES ('alex78', 'Aleksey Ivanov', 'alex78@mail.ru', 0xb1b3773a05c0ed0176787a4f1574ff0075f7521e)");
	mysql_query(&mysql, "INSERT INTO users(username, name, email, hash_password) VALUES ('terinator', 'Alex Topor', 'terminator@mail.ru', 0x23c6834b1d353eabf976e524ed489c812ff86a7d)");

	mysql_query(&mysql, "INSERT INTO messages(sender_id, receiver_id, text, time, isReceived, isRead) VALUES (1, 3, 'Hello!', 1689854113, 0, 0)");


	// Закрываем соединение с базой данных
	mysql_close(&mysql);

	system("Pause");
	return 0;
}