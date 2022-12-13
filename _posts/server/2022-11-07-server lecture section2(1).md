---
title:  "Server lecture section2 [1/4]"
excerpt: "메모리 관리"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.11.07 16:30:00
---

# Reference Counting

```cpp
/*-----------------
	RefCountable
------------------*/
class RefCountable
{
	public:
		RefCountable() : _refCount(1) {}
		virtual ~RefCountable() {}

		int32 GetRefCount() { return _refCount; }
		int32 AddRef() { return ++_refCount; }
		int32 ReleaseRef()
		{
			int32 refCount = --_refCount;
			if (refCount == 0)
			{
				delete this;
			}
			return refCount;
		}
	protected:
		int32 _refCount;
};
```

```cpp
class Wraight : public RefCountable
{
	public:
		int _hp = 150;
		int _posX = 0;
		int _posY = 0;
};

class Missile : public RefCountable
{
	public:
		void SetTarget(Wraight* target)
		{
			_target = target;
			target->AddRef();
		}

		bool Update()
		{
			if (_target == nullptr)
				return true;

			int posX = _target->_posX;
			int posY = _target->_posY;

			if (_target->_hp == 0)
			{
				_target->ReleaseRef();
				_target = nullptr;
				return true;
			}

			return false;
		}
		Wraight* _target = nullptr;
};

int main()
{
	Wraight* wraight = new Wraight();
	Missile* missile = new Missile();
	missile->SetTarget(wraight);

	// 레이스 피격
	wraight->_hp = 0;
	//delete wraight;
	wraight->ReleaseRef();
	wraight = nullptr;


	while (true)
	{
		if (missile)
		{
			if (missile->Update())
			{
				//delete missile;
				missile->ReleaseRef();
				missile = nullptr;
			}
		}
	}
}
```

객체가 다른 객체에 의해 참조되고있음에도 불구하고 삭제될 경우 문제가 발생할 수 있음    
이를 해결하기 위해 `_refCount`를 통해 객체가 참조되고있는 횟수를 카운트    
카운트가 0이 되었을때만 객체를 삭제하여 오류를 방지함     

단, 현재의 `_refCount`를 증감하는 연산은 원자적으로 발생하지 않으므로 싱글 쓰레드에서만 정상적으로 동작    
멀티쓰레드에서도 동일하게 작동하게 하려면 `atomic` 변수로 선언해주어야함    
그러나 `_target`의 지정과 `_refCount`의 증감 역시 원자적으로 발생하지 않으므로 둘 사이에 `ReleaseRef()` 등이 실행될 경우 여전히 문제가 발생할 수 있음    

```cpp
/*--------------
	SharedPtr
---------------*/

template<typename T>
class TSharePtr
{
	public:
		TSharePtr() {}
		TSharePtr(T* ptr) { Set(ptr); }
		
		// 복사
		TSharePtr(const TSharePtr& rhs) { Set(rhs._ptr); }
		// 이동
		TSharePtr(TSharePtr&& rhs) { _ptr = rhs._ptr; rhs._ptr = nullptr; }
		// 상속 관계 복사
		template<typename U>
		TSharePtr(const TSharePtr<U>& rhs) { Set(static_cast<T*>(rhs._ptr)); }

		~TSharePtr() { Release(); }

	public:
		// 복사 연산자
		TSharePtr& operator=(const TSharePtr& rhs)
		{
			if (_ptr != rhs._ptr)
			{
				Release();
				Set(rhs._ptr);
			}
			return *this;
		}

		// 이동 연산자
		TSharePtr& operator=(TSharePtr&& rhs)
		{
			Release();
			_ptr = rhs._ptr;
			rhs._ptr = nullptr;
			return *this;
		}
		bool		operator==(const TSharePtr& rhs) const { return _ptr == rhs._ptr; }
		bool		operator==(T* ptr) const { return _ptr == ptr; }
		bool		operator!=(const TSharePtr& rhs) const { return _ptr != rhs._ptr; }
		bool		operator!=(T* ptr) const { return _ptr != ptr; }
		bool		operator<(const TSharePtr& rhs) const { return _ptr < rhs._ptr; }

		T*			operator*() { return _ptr; }
		const T*	operator*() const { return _ptr; }
					operator T* () const { return _ptr; } // 캐스팅
		T*			operator->() { return _ptr; }
		const T*	operator->() const { return _ptr; }

		bool IsNull() { return _ptr == nullptr; }

	private:
		inline void Set(T* ptr)
		{
			_ptr = ptr;
			if (ptr)
				ptr->AddRef();
		}

		inline void Release()
		{
			if (_ptr != nulltpr)
			{
				_ptr->ReleaseRef();
				_ptr = nullptr;
			}
		}

	private:
		T* _ptr = nullptr;
};
```

따라서 스마트 포인터의 형식을 사용하여 문제를 해결    
`TSharedPtr`이 어딘가에서 사용될경우 `_refCount`는 최소한 1 이상이 보장되기 때문에(객체가 존재하는 경우 `Set()`함수가 호출되어 카운트가 무조건 1 올라가게됨) 위에서와 같은 문제가 발생하지 않음    
단, 모든 경우에 스마트 포인트를 사용해주어야 함    
복사 대신 참조로 넘겨주어 `_refCount`를 증가시키지 않는 방법도 고려할 수 있음    

***

# 스마트 포인터

스마트 포인터를 직접 만들어서 사용할 경우
* 이미 만들어진 클래스 대상으로 사용 불가
* 순환(Cycle) 문제 발생 (특히 상속관계인 경우)    
	단, `shared_ptr`의 경우에도 순환 문제가 발생할 수 있음    

표준 스마트 포인터에는 `unique_ptr`, `shared_ptr`, `weak_ptr`이 있음

`unique_ptr`
* 일반 포인터와 비슷하나 복사가 막힘    

`shared_ptr`과 `weak_ptr`
* 사용하는 클래스 객체와 `RefCountBlock` 두가지의 메모리 영역을 가짐
* `RefCountBlock`에는 `useCount`와 `weakCount`가 존재
	* `useCount` : `shared_ptr`로 참조하고있는 횟수
	* `weakCount` : `weak_ptr`로 참조하고있는 횟수

`shared_ptr`
* `shared_ptr<Knight> spr = make_shared<Knight>();`과 같이 `make_shared` 사용시 객체와 `RefCountingBlock`을 하나로 합쳐서 할당함
* `UseCount`가 0이 되면 객체가 제거되지만 `weakCount`가 남아있을경우 `RefCountBlock`은 제거하지 않음
* `shared_ptr`끼리 서로 참조할경우 순환 문제 발생

`weak_ptr`
* `bool expired = wpr.expired();` 또는 `shared_ptr<Knight> spr2 = wpr.lock();` 방식을 사용하여 존재 여부를 확인해야함
* 객체 자체에는 영향을 주지 않음
* 순환 문제를 방지할 수 있음

[`shared_ptr`과 `weak_ptr`에 대한 자세한 내용](https://modoocode.com/252)    




