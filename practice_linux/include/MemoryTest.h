
#ifndef __MEMORY_TEST_H__
#define __MEMORY_TEST_H__

class CMemoryTest
{
public:
	CMemoryTest();
	~CMemoryTest();
	
	void 	AllocDynamicMemory	();
	void	AlignMem			();
	
	void	DataSegment			();
	void	AnonymousMapping	();
	void	UseDevZeroObsolete	();
	
	void	AdvancedMemoryAlloc	();
	void	AllocBasedOnStack	( int a );
	void	ManipulateMem		();
};

#endif
