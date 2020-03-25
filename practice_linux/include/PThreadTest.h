
class CPThreadTest
{
public:
	CPThreadTest();
	~CPThreadTest();
	
	int CreateJoin	();
	
	// Callback functions for threads
	
protected:
	static int	m_retThread1;
	static int	m_retThread2;
		
	static void* ThreadProc1	(void* arg);
	static void* ThreadProc2	(void* arg);
};
