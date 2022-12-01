/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoGCode1Factory
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию фабрики для компонента
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2021 Klimov Alexey. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_ECOGCODE1_FACTORY_H__
#define __C_ECOGCODE1_FACTORY_H__

#include "IEcoSystem1.h"

typedef struct CEcoGCode1Factory {

    /* Таблица функций интерфейса IEcoComponentFactory */
    IEcoComponentFactoryVTbl* m_pVTblICF;

    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Данные компонентов для фабрики */
    CreateInstance m_pInstance;
    InitInstance m_pInitInstance;
    char_t m_Name[64];
    char_t m_Version[16];
    char_t m_Manufacturer[64];
	
} CEcoGCode1Factory;

#endif /* __C_ECOGCODE1_FACTORY_H__ */
