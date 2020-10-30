
#ifdef __cplusplus
extern "C" {
#endif

class CTest
{
public:
  CTest() {};
  ~CTest() {};
  
  void Function();
  
private:
  int x;
};
  
class CBall
{
public:
  CBall() { x = 0; };
  ~CBall() {};
  
  virtual void Throw();
  
private:
  int x;
};

class CBaseball : public CBall
{
public:
  CBaseball() {};
  ~CBaseball() {};
  
  void Throw();
  
private :
  int x;
};

#ifdef __cplusplus
}
#endif