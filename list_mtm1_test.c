/*
 * list_mtm1_test.c
 *
 *  Created on: May 7, 2015
 *      Author: or
 */
#include <stdio.h>
#include <stdlib.h>
#include "list_mtm1.h"
#include "TestsMacros.h"
#define LIMIT 15
#define ERROR -1
//list of int
void freeInt(ListElement element) {
	free(element);
}

ListElement copyInt(ListElement element) {
	int* copy = malloc(sizeof(int));
	if (!(copy)) {
		return NULL;
	}
	*copy = *(int*) element;
	return copy;
}

ListElement badCopy(ListElement element) {
	return NULL;
}
/*
 *
 * tests
 */

static bool listCreateTest() {
	bool result = true;
	TEST_EQUALS(result, listCreate(NULL,NULL), NULL);
	TEST_EQUALS(result, listCreate(NULL,freeInt), NULL);
	TEST_EQUALS(result, listCreate(copyInt,NULL), NULL);
	List newList = listCreate(copyInt, freeInt);
	TEST_DIFFERENT(result, newList, NULL);

	listDestroy(newList);
	return result;
}

static bool listCopyTest() {
	bool result = true;
	List newList = listCreate(copyInt, freeInt);
	TEST_DIFFERENT(result, newList, NULL);

	List copiedList = listCopy(NULL);
	TEST_EQUALS(result, copiedList, NULL);

	copiedList = listCopy(newList);
	TEST_DIFFERENT(result, copiedList, NULL);
	TEST_DIFFERENT(result, newList, copiedList);
	TEST_EQUALS(result, listGetSize(newList), listGetSize(copiedList));

	TEST_EQUALS(result, listInsertFirst(newList, 1), LIST_SUCCESS);
	TEST_EQUALS(result, listInsertLast(newList, 2), LIST_SUCCESS);
	TEST_EQUALS(result, listInsertLast(newList, 3), LIST_SUCCESS);
	TEST_EQUALS(result, listInsertLast(newList, 4), LIST_SUCCESS);

	TEST_EQUALS(result, listGetFirst(copiedList), 1);
	TEST_EQUALS(result, listGetNext(copiedList), 2);
	TEST_EQUALS(result, listGetNext(copiedList), 3);
	TEST_EQUALS(result, listGetNext(copiedList), 4);
	TEST_EQUALS(result, listGetNext(copiedList), NULL);

	TEST_EQUALS(result, listGetFirst(copiedList), 1);
	TEST_EQUALS(result, listGetFirst(newList), 1);
	TEST_EQUALS(result, listGetNext(copiedList), 2);
	TEST_DIFFERENT(result, listGetCurrent(copiedList), listGetCurrent(newList));

	TEST_EQUALS(result, listInsertLast(newList, 5), LIST_SUCCESS);
	TEST_EQUALS(result, listGetSize(newList), 5);
	TEST_EQUALS(result, listGetSize(copiedList), 4);

	listDesrtoy(newList);
	listDestroy(copiedList);

	return result;
}

static bool listClearTest() {
	bool result = true;
	List newList = listCreate(copyInt, freeInt);
	TEST_DIFFERENT(result, newList, NULL);
	for (int i = 0; i < LIMIT; ++i) {
		TEST_EQUALS(result, listInsertLast(newList, i), LIST_SUCCESS);
	}
	List copy = listCopy(newList);
	TEST_EQUALS(result, listGetSize(NULL), ERROR);
	TEST_EQUALS(result, listGetSize(newList), LIMIT);
	TEST_EQUALS(result, listClear(newList), LIST_SUCCESS);
	TEST_EQUALS(result, listGetSize(newList), 0);
	TEST_EQUALS(result, listGetSize(copy), LIMIT);

	listDestroy(newList);
	listDestroy(copy);
	return result;
}

static bool listGetFirstTest() {
	bool result = true;
	List newList = listCreate(copyInt, freeInt);
	TEST_DIFFERENT(result, newList, NULL);

	TEST_EQUALS(result, listGetFirst(NULL), NULL);
	TEST_EQUALS(result, listGetFirst(newList), NULL);

	for (int i = 0; i < LIMIT; ++i) {
		TEST_EQUALS(result, listInsertFirst(newList, i), LIST_SUCCESS);
		TEST_EQUALS(result, listGetFirst(newList), i);
	}

	List copy = listCopy(newList);
	TEST_EQUALS(result, listGetFirst(copy), listGetCurrent(copy));
	TEST_EQUALS(result, listGetNext(copy), 1);
	TEST_DIFFERENT(result, listGetCurrent(newList), listGetCurrent(copy));

	for (int i = LIMIT - 1; i > 0; --i) {
		TEST_EQUALS(result, listGetNext(newList), i - 1)
	}

	listDestroy(newList);
	listDestroy(copy);
	return result;
}

static bool listInsertFirstTest() {
	bool result = true;
	List badList = listCreate(badCopy, freeInt);
	TEST_DIFFERENT(result, badList, NULL);
	TEST_EQUALS(result, listInsertFirst(NULL,0), NULL);

	TEST_EQUALS(result, listInsertFirst(badList, 3), LIST_OUT_OF_MEMORY);
	listDestroy(badList);

	List newList = listCreate(copyInt, freeInt);
	TEST_DIFFERENT(result, newList, NULL);

	for (int i = 0; i < LIMIT; ++i) {
		TEST_EQUALS(result, listInsertFirst(newList, i), LIST_SUCCESS);
		TEST_EQUALS(result, listGetFirst(newList), i);
	}

	TEST_EQUALS(result, listGetFirst(newList), 0);
	List copy = listCopy(newList);
	TEST_EQUALS(result, listInsertFirst(newList, -1), LIST_SUCCESS);
	TEST_DIFFERENT(result, listGetSize(newList), listGetSize(copy));
	TEST_EQUALS(result, listGetCurrent(newList), listGetCurrent(copy));

	listDestroy(newList);
	listDestroy(copy);
	return result;
}

static bool listGetNextTest() {
	bool result = true;
	List newList = listCreate(copyInt, freeInt);
	TEST_DIFFERENT(result, newList, NULL);

	TEST_EQUALS(result, listGetNext(NULL), NULL);
	TEST_EQUALS(result, listGetNext(newList), NULL);

	for (int i = 0; i < LIMIT; ++i) {
		TEST_EQUALS(result, listInsertLast(newList, i), LIST_SUCCESS);
	}

	TEST_EQUALS(result, listGetFirst(newList), 0);
	for (int i = 1; i < LIMIT; ++i) {
		TEST_EQUALS(result, listGetNext(newList), i);
	}

	listDestroy(newList);
	return result;
}

static bool listGetSizeTest() {
	bool result = true;
	List newList = listCreate(copyInt, freeInt);
	TEST_DIFFERENT(result, newList, NULL);

	TEST_EQUALS(result, listGetSize(NULL), ERROR);
	TEST_EQUALS(result, listGetSize(newList), ERROR);

	for (int i = 0; i < LIMIT; ++i) {
		TEST_EQUALS(result, listInsertFirst(newList, i), LIST_SUCCESS);
		TEST_EQUALS(result, listGetSize(newList), i + 1);
	}

	TEST_EQUALS(result, listGetFirst(newList), 0);

	for (int i = 0; i < LIMIT; ++i) {
		TEST_EQUALS(result, listRemoveFirst(newList), LIST_SUCCESS);
		TEST_EQUALS(result, listGetSize(newList), LIMIT - 1 - i);
	}
	listDestroy(newList);
	return result;
}

static bool listInsertBeforeCurrentTest() {
	bool result = true;
	List newList = listCreate(copyInt, freeInt);
	TEST_DIFFERENT(result, newList, NULL);

	for (int i = 0; i <= LIMIT; ++i) {
		TEST_EQUALS(result, listInsertBeforeCurrent(newList, i), LIST_SUCCESS);
		TEST_EQUALS(result, listGetSize(newList), i + 1);
	}

	TEST_EQUALS(result, listGetFirst(newList), LIMIT);

	for (int i = LIMIT; i > 0; --i) {
		TEST_EQUALS(result, listGetNext(newList), i - 1);
	}

	TEST_EQUALS(result, listClear(newList), LIST_SUCCESS);

	TEST_EQUALS(result, listInsertFirst(newList, -1), LIST_SUCCESS);

	TEST_EQUALS(result, listGetFirst(newList), -1);
	TEST_EQUALS(result, listGetNext(newList), -1);
	for (int i = 0; i <= LIMIT; ++i) {
		TEST_EQUALS(result, listInsertBeforeCurrent(newList, i), LIST_SUCCESS);
		TEST_EQUALS(result, listGetSize(newList), i + 1 + 2);
	}

	TEST_EQUALS(result, listGetFirst(newList), -1);
	for (int i = LIMIT; i >= 0; --i) {
		TEST_EQUALS(result, listGetNext(newList), i);
	}

	listDestroy(newList);
	return result;
}

bool listRemoveCurrentTest() {
	bool result = true;
	List newList = listCreate(copyInt, freeInt);
	TEST_DIFFERENT(result, newList, NULL);

	TEST_EQUALS(result, listRemoveCurrent(NULL), LIST_NULL_ARGUMENT);
	TEST_EQUALS(result, listRemoveCurrent(newList), LIST_INVALID_CURRENT);

	for (int i = 0; i <= LIMIT; ++i) {
		TEST_EQUALS(result, listInsertBeforeCurrent(newList, i), LIST_SUCCESS);
		TEST_EQUALS(result, listGetSize(newList), i + 1);
		TEST_EQUALS(result, listGetCurrent(newList), NULL);
	}

	List copy = listCopy(newList);
	TEST_DIFFERENT(result, copy, NULL);
	int copySize = listGetSize(copy);
	TEST_EQUALS(result, listGetFirst(copy), listGetFirst(newList));
	for (int i = 0; i <= LIMIT; ++i) {
		TEST_EQUALS(result, listGetFirst(newList), i);
		TEST_EQUALS(result, listRemoveCurrent(newList), LIST_SUCCESS);
		TEST_EQUALS(result, listGetSize(newList), LIMIT);
		TEST_EQUALS(result, listGetCurrent(newList), LIST_INVALID_CURRENT);
		TEST_EQUALS(result, listGetSize(copy), copySize);
	}

	listDesrtoy(newList);
	listDestroy(copy);
	return result;
}

int main() {
	RUN_TEST(listCreateTest);
	RUN_TEST(listCopyTest);
	RUN_TEST(listClearTest);
	RUN_TEST(listGetFirstTest);
	RUN_TEST(listInsertFirstTest);
	RUN_TEST(listGetNextTest);
	RUN_TEST(listGetSizeTest);
	RUN_TEST(listInsertBeforeCurrentTest);
	return 0;
}

