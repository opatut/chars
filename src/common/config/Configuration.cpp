#include "Configuration.hpp"

#include <boost/lexical_cast.hpp>

Configuration::Configuration() {
    SetName("");
    SetParent(NULL);
}

Configuration::Configuration(const std::string& file) {
    SetName("");
    SetFile(file);
    SetParent(NULL);
}

bool Configuration::Load() {
    std::ifstream fin(GetFile());
    YAML::Parser parser(fin);
    YAML::Node doc;
    if(!parser.GetNextDocument(doc))
        return false;
    ParseNode(doc, "");
    return true;
}


bool Configuration::Save() {
    YAML::Emitter emitter;
    SaveNode(emitter);
    std::ofstream fout(GetFile());
    fout << emitter.c_str();
    fout.close();

    return true;
}

void Configuration::ParseNode(const YAML::Node& node, const std::string& path) {
    ConfigurationNode* n = this;
    if(path != "") {
        // get string component
        std::string psub = path.substr(0, path.find_last_of('.'));
        ConfigurationNode* p = GetSubnode( psub );
        if (p == NULL) p = this;

        int l = path.find_last_of('.');
        std::string newname = (l == -1 ? path : path.substr(l + 1));
        n = new ConfigurationNode(newname);
        n->SetParent(p);
        p->AttachNode(*n);
        n = GetSubnode(path);
    }

    n->SetIsSequence(false);
    if(node.Type() == YAML::NodeType::Map) {
        for(YAML::Iterator iter = node.begin(); iter != node.end(); ++iter) {
            std::string key;
            iter.first() >> key;
            ParseNode(iter.second(), path + (path!=""?".":"") + key);
        }
    } else if(node.Type() == YAML::NodeType::Sequence) {
        int i = 0;
        n->SetIsSequence(true);
        for(YAML::Iterator iter = node.begin(); iter != node.end(); ++iter) {
            ParseNode(*iter, path + "." + boost::lexical_cast<std::string>(i));
            ++i;
        }
    } else if(node.Type() == YAML::NodeType::Scalar) {
        std::string v;
        node >> v;
        n->SetValueString(v);
    }
}

void Configuration::SaveNode(YAML::Emitter& out, const std::string& path) {
    ConfigurationNode* n = GetSubnode(path);
    if(n == NULL) return;

    if(n->GetValueString() != "") {
        // value type
        if(n->GetParent() != NULL && !n->GetParent()->IsSequence())
            out << YAML::Key << n->GetName() << YAML::Value;
        out << n->GetValueString();
    } else if(n->IsSequence()){
        if(path!="") out << YAML::Key << n->GetName() << YAML::Value;
        out << YAML::BeginSeq;
        for(auto iter = n->GetSubnodes().begin(); iter != n->GetSubnodes().end(); ++iter ) {
            SaveNode(out, path + (path==""?"":".") + iter->second.GetName() );
        }
        out << YAML::EndSeq;
    } else {
        if(path!="") out << YAML::Key << n->GetName() << YAML::Value;
        out << YAML::BeginMap;
        for(auto iter = n->GetSubnodes().begin(); iter != n->GetSubnodes().end(); ++iter ) {
            ConfigurationNode& node = iter->second;
            SaveNode(out, path + (path==""?"":".") + node.GetName() );
        }
        out << YAML::EndMap;
    }
}

void Configuration::SetFile(const std::string& file) {
    mFile = file;
}

const std::string& Configuration::GetFile() const {
    return mFile;
}
