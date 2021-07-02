CREATE OR REPLACE PROCEDURE clear_all_tables(username IN VARCHAR) AS $$
DECLARE
statements CURSOR FOR
SELECT tablename FROM pg_tables
WHERE tableowner = username AND schemaname = 'public';
BEGIN
FOR stmt IN statements LOOP
        EXECUTE 'TRUNCATE TABLE ' || quote_ident(stmt.tablename) || ' CASCADE;';
END LOOP;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE PROCEDURE clear_table(TEXT)
LANGUAGE plpgsql AS $$
BEGIN
execute
        'TRUNCATE TABLE' || $1 || ' CASCADE;';
END$$;


CREATE OR REPLACE PROCEDURE create_database(TEXT, TEXT)
LANGUAGE plpgsql
AS $$
BEGIN
    perform dblink_exec('dbname='||current_database()||' user='||current_user||' password='|| $2,
        'create database ' || $1 || ' with owner= '|| current_user);
END
$$;

CREATE OR REPLACE PROCEDURE install_dblink()
LANGUAGE plpgsql
AS $$
BEGIN
    create extension dblink;
END
$$;

-- CREATE OR REPLACE PROCEDURE delete_database(TEXT, TEXT)
-- LANGUAGE plpgsql AS $$
-- BEGIN
--     perform
-- pg_terminate_backend(pid)
--     FROM
--         pg_stat_activity
--     WHERE
--         pid <> pg_backend_pid() AND datname = $1 ;
--     perform dblink_exec('dbname='||current_database()||' user='||current_user||
--         ' password='|| $2, 'DROP DATABASE IF EXISTS ' || $1);
-- END$$;

CREATE OR REPLACE PROCEDURE create_tables()
LANGUAGE plpgsql AS $$
BEGIN
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
END;
$$

CREATE OR REPLACE PROCEDURE delete_record(TEXT, INTEGER)
LANGUAGE plpgsql AS $$
BEGIN
EXECUTE 'DELETE FROM ' || $1 || ' WHERE "ИДЕНТИФИКАТОР" = ' || $2;
END$$;

-- CREATE OR REPLACE PROCEDURE delete_by_name_from_post(TEXT)
-- LANGUAGE plpgsql AS $$
-- BEGIN
-- DELETE FROM ДОЛЖНОСТЬ WHERE НАЗВАНИЕ LIKE $1;
-- END$$;

-- CREATE OR REPLACE FUNCTION find(TEXT)
-- RETURNS TABLE (ИДЕНТИФИКАТОР INTEGER, ИМЯ TEXT, ФАМИЛИЯ TEXT, ОТЧЕСТВО TEXT, ТЕЛЕФОН INTEGER, "ПАССПОРТ, СЕРИЯ" INTEGER, "ПАССПОРТ, НОМЕР" INTEGER) AS $$
-- BEGIN
-- RETURN QUERY  (SELECT * FROM КЛИЕНТ WHERE КЛИЕНТ.ТЕЛЕФОН LIKE $1);
-- END;
-- $$ LANGUAGE plpgsql VOLATILE;

-- CREATE OR REPLACE FUNCTION insert(TEXT, TEXT, TEXT)
-- LANGUAGE plpgsql AS $$
-- BEGIN
-- PREPARE test (TEXT, TEXT, TEXT) as
--     INSERT INTO $1 ($2) VALUES ($3);
-- EXECUTE test($1, $2, $3)
-- END$$;

-- CREATE OR REPLACE FUNCTION get_rows(text) RETURNS SETOF record AS $$
-- DECLARE
-- r record;
-- BEGIN
-- FOR r IN EXECUTE 'SELECT * from ' || $1 LOOP
-- RETURN NEXT r;
-- END LOOP;
-- RETURN;
-- END;
-- $$ LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE PROCEDURE rename_table(tbl regclass, newname TEXT) AS $$
BEGIN
	EXECUTE 'ALTER TABLE ' || tbl || ' RENAME TO ' || newname;
END
$$ LANGUAGE plpgsql;

CREATE FUNCTION public.cost_update()
    RETURNS trigger
    LANGUAGE 'plpgsql'
COST 100
VOLATILE NOT LEAKPROOF
AS $BODY$
BEGIN
UPDATE "АБОНИМЕНТ" SET "ЦЕНА" = NEW."НОВАЯ СТОИМОСТЬ" where NEW."ИДЕНТИФИКАТОР АБОНИМЕНТА" = "АБОНИМЕНТ"."ИДЕНТИФИКАТОР";
RETURN NEW;
END;
$BODY$;

CREATE TRIGGER "СТОИМОСТЬ С УЧЕТОМ АКЦИЙ"
    AFTER INSERT
    ON public."АКЦИИ"
    FOR EACH ROW
    EXECUTE FUNCTION public.cost_update();
