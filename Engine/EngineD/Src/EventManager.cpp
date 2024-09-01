#include "Precompiled.h"
#include "EventManager.h"

using namespace EngineD;

namespace
{
	std::unique_ptr<EventManager> sEventManager;
}

void EventManager::StaticInitialize()
{
	ASSERT(sEventManager == nullptr, "AudioSystem: is already initialized");
	sEventManager = std::make_unique<EventManager>();
	sEventManager->Initialize();
}

void EventManager::StaticTerminate()
{
	if (sEventManager != nullptr)
	{
		sEventManager->Terminate();
		sEventManager.reset();
	}
}

EventManager* EventManager::Get()
{
	ASSERT(sEventManager == nullptr, "AudioSystem: is not initialized");
	return sEventManager.get();
}

void EventManager::Broadcast(const Event* event)
{
	sEventManager->BroadcastPrivate(event);
}

EventManager::~EventManager()
{
}

void EventManager::Initialize()
{
	mEventListeners.clear();
}

void EventManager::Terminate()
{
	mEventListeners.clear();
}

uint32_t EventManager::AddListener(EventType eventType, const EventCallback& cb)
{
	mEventListeners[eventType][++mListenerId] = cb;
	return mListenerId;
}

void EventManager::RemoveListener(EventType eventType, uint32_t listenerId)
{
	auto listenersIter = mEventListeners.find(eventType);

	if (listenersIter != mEventListeners.end())
	{
		auto iter = listenersIter->second.find(listenerId);
		if (iter != listenersIter->second.end())
		{
			listenersIter->second.erase(iter);
		}
	}
}

void EventManager::BroadcastPrivate(const Event* event)
{
	auto listenersIter = mEventListeners.find(event);

	if (listenersIter != mEventListeners.end())
	{
		for (auto& cb : listeners)
		{
			cb.second(event);
		}
	}
}
