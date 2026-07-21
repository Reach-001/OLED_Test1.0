/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
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
/** ============================================================================
 *  @file       List.h
 *
 *  @brief      Linked List interface for use in drivers
 *              驱动层双向链表接口，提供线程安全的 FIFO/LIFO 操作
 *
 *  This module provides simple doubly-link list implementation. There are two
 *  main structures:
 *     - ::List_List: The structure that holds the start of a linked list. There
 *  is no API to create one. It is up to the driver to provide the structure
 *  itself.
 *     - ::List_Elem: The structure that must be in the structure that is placed
 *  onto a linked list. Generally it is the first field in the structure. For
 *  example:
 *  @code
 *  typedef struct {
 *      List_Elem elem;
 *      void *buffer;
 *  } MyStruct;
 *  @endcode
 *
 *  The following shows how to create a linked list with three elements.
 *
 *  @code
 *  + denotes null-terminated
 *          _______        _______        _______      _______
 *         |_______|----->|_______|----->|_______|--->|_______|--//---,
 *    ,----|_______|    ,-|_______|<-----|_______|<---|_______|<-//-, +
 *    |      List       +   elem           elem          elem       |
 *    |_____________________________________________________________|
 *  @endcode
 *
 *  The APIs ::List_get, ::List_put, and ::List_putHead are
 *  atomic. The other APIs are not necessarily atomic. In other words, when
 *  traversing a linked list, it is up to the application to provide
 *  thread-safety (e.g. HwiP_disable/restore or MutexP_pend/post).
 *
 *  Initializing and adding an element to the tail and removing it
 *  @code
 *  typedef struct {
 *      List_Elem elem;
 *      void *buffer;
 *  } MyStruct;
 *
 *  List_List list;
 *  MyStruct foo;
 *  MyStruct *bar;
 *
 *  List_clearList(&list);
 *  List_put(&list, (List_Elem *)&foo);
 *  bar = (MyStruct *)List_get(&list);
 *  @endcode
 *
 *  The ::List_put and ::List_get APIs are used to maintain a first-in first-out
 *  (FIFO) linked list.
 *
 *  The ::List_putHead and ::List_get APIs are used to maintain a last-in first-out
 *  (LIFO) linked list.
 *
 *  Traversing a list from head to tail. Note: thread-safety calls are
 *  not shown here.
 *  @code
 *  List_List list;
 *  List_Elem *temp;
 *
 *  for (temp = List_head(&list); temp != NULL; temp = List_next(temp)) {
 *       printf("address = 0x%x\n", temp);
 *  }
 *  @endcode
 *
 *  Traversing a list from tail to head. Note: thread-safety calls are
 *  not shown here.
 *  @code
 *  List_List list;
 *  List_Elem *temp;
 *
 *  for (temp = List_tail(&list); temp != NULL; temp = List_prev(temp)) {
 *       printf("address = 0x%x\n", temp);
 *  }
 *  @endcode
 *
 *  ============================================================================
 */

#ifndef ti_drivers_utils_List__include
#define ti_drivers_utils_List__include

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 链表节点元素，必须作为用户结构体的第一个字段嵌入 */
typedef struct List_Elem_ {
    struct List_Elem_ *next; /* 指向下一个节点 */
    struct List_Elem_ *prev; /* 指向上一个节点 */
} List_Elem;

/* 链表头结构，持有链表的头尾指针，由驱动自行分配 */
typedef struct {
    List_Elem *head; /* 指向链表第一个元素 */
    List_Elem *tail; /* 指向链表最后一个元素 */
} List_List;

/*!
 *  @brief  Function to initialize the contents of a List_List
 *          初始化链表，将头尾指针清空
 *
 *  @param  list Pointer to a List_List structure that will be used to
 *               maintain a linked list
 *               指向待初始化的链表头结构体
 */
extern void List_clearList(List_List *list);

/*!
 *  @brief  Function to test whether a linked list is empty
 *          判断链表是否为空
 *
 *  @param  list A pointer to a linked list
 *               指向链表头结构体
 *
 *  @return true if empty, false if not empty
 *          链表为空返回 true，否则返回 false
 */
static inline bool List_empty(List_List *list)
{
    return (list->head == NULL);
}

/*!
 *  @brief  Function to atomically get the first elem in a linked list
 *          原子操作：从链表头部取出并移除第一个元素
 *
 *  @param  list A pointer to a linked list
 *               指向链表头结构体
 *
 *  @return Pointer the first elem in the linked list or NULL if empty
 *          指向取出的元素，链表为空时返回 NULL
 */
extern List_Elem *List_get(List_List *list);

/*!
 *  @brief  Function to return the head of a linked list
 *          获取链表头部元素（不移除），常用于链表遍历
 *
 *  This function does not remove the head, it simply returns a pointer to
 *  it. This function is typically used when traversing a linked list.
 *
 *  @param  list A pointer to the linked list
 *               指向链表头结构体
 *
 *  @return Pointer to the first elem in the linked list or NULL if empty
 *          指向链表第一个元素，链表为空时返回 NULL
 */
static inline List_Elem *List_head(List_List *list)
{
    return (list->head);
}

/*!
 *  @brief  Function to insert an elem into a linked list
 *          在链表中指定元素之前插入新元素
 *
 *  @param  list A pointer to the linked list
 *               指向链表头结构体
 *
 *  @param  newElem New elem to insert
 *                  待插入的新元素
 *
 *  @param  curElem Elem to insert the newElem in front of.
 *          This value cannot be NULL.
 *                  新元素将插入到此元素之前，不可为 NULL
 */
extern void List_insert(List_List *list, List_Elem *newElem,
                         List_Elem *curElem);

/*!
 *  @brief  Function to return the next elem in a linked list
 *          获取链表中当前元素的下一个元素（不移除），用于正向遍历
 *
 *  This function does not remove the elem, it simply returns a pointer to
 *  next one. This function is typically used when traversing a linked list.
 *
 *  @param  elem Elem in the list
 *               当前链表元素
 *
 *  @return Pointer to the next elem in linked list or NULL if at the end
 *          指向下一个元素，已到末尾时返回 NULL
 */
static inline List_Elem *List_next(List_Elem *elem)
{
    return (elem->next);
}

/*!
 *  @brief  Function to return the prev elem in a linked list
 *          获取链表中当前元素的上一个元素（不移除），用于反向遍历
 *
 *  This function does not remove the elem, it simply returns a pointer to
 *  prev one. This function is typically used when traversing a linked list.
 *
 *  @param  elem Elem in the list
 *               当前链表元素
 *
 *  @return Pointer to the prev elem in linked list or NULL if at the beginning
 *          指向上一个元素，已在头部时返回 NULL
 */
static inline List_Elem *List_prev(List_Elem *elem)
{
    return (elem->prev);
}

/*!
 *  @brief  Function to atomically put an elem onto the end of a linked list
 *          原子操作：将元素追加到链表尾部（FIFO 入队）
 *
 *  @param  list A pointer to the linked list
 *               指向链表头结构体
 *
 *  @param  elem Element to place onto the end of the linked list
 *               待追加到链表尾部的元素
 */
extern void List_put(List_List *list, List_Elem *elem);

/*!
 *  @brief  Function to atomically put an elem onto the head of a linked list
 *          原子操作：将元素插入到链表头部（LIFO 入栈）
 *
 *  @param  list A pointer to the linked list
 *               指向链表头结构体
 *
 *  @param  elem Element to place onto the beginning of the linked list
 *               待插入到链表头部的元素
 */
extern void List_putHead(List_List *list, List_Elem *elem);

/*!
 *  @brief  Function to remove an elem from a linked list
 *          从链表中移除指定元素
 *
 *  @param  list A pointer to the linked list
 *               指向链表头结构体
 *
 *  @param  elem Element to be removed from a linked list
 *               待从链表中移除的元素
 */
extern void List_remove(List_List *list, List_Elem *elem);

/*!
 *  @brief  Function to return the tail of a linked list
 *          获取链表尾部元素（不移除），常用于反向遍历
 *
 *  This function does not remove the tail, it simply returns a pointer to
 *  it. This function is typically used when traversing a linked list.
 *
 *  @param  list A pointer to the linked list
 *               指向链表头结构体
 *
 *  @return Pointer to the last elem in the linked list or NULL if empty
 *          指向链表最后一个元素，链表为空时返回 NULL
 */
static inline List_Elem *List_tail(List_List *list)
{
    return (list->tail);
}

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_utils_List__include */
