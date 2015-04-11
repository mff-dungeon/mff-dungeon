#ifndef STATS_HPP
#define	STATS_HPP

#include "../../common.hpp"
#include "../../Actions/Action.hpp"
#include "../Human.hpp"
#include "Base.hpp"

namespace Dungeon {
	class StatReq : public Base {
	public:
		StatReq() { };
		StatReq(const objId& id) : Base(id) { };
		~StatReq() { };
		StatReq(Human::Stats stat, int value) {
			this->stat = stat;
			this->value = value;
		};
		virtual void getActions(ActionDescriptor* ad) { };
		int getValue() const;
		StatReq* setValue(int value);
		Human::Stats getStat() const;
		StatReq* setStat(Human::Stats stat);

		virtual void registerProperties(IPropertyStorage& storage);
	private:
		int value;
		Human::Stats stat;

		PERSISTENT_DECLARATION(StatReq, Base);
	};

	class RaiseStatAction : public Action {
	public:
		RaiseStatAction(const string& type = "raise-stat") : Action(type) { };

		void selectStat(const string& statName, ActionDescriptor* ad);

		virtual bool match(const string& command, ActionDescriptor* ad);
		virtual void commit(ActionDescriptor* ad);
		virtual void explain(ActionDescriptor* ad);
	private:
		Human::Stats selectedStat = Human::End;
	};
}

#endif /* STATS_HPP */
