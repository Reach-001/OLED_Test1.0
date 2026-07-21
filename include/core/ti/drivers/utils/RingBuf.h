/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
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
 * RingBuf.h - 字节型环形缓冲区接口，适用于串口收发等字节流场景
 */

#ifndef ti_drivers_utils_RingBuf__include
#define ti_drivers_utils_RingBuf__include

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 环形缓冲区对象结构体 */
typedef struct {
    unsigned char      *buffer;    /* 数据缓冲区指针（由调用者分配） */
    size_t              length;    /* 缓冲区总容量（字节数） */
    size_t              count;     /* 当前已存入的字节数 */
    size_t              head;      /* 写入位置索引（下次写入 head+1） */
    size_t              tail;      /* 读取位置索引 */
    size_t              maxCount;  /* 历史最大使用量（高水位标记） */
} RingBuf_Object, *RingBuf_Handle;

/*!
 *  @brief  Initialize circular buffer
 *          初始化环形缓冲区
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *                  指向环形缓冲区对象
 *
 *  @param  bufPtr  Pointer to data buffer to be used for the circular buffer.
 *                  The buffer is NOT stored in RingBuf_Object.
 *                  指向由调用者提供的数据缓冲区（不存储在对象内部）
 *
 *  @param  bufSize The size of bufPtr in number of unsigned chars.
 *                  缓冲区大小（字节数）
 */
void RingBuf_construct(RingBuf_Handle object, unsigned char *bufPtr,
    size_t bufSize);

/*!
 *  @brief  Flush all the data from the buffer.
 *          清空缓冲区中的所有数据（保留历史最大使用量）
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *                  指向环形缓冲区对象
 */
void RingBuf_flush(RingBuf_Handle object);

/*!
 *  @brief  Get an unsigned char from the end of the circular buffer and remove
 *          it.
 *          从缓冲区头部取出一个字节并移除
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *                  指向环形缓冲区对象
 *
 *  @param  data    Pointer to an unsigned char to be filled with the data from
 *                  the front of the circular buffer.
 *                  指向接收数据的字节变量
 *
 *  @return         Number of unsigned chars on the buffer after taking it out
 *                  of the circular buffer. If it returns -1, the circular
 *                  buffer was already empty and data is invalid.
 *                  取出后缓冲区剩余字节数；缓冲区为空时返回 -1
 */
int RingBuf_get(RingBuf_Handle object, unsigned char *data);

/*!
 *  @brief  Advance the get index and decrement the buffer count. This function
 *          should normally be called from a context where HWI is disabled. For
 *          efficiency, it is incumbent on the caller to ensure mutual exclusion
 *          with the proper HWI lock.
 *          推进读取索引并递减计数（零拷贝消费），需在关中断下调用
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *                  指向环形缓冲区对象
 *
 *  @param size     Number of unsigned characters to advance the get index.
 *                  要消费的字节数
 *
 *  @return         Number of unsigned chars that we were able to be advanced.
 *                  实际消费的字节数
 */
int RingBuf_getConsume(RingBuf_Handle object, size_t size);

/*!
 *  @brief  Get the number of unsigned chars currently stored on the circular
 *          buffer.
 *          获取缓冲区中当前存储的字节数
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *          variables to operate a circular buffer.
 *          指向环形缓冲区对象
 *
 *  @return         Number of unsigned chars on the circular buffer.
 *                  当前存储的字节数
 */
int RingBuf_getCount(RingBuf_Handle object);

/*!
 *  @brief  A high-water mark indicating the largest number of unsigned chars
 *          stored on the circular buffer since it was constructed.
 *          获取缓冲区历史最大使用量（高水位标记）
 *
 *  @return         Get the largest number of unsigned chars that were at one
 *                  point in the circular buffer.
 *                  历史上同时存储的最大字节数
 */
int RingBuf_getMaxCount(RingBuf_Handle object);

/*!
 *  @brief  Get one or more unsigned chars from the end of the circular buffer and
 *          remove them.
 *          从缓冲区头部批量取出 n 个字节并移除
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *                  指向环形缓冲区对象
 *
 *  @param  data    Pointer to an unsigned char to be filled with the data from
 *                  the front of the circular buffer.
 *                  指向接收数据的缓冲区
 *
 *  @param  n       number of unsigned chars to try and remove.
 *                  期望取出的字节数
 *
 *  @return         Number of unsigned chars successfully removed from the buffer
 *                  and copied into \a data. May be 0 or less than \a n.
 *                  实际取出的字节数，可能为 0 或小于 n
 */
int RingBuf_getn(RingBuf_Handle object, unsigned char *data, size_t n);

/*!
 *  @brief  Get a pointer reference to the next chunk of linear memory available for
 *          accessing data in the buffer. This function should
 *          normally be called from a context where HWI is disabled. For
 *          efficiency, it is incumbent on the caller to ensure mutual
 *          exclusion with the proper HWI lock.
 *          获取可读数据的线性内存指针（零拷贝读取），需在关中断下调用
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *                  指向环形缓冲区对象
 *
 *  @param  data    Reference to a pointer to set for the memory location in the
 *                  buffer where data can accessed.
 *                  输出参数：指向可读数据的起始地址
 *
 *  @return         Number of unsigned chars in linear memory where data
 *                  can be accessed, or 0 if it's empty.
 *                  可连续读取的字节数，缓冲区为空时返回 0
 */
int RingBuf_getPointer(RingBuf_Handle object, unsigned char **data);

/*!
 *  @brief  Function to determine if the circular buffer is full or not.
 *          判断缓冲区是否已满
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *                  指向环形缓冲区对象
 *
 *  @return         true if circular buffer is full, else false.
 *                  缓冲区已满返回 true，否则返回 false
 */
bool RingBuf_isFull(RingBuf_Handle object);

/*!
 *  @brief  Get an unsigned char from the end of the circular buffer without
 *          removing it.
 *          查看缓冲区头部字节但不移除（窥视）
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *                  指向环形缓冲区对象
 *
 *  @param  data    Pointer to an unsigned char to be filled with the data from
 *                  the front of the circular buffer. This function does not
 *                  remove the data from the circular buffer. Do not evaluate
 *                  data if the count returned is equal to 0.
 *                  指向接收数据的字节变量（数据不会被移除）
 *
 *  @return         Number of unsigned chars on the circular buffer. If the
 *                  number != 0, then data will contain the unsigned char at the
 *                  end of the circular buffer.
 *                  缓冲区中的字节数；返回 0 时 data 内容无效
 */
int RingBuf_peek(RingBuf_Handle object, unsigned char *data);

/*!
 *  @brief  Put an unsigned char into the end of the circular buffer.
 *          向缓冲区尾部写入一个字节
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *                  指向环形缓冲区对象
 *
 *  @param  data    unsigned char to be placed at the end of the circular
 *                  buffer.
 *                  待写入的字节数据
 *
 *  @return         Number of unsigned chars on the buffer after it was added,
 *                  or -1 if it's already full.
 *                  写入后缓冲区字节数；缓冲区已满时返回 -1
 */
int RingBuf_put(RingBuf_Handle object, unsigned char data);

/*!
 *  @brief  Advance the committed put index and increment the buffer count. This
 *          function should normally be called from a context where HWI is disabled.
 *          For efficiency, it is incumbent on the caller to ensure mutual
 *          exclusion with the proper HWI lock.
 *          推进写入索引并递增计数（零拷贝提交），需在关中断下调用
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *                  指向环形缓冲区对象
 *
 *  @param size     Number of unsigned characters to commit to the put index.
 *                  要提交的字节数
 *
 *  @return         Number of unsigned chars that we were able to be committed.
 *                  实际提交的字节数
 */
int RingBuf_putAdvance(RingBuf_Handle object, size_t size);

/*!
 *  @brief  Put one or more unsigned chars into the end of the circular buffer.
 *          向缓冲区尾部批量写入 n 个字节
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *                  指向环形缓冲区对象
 *
 *  @param  data    unsigned chars to be placed at the end of the circular
 *                  buffer.
 *                  指向待写入的数据缓冲区
 *
 *  @param  n       number of unsigned chars to try and remove.
 *                  期望写入的字节数
 *
 *  @return         Number of unsigned chars placed into the buffer. May be 0 or
 *                  less than \a n.
 *                  实际写入的字节数，可能为 0 或小于 n
 */
int RingBuf_putn(RingBuf_Handle object, unsigned char *data, size_t n);

/*!
 *  @brief  Get a pointer reference to the next chunk of linear memory available for
 *          adding data to the buffer. This function should normally be called from
 *          a context where HWI is disabled. For efficiency, it is incumbent on the
 *          caller to ensure mutual exclusion with the proper HWI lock.
 *          获取可写入数据的线性内存指针（零拷贝写入），需在关中断下调用
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *                  指向环形缓冲区对象
 *
 *  @param  data    Reference to a pointer to set for the memory location in the
 *                  buffer where more data can be added.
 *                  输出参数：指向可写入区域的起始地址
 *
 *  @return         Number of unsigned chars available in linear memory where data
 *                  can be added, or 0 if it's already full.
 *                  可连续写入的字节数，缓冲区已满时返回 0
 */
int RingBuf_putPointer(RingBuf_Handle object, unsigned char **data);

/*!
 *  @brief  Return the number of unsigned characters that the buffer has space for.
 *          This function should normally be called from a context where HWI is
 *          disabled. For efficiency, it is incumbent on the caller to ensure mutual
 *          exclusion with the proper HWI lock.
 *          获取缓冲区剩余可用空间（字节数），需在关中断下调用
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *                  指向环形缓冲区对象
 *
 *  @return         Number of unsigned chars that the buffer has space for.
 *                  剩余可用字节数
 */
static inline size_t RingBuf_space(RingBuf_Handle object)
{
    return (object->length - object->count);
}


#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_utils_RingBuf__include */
