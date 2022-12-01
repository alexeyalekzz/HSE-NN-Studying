/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoGCode1CNCSink
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoGCode1CNCSink
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2021 Klimov Alexey. All rights reserved.
 * </автор>
 *
 */

#include "CEcoGCode1CNCSink.h"
#include "IEcoInterfaceBus1.h"

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoGCode1Events
 * </описание>
 *
 */
int16_t CEcoGCode1CNCSink_QueryInterface(/* in */ struct IEcoGCode1Events* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoGCode1CNCSink* pCMe = (CEcoGCode1CNCSink*)me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoGCode1Events) ) {
        *ppv = &pCMe->m_pVTblIEvents;
        pCMe->m_pVTblIEvents->AddRef((IEcoGCode1Events*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEvents;
        pCMe->m_pVTblIEvents->AddRef((IEcoGCode1Events*)pCMe);
    }
    else {
        *ppv = 0;
        return -1;
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoGCode1Events
 * </описание>
 *
 */
uint32_t CEcoGCode1CNCSink_AddRef(/* in */ struct IEcoGCode1Events* me) {
    CEcoGCode1CNCSink* pCMe = (CEcoGCode1CNCSink*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoGCode1Events
 * </описание>
 *
 */
uint32_t CEcoGCode1CNCSink_Release(/* in */ struct IEcoGCode1Events* me) {
    CEcoGCode1CNCSink* pCMe = (CEcoGCode1CNCSink*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoGCode1CNCSink((IEcoGCode1Events*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция OnCommand
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int8_t CEcoGCode1CNCSink_OnCommand(/* in */ struct IEcoGCode1Events* me, /* in */ uint8_t todo) {
    CEcoGCode1CNCSink* pCMe = (CEcoGCode1CNCSink*)me;

    /* Проверка указателей */
    if (me == 0) {
        return -1;
    }

    pCMe->m_pILog->pVTbl->Info(pCMe->m_pILog, "Command TO DO");

    return 0;
}


/* Create Virtual Table */
IEcoGCode1EventsVTbl g_x4EC638CD188548BB9D7A7A09F8B458ACVTblCNC = {
    CEcoGCode1CNCSink_QueryInterface,
    CEcoGCode1CNCSink_AddRef,
    CEcoGCode1CNCSink_Release,
    CEcoGCode1CNCSink_OnCommand
};

/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
int16_t createCEcoGCode1CNCSink(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* in */ IEcoLog1* pILog, /* out */ IEcoGCode1Events** pIEvents) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoGCode1CNCSink* pCMe = 0;

    /* Проверка указателей */
    if (pIEvents == 0 || pIUnkSystem == 0) {
        return result;
    }


   /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem1, (void **)&pISys);

    /* Проверка */
    if (result != 0 && pISys == 0) {
        return result;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Получение интерфейса распределителя памяти */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoGCode1CNCSink*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoGCode1CNCSink));

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoGCode1Events */
    pCMe->m_pVTblIEvents = &g_x4EC638CD188548BB9D7A7A09F8B458ACVTblCNC;

    /* Получение интерфейса для работы со строкой */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoString1, 0, &IID_IEcoString1, (void **)&pCMe->m_pIString);
    if (result != 0 || pCMe->m_pIString == 0) {
        /* Освобождение в случае ошибки */
        deleteCEcoGCode1CNCSink((IEcoGCode1Events*)pCMe);
    }

    /* Инициализация данных */
    pCMe->m_pILog = pILog;

    /* Возврат указателя на интерфейс */
    *pIEvents = (IEcoGCode1Events*)pCMe;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);
    pISys->pVTbl->Release(pISys);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
void deleteCEcoGCode1CNCSink(/* in */ IEcoGCode1Events* pIEvents) {
    CEcoGCode1CNCSink* pCMe = (CEcoGCode1CNCSink*)pIEvents;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEvents != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_pIString != 0 ) {
            pCMe->m_pIString->pVTbl->Release(pCMe->m_pIString);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}
