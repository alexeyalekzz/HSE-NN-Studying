/*
 * <кодировка символов>
 *   Cyrillic (Windows) - Codepage 1251
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoGCodeProgram1
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoGCodeProgram1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2021 Klimov Alexey. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "CEcoGCodeProgram1.h"

/* Выделяем память под один экземпляр */
CEcoGCodeProgram1 g_xF5ACD69A4AFB40FFB92DDFC37ADA56C6_EcoGCodeProgram1 = {0};

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoGCode1Program
 * </описание>
 *
 */
int16_t CEcoGCodeProgram1_QueryInterface(/* in */ struct IEcoGCode1Program* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoGCodeProgram1* pCMe = (CEcoGCodeProgram1*) me;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return result;
    }

    /* Проверка и получение запрошенного интерфейса */
    if (IsEqualUGUID(riid, &IID_IEcoGCode1Program)) {
        *ppv = &pCMe->m_pVTblIProg;
        pCMe->m_pVTblIProg->AddRef(me);
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblIProg;
        pCMe->m_pVTblIProg->AddRef(me);
    } else {
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
 *   Функция AddRef для интерфейса IEcoGCode1Program
 * </описание>
 *
 */
uint32_t CEcoGCodeProgram1_AddRef(/* in */ struct IEcoGCode1Program* me) {
    CEcoGCodeProgram1* pCMe = (CEcoGCodeProgram1*)me;

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
 *   Функция Release для интерфейса IEcoGCode1Program
 * </описание>
 *
 */
uint32_t CEcoGCodeProgram1_Release(/* in */ struct IEcoGCode1Program* me) {
    CEcoGCodeProgram1* pCMe = (CEcoGCodeProgram1*)me;

    /* Проверка указателя */
    if (me == 0) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoGCodeProgram1((IEcoGCode1Program*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Resize
 * </сводка>
 *
 * <описание>
     Выделяет память на указанное количество кадров
 * </описание>
 *
 */
void CEcoGCodeProgram1_Resize(/* in */ struct IEcoGCode1Program* me, /* in */ uint32_t new_size) {
    CEcoGCodeProgram1* pCMe = (CEcoGCodeProgram1*)me;
    IEcoMemoryAllocator1* pIMem = pCMe->m_pIMem;
    uint32_t i;
	FRAME* temp;

    if (new_size < pCMe->m_frames_count) {
        return;
    }

    temp = (FRAME*)pIMem->pVTbl->Alloc(pIMem, new_size * sizeof(FRAME));

    for (i = 0; i < pCMe->m_frames_count; ++i) {
        temp[i].data = pCMe->m_frames[i].data;
    }

    for (i = pCMe->m_frames_capacity; i < new_size; ++i) {
        temp[i].data = (float*)pIMem->pVTbl->Alloc(pIMem, FRAME_SIZE * sizeof(float));
    }

	if (!pCMe->m_frames) {	
		pIMem->pVTbl->Free(pIMem, pCMe->m_frames);
	}

    pCMe->m_frames_capacity = new_size;
    pCMe->m_frames = temp;
}

/*
 *
 * <сводка>
 *   Функция AddFrame
 * </сводка>
 *
 * <описание>
     Добавляет кадр к программе
 * </описание>
 *
 */
void CEcoGCodeProgram1_AddFrame(/* in */ struct IEcoGCode1Program* me, /* in */ FRAME frame) {
    CEcoGCodeProgram1* pCMe = (CEcoGCodeProgram1*)me;
    uint32_t i;
	FRAME* newFrame;

    if (pCMe->m_frames_count == pCMe->m_frames_capacity) {
        pCMe->m_pVTblIProg->Resize((IEcoGCode1Program*)pCMe, pCMe->m_frames_capacity + 3);
    }

	/* Копирование значений */
	newFrame = &pCMe->m_frames[pCMe->m_frames_count];
    for (i = 0; i < FRAME_SIZE; ++i) {
        newFrame->data[i] = frame.data[i];
    }
	newFrame->seen = frame.seen;

    pCMe->m_frames_count++;
}

/*
 *
 * <сводка>
 *   Функция GetFrame
 * </сводка>
 *
 * <описание>
     Возвращает указанный кадр
 * </описание>
 *
 */
int16_t CEcoGCodeProgram1_GetFrame(/* in */ struct IEcoGCode1Program* me, /* in */ uint32_t i, /* out */ FRAME** frame) {
    CEcoGCodeProgram1* pCMe = (CEcoGCodeProgram1*)me;

    if (i < pCMe->m_frames_count) {
        *frame = &pCMe->m_frames[i];
        return 0;
    } else {
        *frame = 0;
        return -1;
    }
}

/* Create Virtual Table */
IEcoGCode1ProgramVTbl g_x699E64B107E94C3F89754E810DC4CF7FVTbl = {
    CEcoGCodeProgram1_QueryInterface,
    CEcoGCodeProgram1_AddRef,
    CEcoGCodeProgram1_Release,
    CEcoGCodeProgram1_Resize,
    CEcoGCodeProgram1_AddFrame,
    CEcoGCodeProgram1_GetFrame
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
int16_t createCEcoGCodeProgram1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoGCode1Program** ppIProg) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoGCodeProgram1 * pCMe = 0;

//#ifdef ECO_DLL
    /* Проверка указателей */
    if (ppIProg == 0 || pIUnkSystem == 0) {
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
    pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 && pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoGCodeProgram1*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoGCodeProgram1));

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;
//#else
    /* Получение указателя на структуру компонента прошивки расположенной в области инициализированных данных */
    // pCMe = &g_xF5ACD69A4AFB40FFB92DDFC37ADA56C6_EcoGCodeProgram1;
//#endif
    /* Проверка счетчика ссылок на компонент */
    if (pCMe->m_cRef == 0 ) {
        /* Установка счетчика ссылок на компонент */
        pCMe->m_cRef = 1;

        /* Создание таблицы функций интерфейса IEcoEV3Instruction1 */
        pCMe->m_pVTblIProg = &g_x699E64B107E94C3F89754E810DC4CF7FVTbl;

        /* Инициализация данных */
        pCMe->m_frames = 0;
        pCMe->m_frames_count = 0;
        pCMe->m_frames_capacity = 0;
    }

    /* Возврат указателя на интерфейс */
    *ppIProg = (IEcoGCode1Program*)pCMe;

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
void deleteCEcoGCodeProgram1(/* in */ IEcoGCode1Program* pIProg) {
    CEcoGCodeProgram1* pCMe = (CEcoGCodeProgram1*)pIProg;
    IEcoMemoryAllocator1* pIMem = 0;
    uint32_t i;

    if (pIProg != 0) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_frames != 0 ) {
            for (i = 0; i < pCMe->m_frames_capacity; ++i) {
                pIMem->pVTbl->Free(pIMem, pCMe->m_frames[i].data);
            }
            pIMem->pVTbl->Free(pIMem, pCMe->m_frames);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}
