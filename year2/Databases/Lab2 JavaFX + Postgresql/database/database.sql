CREATE TYPE ПОМЕЩЕНИЕ AS ENUM 
(
    'Бассейн',
    'Тренажерный зал',
    'Малый зал',
    'Большой зал'
);

CREATE TABLE ДОЛЖНОСТЬ
(
    ИДЕНТИФИКАТОР             SERIAL          PRIMARY KEY,
    НАЗВАНИЕ                  VARCHAR(64)     NOT NULL,
    "РАБОЧЕЕ ВРЕМЯ ЗА ДЕНЬ"   SMALLINT        NOT NULL,
    "ПЛАТА ЗА ЧАС"            SMALLINT        NOT NULL,
    CHECK ("РАБОЧЕЕ ВРЕМЯ ЗА ДЕНЬ" > 0 AND "ПЛАТА ЗА ЧАС" > 0)
);

CREATE TABLE КЛИЕНТ
(
    ИДЕНТИФИКАТОР       SERIAL          PRIMARY KEY,
    ИМЯ                 VARCHAR(64)     NOT NULL,
    ФАМИЛИЯ             VARCHAR(64)     NOT NULL,
    ОТЧЕСТВО            VARCHAR(64)     NOT NULL,
    ТЕЛЕФОН             BIGINT          NOT NULL,
    "ПАССПОРТ, СЕРИЯ"   INTEGER         NOT NULL,
    "ПАССПОРТ, НОМЕР"   INTEGER         NOT NULL
);

CREATE TABLE АБОНЕМЕНТ
(
    ИДЕНТИФИКАТОР                SERIAL         PRIMARY KEY,
    ЦЕНА             	         INT        	NOT NULL,
    "ПРОДОЛЖИТЕЛЬНОСТЬ, мес"     SMALLINT    	NOT NULL,
    НАЗВАНИЕ                     VARCHAR(64)    NOT NULL,
    "РАЗРЕШЕННЫЕ АКТИВНОСТИ"     TEXT[]         NOT NULL
);

CREATE TABLE РАБОТНИК
(
    ИДЕНТИФИКАТОР       SERIAL          PRIMARY KEY,
    ДОЛЖНОСТЬ           SERIAL          NOT NULL REFERENCES ДОЛЖНОСТЬ (ИДЕНТИФИКАТОР) ON DELETE CASCADE,
    ИМЯ                 VARCHAR(64)     NOT NULL,
    ФАМИЛИЯ             VARCHAR(64)     NOT NULL,
    ОТЧЕСТВО            VARCHAR(64)     NOT NULL,
    ТЕЛЕФОН             BIGINT          NOT NULL
);

CREATE TABLE ДОГОВОР
(
    ИДЕНТИФИКАТОР           SERIAL          PRIMARY KEY,
    КЛИЕНТ                  SERIAL          NOT NULL REFERENCES КЛИЕНТ (ИДЕНТИФИКАТОР) ON DELETE CASCADE,
    АБОНЕМЕНТ 	            SERIAL          NOT NULL REFERENCES АБОНЕМЕНТ (ИДЕНТИФИКАТОР) ON DELETE CASCADE,
    "НАЧАЛО ДЕЙСТВИЯ"       DATE            NOT NULL,
    "КОНЕЦ ДЕЙСТВИЯ"        DATE            NOT NULL,
    "СКИДКА, %"             SMALLINT        NOT NULL,
    CHECK("НАЧАЛО ДЕЙСТВИЯ" < "КОНЕЦ ДЕЙСТВИЯ")
);

CREATE TABLE РАСПИСАНИЕ
(
    ИДЕНТИФИКАТОР           SERIAL          PRIMARY KEY,
    ТРЕНЕР                  SERIAL          NOT NULL REFERENCES РАБОТНИК (ИДЕНТИФИКАТОР) ON DELETE CASCADE,
    АКТИВНОСТЬ              TEXT            NOT NULL,
    ЗАЛ                     ПОМЕЩЕНИЕ       NOT NULL,
    НАЧАЛО                  TIMESTAMP       NOT NULL,
    КОНЕЦ                   TIMESTAMP       NOT NULL,
    CHECK("НАЧАЛО" < "КОНЕЦ")
);

CREATE TABLE АКЦИИ
(
    "ИДЕНТИФИКАТОР АБОНЕМЕНТА"  SERIAL      PRIMARY KEY REFERENCES АБОНЕМЕНТ (ИДЕНТИФИКАТОР) ON DELETE CASCADE,
    "РАЗМЕР СКИДКИ, %"          SMALLINT    NOT NULL,
    "НАЧАЛО АКЦИИ"              DATE        NOT NULL,
    "КОНЕЦ АКЦИИ"               DATE        NOT NULL,
    "НОВАЯ СТОИМОСТЬ"           INT         NOT NULL,
    CHECK(("РАЗМЕР СКИДКИ, %" >= 0 AND "РАЗМЕР СКИДКИ, %" <= 100)
     AND ("НАЧАЛО АКЦИИ" < "КОНЕЦ АКЦИИ"))
);
