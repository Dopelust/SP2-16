#pragma once
#ifndef UTILITY_H
#define UTILITY_H

#include "vertex.h"
#include "Mtx44.h"

Position operator*(const Mtx44& lhs, Position rhs);

#endif