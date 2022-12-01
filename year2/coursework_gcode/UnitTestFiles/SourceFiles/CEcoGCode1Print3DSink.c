/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoGCode1Print3DSink
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoGCode1Print3DSink
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2021 Klimov Alexey. All rights reserved.
 * </автор>
 *
 */

#include "CEcoGCode1Print3DSink.h"
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
int16_t CEcoGCode1Print3DSink_QueryInterface(/* in */ struct IEcoGCode1Events* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoGCode1Print3DSink* pCMe = (CEcoGCode1Print3DSink*)me;

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
uint32_t CEcoGCode1Print3DSink_AddRef(/* in */ struct IEcoGCode1Events* me) {
    CEcoGCode1Print3DSink* pCMe = (CEcoGCode1Print3DSink*)me;

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
uint32_t CEcoGCode1Print3DSink_Release(/* in */ struct IEcoGCode1Events* me) {
    CEcoGCode1Print3DSink* pCMe = (CEcoGCode1Print3DSink*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoGCode1Print3DSink((IEcoGCode1Events*)pCMe);
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
int8_t CEcoGCode1Print3DSink_OnCommand(/* in */ struct IEcoGCode1Events* me, /* in */ uint8_t todo) {
    CEcoGCode1Print3DSink* pCMe = (CEcoGCode1Print3DSink*)me;

    /* Проверка указателей */
    if (me == 0) {
        return -1;
    }

    pCMe->m_pILog->pVTbl->Info(pCMe->m_pILog, "Command TO DO");

    return 0;
}


/* Create Virtual Table */
IEcoGCode1EventsVTbl g_x4EC638CD188548BB9D7A7A09F8B458ACVTblPrint3D = {
    CEcoGCode1Print3DSink_QueryInterface,
    CEcoGCode1Print3DSink_AddRef,
    CEcoGCode1Print3DSink_Release,
    CEcoGCode1Print3DSink_OnCommand
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
int16_t createCEcoGCode1Print3DSink(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* in */ IEcoLog1* pILog, /* out */ IEcoGCode1Events** pIEvents) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoGCode1Print3DSink* pCMe = 0;

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
    pCMe = (CEcoGCode1Print3DSink*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoGCode1Print3DSink));

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoGCode1Events */
    pCMe->m_pVTblIEvents = &g_x4EC638CD188548BB9D7A7A09F8B458ACVTblPrint3D;

    /* Получение интерфейса для работы со строкой */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoString1, 0, &IID_IEcoString1, (void **)&pCMe->m_pIString);
    if (result != 0 || pCMe->m_pIString == 0) {
        /* Освобождение в случае ошибки */
        deleteCEcoGCode1Print3DSink((IEcoGCode1Events*)pCMe);
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
void deleteCEcoGCode1Print3DSink(/* in */ IEcoGCode1Events* pIEvents) {
    CEcoGCode1Print3DSink* pCMe = (CEcoGCode1Print3DSink*)pIEvents;
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
