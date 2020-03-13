

class CTest
{
public:
    CTest() { a = 0; };
    virtual ~CTest() {};

    virtual int Func();

private:
    int a;
};

class CTestChild : public CTest
{
public:
	CTestChild() { }
	~CTestChild() { }
	
	int Func();
};
