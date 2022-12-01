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
 *   Данный заголовок описывает реализацию компонента CEcoGCode1Print3DSink
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2021 Klimov Alexey. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_ECO_G_CODE_1_PRINT_3D_SINK_H__
#define __C_ECO_G_CODE_1_PRINT_3D_SINK_H__


#include "IEcoGCode1.h"
#include "IEcoSystem1.h"
#include "IdEcoString1.h"
#include "IEcoLog1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoGCode1Print3DSink {

    /* Таблица функций интерфейса IEcoGCode1Events */
    IEcoGCode1EventsVTbl* m_pVTblIEvents;

    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;

    /* Интерфейс для работы со строкой */
    IEcoString1* m_pIString;

    /* Указатель на интерфейс работы с журналом */
    IEcoLog1* m_pILog;

    /* Данные экземпляра */

} CEcoGCode1Print3DSink, *CEcoGCode1Print3DSinkPtr;

/* Создание экземпляра */
int16_t createCEcoGCode1Print3DSink(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* in */ IEcoLog1* pILog, /* out */ IEcoGCode1Events** ppIEvents);
/* Удаление */
void deleteCEcoGCode1Print3DSink(/* in */ IEcoGCode1Events* pIEvents);

#endif /* __C_ECO_G_CODE_1_PRINT_3D_SINK_H__ */
