
class CFileTestAdv
{
public:
	CFileTestAdv();
	~CFileTestAdv();
	
	void 	WriteVector	();
	void 	ReadVector	();
		
	void	Epoll		();	
	
	void	MemoryMappedRead	();
	void	MemoryMappedWrite	();
	
	void	GetFileINode			(const char* pFilePath);
	void	SortFilebyPhysicalBlock	(const char* pFilePath);	// with 'sudo'
		
protected:
	// For SortFilebyPhysicalBlock()
	void	PrintBlocks				(int fd);
	int		GetBlock				(int fd, int logical_block);
	int		GetNrBlocks				(int fd);
};
