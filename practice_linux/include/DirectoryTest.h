
class CDirectoryTest
{
public:
	CDirectoryTest();
	~CDirectoryTest();
	
	void	Stat				(int argc, const char* argv[]);
	void	ChangePermission	(int argc, const char* argv[]);
	void	ChangeOwnership		(int argc, const char* argv[]);	
	void	ChangeOwnershipRoot	(int argc, const char* argv[]);	// with 'sudo'
	
	void	GetCurrentDir			();
	void	ChangeCurrentDir		(int argc, const char* argv[]);
	void	ChangeCurrentDirResotore(int argc, const char* argv[]);
	
	void	MakeDir					(int argc, const char* argv[]);
	void	RemoveDir				(int argc, const char* argv[]);
	void	DirStream				(int argc, const char* argv[]);
	
	void	MakeHardLink			(int argc, const char* argv[]);
	void	MakeSymbolicLink		(int argc, const char* argv[]);	
	void	DoUnLink				(int argc, const char* argv[]);	
	
	void	MoveFile				(int argc, const char* argv[]);
	
	void	TestInotify				(int argc, const char* argv[]);
};
