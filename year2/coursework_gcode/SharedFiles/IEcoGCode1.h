/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IEcoGCode1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает интерфейс IEcoGCode1
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

#ifndef __I_ECO_G_CODE_1_H__
#define __I_ECO_G_CODE_1_H__

#include "IEcoBase1.h"

/* IEcoGCode1 IID = {699E64B1-07E9-4C3F-8975-4E810DC4CF7F} */
#ifndef __IID_IEcoGCode1Program
static const UGUID IID_IEcoGCode1Program = {0x01, 0x10, 0x69, 0x9E, 0x64, 0xB1, 0x07, 0xE9, 0x4C, 0x3F, 0x89, 0x75, 0x4E, 0x81, 0x0D, 0xC4, 0xCF, 0x7F};
#endif /* __IID_IEcoGCode1Program */

#define	LMASK(с) (1<<((с) - 'A'))
#define	SEEN(f, c) (((f)->seen & LMASK(c)) != 0)
#define	GETVAL(f, c) f->data[(c) -'A']

typedef struct FRAME {
  uint32_t seen;
  float* data;
} FRAME;

typedef struct IEcoGCode1ProgramVTbl {

    /* IEcoUnknown */
    int16_t (*QueryInterface)(/* in */ struct IEcoGCode1Program* me, /* in */ const UGUID* riid, /* out */ void **ppv);
    uint32_t (*AddRef)(/* in */ struct IEcoGCode1Program* me);
    uint32_t (*Release)(/* in */ struct IEcoGCode1Program* me);

    /* IEcoGCode1Program */
    void (*Resize)(/* in */ struct IEcoGCode1Program* me, /* in */ uint32_t new_size);
    void (*AddFrame)(/* in */ struct IEcoGCode1Program* me, /* in */ FRAME frame);
    int16_t (*GetFrame)(/* in */ struct IEcoGCode1Program* me, /* in */ uint32_t i, /* out */ FRAME** frame);

} IEcoGCode1ProgramVTbl, *IEcoGCode1ProgramVTblPtr;

interface IEcoGCode1Program {
    struct IEcoGCode1ProgramVTbl *pVTbl;
} IEcoGCode1Program;

/* IEcoGCode1Events IID = {4EC638CD-1885-48BB-9D7A-7A09F8B458AC} */
#ifndef __IID_IEcoGCode1Events
static const UGUID IID_IEcoGCode1Events = {0x01, 0x10, 0x4E, 0xC6, 0x38, 0xCD, 0x18, 0x85, 0x48, 0xBB, 0x9D, 0x7A, 0x7A, 0x09, 0xF8, 0xB4, 0x58, 0xAC};
#endif /* __IID_IEcoGCode1Events */

typedef struct IEcoGCode1EventsVTbl {

    /* IEcoUnknown */
    int16_t (*QueryInterface)(/* in */ struct IEcoGCode1Events* me, /* in */ const UGUID* riid, /* out */ void **ppv);
    uint32_t (*AddRef)(/* in */ struct IEcoGCode1Events* me);
    uint32_t (*Release)(/* in */ struct IEcoGCode1Events* me);

    /* IEcoGCode1Events */
    int8_t (*OnProgram)(/* in */ struct IEcoGCode1Events* me, /* in */ IEcoGCode1Program* prog);
    int8_t (*OnFrame)(/* in */ struct IEcoGCode1Events* me, /* in */ FRAME* frame);

} IEcoGCode1EventsVTbl, *IEcoGCode1EventsVTblPtr;

interface IEcoGCode1Events {
    struct IEcoGCode1EventsVTbl *pVTbl;
} IEcoGCode1Events;

/* IEcoGCode1 IID = {7B7E34CE-5DDF-47EB-A51D-4D964EE0644C} */
#ifndef __IID_IEcoGCode1
static const UGUID IID_IEcoGCode1 = {0x01, 0x10, 0x7B, 0x7E, 0x34, 0xCE, 0x5D, 0xDF, 0x47, 0xEB, 0xA5, 0x1D, 0x4D, 0x96, 0x4E, 0xE0, 0x64, 0x4C};
#endif /* __IID_IEcoGCode1 */

typedef struct IEcoGCode1VTbl {

    /* IEcoUnknown */
    int16_t (*QueryInterface)(/* in */ struct IEcoGCode1* me, /* in */ const UGUID* riid, /* out */ void **ppv);
    uint32_t (*AddRef)(/* in */ struct IEcoGCode1* me);
    uint32_t (*Release)(/* in */ struct IEcoGCode1* me);

    /* IEcoGCode1 */
    IEcoGCode1Program* (*newProgram)(/* in */ struct IEcoGCode1* me, /* in */ uint32_t initCapacity);
    int16_t (*EncodeMessageByProgram)(/* in */ struct IEcoGCode1* me, /* in */ IEcoGCode1Program* pIProgram, /* in | out */ uint8_t** data, /* in | out */ uint16_t* size);
    int16_t (*DecodeMessage)(/* in */ struct IEcoGCode1* me, /* in */ uint8_t* data, /* in  */ uint16_t size, /* out */ IEcoGCode1Events* pICommandEvents);

} IEcoGCode1VTbl, *IEcoGCode1VTblPtr;

interface IEcoGCode1 {
    struct IEcoGCode1VTbl *pVTbl;
} IEcoGCode1;


#endif /* __I_ECO_G_CODE_1_H__ */
