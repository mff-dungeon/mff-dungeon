#include <stdlib.h>

#include "ActionDescriptor.hpp"
#include "../Output/Output.hpp"

namespace Dungeon {

	ActionDescriptor::ActionDescriptor(Driver* driver) : driver(driver) {
		id = rand();
		gm = nullptr;
		caller = nullptr;
		action = nullptr;
	}

	ActionDescriptor::~ActionDescriptor() {
		if (action) action->forget();
	}

	void ActionDescriptor::enqueued(GameManager* gm) {
		this->gm = gm;
	}

	void ActionDescriptor::assigned(ObjectPointer caller) {
		this->caller = caller;
	}

	void ActionDescriptor::matched(Action* action) {
		setAction(action);
	}

	void ActionDescriptor::setAction(Action* action) {
		if (action) action->remember();
		if (this->action) this->action->forget();
		this->action = action;
	}

	Action* ActionDescriptor::getAction() {
		return action;
	}

	Human* ActionDescriptor::getCaller() {
		return caller.unsafeCast<Human>();
	}

	GameManager* ActionDescriptor::getGM() {
		return gm;
	}

	bool ActionDescriptor::isValid(Driver* driver) {
		return gm && caller && action && driver == this->driver;
	}

	void ActionDescriptor::addSentence(const string& msg) {
		string trimmed = Utils::trim(msg);
		if (trimmed == "") return;

		Output::PlainString* ps = new Output::PlainString(trimmed);
		if (replyFormat == ReplyFormat::List) {
			listedMessage.insert(ps);
		} else {
			sentencedMessage.insert(ps);
		}
	}
	
	ActionDescriptor& ActionDescriptor::operator<<(const string& msg) {
		currentSentence << msg;
		return *this;
	}

	ActionDescriptor& ActionDescriptor::operator<<(char const* msg) {
		currentSentence << msg;
		return *this;
	}

	ActionDescriptor& ActionDescriptor::operator<<(const int& msg) {
		currentSentence << msg;
		return *this;
	}
	
	ActionDescriptor& ActionDescriptor::operator<<(ActionDescriptor::EndOfSentence*(*endofsentence)()) {
		this->addSentence(currentSentence.str());
		currentSentence.str("");
		return *this;
	}
	
	ActionDescriptor& ActionDescriptor::operator<<(Output::Base* o) {
		message.insert(o);
		return *this;
	}	
	ActionDescriptor& ActionDescriptor::operator<<(Output::Container::ptr_t&& o) {
		message.insert(move(o));
		return *this;
	}	

	ActionDescriptor::EndOfSentence *eos() {
		LOGS(Error) << "Tried to call invalid method eos." << LOGF;
		throw GameException("AD: Method eos is not callable.");
	}

	ActionDescriptor* ActionDescriptor::setReplyFormat(ReplyFormat format) {
		*this << eos;
		flushContainers();
		replyFormat = format;
		return this;
	}
	
	ActionDescriptor& ActionDescriptor::operator=(const ActionDescriptor& right) {
		if (this == &right)
			return *this;

		throw logic_error("AD cannot be copied.");
	}

	TextActionDescriptor::TextActionDescriptor(Driver* driver) : ActionDescriptor(driver) { }

	const Output::Base& TextActionDescriptor::getReply() {
		return getOutputContainer();
	}

	void TextActionDescriptor::clearReply() {
		flushContainers();
		message.clear();
	}
	
	string ActionDescriptor::formatMessage(string msg)
	{
		LOGS(Verbose) << "Input: " << msg << LOGF;
		string out;
		for (auto it = msg.begin(); it != msg.end(); ++it) {
			if (*it == '%') {
				if (++it == msg.end()) --it;
				switch(*it) {
				case '%': // Also at the end of string
					out.append("%");
					break;
				case 'u':
					out.append(getCaller()->getName());
					break;
				default:
					LOGS(Error) << "Unexpected modifier %" << *it << "!" << LOGF;
					out.push_back('%');
					out.push_back(*it);
				}
			} else {
				out.push_back(*it);
			}
		}
		LOGS(Verbose) << "Output: " << out << LOGF;
		return std::move(out);
	}

}

