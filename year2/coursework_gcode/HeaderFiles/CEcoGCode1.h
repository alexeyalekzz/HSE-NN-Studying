/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoGCode1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию компонента CEcoGCode1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2021 Klimov Alexey. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_ECOGCODE1_H__
#define __C_ECOGCODE1_H__

#include "IEcoGCode1.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoGCode1 {

    /* Таблица функций интерфейса IEcoGCode1 */
    IEcoGCode1VTbl* m_pVTblIEcoGCode1;

    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;

    /* Системный интерфейс */
    IEcoSystem1* m_pISys;

} CEcoGCode1, *CEcoGCode1Ptr;

/* Инициализация экземпляра */
int16_t initCEcoGCode1(/*in*/ struct IEcoGCode1* me, /* in */ IEcoUnknown *pIUnkSystem);
/* Создание экземпляра */
int16_t createCEcoGCode1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoGCode1** ppIEcoGCode1);
/* Удаление */
void deleteCEcoGCode1(/* in */ IEcoGCode1* pIEcoGCode1);

#endif /* __C_ECOGCODE1_H__ */
