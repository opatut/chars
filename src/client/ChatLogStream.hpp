#ifndef CHATLOGSTREAM_HPP
#define CHATLOGSTREAM_HPP

#include <iostream>
#include <MyGUI_Edit.h>

class ChatLogStream {
public:
    void SetupLogger();
    void UpdateLog(MyGUI::Edit* edit);
private:
    std::ostringstream mStream;
};

#endif
