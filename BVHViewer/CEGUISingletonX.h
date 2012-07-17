#ifndef _CEGUI_SINGLETON_X_H_
#define _CEGUI_SINGLETON_X_H_
 
#if defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable : 4786)
#endif
 
namespace CEGUI
{
    /*************************************************************************
        Class: CEGUISingletonX
        Alternative for the existing CEGUI Singleton class. This one doesn't
        require the Singleton to be explicitly instantiated by 'new'.
    *************************************************************************/
    template<class T>
    class CEGUISingletonX
    {
        private:
            class _InstPtr
            {
                public:
                    _InstPtr() : m_ptr(0) {}
                    ~_InstPtr() { delete m_ptr; }
                    T* get() { return m_ptr; }
                    void Set(T* p)
                    {
                        if(p!= 0)
                        {
                            delete m_ptr;
                            m_ptr = p;
                        }
                    }
                private:
                    T* m_ptr;
            };
 
            static _InstPtr sm_ptr;
            CEGUISingletonX();
            CEGUISingletonX(const CEGUISingletonX&);
            CEGUISingletonX& operator=(const CEGUISingletonX&);
 
        public:
            static T& getSingleton()
            {
                if(sm_ptr.get() == 0)
                {
                    sm_ptr.Set(new T());
                }
                return *sm_ptr.get();
            }
 
            static T* getSingletonPtr()
            {
                if(sm_ptr.get() == 0)
                {
                    sm_ptr.Set(new T());
                }
                return sm_ptr.get();
            }
    };
}
#endif