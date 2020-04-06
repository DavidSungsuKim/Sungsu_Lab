
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
	
	void	GetFileINode			(int argc, const char* argv[]);
	void	SortFilebyPhysicalBlock	(int argc, const char* argv[]);	// with 'sudo'
		
protected:
	// For SortFilebyPhysicalBlock()
	void	PrintBlocks				(int fd);
	int		GetBlock				(int fd, int logical_block);
	int		GetNrBlocks				(int fd);
};
