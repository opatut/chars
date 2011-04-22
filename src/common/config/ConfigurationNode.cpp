#include "ConfigurationNode.hpp"

#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "common/log/Logger.hpp"

ConfigurationNode::ConfigurationNode() {
    SetParent(NULL);
    SetName("");
}

ConfigurationNode::ConfigurationNode(const std::string& name) {
    SetName(name);
    SetParent(NULL);
}

void ConfigurationNode::AttachNode(ConfigurationNode node) {
    mSubnodes[node.GetName()] = node;
}

ConfigurationNode* ConfigurationNode::GetSubnode(const std::string& path) {

    std::vector<std::string> splits;
    if(path != "") {
        std::string p = boost::algorithm::trim_copy(path);
        boost::split(splits, p, boost::is_any_of("."));
    }

    if(splits.size() > 0 && splits[0] != "") {
        if(mSubnodes.find(splits[0]) != mSubnodes.end()) {
            // find sub-subnode
            int l = splits.at(0).size();
            if(l < path.length() - 1) l += 1;
            std::string subpath = path.substr( l );
            return mSubnodes[splits[0]].GetSubnode(subpath);
        } else {
            // invalid path!
            return NULL;
        }
    } else {
        return this;
    }
}

std::map<std::string, ConfigurationNode>& ConfigurationNode::GetSubnodes() {
    return mSubnodes;
}

ConfigurationNode* ConfigurationNode::GetParent() {
    if(mParent == NULL)
        Logger::GetLogger().Warning("ConfigurationNode \"" + GetName()+ "\" has no parent.");
    return mParent;
}

ConfigurationNode* ConfigurationNode::GetRootNode() {
    if(mParent == NULL)
        return this;
    else
        return mParent->GetRootNode();
}

void ConfigurationNode::SetParent(ConfigurationNode* parent) {
    mParent = parent;
}

void ConfigurationNode::SetName(const std::string& name) {
    mName = name;
}

void ConfigurationNode::SetValueString(const std::string& value) {
    mValue = value;
}

void ConfigurationNode::SetValueInt(int value) {
    SetValueString(boost::lexical_cast<std::string>(value));
}

void ConfigurationNode::SetValueFloat(float value) {
    SetValueString(boost::lexical_cast<std::string>(value));
}

void ConfigurationNode::SetValueBool(bool value) {
    SetValueString(boost::lexical_cast<std::string>(value));
}

const std::string& ConfigurationNode::GetName() {
    return mName;
}

const std::string& ConfigurationNode::GetValueString(const std::string& path, const std::string& def) {
    if(path == "") {
        return mValue;
    } else {
        ConfigurationNode* node = GetSubnode(path);
        if(node == NULL) {
            node = GetRootNode()->GetSubnode(path);
        }
        if(node == NULL) {
            Logger::GetLogger().Debug("Could not find node at path " + path + " to get value from. Returning default value: " + def);
            return def;
        }
        else {
            return node->GetValueString();
        }
    }
}

int ConfigurationNode::GetValueInt(const std::string& path, int def) {
    return boost::lexical_cast<int>(GetValueString(path, boost::lexical_cast<std::string>(def)));
}

float ConfigurationNode::GetValueFloat(const std::string& path, float def) {
    return boost::lexical_cast<float>(GetValueString(path, boost::lexical_cast<std::string>(def)));
}

bool ConfigurationNode::GetValueBool(const std::string& path, bool def) {
    return boost::lexical_cast<bool>(GetValueString(path, boost::lexical_cast<std::string>(def)));
}

void ConfigurationNode::SetIsSequence(bool is) {
    mIsSequence = is;
}

bool ConfigurationNode::IsSequence() const {
    return mIsSequence;
}
