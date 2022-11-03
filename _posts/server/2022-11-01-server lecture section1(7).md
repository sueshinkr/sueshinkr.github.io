---
title:  "Server lecture section1 [7/7]"
excerpt: "멀티쓰레드 프로그래밍"

categories:
  - Server_lecture
tags:
  - [Server]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.11.01 18:00:00
---

# Reader-Writer Lock

```cpp
```

`Write`정보와 `Read`정보를 하나의 `int32`형 데이터에 비트플래그를 통해 묶어서 사용    

`WriteLock`이 걸려있는 상태에서 동일 쓰레드가 쓰거나 읽는 것은 허용    
반면 `ReadLock`이 걸려있는 상태에서 동일 쓰레드가 읽는 것은 허용하지만, 쓰는 것은 허용하지 않음    



***

# DeadLock 탐지

***

# 연습문제
