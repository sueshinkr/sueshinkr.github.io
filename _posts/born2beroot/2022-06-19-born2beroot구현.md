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
asdfasdfasdfasd
# born2beroot

born2beroot는 가상머신을 활용해보는 과제이다.    
'VirtualBox(또는 UTM)'에서 새로운 가상머신을 생성하고, 그곳에 'CentOS' 또는 'Debian' 운영체제를 설치하여 이것저것 해보는 것이 주된 목표이다.    

# born2beroot 과제 요구사항 구현
1. [User](#user-설정)
2. [Password](#password-설정)
3. [Host&Partition](#host--partition-설정)
4. [Sudo](#sudo-설정)
5. [UFW](#ufw-설정)
6. [SSH](#ssh-설정)
7. [Script monitoring](#script-monitoring-설정)

# BONUS!
1. [Partition](#partition-설정)
2. [Lighttpd](#lighttpd-설정)
3. [PHP](#php-설정)
4. [MariaDB](#mariadb-설정)
5. [WordPress](#wordpress-설정)
6. [FTP](#ftp-설정)

***

# User 설정

* 그룹 추가 : `groupadd (groupname)`
* 그룹 삭제 : `groupdell (groupname)`
* 그룹 확인 : `getent group`
* 유저 추가 : `adduser (username)`, `useradd`의 경우 모든 설정들을 수동으로 명시해주어야 함    
* 유저 삭제 : `sudo userdel (user)`, `-r`옵션 설정시 홈디렉토리까지 삭제
* 그룹에서 유저 삭제 : `sudo deluser (user) (group)`
* `usermod` 명령어    
	* `-l (user) (newname)` : user명 변경
	* `-L/-U (user)` : 잠금 및 잠금해제
	* `-p (password)` : 비밀번호 변경
	* `-aG (group1,group2, ...) (user)` : user를 group에 추가, a옵션이 없을 시 명시되어있지 않은 group에서는 제거
	* `-g (group) (user)` : 
* 현재 user 확인 : `whoami`
* user 정보 확인 : `id (user)`
* user 주소 확인 : `/etc/passwd`

***

# Password 설정

* 신규 유저 암호 생성 : `passwd (user)`
* 암호 변경 : `passwd -e (user)` > 비밀번호 강제로 만료시켜 다음 로그인시 새 비밀번호 입력

## Password 기간 설정
* `chage` 명령어 - 기존 유저의 암호 설정 변경시 사용    
	* `-l (user)` : 유저의 암호 설정 확인    
	* `-d (number)` : 마지막으로 암호 변경한 날짜 수정    
	* `-I (number)` : 암호 만료 후 변경 가능한 유예기간 설정    
	* `-E (YYYY-MM-DD)` : 암호 만료일 설정
	* `-m (number)` : 암호 변경 후 의무 사용일 설정
	* `-M (number)` : 암호 사용가능 최대 일 수 설정
	* `-W (number)` : 암호 만료 전 경고메세지를 보낼 일 수 지정
* `/etc/login.defs` - 신규 유저의 암호 설정 변경 가능    
	* `PASS_MAX_DAYS` : 암호 사용가능 최대 일 수
	* `PASS_MIN_DAYS` : 암호 의무 사용일
	* `PASS_WARN_AGE` : 암호 만료 전 경고메세지를 보낼 일 수
* `/etc/shadow` : 유저들의 암호 설정 확인 가능

## Password 정책 설정
* 현재 암호 정책 확인 : `/etc/pam.d/common-password`
* 패스워드 정책 설정 모듈 추가 : `sudo apt install libpam-pwquality` -> `common-password`파일에 `pam_pwquality.so` 추가
* `pam_pwquality.so`
	* `retry=N` : 암호 입력 가능횟수 설정
	* `minlen=N` : 암호 최소길이 설정, N + credit + 1이 설정 가능한 최소길이가 됨
	* `difok=N` : 기존 암호와 달라야하는 문자 수 설정
	* `ucredit=N` : N<0) 대문자 N개 이상으로 설정, N>0)대문자 N개 이하로 설정 + 이하일경우 그 수만큼 credit+
	* `lcredit=N` : ucredit과 동일한 기능의 소문자 설정
	* `decredit=N` : ucredit과 동일한 기능의 숫자 설정
	* `ocredit=N` : ucredit과 동일한 기능의 특수문자 설정
	* `usercheck=N` : user명이 암호에 들어있는지 검사, 기본값은 1(검사 수행함)
	* `maxrepeat=N` : 같은 문자가 반복 가능한 횟수 설정
	* `enforce_for_root` : root user가 암호 변경시에도 해당 모듈의 조건 추가. 단, root 암호 변경시 이전의 암호는 검사하지 않음

***

# Host / Partition 설정

* hostname 확인 : `hostnamectl`
* hostname 변경 : `sudo hostnamectl set-hostname (newname)`
* partiton 확인 : `lsblk`

# Sudo 설정

`su` 또는 `su -`를 통해 root권한으로 넘어가서 설정.    

* 확인 : `dpkg -l sudo`
* 설치 : `apt-get install sudo`, `-y`옵션 추가시 자동적으로 yes처리


* 편집기 변경 : `sudo update-alternatives --config editor`
* sudo 설정 변경 : `sudo visudo` (`/etc/sudoers` 편집, `.tmp`파일을 통한정합성 및 문법 자동 검사)
	* `secure path` 제한 : `Defaults	secure_path=""`
		* `secure path` - 명령을 수행하기 위해 `sudo`가 실행할 소프트웨어를 찾는 경로. `/A:/B` = A에 없으면 B에서 
	* 암호 입력 제한 : `Defaults	passwd_tries=N`
	* 권한 획득 실패시 메세지 출력 : `Defaults	authfail_message=""`
	* 암호 불일치시 메시지 출력 : `Defaults	badpass_message=""`
	* log 저장 디렉토리 설정 : `Defaults	iolog_dir="/var/log/sudo"`
	* 입력된 명령어 log로 저장 : `Defaults	log_input`
	* 출력 결과 log로 저장 : `Defaults	log_output`
	* `sudo` 명령어 외부실행 제한 : `Defaults	requiretty`
* sudo log 확인 : `sudo ls /var/log/sudo/00/00` (log는 00-ZZ까지 기록되며, 초과될시 새 디렉토리가 생성됨)
	* `log` : sudo 실행시 실행한 위치와 실행한 명령어의 위치정보
	* `stderr` : 오류 발생시 출력되는 정보
	* `stdin` : sudo로 실행한 명령어가 표준 입력을 받은 내용
	* `stdout` : sudo로 실행한 명령어가 표준 출력으로 출력한 내용
	* `timing` : session이 실행된 시간
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

* 포트(Port) : 하나의 운영체제 안에서 소켓을 구분하는 목적으로 사용되는 번호

* 확인 : `sudo ssh -V`
* 설치 : `apt install openssh-server`
* 실행 확인 : `systemctl status ssh`
* 포트 설정 : `/etc/ssh/sshd_config`에서 `Port N`으로 설정 (sshd : 서버, ssh : 클라이언트)
* `root`계정 접속 제한 : `/etc/ssh/sshd_config`에서 `PermitRootLogin`을 no로 설정
* SSH 재시작을 통한 설정 적용 : `sudo systemctl restart ssh`
* SSH를 통한 로그인 : `ssh (user)@(IP) -p (host_port)`

* inet 주소 확인 : `sudo apt-get net-tools` > `ifconfig`

* DHCP 연결 제거 : `/etc/network/interfaces`에서 `iface enp0s3 inet dhcp`를 `ifcae enp0s3 inet static`으로 수정    
	이후 `ip address`와 `netmask`, `gateway`를 설정    
	`gateway`는 `ip route` 명령어를 통해 확인 가능하고, `ip address`와 `netmask`는 `ifconfig`명령어를 통해 확인 가능    
	마지막으로 `ifdown enp0s3`, `ifup enp0s3`로 해당 네트워크 디바이스를 재시작하여 변경사항 적용, `reboot`으로 전체 네트워크 디바이스를 재시작
	* enp0s3 : 네트워크 인터페이스 이름
* 네트워크 상태 확인 : `ss`
	* `-a` : 모든 포트 확인
	* `-t` : TCP 포트 확인
	* `-u` : UDP 포트 확인
	* `-l` : `LISTEN` + `UNCONN` 상태의 포트 확인 (`ss`의 기본값은 `non-listening`)
		* `LISTEN` : 포트가 열려있는 상태, 연결 대기중
		* `SYS-SENT` : 연결을 요청한 상태
		* `SYN_RECEIVED` : 연결 요청에 응답 후 확인 대기중
		* `ESTABLISHED` : 연결되어있음
		* `CLOSED` : 연결이 끊어짐
		* `CLOSE_WAIT` : 연결이 종료되기를 대기중
		* `TIME_WAIT` : 연결은 종료되었으나 원격의 수신 보장을 위해 기다리고있는 상태
	* `-p` : 프로세스명 표시
	* `-n` : 호스트 / 포트 / 사용자 이름을 숫자로 표시


***

# Script Monitoring 설정
## Cron

* `/etc/crontab` : system wide crontab 설정
* `crontab -e` : 개별 유저의 crontab 설정. `root`도 개별 crontab을 가질 수 있음

* cron 목록 조회 : `crontab -l`
* cron 편집 : `crontab -e` 
* cron 설정 : `*(분) *(시간) *(날짜) *(월) *(요알) (command)`
	* `-` - 사이 모든 값, ex)2-4 = 2, 3, 4
	* `,` - 지정 값
	* `/` - 주기로 설정, ex)/10 = 10마다
	* 30초마다 실행되도록 설정 : `*/1 * * * * sleep 30; (command)` 추가
* cron 실행(재실행) : `service cron start(restart)`
* cron 중지 : `service cron stop`
* cron 상태 확인 : `service cron status`
* 서버 시작시 cron 실행(중지) : `systemctl enable(disable) cron`

## Monitoring.sh

* `#!/bin/bash` : 해당 파일을 bash 쉘로 실행시키겠다는 의미

1. 운영 체제 및 해당 커널 버전의 아키텍쳐
	* `uname` : 시스템 운영체제 정보 출력, `-a` 옵션 사용시 시스템 전체 정보 출력
2. 물리적 프로세스의 수
	* `nproc` : 사용 가능한 프로세스의 수 출력, `--all` 옵션 사용시 모든 프로세스 개수 출력
3. 가상 프로세서의 수
	* 가상 프로세서(virtual processor) : 가상머신에 할당된 cpu 코어. 실제 코어 수보다 많을 수 있음
	* `/proc/cpuinfo` : cpu 코어의 세부사항이 담긴 파일. `grep processor | wc -l`으로 가상 프로세서의 수를 확인 
4. 서버에서 현재 사용 가능한 RAM 및 사용률
	* `free` : 시스템의 메모리 현황 출력, `-m` 옵션 사용시 MB 단위로 출력. `total`과 `free` 메모리를 적절하게 참조하여 사용률을 백분률로 표시할 수 있음
5. 서버의 현재 사용 가능한 메모리 및 사용률
	* `df` : 마운트된 파일 시스템의 크기와 용량 출력, `-P` 옵션 사용시 경로가 길어도 한줄로 출력
	* `awk`에서 `END`패턴 사용시 모든 레코드를 처리한 후 `END`에 지정된 액션을 실행
6. 프로세서의 현재 사용률
	* `mpstat` : `sysstat`패키지에 포함된 명령어. 모든 cpu에 대한 정보 출력
7. 마지막 재부팅 날짜 및 시간
	* `who` : 현재 시스템에 접속한 사용자 출력, `-b` 옵션 사용시 마지막 부팅 정보 출력
8. LVM의 활성 상태 여부
	* `lsblk` : 트리 형식으로 모든 스토리지 디바이스 출력
9. 활성 연결 수
	* `ss` : socket 상태 출력, `-t` 옵션 사용시 `TCP` 연결만을 출력
10. 서버를 사용하는 사용자 수
	* `who` 명령어 사용
11. 서버의 IPv4 주소 및 해당 MAC(Media Access Control) 주소
	* `hostname` : 호스트네임을 출력, `-I` 옵션 사용시 호스트의 모든 네트워크 주소 출력
	* `ip link` : 네트워크 인터페이스를 출력
12. sudo 프로그램으로 실행된 명령 수
	* `/var/log/auth.log` : 사용자 로그인이나 사용된 인증방법 등의 시스템 인증 정보가 기록됨. `grep -a(바이너리 파일을 텍스트 파일처럼 취급) sudo | grep COMMAND | wc -l`로 sudo 명령 수 확인
	* `jounalctl` : `systemd`의 서비스 로그를 조회하는 명령어. `_COMM=sudo` 명령어로 sudo 로그만 확인.
		* 저널링 파일 시스템(`journaling file system`) - 시스템의 일정 부분을 시스템 변경사항 기록용으로 할당해두어 백업이나 복구가 가능해진 시스템. `ex4(extended file system4)`가 이 시스템으로 되어있으며 리눅스 배포판들은 이를 기본 파일 시스템으로 채택하는 경향이 있음.

***

# partition 설정

* `/` : `root` 최상위 마운트 파티션. 비교적 크기가 작은 `/bin, /etc`를 포함
* `/swap` : 스왑 파티션. 가상 메모리로 실제 물리적인 램이 부족할 때 대신 사용
* `/home` : 사용자 계정 파티션. 사용자 계정이 위치하며, 웹 호스팅 서비스를 할 경우 해당 파티션의 용량을 가능한 한 크게 설정
* `/var` : 로그 파일 파티션. 시스템의 로그 파일들이 저장되며 공간을 많이 차지하기 때문에 디스크 용량 부족 현상이 생기지 않도록 처리
* `/srv` : 서버 파티션. 프로토콜을 이용한 외부 사용자와의 공유에 사용
* `/tmp` : 임시 파티션. 임시 파일들을 저정하거나 임시로 작업을 진핼할 때 사용
* `/var/log` : 프로그램의 로그파일들이 따로 저장되는 파티션. 이외에는 `/var`과 동일
* `/boot` : static files of the boot loader
* `/usr` : static data
* `/opt` : add-on application software packages
* `/usr/local` : local hierachy


# Lighttpd 설정

* 설치 : `apt-get install lighttpd`
* 서버 시작/중지/부팅시 활성화 : `systemctl start/stop/enable lighttpd.service`
* 상태 확인 : `systemctl status lighttpd`
* fastcgi 적용 : 
	1. `/etc/lighttpd/conf-available/15-fastcgi-php.conf`파일 편집 - `"Bin-path", "socket"` 주석처리 후 `"socket" => "/var/run/php/php(ver)-fpm.sock"` 추가    
	2. `lighttpd-enable-mod fastcgi/fatcgi-php`    
* 재시작 : `service lighttpd force-reload`, `initscripts` 패키지 필요
* 연결 포트 확인 : `/etc/lighttpd/lighttpd.conf`
* 연결 확인 : `(ip):8080`으로 접속 (포트포워딩 필요)

***

# PHP 설정

* 설치 : `apt-get install php-fpm`
* `cgi.fix_pathinfo` 활성화 : `/etc/php/(ver)/fpm/php.ini`에서 `cgi.fix_pathinfo=1` 주석 해제    
	해당 기능을 통해 스크립트와 파일 경로를 적절하게 구분할 수 있으나 보안상의 취약점이 생성됨    
	[자세한 내용은 링크 참조](https://serverfault.com/questions/627903/is-the-php-option-cgi-fix-pathinfo-really-dangerous-with-nginx-php-fpm)    
* db와 연동 : `apt install php(ver)-mysql`
* lighttpd와의 연결 확인 : `/var/www/html/info.php`를 아래와 같이 작성 후 `(ip):8080/info.php`로 접속
	```
	<?php
		phpinfo();
    	?>`
	```


***

# MariaDB 설정
* 설치 : `apt-get install mariadb-server mariadb-client`
* db 서버 시작/중지/부팅시 활성화 : `systemctl start/stop/enable mariadb(또는 mysql.service)`
* db 서버 보안설정 : `mysql_secure_installation`
* db 서버 재시작 : `systemctl restart mariadb(또는 mysql.service)`

* MariaDB 서비스 실행 : `mysql (db)`, `-u (user) -p`로 sql문을 실행할 유저 선택
	* db 생성 : `create database (db)`
	* db 확인 : `show databases`
	* 유저 생성 : `create user '(user)'@'localhost' identified by '(user passwd)';`
	* 유저의 db 권한 생성 : `grant all on (db).* to '(user)'@'localhost' identified by '(user passwd)&#96 with grant option;`
	* 변경사항 즉시 반영 : `flush privileges;`
	* 종료 : `exit;`

***

# WordPress 설정
* `Wordpress` 설치 : `lighttpd`의 설정에서 `server.document-root`로 설정되어있던 디렉토리에 설치    
	* `apt-get install wget`
	* `wget -O /tmp/wordpress.tar.gz "http://wordpress.org/latest.tar.gz"`
	* `tar -xvzf /tmp/wordpress.tar.gz -C /var/www/html`
* `Wordpress`와 db 연동 : `/var/www/html/wordpress/wp-config-sample.php`파일 편집    
	* `DB_NAME`, `DB_USER`, `DB_PASSWORD`, `DB_HOST` 설정
	* 인증키 설정 - `https://api.wordpress.org/secret-key/1.1/salt/`의 키 복붙    
	* sample파일 이름 변경 - `/var/www/html/wordpress/wp-config.php`
	* 확인 - `(ip):8080/wordpress` 접속

***

# FTP 설정
* 서버 설치 : `apt install vsftpd`
* 클라이언트 설치 : `apt install ftp`
* `/etc/vsttpd.conf`    
[참고한 링크](https://onlyit.tistory.com/entry/VSFTP-%ED%99%98%EA%B2%BD%EC%84%A4%EC%A0%95vsftpdconf)    
	* 업로드 활성화 - `write_enables=YES`
	* 사용자의 상위 디렉토리 접근 차단(홈 디렉토리를 루트 디렉토리로 간주) : `chroot_local_user=YES`
	* 가상유저들을 지칭할 가상의 변수 지정 : `user_sub_token=$USER`
	* 가상유저들이 로그인 후 이동될 디렉토리 지정 : `local_root=/home/$USER/ftp`
	* 해당 디렉토리의 권한 설정 : `chown nobody:nogroup /home/(username)/ftp`, `chmod a-w /home/(username)/ftp`
	* 명시된 사용자만 로그인을 허용 : `userlist_enable=YES`, `userlist_deny=NO`
	* 명시된 사용자를 읽어올 파일 지정 : `userlist_file=/etc/vsftpd.userlist`
* 연결할 기기의 환경 설정(mac의 경우)
	* 클라이언트 설치 : `brew install inetutils`
	* 연결 : `ftp (ip)`
* `ftp` 명령어
	* `bye` : 연결 종료
	* `put` : 로컬의 파일을 `ftp` 서버로 전송
	* `get` : `ftp` 서버의 파일을 로컬에 내려받음
	* `cd`, `ls`, `dir`, `mkdir` ....