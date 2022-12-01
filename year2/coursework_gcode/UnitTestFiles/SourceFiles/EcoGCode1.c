/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoGCode1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2021 Klimov Alexey. All rights reserved.
 * </автор>
 *
 */


/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoString1.h"
#include "IdEcoList1.h"
#include "IdEcoLog1.h"
#include "IEcoLog1FileAffiliate.h"
#include "IdEcoGCode1.h"

#include "CEcoGCode1CNCSink.h"
#include "CEcoGCode1CollaborativeRobotSink.h"
#include "CEcoGCode1Print3DSink.h"
#include "CEcoGCode1SwarmRobotSink.h"

/*
 * Пример кадра : G91 G1 X10 Y00 F100
   G91 - относительная система координат
   G01 X10 Y00 - линейная интерполяция, перемещение по оси X
                 условно ось X -/+ движение вперед/назад;
                 условно ось X -/+ движение вправо/влево;
   F100 - скорость подачи (скорость перемещения)
*/
byte_t startMotor[22] = "G91\n"
						"G01 X10 Y00 F100\n";

/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    /* Указатель на интерфейс работы со строкой */
    IEcoString1* pIString = 0;
    /* Указатель на интерфейс работы с журналом */
    IEcoLog1* pILog = 0;
    IEcoLog1FileAffiliate* pIFileAffiliate = 0;
    /* Указатель на тестируемый интерфейс */
    IEcoGCode1* pIEcoGCode1 = 0;
    IEcoGCode1Events* pIEventsPrint3D = 0;
    IEcoGCode1Events* pIEventsCNC = 0;
    IEcoGCode1Events* pIEventsCobot = 0;
    IEcoGCode1Events* pIEventsSwarm = 0;

    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem1, (void **)&pISys);
        if (result != 0 && pISys == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoGCode1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_F5ACD69A4AFB40FFB92DDFC37ADA56C6);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
    /* Регистрация статического компонента для работы со строкой */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoString1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_84CC0A7DBABD44EEBE749C9A8312D37E);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoList1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_53884AFC93C448ECAA929C8D3A562281);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    /* Регистрация статического компонента для работы с журналом */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLog1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_97322B6765B74342BBCE38798A0B40B5);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif

    /* Получение интерфейса для работы со строкой */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoString1, 0, &IID_IEcoString1, (void**) &pIString);
    if (result != 0|| pIString == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

    /* Получение интерфейса для работы с журналом */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLog1, 0, &IID_IEcoLog1, (void**) &pILog);
    if (result != 0|| pILog == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLog1, 0, &IID_IEcoLog1FileAffiliate, (void**) &pIFileAffiliate);
    if (result != 0|| pIFileAffiliate == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }
    pILog->pVTbl->AddAffiliate(pILog, (IEcoLog1Affiliate*)pIFileAffiliate);
    pIFileAffiliate->pVTbl->Release(pIFileAffiliate);

    /* Создаем обработчик событий для платформы групового робота */
    result = createCEcoGCode1SwarmRobotSink((IEcoUnknown*)pISys, 0, pILog, &pIEventsSwarm);
    if (result != 0 && pIEventsSwarm == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

    /* Получение тестируемого интерфейса */
    pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoGCode1, 0, &IID_IEcoGCode1, (void**) &pIEcoGCode1);
    if (result != 0 && pIEcoGCode1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

    /* Декодируем и обрабатываем кадр */
    result = pIEcoGCode1->pVTbl->DecodeMessage(pIEcoGCode1, startMotor, 22, pIEventsSwarm);

	pILog->pVTbl->InfoFormat(pILog, "Function returned: %d", result);

Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoGCode1 != 0) {
        pIEcoGCode1->pVTbl->Release(pIEcoGCode1);
    }

    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}

