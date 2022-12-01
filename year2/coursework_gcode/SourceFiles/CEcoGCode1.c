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
 *   Данный исходный код описывает реализацию интерфейсов CEcoGCode1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2021 Klimov Alexey. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "CEcoGCode1.h"
#include "CEcoGCodeProgram1.h"

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoGCode1
 * </описание>
 *
 */
int16_t CEcoGCode1_QueryInterface(/* in */ struct IEcoGCode1* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoGCode1* pCMe = (CEcoGCode1*)me;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return result;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoGCode1) ) {
        *ppv = &pCMe->m_pVTblIEcoGCode1;
        pCMe->m_pVTblIEcoGCode1->AddRef((IEcoGCode1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoGCode1;
        pCMe->m_pVTblIEcoGCode1->AddRef((IEcoGCode1*)pCMe);
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
 *   Функция AddRef для интерфейса IEcoGCode1
 * </описание>
 *
 */
uint32_t CEcoGCode1_AddRef(/* in */ struct IEcoGCode1* me) {
    CEcoGCode1* pCMe = (CEcoGCode1*)me;

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
 *   Функция Release для интерфейса IEcoGCode1
 * </описание>
 *
 */
uint32_t CEcoGCode1_Release(/* in */ struct IEcoGCode1* me) {
    CEcoGCode1* pCMe = (CEcoGCode1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoGCode1((IEcoGCode1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция new_Program
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
IEcoGCode1Program* CEcoGCode1_new_Program(/* in */ struct IEcoGCode1* me, /* in */ uint32_t initCapacity) {
    CEcoGCode1* pCMe = (CEcoGCode1*)me;
	IEcoGCode1Program* res;

    /* Проверка указателей */
    if (me == 0) {
        return 0;
    }

	createCEcoGCodeProgram1((IEcoUnknown*)pCMe->m_pISys, 0, &res);

	if (initCapacity != 0) {
		res->pVTbl->Resize(res, initCapacity);
	}

    return res;
}

void reverseStr(uint8_t* str, int32_t len) {
	int32_t i = 0, j = len - 1, temp;
	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

void ftoa_(double val, uint8_t* str, uint8_t** end, int32_t afterpoint) {
	int32_t ipart = (int32_t)val;
	double fpart = val - (double)ipart;
	int32_t i = 0, j;

	if (ipart != 0) {
		while (ipart) {
			str[i++] = (ipart % 10) + '0';
			ipart /= 10;
		}
	} else {
		str[i++] = '0';
	}

	if (i > 1) {
		reverseStr(str, i);
	}

	j = i;
	if (afterpoint != 0) {
	str[j++] = '.';

		while (afterpoint) {
			str[j++] = (int64_t)(fpart * 10) + '0';
			fpart = (fpart * 10) - (double)((int64_t)(fpart * 10)) + 0.0000001;
			afterpoint--;
		}
	}
	*end = str + j - 1;
}

/*
 *
 * <сводка>
 *   Функция EncodeMessageByProgram
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t CEcoGCode1_EncodeMessageByProgram(/* in */ struct IEcoGCode1* me, /* in */ IEcoGCode1Program* pIProgram, /* in | out */ uint8_t** data, /* in | out */ uint16_t* size) {
    CEcoGCode1* pCMe = (CEcoGCode1*)me;
	CEcoGCodeProgram1* pCProg = (CEcoGCodeProgram1*)pIProgram;
	uint8_t *gcode, *end;
	uint32_t i, len = 0, c, bufsize;

    /* Проверка указателей */
    if (me == 0 || pIProgram == 0) {
        return -1;
    }

	/* Выделение памяти */
	bufsize = 30 * pCProg->m_frames_count * sizeof(uint8_t);
	gcode = (uint8_t*)pCProg->m_pIMem->pVTbl->Alloc(pCProg->m_pIMem, bufsize);

	/* Перевод в строку */
	for (i = 0; i < pCProg->m_frames_count; ++i) {
		FRAME* frame = 0;
		pCProg->m_pVTblIProg->GetFrame(pIProgram, i, &frame);
		
		for (c = 'A'; c <= 'Z'; ++c) {
			if (SEEN(frame, c)) {
				gcode[len++] = c;
				ftoa_(GETVAL(frame, c), &gcode[len], &end, 2);
				len = end - gcode + 1;
				gcode[len++] = ' ';
			}
		}
		len--;
		gcode[len++] = '\n';
	}
	gcode[len++] = '\0';

	/* Выделение точного объема памяти и копирование */
	*data = (uint8_t*)pCProg->m_pIMem->pVTbl->Alloc(pCProg->m_pIMem, len * sizeof(uint8_t));
	for (i = 0; i < len; ++i) {
		(*data)[i] = gcode[i];
	}

	/* Освобождение временного блока памяти */
	pCProg->m_pIMem->pVTbl->Free(pCProg->m_pIMem, gcode);

	*size = len;
    return 0;
}

float atof_(uint8_t* s, uint8_t** end) {
	int32_t f = 0, m = 0, sign, d = 1;

	sign = (s[0] == '-') ? -1 : 1;
	if (s[0] == '-' || s[0] == '+') {
		s++;
	}

	for (; *s >= '0' && *s <= '9'; s++) {
		f = (*s - '0') + f * 10;
	}
	if (*s == '.') {
		for (s += 1; *s >= '0' && *s <= '9'; s++) {
			m = (*s - '0') + m * 10;
			d *= 10;
		}
	}
	*end = s - 1;
	return sign * (f + (float) m / d);
}


void decode_line(uint8_t* line, FRAME* frame, uint8_t** end) {
    uint32_t i = 0;
    uint32_t seen = 0;
    uint8_t c, *e;
    float v;

    while (line[i] != 10 && line[i] != 13) {
        c = line[i];

        if (c >= 'A' && c <= 'Z') {
            v = atof_(&line[i + 1], &e);

            if (e > &line[i + 1]) {
                seen |= LMASK(c);
                frame->data[c - 'A'] = v;
                i = e - line;
            }
        }

        i++;
    }

    *end = &line[i];
    frame->seen = seen;
}

/*
 *
 * <сводка>
 *   Функция DecodeMessage
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t CEcoGCode1_DecodeMessage(/* in */ struct IEcoGCode1* me, /* in */ uint8_t* data, /* in  */ uint16_t size, /* out */ IEcoGCode1Events* pICommandEvents) {
    CEcoGCode1* pCMe = (CEcoGCode1*)me;
    IEcoGCode1Program* pIProg = 0;
    uint32_t frame_cntr = 0, i = 0;
    uint8_t* end = data;

	float tempdata[FRAME_SIZE * sizeof(float)];
	FRAME temp_frame = {
		0,
		tempdata
	};

    /* Проверка указателей */
    if (me == 0 || data == 0 || pICommandEvents == 0 || size == 0) {
        return -1;
    }

    /* Подсчет кадров */
    while (i < size) {
        if (data[i] == 10 || data[i] == 13) {
            frame_cntr++;
        }
        i++;
    }

    /* Проверка */
    if (frame_cntr == 0) {
        return -1;
    }

	pIProg = pCMe->m_pVTblIEcoGCode1->newProgram(me, frame_cntr);

    /* Декодирование */
    i = 0;
    while (i < size) {
        decode_line(data + i, &temp_frame, &end);
        pIProg->pVTbl->AddFrame(pIProg, temp_frame);
        /* Передача кадра */
		pICommandEvents->pVTbl->OnFrame(pICommandEvents, &temp_frame);
        i = end - data + 1;
    }

    /* Передача программы */
    pICommandEvents->pVTbl->OnProgram(pICommandEvents, pIProg);
    return 0;
}

/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция инициализации экземпляра
 * </описание>
 *
 */
int16_t initCEcoGCode1(/*in*/ struct IEcoGCode1* me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    CEcoGCode1* pCMe = (CEcoGCode1*)me;

    /* Проверка указателей */
    if (me == 0) {
        return -1;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    return 0;
}

/* Create Virtual Table */
IEcoGCode1VTbl g_x7B7E34CE5DDF47EBA51D4D964EE0644CVTbl = {
    CEcoGCode1_QueryInterface,
    CEcoGCode1_AddRef,
    CEcoGCode1_Release,
    CEcoGCode1_new_Program,
    CEcoGCode1_EncodeMessageByProgram,
    CEcoGCode1_DecodeMessage
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
int16_t createCEcoGCode1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoGCode1** ppIEcoGCode1) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoGCode1* pCMe = 0;

    /* Проверка указателей */
    if (ppIEcoGCode1 == 0 || pIUnkSystem == 0) {
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
    pCMe = (CEcoGCode1*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoGCode1));

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoGCode1 */
    pCMe->m_pVTblIEcoGCode1 = &g_x7B7E34CE5DDF47EBA51D4D964EE0644CVTbl;

    /* Возврат указателя на интерфейс */
    *ppIEcoGCode1 = (IEcoGCode1*)pCMe;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

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
void deleteCEcoGCode1(/* in */ IEcoGCode1* pIEcoGCode1) {
    CEcoGCode1* pCMe = (CEcoGCode1*)pIEcoGCode1;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoGCode1 != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}
