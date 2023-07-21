#include <iostream>
#include <string>
#include <memory>
#include <map>

class P
{
public:
    typedef std::shared_ptr<P> ptr;
    P(std::string name) : m_name(name) {}

    virtual std::string toString() const = 0;

protected:
    std::string m_name;
};

template <class T>
class C : public P
{
public:
    typedef std::shared_ptr<C> ptr;
    C(std::string name, T t) : P(name), m_val(t) {}

    std::string toString() const override
    {
        return this->m_name;
    }

    T getVal()
    {
        return this->m_val;
    }

private:
    T m_val;
};

class Tclass
{
public:
    typedef std::map<int, P::ptr> newMap;

    template <class T>
    static typename C<T>::ptr
    LookUp(const std::string &name,
           const T &default_value)
    {
        newMap m;
        m[1] = std::make_shared<C<int>>(std::to_string(1), 1);
        return std::dynamic_pointer_cast<C<int>>(m[1]);
    }
};

main()
{
    C<int>::ptr c = Tclass::LookUp("1", (int)1);
    c->getVal();
}