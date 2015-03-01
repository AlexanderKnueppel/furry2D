#ifndef __FURRY_CORE_CHANNEL_H__
#define __FURRY_CORE_CHANNEL_H__  

/*
* ****************************************
*
* This file is part of Furry2D, a simple gameframework for 2D desktop games.
*
* Copyright (c) 2015 Furry2D. All rights reserved.
*
* For the full copyright and license information, please view the LICENSE.txt
* file that was distributed with this source code.
*
* \Author Alexander Knueppel
*
* ****************************************
*/

FURRY_NS_BEGIN

#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>

class Channel;

namespace detail {
	template <typename tMessage>
	class ChannelQueue {
	public:
		typedef std::function<void(const tMessage&)> Handler;
		typedef std::pair<void*, Handler> HandlerPair;

		static ChannelQueue& instance() {
			static ChannelQueue anInstance;

			return anInstance;
		}

		template <typename tHandler>
		void add(tHandler* handler) {
			vector_.push_back(std::make_pair((void*)handler, createHandler(handler)));
		}

		template <typename tHandler>
		void remove(tHandler* handler) {
			vector_.remove_if(
				[handler](const HandlerPair& pair) {
				return (handler == pair.first);
			}
			);
		}

		void broadcast(const tMessage& message) {
			auto localVector = vector_.copyInternals();

			for (const auto& pair : localVector)
				pair.second(message);
		}

	private:
		ChannelQueue() {
		}

		template <typename tHandler>
		Handler createHandler(tHandler* handler) {
			return [handler](const tMessage& message) { (*handler)(message); };
		}

		typedef std::function<void(const tMessage&)> Handler;
		typedef std::pair<void*, Handler> HandlerPair;

		ConcurrentVector<HandlerPair> vector_;
	};
}

class Channel {
public:
	template <typename tEvent, class tHandler>
	static void add(tHandler* handler) {
		detail::ChannelQueue<tEvent>::instance().add(handler);
	}

	template <typename tEvent, class tHandler>
	static void remove(tHandler* handler) {
		detail::ChannelQueue<tEvent>::instance().remove(handler);
	}

	template <typename tEvent>
	static void broadcast(const tEvent& message) {
		detail::ChannelQueue<tEvent>::instance().broadcast(message);
	}
};

// This is the contract that an object should fulfill for correct usage with a Channel
// Note that this is not an actual required base class, even though it can be used as one
template <typename tMessage>
class MessageHandler {
public:
	MessageHandler() {
		Channel::add<tMessage>(this);
	}

	MessageHandler(const MessageHandler&) {
		Channel::add<tMessage>(this);
	}

	~MessageHandler() {
		Channel::remove<tMessage>(this);
	}

	virtual void operator()(const tMessage&) = 0;
};

FURRY_NS_END

#endif