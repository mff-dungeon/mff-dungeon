#include "WorldDumper.hpp"
#include "Objects/IDescriptable.hpp"
#include "dynamic.hpp"
#include <sstream>
#include <unistd.h>

namespace Dungeon {
	
	string WorldDumper::getTimestamp() const {
        std::time_t t = std::time(NULL);
        char mbstr[100];
        std::strftime(mbstr, sizeof(mbstr), "%y.%m.%d-%H:%M:%S", std::localtime(&t));
        return std::string(mbstr);
	}

	string DotDumper::startDump() {
		string path = "dumps/world-" + getTimestamp() + ".dot";
		unlink("dumps/world-last.dot");
		symlink(path.substr(6).c_str(), "dumps/world-last.dot");
		file.open(path.c_str());
		file << "digraph world {\n";
		return path;
	}
	
	void DotDumper::dumpObject(ObjectPointer obj) {
		stringstream ss;
		ss << "n" << objects.size();
		string node = ss.str();
		objects[obj->getId()] = node;
		string shape = "box";
		if (obj->instanceOf(Human)) {
			shape = "ellipse";
		} else if (obj->instanceOf(Alive)) {
			shape = "octagon";
		} else if (obj->instanceOf(Room)) {
			shape = "house";
		} else if (obj->instanceOf(Door)) {
			shape = "rarrow";
		}
		file << node << " [label=";
		if (obj->instanceOf(IDescriptable)) {
			IDescriptable* desc = obj.unsafeCast<IDescriptable>();
			file <<  "<<b>" << desc->getName() << "</b><br/>id: " << obj->getId() << "<br/>";
			obj->beforeStore(*this);
			obj->registerProperties(*this);
			obj->afterStore(*this);
			file << ">";
		} else {
			
			file << "\"" << obj->getId() << "\"";
		}
		file << ", shape=" + shape + "];\n";
		
		for (RelationList::value_type& relPair : obj->getRelations(Relation::Master)) {
			for (ObjectMap::value_type& objPair : relPair.second) {
				if (objects.find(objPair.first) != objects.end()) {
					file << node << " -> " << objects[objPair.first] 
					     << " [label=\"" << relPair.first << "\"]\n";
				}
			}
		}
		
		for (RelationList::value_type& relPair : obj->getRelations(Relation::Slave)) {
			for (ObjectMap::value_type& objPair : relPair.second) {
				if (objects.find(objPair.first) != objects.end()) {
					file << objects[objPair.first] << " -> " << node
					     << " [label=\"" << relPair.first << "\"]\n";
				}
			}
		}
	}
	
	IPropertyStorage& DotDumper::have(int& prop, string id, string desc, bool editable) {
		file << "<b>" << id << "</b>: " << prop << "<br/>";
		return *this;
	}
	
	IPropertyStorage& DotDumper::have(string& prop, string id, string desc, bool editable) {
		file << "<b>" << id << "</b>: " << prop << "<br/>";
		return *this;
	}
	
	IPropertyStorage& DotDumper::have(bool& prop, string id, string desc, bool editable) {
		file << "<b>" << id << "</b>: " << (prop ? "yes" : "no") << "<br/>";
		return *this;
	}


	
	void DotDumper::endDump() {
		file << "}\n" << endl;
		file.close();
		
	}
	
	


}

