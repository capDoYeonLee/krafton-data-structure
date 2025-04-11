//////////////////////////////////////////////////////////////////////////////////

/* CE1007/CZ1007 Data Structures
Lab Test: Section A - Linked List Questions
Purpose: Implementing the required functions for Question 3 */

//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////

typedef struct _listnode // listtnode 구조체
{
	int item;                 // 저장할 데이터 (4 byte)
	struct _listnode *next;   // 다음 노드를 가리키는 포인터 (8 byte) 
} ListNode;			          // struct _listnode 구조체의 Alias (변경 금지)

typedef struct _linkedlist    // linkedlist 구조체
{
	int size;                 // 연결리스트에 들어있는 노드의 개수
	ListNode *head;           // 연결리스트의 헤드(첫 번째 노드)를 가리키는 포인터
} LinkedList;			      // struct _linkedlist 구조체의 Alias (변경 금지)


//////////////////////// function prototypes /////////////////////////////////////

// You should not change the prototype of this function
void moveOddItemsToBack(LinkedList *ll); // ⚡️ 홀수인 노드를 모두 뒤로 이동

void printList(LinkedList *ll); // 링크드 리스트 출력
void removeAllItems(LinkedList *ll); // 모든 노드 삭제
ListNode * findNode(LinkedList *ll, int index); // 특정 노드 탐색
int insertNode(LinkedList *ll, int index, int value); // 특정 노드 삽입
int removeNode(LinkedList *ll, int index); // 특정 노드 삭제

//////////////////////////// main() //////////////////////////////////////////////

int main()
{
	LinkedList ll; // 링크드 리스트 선언
	int c, i, j; // c: 기능 선택 변수, i: 링크드리스트에 넣을 값, j: 
	c = 1;
	// 링크드 리스트 1개 초기화
	ll.head = NULL;
	ll.size = 0;

	printf("1: Insert an integer to the linked list:\n");
	printf("2: Move all odd integers to the back of the linked list:\n");
	printf("0: Quit:\n");

	while (c != 0)
	{
		printf("Please input your choice(1/2/0): ");
		scanf("%d", &c);

		switch (c)
		{
		case 1:
			printf("Input an integer that you want to add to the linked list: ");
			scanf("%d", &i);
			j = insertNode(&ll, ll.size, i);
			printf("The resulting linked list is: ");
			printList(&ll);
			break;
		case 2:
			moveOddItemsToBack(&ll); // You need to code this function
			printf("The resulting linked list after moving odd integers to the back of the linked list is: ");
			printList(&ll);
			removeAllItems(&ll);
			break;
		case 0:
			removeAllItems(&ll);
			break;
		default:
			printf("Choice unknown;\n");
			break;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////

void moveOddItemsToBack(LinkedList *ll)
{
	// 노드를 하나씩 순회하면서 홀수이면 뒤로 보내기

	// ListNode *pre, *cur;
	int index = 0;
	int originalSize = ll->size;

	if (ll == NULL || ll->head == NULL)
		return;
	
	while (index < originalSize) {
		ListNode *node = findNode(ll, index); // findNode로 현재 노드 찾기 -> 포인터로 관리하면 좋을 것 같은데
		if (node == NULL) break;

		// 현재 노드(cur)의 값이 홀수라면 
		if (node->item % 2 != 0) {
			// 현재 노드 삭제: removeNode(cur) // 삭제 안 하는 방법 찾아보기
			// 리스트 맨 뒤에 현재 노드 삽입: insertNode(cur)
			int value = node->item;
			removeNode(ll, index); // 현재 노드를 삭제하고 이전 노드의 next가 다음 노드를 바라보도록 
			insertNode(ll, ll->size, value);
		} else {
			index++; // 현재 노드(cur)의 값이 짝수라면 다음으로 건너뛰기
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////

void printList(LinkedList *ll){
	// 링크드 리스트 출력

	ListNode *cur;
	if (ll == NULL)
		return;
	cur = ll->head;

	if (cur == NULL)
		printf("Empty");
	while (cur != NULL)
	{
		printf("%d ", cur->item); // 현재 노드의 데이터 출력
		cur = cur->next; // 현재 노드의 포인터를 다음 노드로 이동
	}
	printf("\n");
}


void removeAllItems(LinkedList *ll){
	// 모든 노드 삭제

	ListNode *cur = ll->head; // 현재 노드에 링크드 리스트의 헤드를 할당
	ListNode *tmp; // 임시 노드

	while (cur != NULL){
		tmp = cur->next; // 임시 노드에 현재 노드의 다음 노드 할당
		free(cur); // 현재 노드 삭제
		cur = tmp; // 현재 노드에 임시 노드 할당
	}
	ll->head = NULL;
	ll->size = 0;
}


ListNode *findNode(LinkedList *ll, int index){
	// 특정 노드 탐색

	ListNode *temp;

	if (ll == NULL || index < 0 || index >= ll->size) // 링크드 리스트가 존재하지 않거나 인덱스가 정상 범주가 아닌경우 종료
		return NULL; // Q: 왜 0을 리턴하지 않고 NULL을 리턴하지?

	temp = ll->head;

	if (temp == NULL || index < 0)
		return NULL;

	while (index > 0){
		temp = temp->next;
		if (temp == NULL)
			return NULL;
		index--;
	}

	return temp;
}

int insertNode(LinkedList *ll, int index, int value){
	// 새로운 노드를 링크드 리스트의 원하는 위치에 삽입
	
	ListNode *pre, *cur; // 이전 노드, 현재 노드

	if (ll == NULL || index < 0 || index > ll->size + 1)
		return -1; // 왜 -1을 리턴? 

	// If empty list or inserting first node, need to update head pointer
	if (ll->head == NULL || index == 0){ // 헤드가 없거나 인덱스가 0일 때
		cur = ll->head;
		ll->head = malloc(sizeof(ListNode));
		ll->head->item = value;
		ll->head->next = cur;
		ll->size++;
		return 0;
	}


	// Find the nodes before and at the target position
	// Create a new node and reconnect the links
	if ((pre = findNode(ll, index - 1)) != NULL){ // 원하는 인덱스에 해당하는 노드의 이전 노드가 있을 때
		cur = pre->next; // 이전 노드의 next를 현재노드로 
		pre->next = malloc(sizeof(ListNode)); // 이전 노드의 next를 위해 ListNode의 size만큼의 메모리 할당
		pre->next->item = value; // 이전 노드의 next의 item에 파라미터로 받은 value 할당
		pre->next->next = cur;
		ll->size++;
		return 0;
	}

	return -1;
}


int removeNode(LinkedList *ll, int index){
	// 특정 노드 삭제

	ListNode *pre, *cur;

	// Highest index we can remove is size-1
	if (ll == NULL || index < 0 || index >= ll->size)
		return -1;

	// If removing first node, need to update head pointer
	if (index == 0){
		cur = ll->head->next;
		free(ll->head);
		ll->head = cur;
		ll->size--;

		return 0;
	}

	// Find the nodes before and after the target position
	// Free the target node and reconnect the links
	if ((pre = findNode(ll, index - 1)) != NULL){

		if (pre->next == NULL)
			return -1;

		cur = pre->next; // 현재 노드를 이전 노드의 next로
		pre->next = cur->next; // 이전 노드의 next를 현재 노드의 next로
		free(cur); // 현재 노드 메모리 할당 해제
		ll->size--;
		return 0;
	}

	return -1;
}
