-- ¬спомогательный запрос
/*
SELECT "id car" as "id_s", "type car"
FROM "cars"
WHERE (("type car" = 'автобус') OR ("type car" = 'маршрутка') OR ("type car" = 'грузовой'));
*/

-- «апрос на категорию водителей
/*
SELECT "id human", "id_car"
FROM "driver staff", (
    SELECT "id car" AS "id_car"
    FROM "cars"
    WHERE ("type car" = 'траспорт вспомогательного характера')
)
WHERE ("id car" = "id_car");
*/

-- 4.ѕолучить данные о распределении пассажирского автотранспорта по маршрутам
/*
SELECT "id car" as "id buses", "type car"
FROM "cars"
WHERE (("type car" = 'автобус') OR ("type car" = 'маршрутка'));
*/

/*
SELECT "route name"
FROM "car atribute"
WHERE ("route name" IS NOT NULL);
*/

/*
SELECT "id buses", "route name"
FROM "car atribute", (
    SELECT "id car" as "id buses"
    FROM "cars"
    WHERE (("type car" = 'автобус') OR ("type car" = 'маршрутка'))
)
WHERE (("route name" IS NOT NULL) AND ("id buses" = "id car"));
*/

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

-- 3. ѕолучить распределение водителей по автомобил€м
/*
SELECT "id human" as "id person", "frist name", "last name", "patronymic (if any)"
FROM "people";
*/

/*
SELECT "id car" as "id auto", "model", "type car"
FROM "cars";
*/

/*
SELECT "id car", "id human"
FROM "driver staff";
*/

SELECT "frist name", "last name", "patronymic (if any)", "model", "type car"
FROM "driver staff", (
    SELECT "id human" as "id person", "frist name", "last name", "patronymic (if any)"
    FROM "people"
), (
    SELECT "id car" as "id auto", "model", "type car"
    FROM "cars"
)
WHERE (("id car" = "id auto") AND ("id human" = "id person"));

-- 8. ѕолучить сведени€ о наличии гаражного хоз€йства в целом и по каждой категории транспорта.

/*
SELECT "id object" as "id garage", "type object"
FROM "objects"
ORDER BY "id object";
*/

/*
SELECT *
FROM "objects contents"
ORDER BY "id object";
*/

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

-- ƒанные о количестве ремонта техники
/*
SELECT "model", "id car", "count_repair"
FROM "cars", (
    SELECT "id car" as "id_car", COUNT("id car") as "count_repair"
    FROM "repairs"
    GROUP BY "id car"
)
WHERE ("id car" = "id_car");
*/

-- ƒанные об общей стоимости каждого ремонта
/*
SELECT "id car", "id repair", "sum_repair"
FROM "repairs", (
    SELECT SUM("cost") as "sum_repair", "id repair" as "id_repair"
    FROM "changes"
    GROUP BY "id repair"
)
WHERE ("id_repair" = "id repair");
*/

/*
SELECT "id car" as "id_car", SUM("sum_repair") as "total_sum"
FROM (
    SELECT "id car", "sum_repair"
    FROM "repairs", (
        SELECT SUM("cost") as "sum_repair", "id repair" as "id_repair"
        FROM "changes"
        GROUP BY "id repair"
    )
    WHERE ("id_repair" = "id repair")
)
GROUP BY "id car";
*/

-- ƒанные о числе ремонтов и их стоимости дл€ каждого автотранспорта
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

-- ƒанные о числе ремонтов и их стоимости дл€ каждого автотранспорта (предыдущий запрос но только)
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

/*
SELECT "id_brigadier", "frist name" as "name brigadier", "last name" as "last name brigadier"
FROM (
    SELECT "id brigadier" as "id_brigadier"
    FROM "brigadiers"
    GROUP BY "id brigadier"
), (
    SELECT "frist name", "last name", "id human" as "id_person1"
    FROM "people"
)
WHERE ("id_person1" = "id_brigadier");
*/

/*
SELECT "frist name" as "name worker", "last name" as "last name worker", "id human"
FROM "brigadiers", (
    SELECT "frist name", "last name", "id human" as "id_person2"
    FROM "people"
)
WHERE ("id_person2" = "id human");
*/

-- Ѕригадиры - рабочие
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

-- ћастера - бригадиры
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

-- Ќачальники - мастера
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

-- 1. ѕолучить данные об автопарке предпри€ти€.
SELECT "id_car1" as "id car", "model", "type car", "mileage", "passanger seats", "standing places", "baggage scape", "load capacity", "maximum load weight", "route name"
FROM (
    SELECT "id car" as "id_car1", "model", "type car", "mileage"
    FROM "cars"
), (
    SELECT "id car" as "id_car2", "passanger seats", "standing places", "baggage scape", "load capacity", "maximum load weight", "route name"
    FROM "car atribute"
)
WHERE ("id_car1" = "id_car2");

-- 14. ѕолучить данные о работах, выполненных указанным специалистом (сварщиком, слесарем и т.д.) в целом и по конкретной автомашине.
/*
SELECT "id human" as "id_human"
FROM "people"
WHERE ("id human" = 41);
*/

/*
SELECT "id repair", "id human", "type staff"
FROM "repairer", (
    SELECT "id human" as "id_human"
    FROM "people"
    WHERE ("id human" = 41)
), (
    SELECT "id human" as "id_staff", "type staff"
    FROM "staff"
)
WHERE (("id human" = "id_human") AND ("id human" = "id_staff"));
*/

/*
SELECT "id repair", "id car", "id human", "type staff"
FROM "repairs", (
    SELECT "id repair" as "id_repair", "id human", "type staff"
    FROM "repairer", (
        SELECT "id human" as "id_human"
        FROM "people"
        WHERE ("id human" = 41)
    ), (
        SELECT "id human" as "id_staff", "type staff"
        FROM "staff"
    )
    WHERE (("id human" = "id_human") AND ("id human" = "id_staff"))
)
WHERE ("id repair" = "id_repair");
*/

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

-- 14. ѕолучить данные о работах, выполненных указанным специалистом (сварщиком, слесарем и т.д.) в целом и по конкретной автомашине.
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