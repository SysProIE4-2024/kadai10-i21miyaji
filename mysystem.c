/*
 *  mysystem.c : system 関数のクローン
 */

#include <stdio.h>
#include <stdlib.h>    // exit, system のため
#include <unistd.h>    // fork, execXX のため
#include <sys/wait.h>  // wait のため
#include "mysystem.h"  // インタフェース

char *execpath = "/bin/sh";
// system関数のクローン
int mysystem(char *command) {
  int status = 0;
  //NULLポインタが渡された時
  if (command == NULL) {
    return 1;
  }
  int pid = 0;
  //子プロセス生成
  if ((pid = fork()) < 0) {
    return -1;  //forkできなかった時
  }
  if (pid != 0) { //親プロセス
    while(wait(&status) != pid)
    ;
  } else {  //子プロセス
    execl(execpath, "sh", "-c", command, NULL);
    perror(execpath);
    exit(127);
  }
  
  return status;
}

/* 実行例
% make    --コンパイル
cc -Wall -std=c99 -o mysysmain mysysmain.c  mysystem.c
% ./mysysmain "ls -l"   --正しく使用した時
mysystem:
total 616
-rw-r--r--  1 miyajiharuto  staff     143  7  4 09:43 Makefile
-rw-r--r--  1 miyajiharuto  staff    2795  7  4 09:43 README.md
-rw-r--r--  1 miyajiharuto  staff  238232  7  4 09:43 README.pdf
-rwxr-xr-x  1 miyajiharuto  staff   50406  7  4 10:00 mysysmain
-rw-r--r--  1 miyajiharuto  staff     925  7  4 09:43 mysysmain.c
-rw-r--r--  1 miyajiharuto  staff     900  7  4 10:00 mysystem.c
-rw-r--r--  1 miyajiharuto  staff      90  7  4 09:43 mysystem.h
retval = 00000000
system:
total 616
-rw-r--r--  1 miyajiharuto  staff     143  7  4 09:43 Makefile
-rw-r--r--  1 miyajiharuto  staff    2795  7  4 09:43 README.md
-rw-r--r--  1 miyajiharuto  staff  238232  7  4 09:43 README.pdf
-rwxr-xr-x  1 miyajiharuto  staff   50406  7  4 10:00 mysysmain
-rw-r--r--  1 miyajiharuto  staff     925  7  4 09:43 mysysmain.c
-rw-r--r--  1 miyajiharuto  staff     900  7  4 10:00 mysystem.c
-rw-r--r--  1 miyajiharuto  staff      90  7  4 09:43 mysystem.h
retval = 00000000
% ./mysysmain "aaa"   --存在しないコマンドを実行しようとした時
mysystem:
sh: aaa: command not found
retval = 00007f00
system:
sh: aaa: command not found
retval = 00007f00
% ./mysysmain "ls -l" "abc"   --mysysmainの使い方を誤った時
使い方 : ./mysysmain コマンド文字列
% ./mysysmain              
使い方 : ./mysysmain コマンド文字列
% ./mysysmain "echo "aaa""    --他のコマンドも使ってみる
mysystem:
aaa
retval = 00000000
system:
aaa
retval = 00000000
*/
