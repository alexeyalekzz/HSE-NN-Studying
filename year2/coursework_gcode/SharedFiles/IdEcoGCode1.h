/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IdEcoGCode1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает интерфейс IdEcoGCode1
 * </описание>
 *
 * <ссылка>
 *
 * </ссылка>
 *
 * <автор>
 *   Copyright (c) 2021 Klimov Alexey. All rights reserved.
 * </автор>
 *
 */

#ifndef __ID_ECOGCODE1_H__
#define __ID_ECOGCODE1_H__

#include "IEcoBase1.h"
#include "IEcoGCode1.h"

/* EcoGCode1 CID = {F5ACD69A-4AFB-40FF-B92D-DFC37ADA56C6} */
#ifndef __CID_EcoGCode1
static const UGUID CID_EcoGCode1 = {0x01, 0x10, 0xF5, 0xAC, 0xD6, 0x9A, 0x4A, 0xFB, 0x40, 0xFF, 0xB9, 0x2D, 0xDF, 0xC3, 0x7A, 0xDA, 0x56, 0xC6};
#endif /* __CID_EcoGCode1 */

/* Фабрика компонента для динамической и статической компановки */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_F5ACD69A4AFB40FFB92DDFC37ADA56C6;
#endif

#endif /* __ID_ECOGCODE1_H__ */
