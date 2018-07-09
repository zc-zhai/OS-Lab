#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

//memory block struction
typedef struct map
{
	unsigned m_size;
	unsigned m_addr;
	struct map *next, *prior;
}mp, *mps;

mps coremap;	//defined as the first block

mps current_map;	//defined as the current block

//initialize the memory
void init_map(void)
{
	//decide the size of each block randomly
	int coresize[20] = { 200, 1800, 1300, 700, 1100, 900, 1600, 400, 1200, 800,
		100, 1900, 300, 1700, 1500, 500, 600, 1400, 1000, 66 };
	int coreaddress[20];
	int i;
	for (i = 0; i<20; i++)
	{
		coreaddress[i] = 2000 * (i + 1);
	}

	//form chain
	mps pHead = (mps)malloc(sizeof(mp));
	mps pTail = NULL, p_new = NULL;

	pHead->m_size = coresize[0];
	pHead->m_addr = coreaddress[0];
	pHead->prior = NULL;
	pHead->next = NULL;
	pTail = pHead;
	for (i = 1; i<20; i++)
	{
		p_new = (mps)malloc(sizeof(mp));

		p_new->m_size = coresize[i];
		p_new->m_addr = coreaddress[i];
		p_new->next = NULL;
		p_new->prior = pTail;
		pTail->next = p_new;
		pTail = p_new;
	}
	pTail->next = pHead;
	pHead->prior = pTail;

	coremap = pHead;
}


//print info of the memory block
void print(void)
{
	mps tmp1, tmp2;

	tmp1 = coremap->prior;

	for (tmp2 = coremap; tmp2 != tmp1; tmp2 = tmp2->next)
	{
		printf("address: %d,size: %d\n", tmp2->m_addr, tmp2->m_size);
	}
	printf("address: %d,size: %d\n", tmp1->m_addr, tmp1->m_size);
}


//allocate memory space
unsigned lmalloc(unsigned size)
{
	unsigned malloc_addr, current_addr;
	mps bp;

	if (current_map == NULL)
		current_map = coremap;

	current_addr = current_map->m_addr;

	bp = current_map;
	if (bp->m_size >= size)
	{
		malloc_addr = bp->m_addr;
		bp->m_addr += size;
		if ((bp->m_size -= size) == 0)
		{
			bp->prior->next = bp->next;
			bp->next->prior = bp->prior;
			current_map = bp->next;
			free(bp);
		}
		else
			current_map = bp;
		return (malloc_addr);
	}

	for (bp = current_map->next; bp->m_addr != current_addr; bp = bp->next)	//search for the whole space
	{
		if (bp->m_size >= size)
		{
			malloc_addr = bp->m_addr;
			bp->m_addr += size;
			if ((bp->m_size -= size) == 0)
			{
				bp->prior->next = bp->next;
				bp->next->prior = bp->prior;
				current_map = bp->next;
				free(bp);
			}
			else
				current_map = bp;
			return (malloc_addr);
		}
	}
	return 0;
}

//free memory space
void lmfree(unsigned size, unsigned free_addr)
{
	mps bp, new_map;

	unsigned addr;
	addr = free_addr;

	//search for two adjacent blocks between which free_addr is
	for (bp = coremap; !((bp->m_addr > addr && bp->prior->m_addr < addr) ||
		(bp->m_addr < addr && bp->prior->m_addr < addr && bp->m_addr < bp->prior->m_addr) ||
		(bp->m_addr > addr && bp->prior->m_addr > addr && bp->m_addr < bp->prior->m_addr)); bp = bp->next);

	if (bp->prior->m_addr + bp->prior->m_size == addr) // case 1&2
	{
		bp->prior->m_size += size;  // case 1
		if (addr + size == bp->m_addr)  // case 2
		{
			bp->prior->m_size += bp->m_size;
			bp->prior->next = bp->next;
			bp->next->prior = bp->prior;
			//free(bp);
		}
	}
	else if (addr + size == bp->m_addr) // case 3
	{
		bp->m_addr -= size;
		bp->m_size += size;
	}
	//case 4
	else if ((addr + size < bp->m_addr && bp->prior->m_addr + bp->prior->m_size < addr) ||
		(addr + size > bp->m_addr && bp->prior->m_addr + bp->prior->m_size < addr && bp->m_addr < bp->prior->m_addr) ||
		(addr + size < bp->m_addr && bp->prior->m_addr + bp->prior->m_size > addr && bp->m_addr < bp->prior->m_addr))
	{
		new_map = (mps)malloc(sizeof(mp));
		new_map->m_size = size;
		new_map->m_addr = free_addr;
		new_map->next = bp;
		new_map->prior = bp->prior;
		bp->prior->next = new_map;
		bp->prior = new_map;
	}
	else printf("Memory release error!!!\n");
}

void main()
{
	char op;
	unsigned arg1, arg2;
	unsigned allo_addr; //defined as the allocated address

	init_map();
	printf("Current info of the memory space:\n");
	print();

	while (1)
	{
		printf("Choose the operation(m[alloc]/f[ree]/p[rint]/q[uit]) >>> ");
		do
		op = getchar();
		while (op == '\n' || op == '\t' || op == ' ');
		if (op == 'm')
		{
			printf("Please input the argument(size) >>> ");
			scanf("%u", &arg1);
			allo_addr = lmalloc(arg1);
			if (allo_addr != 0)
				printf("The address is %u\n", allo_addr);
			else printf("Allocation false!\n");
		}
		else if (op == 'f')
		{
			printf("Please input the arguments(size && address) >>> ");
			scanf("%u%u", &arg1, &arg2);
			lmfree(arg1, arg2);
		}
		else if (op == 'p') print();
		else if (op == 'q') break;
		else printf("Check your input!!!\n");
	}
}



