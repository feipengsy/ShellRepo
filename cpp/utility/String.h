#ifndef SHELL_STRING_H
#define SHELL_STRING_H

namespace shell {

class String {

    public:
        String();
        String(const char* str);
        String(const String& str);
        ~String();

        String& operator=(String str);
        bool operator==(const String& str);

    private:
        void swap(String& other);

    private:
        char* _data;

};


}  // namespace shell


#endif
