#pragma once

class IObserver {
public:
	virtual ~IObserver() = default;
	virtual void OnDependencyChanged() = 0;
};

class IObservable {
public:
	virtual ~IObservable() = default;
	virtual void AddObserver(IObserver& observer) = 0;
	virtual void RemoveObserver(IObserver& observer) = 0;
	virtual void NotifyObservers() = 0;
};