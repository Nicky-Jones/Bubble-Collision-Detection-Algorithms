
class Application
{
protected:
    int height;
    int width;
	float nRange;
	float timeinterval;
public:
    virtual void initGraphics();
    virtual void display();
	virtual void update();
    virtual void resize(int width, int height);
	int getheight();
	int getwidth();
	float getTimeinterval();
	void setTimeinterval(float timeint);
	virtual void keyboard(unsigned char key, int x, int y);
 };



    

