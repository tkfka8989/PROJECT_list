#!/bin/sh

all : server_exe client_exe
CC=gcc

server_exe : epollserver.c
  $(CC) -o $@ $<
  
client_exe : epollclient.c
  $(CC) -o $@ $<
  
clean :
  rm server_exe client_exe
