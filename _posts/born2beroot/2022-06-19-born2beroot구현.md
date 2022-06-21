---
title:  "[born2beroot] born2beroot 구현"
excerpt: "born2beroot 이것저것"

categories:
  - born2beroot
tags:
  - [42seoul, born2beroot]
toc: true
toc_sticky: true
toc_label: "목차"
date: 2022.06.19 12:40:00
---

# born2beroot

born2beroot는 가상머신을 활용해보는 과제이다.    
'VirtualBox(또는 UTM)'에서 새로운 가상머신을 생성하고, 그곳에 'CentOS' 또는 'Debian' 운영체제를 설치하여 이것저것 해보는 것이 주된 목표이다.    

# born2beroot 과제 요구사항 구현
1. [User](#user-설정)
2. [Password](#password-설정)
3. [Host&Partition](#host--partition-설정)
4. [Sudo](#sudo-설정)
5. [UFW](#ufw-설정)
6. [SSH]
7. [Script monitoring]

***

# User 설정

* 그룹 추가 : `groupadd (groupname)`
* 그룹 삭제 : `groupdell (groupname)`
* 그룹 확인 : `getent group`
* 유저 추가 : `useradd (username)`    
또는 `useradd -g (groupname) (username)`으로 유저생성과 그룹설정 동시에 가능
* 유저 삭제 : `sudo userdel -r (user)`
* 그룹에서 유저 삭제 : `sudo deluser (user) (group)`
* `usermod` 명령어    
	`-l (user) (newname)` : user명 변경
	`-L/-U (user)` : 잠금 및 잠금해제
	`-p (password)` : 비밀번호 변경
	`-aG (group1,group2, ...) (user)` : user를 group에 추가, a옵션이 없을 시 명시되어있지 않은 group에서는 제거
	`-g (group) (user)` : 
* 현재 user 확인 : `whoami`
* user 정보 확인 : `id (user)`
* user 주소 확인 : `/etc/passwd`

***

# Password 설정

* 신규 유저 암호 생성 : `passwd (user)`
* 암호 변경 : `passwd -e (user)` > 비밀번호 강제로 만료시켜 다음 로그인시 새 비밀번호 입력

## Password 기간 설정
* `chage` 명령어 - 기존 유저의 암호 설정 변경시 사용    
	`-l (user)` : 유저의 암호 설정 확인
	`-d (number)` : 마지막으로 암호 변경한 날짜 수정
	`-I (number)` : 암호 만료 후 변경 가능한 유예기간 설정
	`-E (YYYY-MM-DD)` : 암호 만료일 설정
	`-m (number)` : 암호 변경 후 의무 사용일 설정
	`-M (number)` : 암호 사용가능 최대 일 수 설정
	`-W (number)` : 암호 만료 전 경고메세지를 보낼 일 수 지정
* `/etc/login.defs` - 신규 유저의 암호 설정 변경 가능    
	`PASS_MAX_DAYS` : 암호 사용가능 최대 일 수
	`PASS_MIN_DAYS` : 암호 의무 사용일
	`PASS_WARN_AGE` : 암호 만료 전 경고메세지를 보낼 일 수
* `/etc/shadow` : 유저들의 암호 설정 확인 가능

## Password 정책 설정
* 현재 암호 정책 확인 : `/etc/pam.d/common-password`
* 패스워드 정책 설정 모듈 추가 : `sudo apt install libpam-cracklib` -> `common-password`파일에 `pamcracklib.so` 추가
* `libpam-cracklib`
	`retry=N` : 암호 입력 가능횟수 설정
	`minlen=N` : 암호 최소길이 설정, N + credit + 1이 설정 가능한 최소길이가 됨
	`difok=N` : 기존 암호와 달라야하는 문자 수 설정
	`ucredit=N` : N<0) 대문자 N개 이상으로 설정, N>0)대문자 N개 이하로 설정 + 이하일경우 그 수만큼 credit+
	`lcredit=N` : ucredit과 동일한 기능의 소문자 설정
	`decredit=N` : ucredit과 동일한 기능의 숫자 설정
	`ocredit=N` : ucredit과 동일한 기능의 특수문자 설정
	`reject_username` : user명이 암호에 들어있는지 검사(뒤집힌 경우 포함)
	`maxrepeat=N` : 같은 문자가 반복 가능한 횟수 설정
	`enforce_for_root` : root user가 암호 변경시에도 해당 모듈의 조건 추가. 단, root 암호 변경시 이전의 암호는 검사하지 않음

***

# Host / Partition 설정

* hostname 확인 : `hostnamectl`
* hostname 변경 : `sudo hostnamectl set-hostname (newname)`
* partiton 확인 : `lsblk`

# Sudo 설정

`su` 또는 `su -`를 통해 root권한으로 넘어가서 설정.    

* 확인 : `dpkg -l sudo`
* 설치 : `apt-get install sudo`, `-y`옵션 추가시 자동적으로 yes처리
* sudo 설정 변경 : `sudo visudo` (`/etc/sudoers` 편집, 정합성 및 문법 자동 검사)
	* `secure path` 제한 : `Defaults	secure_path=""`
		* `secure path` - 명령을 수행하기 위해 `sudo`가 실행할 소프트웨어를 찾는 경로. `/A:/B` = A에 없으면 B에서 
	* 암호 입력 제한 : `Defaults	passwd_tries=N`
	* 권한 획득 실패시 메세지 출력 : `Defaults	authfail_message=""`
	* 암호 불일치시 메시지 출력 : `Defaults	badpasswd_message=""`
	* log 저장 디렉토리 설정 : `Defaults	iolog_Dir="/var/log/sudo"`
	* 입력된 명령어 log로 저장 : `Defaults	log_input`
	* 출력 결과 log로 저장 : `Defaults	log_output`
	* `sudo` 명령어 외부실행 제한 : `Defaults	requiretty`
* sudo log 확인 : `sudo ls /var/log/sudo/00/00` (log는 00-ZZ까지 기록되며, 초과될시 새 디렉토리가 생성됨)
	* `log` : sudo 실행시 실행한 위치와 실행한 명령어의 위치정보
	* `stderr` : 오류 발생시 출력되는 정보
	* `stdin` : sudo로 실행한 명령어가 표준 입력을 받은 내용
	* `stdout` : sudo로 실행한 명령어가 표준 출력으로 출력한 내용
	* `timing` : session timing file
	* `ttyin` : sudo로 실행한 명령어가 tty로 입력받은 내용
	* `ttyout` : sudo로 실행한 명령어가 tty로 출력한 내용

***

# UFW 설정

* 확인 : `dpkg -l ufw`
* 설치 : `sudo apt install ufw`    
* ufw 상태 확인 : `sudo ufw status verbose`    
* 부팅시 자동 활성화 : `sudo ufw enable`    
* 기본 설정 : `sudo ufw default allow/deny`
* 포트 연결 설정 : `sudo ufw allow/deny (port)`
* 연결 설정 확인 : `sudo ufw status numbered`
* 연결 설정 제거 : `sudo ufw delete (number)`

***

# SSH 설정

* 확인 : `sudo ssh -V`
* 설치 : `apt install openssh-server`
* 실행 확인 : `systemctl status ssh`
* 포트 설정 : `sudo vi /etc/ssh/sshd_config`에서 Port N (sshd : 서버, ssh : 클라이언트)
* `root`계정 접속 제한 : `/etc/ssh/sshd_config`에서 `PermitRootLogin`을 no로 설정
* SSH 재시작을 통한 설정 적용 : `sudo systemctl restart ssh`
* SSH를 통한 로그인 : `ssh (user)@(IP) -p (host_port)`


* inet 주소 확인 : `sudo apt-get net-tools` > `ifconfig`



