/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoGCode1SwarmRobotSink
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoGCode1SwarmRobotSink
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2021 Klimov Alexey. All rights reserved.
 * </автор>
 *
 */

#include "CEcoGCode1SwarmRobotSink.h"
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
int16_t CEcoGCode1SwarmRobotSink_QueryInterface(/* in */ struct IEcoGCode1Events* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoGCode1SwarmRobotSink* pCMe = (CEcoGCode1SwarmRobotSink*)me;

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
uint32_t CEcoGCode1SwarmRobotSink_AddRef(/* in */ struct IEcoGCode1Events* me) {
    CEcoGCode1SwarmRobotSink* pCMe = (CEcoGCode1SwarmRobotSink*)me;

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
uint32_t CEcoGCode1SwarmRobotSink_Release(/* in */ struct IEcoGCode1Events* me) {
    CEcoGCode1SwarmRobotSink* pCMe = (CEcoGCode1SwarmRobotSink*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoGCode1SwarmRobotSink((IEcoGCode1Events*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}


int8_t CEcoGCode1SwarmRobotSink_OnProgram(/* in */ struct IEcoGCode1Events* me, /* in */ IEcoGCode1Program* prog) {
	CEcoGCode1SwarmRobotSink* pCMe = (CEcoGCode1SwarmRobotSink*)me;
    uint32_t i = 0;
    int8_t command = 0;
    float_t x, y, f;
    FRAME* frame;

    pCMe->m_pILog->pVTbl->Info(pCMe->m_pILog, "\'OnProgram\' called");

    /* Проверка указателей */
    if (me == 0 || prog == 0) {
        return -1;
    }

    while (prog->pVTbl->GetFrame(prog, i, &frame) == 0) {
        if (SEEN(frame, 'G')) {
            command = (int8_t)GETVAL(frame, 'G');
            switch(command) {
                case 91:
                    pCMe->m_pILog->pVTbl->Info(pCMe->m_pILog, "Incremental motion");
                    break;
                case 1:
                    x = GETVAL(frame, 'X');
                    y = GETVAL(frame, 'Y');
                    f = GETVAL(frame, 'F');
                    pCMe->m_pILog->pVTbl->InfoFormat(pCMe->m_pILog, "Linear move to: \n"
                                                                    "\tX: %.2f\n"
                                                                    "\tY: %.2f\n"
                                                                    "\tWith speed F: %.2f", x, y, f);
                    break;
            }
        }
        i++;
    }
	return 0;
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
int8_t CEcoGCode1SwarmRobotSink_OnFrame(/* in */ struct IEcoGCode1Events* me, /* in */ FRAME* frame) {
    CEcoGCode1SwarmRobotSink* pCMe = (CEcoGCode1SwarmRobotSink*)me;
	int8_t command = 0;
    float_t x, y, f;

    pCMe->m_pILog->pVTbl->Info(pCMe->m_pILog, "\'OnFrame\' called");

    /* Проверка указателей */
    if (me == 0 || frame == 0) {
        return -1;
    }
	
    if (SEEN(frame, 'G')) {
        command = (int8_t)GETVAL(frame, 'G');
        switch(command) {
            case 91:
                pCMe->m_pILog->pVTbl->Info(pCMe->m_pILog, "Incremental motion");
                break;
            case 1:
                x = GETVAL(frame, 'X');
                y = GETVAL(frame, 'Y');
                f = GETVAL(frame, 'F');
                pCMe->m_pILog->pVTbl->InfoFormat(pCMe->m_pILog, 
												"Linear move to: \n"
												"\tX: %.2f\n"
												"\tY: %.2f\n"
												"\tWith speed F: %.2f", x, y, f);
                break;       
        }
    }

    return 0;
}

/* Create Virtual Table */
IEcoGCode1EventsVTbl g_x4EC638CD188548BB9D7A7A09F8B458ACVTblSwarm = {
    CEcoGCode1SwarmRobotSink_QueryInterface,
    CEcoGCode1SwarmRobotSink_AddRef,
    CEcoGCode1SwarmRobotSink_Release,
	CEcoGCode1SwarmRobotSink_OnProgram,
    CEcoGCode1SwarmRobotSink_OnFrame
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
int16_t createCEcoGCode1SwarmRobotSink(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* in */ IEcoLog1* pILog, /* out */ IEcoGCode1Events** pIEvents) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoGCode1SwarmRobotSink* pCMe = 0;

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
    pCMe = (CEcoGCode1SwarmRobotSink*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoGCode1SwarmRobotSink));

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoGCode1Events */
    pCMe->m_pVTblIEvents = &g_x4EC638CD188548BB9D7A7A09F8B458ACVTblSwarm;

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
void deleteCEcoGCode1SwarmRobotSink(/* in */ IEcoGCode1Events* pIEvents) {
    CEcoGCode1SwarmRobotSink* pCMe = (CEcoGCode1SwarmRobotSink*)pIEvents;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEvents != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_pILog != 0 ) {
            pCMe->m_pILog->pVTbl->Release(pCMe->m_pILog);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}
