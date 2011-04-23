#include "ChatLogStream.hpp"

#include "common/log/Logger.hpp"

void ChatLogStream::SetupLogger() {

    Logger::GetLogger("Chat").GetStream("debug")->SetStream(mStream);
    Logger::GetLogger("Chat").GetStream("info")->SetStream(mStream);
    Logger::GetLogger("Chat").GetStream("warning")->SetStream(mStream);
    Logger::GetLogger("Chat").GetStream("error")->SetStream(mStream);

    Logger::GetLogger("Chat").GetStream("debug")->SetFormat("#CCCCCC%3$s");
    Logger::GetLogger("Chat").GetStream("info")->SetFormat("#FFFF00%3$s");
    Logger::GetLogger("Chat").GetStream("warning")->SetFormat("#0000FF%3$s");
    Logger::GetLogger("Chat").GetStream("error")->SetFormat("#FF0000%3$s");

}

void ChatLogStream::UpdateLog(MyGUI::Edit* edit) {
    edit->setCaption(mStream.str().c_str());
}
