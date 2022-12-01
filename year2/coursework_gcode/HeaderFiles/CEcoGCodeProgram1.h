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
 *   Данный заголовок описывает реализацию компонента CEcoGCodeProgram1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2021 Klimov Alexey. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_ECO_G_CODE_PROGRAM_1_H__
#define __C_ECO_G_CODE_PROGRAM_1_H__

#include "IEcoGCode1.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

#define FRAME_SIZE 26

typedef struct CEcoGCodeProgram1 {

  /* Таблица функций интерфейса IEcoGCode1Program */
  IEcoGCode1ProgramVTbl* m_pVTblIProg;

  /* Счетчик ссылок */
  uint32_t m_cRef;

  /* Интерфейс для работы с памятью */
  IEcoMemoryAllocator1* m_pIMem;

  /* Системный интерфейс */
  IEcoSystem1* m_pISys;

  /* Данные экземпляра */
  FRAME* m_frames;

  uint32_t m_frames_count;

  uint32_t m_frames_capacity;

} CEcoGCodeProgram1, *CEcoGCodeProgram1Ptr;

/* Создание экземпляра */
int16_t createCEcoGCodeProgram1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoGCode1Program** ppIProg);
/* Удаление */
void deleteCEcoGCodeProgram1(/* in */ IEcoGCode1Program* pIProg);

#endif  /* __C_ECO_G_CODE_PROGRAM_1_H__ */
