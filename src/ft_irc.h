#pragma once
#include <bits/types/sig_atomic_t.h>
#include <csignal>
#include <iostream>
#include <signal.h>

extern volatile sig_atomic_t stop;
void setupsig();
