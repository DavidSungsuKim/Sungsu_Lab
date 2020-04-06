
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
};
