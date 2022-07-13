-- Таблицы с PRIMARY KEY

CREATE TABLE "cars" (
	"id car" INT PRIMARY KEY,
	"model" VARCHAR(68),
	"type car" VARCHAR(68),
	"mileage" INT NOT NULL
);

CREATE TABLE "people" (
	"id human" INT PRIMARY KEY,
	"frist name" VARCHAR(68) NOT NULL,
	"last name" VARCHAR(68) NOT NULL,
	"patronymic (if any)" VARCHAR(68)
);

CREATE TABLE "repairs" (
	"id repair" INT PRIMARY KEY,
	"id car" INT NOT NULL
);

CREATE TABLE "objects" (
	"id object" INT PRIMARY KEY,
	"type object" VARCHAR(68) NOT NULL
);

-- Таблицы с FOREIGN KEY

CREATE TABLE "car atribute" (
	"id car" INT UNIQUE NOT NULL,
	"passanger seats" INT,
	"standing places" INT,
	"baggage scape" real,
	"load capacity" real,
	"maximum load weight" real,
	"route name" VARCHAR(68),
    FOREIGN KEY ("id car") REFERENCES "cars"("id car")
);

CREATE TABLE "driver staff" (
	"id human" INT NOT NULL,
	"id car" INT NOT NULL,
    FOREIGN KEY ("id car") REFERENCES "cars"("id car"),
    FOREIGN KEY ("id human") REFERENCES "people"("id human")
);

CREATE TABLE "staff" (
	"id human" INT NOT NULL,
	"type staff" VARCHAR(68) NOT NULL,
    FOREIGN KEY ("id human") REFERENCES "people"("id human")
);

CREATE TABLE "brigadiers" (
	"id brigadier" INT NOT NULL,
	"id human" INT NOT NULL,
    FOREIGN KEY ("id brigadier") REFERENCES "people"("id human")--,
    --FOREIGN KEY ("id human") REFERENCES "people"("id human")
);

CREATE TABLE "masters" (
	"id master" INT NOT NULL,
	"id brigadier" INT NOT NULL,
    FOREIGN KEY ("id master") REFERENCES "people"("id human")--,
    --FOREIGN KEY ("id brigadier") REFERENCES "brigadiers"("id brigadier")
);

CREATE TABLE "chiefs" (
	"id chief" INT NOT NULL,
	"id master" INT NOT NULL,
    FOREIGN KEY ("id chief") REFERENCES "people"("id human")--,
    --FOREIGN KEY ("id master") REFERENCES "masters"("id master")
);

CREATE TABLE "objects contents" (
	"id object" INT NOT NULL,
	"id car" INT NOT NULL,
	"status" VARCHAR(68),
    FOREIGN KEY ("id object") REFERENCES "objects"("id object"),
    FOREIGN KEY ("id car") REFERENCES "cars"("id car")
);

CREATE TABLE "repairer" (
	"id repair" INT NOT NULL,
	"id human" INT NOT NULL,
    FOREIGN KEY ("id repair") REFERENCES "repairs"("id repair"),
    FOREIGN KEY ("id human") REFERENCES "people"("id human")
);

CREATE TABLE "changes" (
	"id repair" INT NOT NULL,
	"type change" VARCHAR(68) NOT NULL,
	"cost" FLOAT NOT NULL,
    FOREIGN KEY ("id repair") REFERENCES "repairs"("id repair")
);

-- Автоинкременты и их триггеры

CREATE SEQUENCE sq_cars
START WITH 0 
INCREMENT BY 1 
NOMAXVALUE
MINVALUE 0;

CREATE SEQUENCE sq_people
START WITH 0 
INCREMENT BY 1 
NOMAXVALUE
MINVALUE 0;

CREATE SEQUENCE sq_repairs
START WITH 0 
INCREMENT BY 1 
NOMAXVALUE
MINVALUE 0;

CREATE SEQUENCE sq_objects
START WITH 0 
INCREMENT BY 1 
NOMAXVALUE
MINVALUE 0;

CREATE OR REPLACE TRIGGER tr_cars before INSERT ON "cars" FOR each row
BEGIN
    SELECT sq_cars.NEXTVAL
    INTO :new."id car"
    FROM dual;
END;
/

CREATE OR REPLACE TRIGGER tr_people before INSERT ON "people" FOR each row
BEGIN
    SELECT sq_people.NEXTVAL
    INTO :new."id human"
    FROM dual;
END;
/

CREATE OR REPLACE TRIGGER tr_repairs before INSERT ON "repairs" FOR each row
BEGIN
    SELECT sq_repairs.NEXTVAL
    INTO :new."id repair"
    FROM dual;
END;
/

CREATE OR REPLACE TRIGGER tr_objects before INSERT ON "objects" FOR each row
BEGIN
    SELECT sq_objects.NEXTVAL
    INTO :new."id object"
    FROM dual;
END;
/

-- Триггеры

/*
CREATE TRIGGER tr_delete_car BEFORE DELETE ON "cars" FOR EACH ROW
BEGIN
DELETE FROM "car atribute" WHERE (SELECT "id car" as "id" FROM "cars") = "id car";
END;
/
*/

CREATE TRIGGER tr_insert_car BEFORE INSERT ON "cars" FOR EACH ROW
BEGIN
    IF :NEW."mileage" < 0 THEN
        raise_application_error(-20001, 'mileage cannot < 0');
    END IF;
END;
/

-- Входные данные 

INSERT ALL 
    INTO "people" VALUES (sq_people.NEXTVAL, 'Новиков', 'Михаил', 'Максимович') --с 1 по 40 водители
    INTO "people" VALUES (sq_people.NEXTVAL, 'Олейников', 'Максим', 'Дамирович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Марков', 'Алексей', 'Александрович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Березин', 'Вадим', 'Владиславович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Карасев', 'Максим', 'Семёнович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Попов', 'Серафим', 'Ильич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Савин', 'Филипп', 'Андреевич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Кузнецов', 'Лев', 'Маркович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Гусев', 'Филипп ', 'Родионович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Ефимов', 'Давид', 'Алексеевич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Петров', 'Демьян', NULL)
    INTO "people" VALUES (sq_people.NEXTVAL, 'Федорова', 'Алёна', 'Матвеевна')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Попова', 'Алиса', 'Данииловна')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Гусев', 'Демид', 'Михайлович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Комарова', 'Александра', 'Павловна')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Федотова', 'Анна', 'Степановна')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Соловьева', 'Оливия', 'Тимуровна')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Дмитриев', 'Дмитрий', 'Александрович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Васильев', 'Александр', 'Алексеевич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Никольский', 'Эмин', 'Савельевич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Жилин', 'Арсений', 'Антонович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Соловьев', 'Алексей', 'Георгиевич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Орлов', 'Иван', NULL)
    INTO "people" VALUES (sq_people.NEXTVAL, 'Самойлов', 'Ян', 'Данилович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Кудряшов', 'Роман', 'Алексеевич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Захаров', 'Игорь', 'Родионович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Куликов', 'Макар', 'Кириллович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Борисов', 'Владимир', 'Артемьевич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Романов', 'Егор', 'Сергеевич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Миронов', 'Павел', 'Владимирович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Краснов', 'Марк', 'Андреевич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Белов', 'Григорий', 'Иванович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Токарев', 'Степан', 'Андреевич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Семенов', 'Иван', 'Владимирович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Абрамов', 'Кирилл', NULL)
    INTO "people" VALUES (sq_people.NEXTVAL, 'Грачев', 'Макар', 'Павлович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Лукин', 'Иван', 'Даниилович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Константинов', 'Григорий', 'Львович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Трофимов', 'Роман', 'Давидович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Кондратьев', 'Елисей', 'Денисович') 
    INTO "people" VALUES (sq_people.NEXTVAL, 'Касьянов', 'Максим', 'Дмитриевич') -- с 41 по 55 - персонал
    INTO "people" VALUES (sq_people.NEXTVAL, 'Токарева', 'Милана', 'Владиславовна')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Софронов', 'Арсений', 'Георгиевич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Лукина', 'Анастасия', 'Дмитриевна')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Смирнов', 'Глеб', 'Кириллович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Давыдов', 'Мирон', 'Денисович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Сухарева', 'Лидия', 'Ивановна')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Устинов', 'Тимур', 'Фёдорович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Баранов', 'Максим', 'Богданович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Соколов', 'Леонид', 'Георгиевич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Антонов', 'Руслан', 'Мирославович') 
    INTO "people" VALUES (sq_people.NEXTVAL, 'Озеров', 'Марк', 'Глебович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Зотов', 'Михаил', 'Александрович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Глебов', 'Артём', 'Романович') 
    INTO "people" VALUES (sq_people.NEXTVAL, 'Журавлев', 'Фёдор', 'Иванович')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Наумов', 'Андрей', 'Максимович') -- с 56 по 65 - мастера и начальники + 1 бригадир - бригадир особого транспорта
    INTO "people" VALUES (sq_people.NEXTVAL, 'Коротков', 'Даниил', 'Александрович') -- мастер особого трансопрта
    INTO "people" VALUES (sq_people.NEXTVAL, 'Жилин', 'Давид', 'Артёмович') -- начальник особого транспорта
    INTO "people" VALUES (sq_people.NEXTVAL, 'Самсонов', 'Герман', 'Александрович') -- мастер 1 и 2 таксистов
    INTO "people" VALUES (sq_people.NEXTVAL, 'Гришин', 'Марсель', 'Данилович') -- мастер 1 и 2 грузового
    INTO "people" VALUES (sq_people.NEXTVAL, 'Корнев', 'Андрей', 'Миронович') -- мастер 1 и 2 машруток и 1 и 2 автобусов
    INTO "people" VALUES (sq_people.NEXTVAL, 'Молчанов', 'Александр', 'Егорович') -- начальник автомобилей
    INTO "people" VALUES (sq_people.NEXTVAL, 'Голованов', 'Никита', 'Егорович') -- мастер 1ой и 2ой рабочих
    INTO "people" VALUES (sq_people.NEXTVAL, 'Миронов', 'Виталий', 'Вячеславович') -- мастер 3 и 4 рабочих
    INTO "people" VALUES (sq_people.NEXTVAL, 'Лаптев', 'Даниил', 'Александрович') -- начальник рабочих
    INTO "people" VALUES (sq_people.NEXTVAL, 'Соловьев', 'Михаил', 'Богданович') -- с 66 по 69 водители особого назначения
    INTO "people" VALUES (sq_people.NEXTVAL, 'Кириллов', 'Юрий', 'Тимофеевич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Федоров', 'Давид', 'Матвеевич')
    INTO "people" VALUES (sq_people.NEXTVAL, 'Филимонов', 'Семён', 'Михайлович')
    
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Volvo', 'грузовой', 23743)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Citroen', 'маршрутка', 23234)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Peugeot', 'маршрутка', 57345)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Mercedes', 'маршрутка', 138432)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'ГАЗель', 'маршрутка', 8543)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Kia', 'такси', 98387)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Lada', 'такси', 37434)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Ford', 'маршрутка', 32744)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Hundai', 'такси', 98734)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Renault', 'такси', 23454)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Skoda', 'такси', 37453)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Nissan', 'такси', 1234)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Ford', 'такси', 9342)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Chevrolet', 'такси', 86934)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Toyota', 'такси', 15832)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Isuzu', 'грузовой', 64243)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Man', 'грузовой', 7523)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'КамАЗ', 'грузовой', 68423)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'ГАЗель', 'грузовой', 45342)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'ГАЗ', 'грузовой', 67483)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'ЗИЛ', 'грузовой', 85734)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'УРАЛ', 'грузовой', 57393)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'МАРЗ', 'автобус', 53893)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'НефАЗ', 'автобус', 9345)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'ЛиАЗ', 'автобус', 34536)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'ПАЗ', 'автобус', 153243)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'ПАЗ', 'автобус', 65635)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Scania', 'траспорт вспомогательного характера', 34243)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Iveco', 'траспорт вспомогательного характера', 234)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'Daf', 'траспорт вспомогательного характера', 52354)
    INTO "cars" VALUES (sq_cars.NEXTVAL, 'ГАЗель', 'траспорт вспомогательного характера', 52354)

    INTO "car atribute" VALUES (1, 2, NULL, NULL, 10, 7, NULL)
    INTO "car atribute" VALUES (2, 18, 6, NULL, NULL, NULL, 'A->B')
    INTO "car atribute" VALUES (3, 16, 5, NULL, NULL, NULL, 'C->D')
    INTO "car atribute" VALUES (4, 20, 8, NULL, NULL, NULL, 'E->F')
    INTO "car atribute" VALUES (5, 15, 5, NULL, NULL, NULL, 'H->I')
    INTO "car atribute" VALUES (6, 4, NULL, 0.48, NULL, NULL, NULL)
    INTO "car atribute" VALUES (7, 4, NULL, 0.4, NULL, NULL, NULL)
    INTO "car atribute" VALUES (8, 14, 4, NULL, NULL, NULL, 'J->K')
    INTO "car atribute" VALUES (9, 4, NULL, 0.48, NULL, NULL, NULL)
    INTO "car atribute" VALUES (10, 4, NULL, 0.51, NULL, NULL, NULL)
    INTO "car atribute" VALUES (11, 4, NULL, 0.53, NULL, NULL, NULL)
    INTO "car atribute" VALUES (12, 6, NULL, 0.7, NULL, NULL, NULL)
    INTO "car atribute" VALUES (13, 4, NULL, 0.4, NULL, NULL, NULL)
    INTO "car atribute" VALUES (14, 4, NULL, 0.45, NULL, NULL, NULL)
    INTO "car atribute" VALUES (15, 6, NULL, 0.75, NULL, NULL, NULL)
    INTO "car atribute" VALUES (16, 1, NULL, NULL, 12, 8, NULL)
    INTO "car atribute" VALUES (17, 2, NULL, NULL, 20, 15, NULL)
    INTO "car atribute" VALUES (18, 2, NULL, NULL, 8, 10, NULL)
    INTO "car atribute" VALUES (19, 2, NULL, NULL, 6, 3, NULL)
    INTO "car atribute" VALUES (20, 2, NULL, NULL, 6, 4, NULL)
    INTO "car atribute" VALUES (21, 1, NULL, NULL, 10, 10, NULL)
    INTO "car atribute" VALUES (22, 5, NULL, NULL, 8, 10, NULL)
    INTO "car atribute" VALUES (23, 45, 15, NULL, NULL, NULL, 'L->M')
    INTO "car atribute" VALUES (24, 30, 30, NULL, NULL, NULL, 'C->D')
    INTO "car atribute" VALUES (25, 45, NULL, 8, NULL, NULL, 'X->Y - межгород')
    INTO "car atribute" VALUES (26, 31, 10, NULL, NULL, NULL, 'A->B')
    INTO "car atribute" VALUES (27, 31, 10, NULL, NULL, NULL, 'E->F')
    INTO "car atribute" VALUES (28, 2, NULL, NULL, NULL, 5, NULL)
    INTO "car atribute" VALUES (29, 4, NULL, NULL, NULL, 3, NULL)
    INTO "car atribute" VALUES (30, 1, NULL, NULL, NULL, NULL, NULL)
    INTO "car atribute" VALUES (31, 5, NULL, NULL, 3, 2, NULL)
    
-- Распределение народа по авто
    INTO "driver staff" VALUES (1, 1)
    INTO "driver staff" VALUES (2, 1)
    INTO "driver staff" VALUES (3, 2)
    INTO "driver staff" VALUES (4, 3)
    INTO "driver staff" VALUES (5, 4)
    INTO "driver staff" VALUES (6, 4)
    INTO "driver staff" VALUES (7, 5)
    INTO "driver staff" VALUES (8, 5)
    INTO "driver staff" VALUES (9, 6)
    INTO "driver staff" VALUES (11, 7)
    INTO "driver staff" VALUES (12, 8)
    INTO "driver staff" VALUES (13, 8)
    INTO "driver staff" VALUES (14, 9)
    INTO "driver staff" VALUES (15, 9)
    INTO "driver staff" VALUES (15, 10)
    INTO "driver staff" VALUES (16, 10)
    INTO "driver staff" VALUES (17, 11)
    INTO "driver staff" VALUES (18, 12)
    INTO "driver staff" VALUES (19, 12)
    INTO "driver staff" VALUES (20, 13)
    INTO "driver staff" VALUES (20, 14)
    INTO "driver staff" VALUES (21, 14)
    INTO "driver staff" VALUES (22, 15)
    INTO "driver staff" VALUES (23, 16)
    INTO "driver staff" VALUES (24, 16)
    INTO "driver staff" VALUES (25, 17)
    INTO "driver staff" VALUES (26, 17)
    INTO "driver staff" VALUES (27, 18)
    INTO "driver staff" VALUES (28, 19)
    INTO "driver staff" VALUES (28, 20)
    INTO "driver staff" VALUES (29, 20)
    INTO "driver staff" VALUES (30, 21)
    INTO "driver staff" VALUES (31, 21)
    INTO "driver staff" VALUES (32, 22)
    INTO "driver staff" VALUES (33, 23)
    INTO "driver staff" VALUES (34, 23)
    INTO "driver staff" VALUES (35, 24)
    INTO "driver staff" VALUES (36, 24)
    INTO "driver staff" VALUES (37, 25)
    INTO "driver staff" VALUES (38, 25)
    INTO "driver staff" VALUES (39, 26)
    INTO "driver staff" VALUES (40, 27)
    INTO "driver staff" VALUES (66, 28)
    INTO "driver staff" VALUES (67, 29)
    INTO "driver staff" VALUES (68, 30)
    INTO "driver staff" VALUES (69, 31)

-- Распределение персонала
    INTO "staff" VALUES (41, 'техник')
    INTO "staff" VALUES (42, 'техник')
    INTO "staff" VALUES (43, 'техник')
    INTO "staff" VALUES (44, 'сварщик')
    INTO "staff" VALUES (45, 'сварщик')
    INTO "staff" VALUES (46, 'сварщик')
    INTO "staff" VALUES (47, 'слесарь')
    INTO "staff" VALUES (48, 'слесарь')
    INTO "staff" VALUES (49, 'слесарь')
    INTO "staff" VALUES (50, 'слесарь')
    INTO "staff" VALUES (51, 'сборщик')
    INTO "staff" VALUES (52, 'сборщик')
    INTO "staff" VALUES (53, 'сборщик')
    INTO "staff" VALUES (54, 'сборщик')
    INTO "staff" VALUES (55, 'сборщик')

-- Бригадиры 
    INTO "brigadiers" VALUES (11, 9)
    INTO "brigadiers" VALUES (11, 11) -- 1 ая бригада таксистов
    INTO "brigadiers" VALUES (11, 14)
    INTO "brigadiers" VALUES (11, 15)
    INTO "brigadiers" VALUES (11, 16)
    INTO "brigadiers" VALUES (17, 17) -- 2 ая бригада таксистов 
    INTO "brigadiers" VALUES (17, 18)
    INTO "brigadiers" VALUES (17, 19)
    INTO "brigadiers" VALUES (17, 20)
    INTO "brigadiers" VALUES (17, 21)
    INTO "brigadiers" VALUES (17, 22)
    INTO "brigadiers" VALUES (3, 3) -- 1ая бригада маршруток
    INTO "brigadiers" VALUES (3, 5)
    INTO "brigadiers" VALUES (3, 6)
    INTO "brigadiers" VALUES (3, 7)
    INTO "brigadiers" VALUES (3, 8)
    INTO "brigadiers" VALUES (4, 4) -- 2ая бригада маршруток
    INTO "brigadiers" VALUES (4, 12)
    INTO "brigadiers" VALUES (4, 13)
    INTO "brigadiers" VALUES (1, 1) -- 1ая бригада грузовиков
    INTO "brigadiers" VALUES (1, 2)
    INTO "brigadiers" VALUES (1, 23)
    INTO "brigadiers" VALUES (1, 24)
    INTO "brigadiers" VALUES (1, 25)
    INTO "brigadiers" VALUES (1, 26)
    INTO "brigadiers" VALUES (30, 30) -- 2ая бригада грузовиков
    INTO "brigadiers" VALUES (30, 27)
    INTO "brigadiers" VALUES (30, 28)
    INTO "brigadiers" VALUES (30, 29)
    INTO "brigadiers" VALUES (30, 31)
    INTO "brigadiers" VALUES (30, 32)
    INTO "brigadiers" VALUES (39, 39) -- 1ая бригада автобусов
    INTO "brigadiers" VALUES (39, 33)
    INTO "brigadiers" VALUES (39, 34)
    INTO "brigadiers" VALUES (39, 35)
    INTO "brigadiers" VALUES (39, 36)
    INTO "brigadiers" VALUES (40, 40) -- 2ая бригада автобусов
    INTO "brigadiers" VALUES (40, 37)
    INTO "brigadiers" VALUES (40, 38)
    INTO "brigadiers" VALUES (56, 66) -- бригада вспомогательного характера, бригадир не числится в рабочих
    INTO "brigadiers" VALUES (56, 67)
    INTO "brigadiers" VALUES (56, 68)
    INTO "brigadiers" VALUES (56, 69)
    INTO "brigadiers" VALUES (51, 51) -- 1ая бригада рабочих 
    INTO "brigadiers" VALUES (51, 41)
    INTO "brigadiers" VALUES (51, 44)
    INTO "brigadiers" VALUES (51, 47)
    INTO "brigadiers" VALUES (48, 48) -- 2ая бригада рабочих
    INTO "brigadiers" VALUES (48, 42)
    INTO "brigadiers" VALUES (48, 52)
    INTO "brigadiers" VALUES (48, 53)
    INTO "brigadiers" VALUES (45, 45) -- 3яя бригада рабочих
    INTO "brigadiers" VALUES (45, 49)
    INTO "brigadiers" VALUES (45, 54)
    INTO "brigadiers" VALUES (46, 46) -- 4ая бригада рабочих
    INTO "brigadiers" VALUES (46, 43)
    INTO "brigadiers" VALUES (46, 50)
    INTO "brigadiers" VALUES (46, 55)

    INTO "masters" VALUES (57, 56) -- мастер особого трансопрта
    INTO "masters" VALUES (59, 11) -- мастер 1 и 2 таксистов
    INTO "masters" VALUES (59, 17)
    INTO "masters" VALUES (60, 1) -- мастер 1 и 2 грузового
    INTO "masters" VALUES (60, 30)
    INTO "masters" VALUES (61, 3) -- мастер 1 и 2 машруток и 1 и 2 автобусов
    INTO "masters" VALUES (61, 4)
    INTO "masters" VALUES (61, 39)
    INTO "masters" VALUES (61, 40)
    INTO "masters" VALUES (63, 51) -- мастер 1ой и 2ой рабочих
    INTO "masters" VALUES (63, 48)
    INTO "masters" VALUES (64, 45)  -- мастер 3 и 4 рабочих
    INTO "masters" VALUES (64, 46)

    INTO "chiefs" VALUES (58, 57) -- начальник особого транспорта
    INTO "chiefs" VALUES (62, 59) -- начальник автомобилей
    INTO "chiefs" VALUES (62, 60)
    INTO "chiefs" VALUES (62, 61)
    INTO "chiefs" VALUES (65, 63) -- начальник рабочих
    INTO "chiefs" VALUES (65, 64)

    INTO "objects" VALUES (sq_objects.NEXTVAL, 'гараж') -- в них стоят такси
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'гараж')
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'гараж')
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'гараж')
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'гараж')
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'гараж')
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'гараж')
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'гараж')
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'гараж')
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'цех') -- траспорт вспомогательного характера
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'цех')
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'бокс') -- 2 4 маршрутка
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'бокс') -- 5 маршрутка и 26 автобусы
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'бокс') -- 1 16 грузовые
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'бокс') -- 8 маршрутка 27 автобус
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'бокс') -- 3 маршрутка
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'ангар') -- 17 21 грузовые и 23 авобус
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'ангар') -- 24 25 автобусы 18 19 грузовые
    INTO "objects" VALUES (sq_objects.NEXTVAL, 'ангар') -- 20 22 грузовые
    
    INTO "objects contents" VALUES (1, 6, NULL) -- авто такси в гаражи
    INTO "objects contents" VALUES (2, 7, NULL)
    INTO "objects contents" VALUES (3, 9, NULL)
    INTO "objects contents" VALUES (4, 10, NULL)
    INTO "objects contents" VALUES (5, 11, NULL)
    INTO "objects contents" VALUES (6, 12, NULL)
    INTO "objects contents" VALUES (7, 13, NULL)
    INTO "objects contents" VALUES (8, 14, NULL)
    INTO "objects contents" VALUES (9, 15, NULL)
    INTO "objects contents" VALUES (10, 28, NULL) -- вспомогательного характера в цехи
    INTO "objects contents" VALUES (10, 29, NULL)
    INTO "objects contents" VALUES (11, 30, NULL)
    INTO "objects contents" VALUES (11, 31, NULL)
    INTO "objects contents" VALUES (12, 2, NULL) -- боксы для маршруток грузовых и автобусов
    INTO "objects contents" VALUES (12, 4, NULL)
    INTO "objects contents" VALUES (13, 5, 'в ремонте')
    INTO "objects contents" VALUES (13, 26, NULL)
    INTO "objects contents" VALUES (14, 1, 'в ремонте')
    INTO "objects contents" VALUES (14, 16, NULL)
    INTO "objects contents" VALUES (15, 8, NULL)
    INTO "objects contents" VALUES (15, 27, 'в ремонте')
    INTO "objects contents" VALUES (16, 3, NULL)
    INTO "objects contents" VALUES (17, 17, NULL) -- ангары для маршруток автобусов и грузовых
    INTO "objects contents" VALUES (17, 21, NULL)
    INTO "objects contents" VALUES (17, 23, NULL)
    INTO "objects contents" VALUES (18, 24, NULL)
    INTO "objects contents" VALUES (18, 25, NULL)
    INTO "objects contents" VALUES (18, 18, NULL)
    INTO "objects contents" VALUES (18, 19, NULL)
    INTO "objects contents" VALUES (19, 20, NULL)
    INTO "objects contents" VALUES (19, 22, NULL)

    INTO "repairs" VALUES (sq_repairs.NEXTVAL, 13)
    INTO "repairs" VALUES (sq_repairs.NEXTVAL, 5)
    INTO "repairs" VALUES (sq_repairs.NEXTVAL, 21)
    INTO "repairs" VALUES (sq_repairs.NEXTVAL, 1)
    INTO "repairs" VALUES (sq_repairs.NEXTVAL, 27)
    INTO "repairs" VALUES (sq_repairs.NEXTVAL, 13)

    INTO "repairer" VALUES (1, 41)
    INTO "repairer" VALUES (1, 47)
    INTO "repairer" VALUES (2, 41)
    INTO "repairer" VALUES (2, 44)
    INTO "repairer" VALUES (2, 47)
    INTO "repairer" VALUES (2, 51)
    INTO "repairer" VALUES (3, 45)
    INTO "repairer" VALUES (3, 49)
    INTO "repairer" VALUES (3, 54)
    INTO "repairer" VALUES (4, 42)
    INTO "repairer" VALUES (4, 48)
    INTO "repairer" VALUES (4, 52)
    INTO "repairer" VALUES (4, 53)
    INTO "repairer" VALUES (5, 43)
    INTO "repairer" VALUES (5, 46)
    INTO "repairer" VALUES (5, 50)
    INTO "repairer" VALUES (5, 55)
    INTO "repairer" VALUES (6, 51)
    INTO "repairer" VALUES (6, 41)
    INTO "repairer" VALUES (6, 54)
    
    INTO "changes" VALUES (1, 'покрышки', 20000)
    INTO "changes" VALUES (1, 'замена масла', 2000)
    INTO "changes" VALUES (2, 'двигатель', 50000)
    INTO "changes" VALUES (2, 'шасси', 10000)
    INTO "changes" VALUES (2, 'мост', 20000)
    INTO "changes" VALUES (3, 'кузов', 15000)
    INTO "changes" VALUES (3, 'кабина', 8000)
    INTO "changes" VALUES (3, 'двигатель', 30000)
    INTO "changes" VALUES (3, 'замена масла', 2500)
    INTO "changes" VALUES (4, 'коробка передач', 25000)
    INTO "changes" VALUES (4, 'оборудование', 70000)
    INTO "changes" VALUES (4, 'шасси', 30000)
    INTO "changes" VALUES (5, 'газовое оборудование', 18000)
    INTO "changes" VALUES (5, 'салон', 15000)
    INTO "changes" VALUES (5, 'двигатель', 35000)
    INTO "changes" VALUES (6, 'оборудование', 6000)

SELECT * FROM dual;

COMMIT;

-- Запросы ------------------------------------------------------------------------------------

-- 4.Получить данные о распределении пассажирского автотранспорта по маршрутам
SELECT "type car", "model", "route name"
FROM "cars", (
    SELECT "id buses", "route name"
    FROM "car atribute", (
        SELECT "id car" as "id buses"
        FROM "cars"
        WHERE (("type car" = 'автобус') OR ("type car" = 'маршрутка'))
    )
    WHERE (("route name" IS NOT NULL) AND ("id buses" = "id car"))
)
WHERE ("id buses" = "id car");

-- 3. Получить распределение водителей по автомобилям

SELECT "frist name", "last name", "patronymic (if any)", "model", "type car"
FROM "driver staff", (
    SELECT "id human" as "id person", "frist name", "last name", "patronymic (if any)"
    FROM "people"
), (
    SELECT "id car" as "id auto", "model", "type car"
    FROM "cars"
)
WHERE (("id car" = "id auto") AND ("id human" = "id person"));

-- 8. Получить сведения о наличии гаражного хозяйства в целом и по каждой категории транспорта.

SELECT "id_object", "type object", "model", "type car", "id_car"
FROM "cars", (
    SELECT "id object" as "id garage", "type object"
    FROM "objects"
), (
    SELECT "id car" as "id_car", "id object" as "id_object"
    FROM "objects contents"
)
WHERE (("id_object" = "id garage") AND ("id_car" = "id car"))
ORDER BY "id_object";

SELECT "id_object", "type object", "model", "id_car"
FROM "cars", (
    SELECT "id object" as "id garage", "type object"
    FROM "objects"
), (
    SELECT "id car" as "id_car", "id object" as "id_object"
    FROM "objects contents"
)
WHERE (("id_object" = "id garage") AND ("id_car" = "id car") AND ("type car" = 'маршрутка'))
ORDER BY "id_object";

-- 6. Получить данные о числе ремонтов и их стоимости для автотранспорта определенной категории, отдельной марки автотранспорта или указанной автомашины

-- Данные о числе ремонтов и их стоимости для каждого автотранспорта
SELECT "model", "type car", "id car", "count_repair", "total_sum"
FROM "cars", (
    SELECT "id_car" as "id_car_", "count_repair"
    FROM "cars", (
        SELECT "id car" as "id_car", COUNT("id car") as "count_repair"
        FROM "repairs"
        GROUP BY "id car"
    )
    WHERE ("id car" = "id_car")
), (
    SELECT "id car" as "id_car__", SUM("sum_repair") as "total_sum"
    FROM (
        SELECT "id car", "sum_repair"
        FROM "repairs", (
            SELECT SUM("cost") as "sum_repair", "id repair" as "id_repair"
            FROM "changes"
            GROUP BY "id repair"
        )
        WHERE ("id_repair" = "id repair")
    )
    GROUP BY "id car"
)
WHERE (("id_car_" = "id_car__") AND ("id_car_" = "id car"));

-- Данные о числе ремонтов и их стоимости для каждого автотранспорта (предыдущий запрос но только)
-- 1 - по конкретной категории трансопрта
-- 2 - по конкретной марки трансопрта
-- 3 - по конкретному авто
SELECT "model", "type car", "id car", "count_repair", "total_sum"
FROM (
    SELECT "model", "type car", "id car", "count_repair", "total_sum"
    FROM "cars", (
        SELECT "id_car" as "id_car_", "count_repair"
        FROM "cars", (
            SELECT "id car" as "id_car", COUNT("id car") as "count_repair"
            FROM "repairs"
            GROUP BY "id car"
        )
        WHERE ("id car" = "id_car")
    ), (
        SELECT "id car" as "id_car__", SUM("sum_repair") as "total_sum"
        FROM (
            SELECT "id car", "sum_repair"
            FROM "repairs", (
                SELECT SUM("cost") as "sum_repair", "id repair" as "id_repair"
                FROM "changes"
                GROUP BY "id repair"
            )
            WHERE ("id_repair" = "id repair")
        )
        GROUP BY "id car"
    )
    WHERE (("id_car_" = "id_car__") AND ("id_car_" = "id car"))
)
WHERE ("type car" = 'грузовой');
--WHERE ("model" = 'Volvo');
--WHERE ("id car" = 13);

--7. Получить данные о подчиненности персонала: рабочие - бригадиры - мастера - начальники участков и цехов.

-- Бригадиры - рабочие
SELECT "name brigadier", "last name brigadier", "id_brigadier" as "id_brigadier", "id human", "name worker", "last name worker"
FROM "brigadiers", (
    SELECT "id_brigadier", "frist name" as "name brigadier", "last name" as "last name brigadier"
    FROM (
        SELECT "id brigadier" as "id_brigadier"
        FROM "brigadiers"
        GROUP BY "id brigadier"
    ), (
        SELECT "frist name", "last name", "id human" as "id_person1"
        FROM "people"
    )
    WHERE ("id_person1" = "id_brigadier")
), (
    SELECT "frist name" as "name worker", "last name" as "last name worker", "id human" as "id_worker"
    FROM "brigadiers", (
        SELECT "frist name", "last name", "id human" as "id_person2"
        FROM "people"
    )
    WHERE ("id_person2" = "id human")
)
WHERE (("id brigadier" = "id_brigadier") AND ("id human" = "id_worker"));

-- Мастера - бригадиры
SELECT "name master", "last name master", "id_master" as "id_master", "id brigadier", "name brigadier", "last name brigadier"
FROM "masters", (
    SELECT "id_master", "frist name" as "name master", "last name" as "last name master"
    FROM (
        SELECT "id master" as "id_master"
        FROM "masters"
        GROUP BY "id master"
    ), (
        SELECT "frist name", "last name", "id human" as "id_person1"
        FROM "people"
    )
    WHERE ("id_person1" = "id_master")
), (
    SELECT "frist name" as "name brigadier", "last name" as "last name brigadier", "id brigadier" as "id_brigadier"
    FROM "masters", (
        SELECT "frist name", "last name", "id human" as "id_person2"
        FROM "people"
    )
    WHERE ("id_person2" = "id brigadier")
)
WHERE (("id master" = "id_master") AND ("id brigadier" = "id_brigadier"));

-- Начальники - мастера
SELECT "name chief", "last name chief", "id_chief" as "id_chief", "id master", "name master", "last name master"
FROM "chiefs", (
    SELECT "id_chief", "frist name" as "name chief", "last name" as "last name chief"
    FROM (
        SELECT "id chief" as "id_chief"
        FROM "chiefs"
        GROUP BY "id chief"
    ), (
        SELECT "frist name", "last name", "id human" as "id_person1"
        FROM "people"
    )
    WHERE ("id_person1" = "id_chief")
), (
    SELECT "frist name" as "name master", "last name" as "last name master", "id master" as "id_master"
    FROM "chiefs", (
        SELECT "frist name", "last name", "id human" as "id_person2"
        FROM "people"
    )
    WHERE ("id_person2" = "id master")
)
WHERE (("id chief" = "id_chief") AND ("id master" = "id_master"));

-- 1. Получить данные об автопарке предприятия.
SELECT "model", "type car", "mileage", "passanger seats", "standing places", "baggage scape", "load capacity", "maximum load weight", "route name"
FROM (
    SELECT "id car" as "id_car1", "model", "type car", "mileage"
    FROM "cars"
), (
    SELECT "id car" as "id_car2", "passanger seats", "standing places", "baggage scape", "load capacity", "maximum load weight", "route name"
    FROM "car atribute"
)
WHERE ("id_car1" = "id_car2");

-- 14. Получить данные о работах, выполненных указанным специалистом (сварщиком, слесарем и т.д.) в целом и по конкретной автомашине.

-- Данные о работах, выполненных указанным специалистом в целом
SELECT "id repair", "id car", "model", "id human", "type staff", "frist name", "last name"
FROM "cars", (
    SELECT "id repair", "id car" as "id_car", "id human", "type staff", "frist name", "last name"
    FROM "repairs", (
        SELECT "id repair" as "id_repair", "id human", "type staff", "frist name", "last name"
        FROM "repairer", (
            SELECT "id human" as "id_human", "frist name", "last name"
            FROM "people"
            WHERE ("id human" = 41)
        ), (
            SELECT "id human" as "id_staff", "type staff"
            FROM "staff"
        )
        WHERE (("id human" = "id_human") AND ("id human" = "id_staff"))
    )
    WHERE ("id repair" = "id_repair")
)
WHERE ("id_car" = "id car");

-- 14. Получить данные о работах, выполненных указанным специалистом по конкретной автомашине.
SELECT "id repair", "id car", "model", "id human", "type staff", "frist name", "last name"
FROM "cars", (
    SELECT "id repair", "id car" as "id_car", "id human", "type staff", "frist name", "last name"
    FROM "repairs", (
        SELECT "id repair" as "id_repair", "id human", "type staff", "frist name", "last name"
        FROM "repairer", (
            SELECT "id human" as "id_human", "frist name", "last name"
            FROM "people"
            WHERE ("id human" = 41)
        ), (
            SELECT "id human" as "id_staff", "type staff"
            FROM "staff"
        )
        WHERE (("id human" = "id_human") AND ("id human" = "id_staff"))
    )
    WHERE ("id repair" = "id_repair")
)
WHERE (("id_car" = "id car") AND ("id car" = 13));

/*
SELECT * FROM "cars";
SELECT * FROM "car atribute";

DELETE FROM "cars" WHERE "id car" = 1;

SELECT * FROM "cars";
SELECT * FROM "car atribute";
*/

-- Удаление таблиц, триггеров, автоинкрементов

DROP SEQUENCE sq_cars;
DROP SEQUENCE sq_people;
DROP SEQUENCE sq_objects;
DROP SEQUENCE sq_repairs;

DROP TRIGGER tr_cars;
DROP TRIGGER tr_people;
DROP TRIGGER tr_objects;
DROP TRIGGER tr_repairs;
DROP TRIGGER tr_delete_car;

DROP TABLE "changes";
DROP TABLE "repairer";
DROP TABLE "objects contents";
DROP TABLE "chiefs";
DROP TABLE "masters";
DROP TABLE "brigadiers";
DROP TABLE "driver staff";
DROP TABLE "staff";
DROP TABLE "car atribute";

DROP TABLE "objects";
DROP TABLE "repairs";
DROP TABLE "people";
DROP TABLE "cars";