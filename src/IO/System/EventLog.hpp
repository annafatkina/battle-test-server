#pragma once

#include "details/PrintFieldVisitor.hpp"

#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <variant>


namespace sw
{
	class EventLog
	{
	public:
		void log(uint64_t, std::monostate){}	

		template <class TEvent>
		void log(uint64_t tick, TEvent&& event)
		{
			std::cout << "[" << tick << "] " << event.Name << " ";
			PrintFieldVisitor visitor(std::cout);
			event.visit(visitor);
			std::cout << std::endl;
		}
	};
}
