# C++相关知识

## 1、C++用于static修饰的函数

static修饰的函数叫`做静态函数`，静态函数有两种，根据其出现的地方来分类：

如果这个静态函数出现在类里，那么它是一个静态成员函数；

一、静态成员函数的作用在于：

①调用这个函数不会访问或者修改任何对象（非static）数据成员。

其实很好理解，类的静态成员(变量和方法)属于类本身，在类加载的时候就会分配内存，

②可以通过类名直接去访问；非静态成员（变量和方法）属于类的对象，所以只有在类的对象产生（创建类的实例）时才会分配内存，然后通过类的对象（实例）去访问。

```cpp
class NotifyManager  : public QObject
{
	Q_OBJECT

public:
	NotifyManager();
	~NotifyManager();

signals:
	void signalSkinChanged(const QColor&color);
	void signalShowTitle();
public:
	static NotifyManager*getInstance();
	//通知其他窗口改变颜色
	void notifyOtherWindowChangedSkin(const QColor& color);
	
private:
	static NotifyManager* instance;
};
·······
在另外一个类中可以使用NotifyManager::getInstance()来调用函数
·······
```

二、如果它不是出现在类中，那么它是一个普通的全局的静态函数。

这样的static函数与普通函数的区别是：用static修饰的函数，限定在本源码文件中，不能被本源码文件以外的代码文件调用。而普通的函数，默认是extern的，也就是说它可以被其它代码文件调用。



