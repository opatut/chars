#ifndef CONFIG_CONFIGURATIONNODE_HPP
#define CONFIG_CONFIGURATIONNODE_HPP

#include <string>

#include <boost/ptr_container/ptr_map.hpp>

class ConfigurationNode {
public:
    ConfigurationNode();
    ConfigurationNode(const std::string& name);

    void AttachNode(ConfigurationNode node);

    ConfigurationNode* GetSubnode(const std::string& path);
    std::map<std::string, ConfigurationNode>& GetSubnodes();
    ConfigurationNode* GetParent();
    ConfigurationNode* GetRootNode();
    void SetParent(ConfigurationNode* parent);

    void SetValueString(const std::string& value);
    void SetValueInt(int value);
    void SetValueFloat(float value);
    void SetValueBool(bool value);

    const std::string& GetName();
    const std::string& GetValueString(const std::string& path = "", const std::string& def = "");
    int GetValueInt(const std::string& path = "", int def = 0);
    float GetValueFloat(const std::string& path = "", float def = 0.f);
    bool GetValueBool(const std::string& path = "", bool def = false);

    void SetName(const std::string& name);

    void SetIsSequence(bool is);
    bool IsSequence() const;
protected:
    bool mIsSequence;
    std::map<std::string, ConfigurationNode> mSubnodes;
    std::string mValue;
    std::string mName;
    ConfigurationNode* mParent;
};

#endif
