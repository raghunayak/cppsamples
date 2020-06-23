#include <map>
#include <memory>
#include <iostream>

using std::string;
using std::shared_ptr;

struct Base
{};

struct Message1 : public Base
{
    static string name() { return "Message1"; }
    explicit Message1(const string& d) { std::cout << "Message1: " << d; }
};

struct Message2 : public Base
{
    static string name() { return "Message2"; }
    explicit Message2(const string& d) { std::cout << "Message2: " << d; }
};

template<class T>
class MessageFactory
{
public:
    using CreateMethod = shared_ptr<T> (*) (const string&);

    MessageFactory() = delete;

    static bool registerClass(const string& name, CreateMethod func)
    {
        if (auto itr = mFuncMap.find(name); itr == mFuncMap.end()) {
            mFuncMap[name] = func;
            return true;
        }
        return false;
    }

    static shared_ptr<T> create(const string& name, const string& d)
    {
        if (auto itr = mFuncMap.find(name); itr != mFuncMap.end()) {
            return itr->second(d); // call the createFunc
        }
        return nullptr;
    }
private:
    static std::map<string, CreateMethod> mFuncMap;
};

template<class T>
std::map<string, typename MessageFactory<T>::CreateMethod> MessageFactory<T>::mFuncMap;

template<class Base, class T>
void registerClass()
{
    MessageFactory<Base>::registerClass(T::name(),
                                        [](const string& d) -> shared_ptr<Base> { return std::make_shared<Base>(T(d)); });
}

void registerClass()
{
    registerClass<Base, Message1>();
    registerClass<Base, Message2>();
}

int main(int argc, char *argv[])
{
    registerClass();

    MessageFactory<Base>::create("Message1", "Hello");
    return 0;
}
