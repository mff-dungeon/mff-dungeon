#ifndef TRAP_HPP
#define	TRAP_HPP

#include "../common.hpp"

namespace Dungeon
{

        /**
         * Event handler, also known as Trap for DnD players.
         * 
         * Trap is a "virtual" object. It exists in the world, but it should not 
         * be located anywhere, nor there should be any relation where trap is slave.
         * If you make a relation trap --> object, and the event handlers are
         * written properly, it should be triggered.
         */
	class Trap : public IObject
	{
	public:
            Trap() : IObject() { };
            Trap(string id) : IObject(id) { };
            virtual ~Trap() { };

            /**
             * Can either process everything (and continue processing original action),
             * or throw TrapException and stop it.
             * 
             * If the trap should not be triggered (probability, has skill etc.) 
             * then just do nothing in this method.
             * 
             * @param event The event given to IObject->triggerTraps
             * @param target The target which triggered the trap
             * @param ad Can be NULL sometimes, then use the exception trigger
             */
            virtual void trigger(string event, ObjectPointer target, ActionDescriptor* ad) { }
            
            /**
             * This one will be called by driver after throwing TrapException
             */
            virtual void exceptionTrigger(ActionDescriptor* ad) { }
            
            /**
             * This method should never be called on Trap.
             */
            virtual void getActions(ActionList * list, ObjectPointer callee) {
                LOGS("Trap", Error) << "Getting actions od Trap? Srsly?" << LOGF;
            }
            
            /**
             * Returns name of relation for that event
             */
            static const string getRelation(const string event) {
                return "trap-" + event;
            }
          
            PERSISTENT_DECLARATION(Trap, IObject)
	};
        
        /**
         * Special exception that happens when a trap is triggered.
         */
        class TrapException : public GameException {
        public:
            TrapException(const ObjectPointer& trap);
            
        private:
            ObjectPointer trap;
        };
        
        /**
         * When there will be more of these, consider making special directory
         */
        class DemoTrap : public Trap {
        public:
            DemoTrap() : Trap() { };
            DemoTrap(string id) : Trap(id) { };
            virtual ~DemoTrap() { };
            virtual void trigger(string event, ObjectPointer target, ActionDescriptor* ad);

        };
}

#endif

