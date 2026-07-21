/*
 * Copyright (c) 2018-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * StructRingBuf.h - 结构体环形缓冲区接口，支持任意大小结构体的 FIFO 存取
 */

#ifndef ti_drivers_utils_StructRingBuf__include
#define ti_drivers_utils_StructRingBuf__include

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 结构体环形缓冲区对象结构体 */
typedef struct {
    uint8_t *buffer;     /* 数据缓冲区指针（由调用者分配） */
    size_t   length;     /* 缓冲区容量（以结构体个数计） */
    size_t   count;      /* 当前已存入的结构体个数 */
    size_t   head;       /* 写入位置索引（下次写入 head+1） */
    size_t   tail;       /* 读取位置索引 */
    size_t   maxCount;   /* 历史最大使用量（高水位标记） */
    size_t   structSize; /* 单个结构体的字节大小 */
} StructRingBuf_Object, *StructRingBuf_Handle;

/*!
 *  @brief  Initialize circular buffer
 *          初始化结构体环形缓冲区
 *
 *  @param  object Pointer to a StructRingBuf Object that contains the member variables to
 *          operate a circular buffer.
 *          指向结构体环形缓冲区对象
 *
 *  @param  bufPtr Pointer to data buffer to be used for the circular buffer. The
 *          buffer is NOT stored in StructRingBuf_Object.
 *          指向由调用者提供的数据缓冲区
 *
 *  @param  bufSize The size of bufPtr in number of structures
 *          缓冲区容量（结构体个数）
 *
 *  @param  structSize The size of a member structure in bytes
 *          单个结构体的字节大小
 */
void StructRingBuf_construct(StructRingBuf_Handle object, void *bufPtr,
    size_t bufSize, size_t structSize);

/*!
 *  @brief  Get an unsigned char from the end of the circular buffer and remove
 *          it.
 *          从缓冲区头部取出一个结构体并移除
 *
 *  @param  object Pointer to a StructRingBuf Object that contains the member variables to
 *          operate a circular buffer.
 *          指向结构体环形缓冲区对象
 *
 *  @param  data   Pointer to an unsigned char to be filled with the data from the
 *          front of the circular buffer.
 *          指向接收数据的结构体缓冲区
 *
 *  @return Number of unsigned chars on the buffer after taking it out of the
 *          circular buffer. If it returns -1, the circular buffer was already
 *          empty and data is invalid.
 *          取出后缓冲区剩余结构体个数；缓冲区为空时返回 -1
 */
int StructRingBuf_get(StructRingBuf_Handle object, void *data);

/*!
 *  @brief  Get the number of unsigned chars currently stored on the circular
 *          buffer.
 *          获取缓冲区中当前存储的结构体个数
 *
 *  @param  object Pointer to a StructRingBuf Object that contains the member variables to
 *          operate a circular buffer.
 *          指向结构体环形缓冲区对象
 *
 *  @return Number of unsigned chars on the circular buffer.
 *                  当前存储的结构体个数
 */
int StructRingBuf_getCount(StructRingBuf_Handle object);

/*!
 *  @brief  Function to determine if the circular buffer is full or not.
 *          判断缓冲区是否已满
 *
 *  @param  object Pointer to a StructRingBuf Object that contains the member variables to
 *          operate a circular buffer.
 *          指向结构体环形缓冲区对象
 *
 *  @return true if circular buffer is full, else false.
 *          缓冲区已满返回 true，否则返回 false
 */
bool StructRingBuf_isFull(StructRingBuf_Handle object);

/*!
 *  @brief  A high-water mark indicating the largest number of unsigned chars
 *          stored on the circular buffer since it was constructed.
 *          获取缓冲区历史最大使用量（高水位标记）
 *
 *  @return Get the largest number of unsigned chars that were at one point in
 *          the circular buffer.
 *          历史上同时存储的最大结构体个数
 */
int StructRingBuf_getMaxCount(StructRingBuf_Handle object);

/*!
 *  @brief  Get an unsigned char from the end of the circular buffer without
 *          remove it.
 *          窥视缓冲区头部结构体但不移除，返回数据指针
 *
 *  @param  object Pointer to a StructRingBuf Object that contains the member variables to
 *          operate a circular buffer.
 *          指向结构体环形缓冲区对象
 *
 *  @param  data   Pointer to an unsigned char to be filled with the data from the
 *          front of the circular buffer. This function does not remove the data
 *          from the circular buffer. Do not evaluate data if the count returned
 *          is equal to 0.
 *          输出参数：指向头部结构体的指针（数据不会被移除）
 *
 *  @return Number of unsigned chars on the circular buffer. If the number != 0,
 *          then data will contain the unsigned char at the end of the circular
 *          buffer.
 *          缓冲区中的结构体个数；返回 0 时 data 内容无效
 */
int StructRingBuf_peek(StructRingBuf_Handle object, void **data);

/*!
 *  @brief  Put an unsigned char into the end of the circular buffer.
 *          向缓冲区尾部写入一个结构体
 *
 *  @param  object Pointer to a StructRingBuf Object that contains the member variables to
 *          operate a circular buffer.
 *          指向结构体环形缓冲区对象
 *
 *  @param  data   unsigned char to be placed at the end of the circular buffer.
 *          指向待写入的结构体数据
 *
 *  @return Number of unsigned chars on the buffer after it was added, or -1 if
 *          it's already full.
 *          写入后缓冲区结构体个数；缓冲区已满时返回 -1
 */
int StructRingBuf_put(StructRingBuf_Handle object, const void *data);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_utils_StructRingBuf__include */
